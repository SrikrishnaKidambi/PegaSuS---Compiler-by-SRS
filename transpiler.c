#include "transpiler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef QUAD_DEFINED
typedef struct {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
} Quad;
#define QUAD_DEFINED
#endif

extern Quad IR[];
extern int  IR_idx;

// when "def/class/if/while" indent_level++ on exit indent_level--

static int indent_level = 0;

static void write_indent(FILE* out) {
    for (int i = 0; i < indent_level; i++)
        fprintf(out, "    ");
}

static void safe_name(const char* in, char* out_buf) {
    static const char* kw[] = {
        "in", "is", "or", "and", "not", "for", "if",
        "else", "elif", "while", "def", "return", "class",
        "pass", "True", "False", "None", "lambda", "yield",
        "import", "from", "global", "del", "with", "as",
        "try", "except", "finally", "raise", NULL
    };
    strcpy(out_buf, in);
    for (int i = 0; kw[i]; i++) {
        if (strcmp(in, kw[i]) == 0) {
            sprintf(out_buf, "_%s", in);
            return;
        }
    }
}

static int is_num(const char* s) {
    if (!s || !*s) return 0;
    char* e;
    strtod(s, &e);
    return (*e == '\0');
}

//label-book keeping
#define MAX_LABELS 256

typedef struct {
    char name[32];
    int  quad_idx;
} LabelInfo;

static LabelInfo labels[MAX_LABELS];
static int       label_cnt = 0;

//It scans all quads once at start and records every label quad and its index position in OPT_IR[].
static void collect_labels(void) {
    label_cnt = 0;
    for (int i = 0; i < IR_idx; i++) {
        if (strcmp(IR[i].op, "label") == 0) {
            strncpy(labels[label_cnt].name, IR[i].result, 31);
            labels[label_cnt].quad_idx = i;
            label_cnt++;
        }
    }
}
//Given a label like "L2", this function returns which quad index it sits at!.
static int label_pos(const char* name) {
    for (int i = 0; i < label_cnt; i++)
        if (strcmp(labels[i].name, name) == 0)
            return labels[i].quad_idx;
    return -1;
}
//A goto is a back edge(loop) if it jumps to a label that appears earlier in the IR than the goto itself. Forward jumps are if/else.
static int is_back_edge(int goto_idx, const char* target) {
    int lp = label_pos(target);
    return (lp >= 0 && lp < goto_idx);
}

//arg accumulator
//python need "t0=add(x,y)"so we must not emit the parameters before so we just push the argument into "arf_buf" and when we finally hit the "call" quad we use "arg_buf" to build the argument list and then reset "arg_cnt" to zero.
static char arg_buf[64][64];
static int  arg_cnt = 0;

//object context
static char current_obj[64] = "";

//consumed map
static int consumed[10000];

// control flow analysis 

typedef enum { CF_IF, CF_WHILE, CF_UNKNOWN } CFKind;

typedef struct {
    CFKind kind;
    int    false_label_pos;
    int    end_label_pos;
    int    goto_pos;
    int    has_else;
} CFInfo;

static CFInfo analyse_ifFalse(int if_pos) {
    CFInfo cf;
    memset(&cf, 0, sizeof(cf));
    cf.kind            = CF_UNKNOWN;
    cf.false_label_pos = -1;
    cf.end_label_pos   = -1;
    cf.goto_pos        = -1;
    cf.has_else        = 0;

    const char* false_lbl = IR[if_pos].result;
    int flp = label_pos(false_lbl);
    if (flp < 0) return cf;

    cf.false_label_pos = flp;

    // find goto just before the false label 
    for (int k = if_pos + 1; k < flp; k++) {
        if (strcmp(IR[k].op, "goto") == 0)
            cf.goto_pos = k;
    }

    if (cf.goto_pos >= 0 && is_back_edge(cf.goto_pos, IR[cf.goto_pos].result)) {
        //while loop — goto jumps back to loop header
        cf.kind          = CF_WHILE;
        cf.end_label_pos = label_pos(IR[cf.goto_pos].result);
        cf.has_else      = 0;
    } else {
        cf.kind = CF_IF;
        if (cf.goto_pos >= 0) {
            const char* end_lbl  = IR[cf.goto_pos].result;
            cf.end_label_pos     = label_pos(end_lbl);
            cf.has_else          = (cf.end_label_pos > cf.false_label_pos + 1);
        } else {
            cf.end_label_pos = flp;
            cf.has_else      = 0;
        }
    }
    return cf;
}

// forward declaration
static void emit_range(FILE* out, int from, int to);

static int find_cond_quad(int if_pos, const char* cond_var) {
    static const char* relops[] = {
        "<", ">", "==", "&&", "||", "!", "<=", ">=", NULL
    };
    // search backwards within a small window
    for (int k = if_pos - 1; k >= 0 && k >= if_pos - 5; k--) {
        if (strcmp(IR[k].result, cond_var) == 0) {
            for (int j = 0; relops[j]; j++) {
                if (strcmp(IR[k].op, relops[j]) == 0)
                    return k;
            }
        }
    }
    return -1;
}

// this builds a python condition string from a relational quad
static void build_cond_str(int cq, char* buf) {
    char a1[64], a2[64];
    safe_name(IR[cq].arg1, a1);
    safe_name(IR[cq].arg2, a2);
    const char* op = IR[cq].op;

    // map IR op to python op 
    const char* pyop = op;
    if      (strcmp(op, "&&") == 0) pyop = "and";
    else if (strcmp(op, "||") == 0) pyop = "or";
    else if (strcmp(op, "!")  == 0) {
        sprintf(buf, "not %s", a1);
        return;
    }

    if (a2[0])
        sprintf(buf, "%s %s %s", a1, pyop, a2);
    else
        sprintf(buf, "%s", a1);
}
// main emitter 
static void emit_range(FILE* out, int from, int to) {
    for (int i = from; i <= to; i++) {
        if (i < 0 || i >= IR_idx) continue;
        if (consumed[i]) continue;

        Quad* q = &IR[i];
        char a1[64], a2[64], res[64];
        safe_name(q->arg1,   a1);
        safe_name(q->arg2,   a2);
        safe_name(q->result, res);

        //func
        if (strcmp(q->op, "func") == 0) {
            fprintf(out, "\n");
            write_indent(out);
            // function name is in arg1
            fprintf(out, "def %s(", a1);
            int pi = i + 1;
            int first = 1;
            while (pi <= to && strcmp(IR[pi].op, "param") == 0) {
                char pn[64]; safe_name(IR[pi].arg1, pn);
                if (!first) fprintf(out, ", ");
                fprintf(out, "%s", pn);
                first = 0;
                consumed[pi] = 1;
                pi++;
            }
            // empty body guard
            fprintf(out, "):\n");
            indent_level++;
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "endfunc") == 0) {
            indent_level--;
            fprintf(out, "\n");
            consumed[i] = 1;
            continue;
        }

        // entity → class
        if (strcmp(q->op, "entity") == 0) {
            fprintf(out, "\n");
            write_indent(out);
            //entity name is in arg1 
            fprintf(out, "class %s:\n", a1);
            indent_level++;
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "end_entity") == 0) {
            indent_level--;
            fprintf(out, "\n");
            consumed[i] = 1;
            continue;
        }

        // method 
        if (strcmp(q->op, "method") == 0) {
            fprintf(out, "\n");
            write_indent(out);
            //method name is in arg1 
            fprintf(out, "def %s(self", a1);
            int pi = i + 1;
            while (pi <= to && strcmp(IR[pi].op, "param") == 0) {
                char pn[64]; safe_name(IR[pi].arg1, pn);
                fprintf(out, ", %s", pn);
                consumed[pi] = 1;
                pi++;
            }
            fprintf(out, "):\n");
            indent_level++;
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "end_method") == 0) {
            indent_level--;
            fprintf(out, "\n");
            consumed[i] = 1;
            continue;
        }

        //constructor
        if (strcmp(q->op, "constr") == 0) {
            fprintf(out, "\n");
            write_indent(out);
            fprintf(out, "def __init__(self");
            int pi = i + 1;
            while (pi <= to && strcmp(IR[pi].op, "param") == 0) {
                char pn[64]; safe_name(IR[pi].arg1, pn);
                fprintf(out, ", %s", pn);
                consumed[pi] = 1;
                pi++;
            }
            fprintf(out, "):\n");
            indent_level++;
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "end_constr") == 0) {
            indent_level--;
            consumed[i] = 1;
            continue;
        }

        // ---- param (already consumed above)
        if (strcmp(q->op, "param") == 0) {
            consumed[i] = 1;
            continue;
        }

        // label / goto consumed by ifFalse look-ahead
        if (strcmp(q->op, "label") == 0) {
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "goto") == 0) {
            consumed[i] = 1;
            continue;
        }

        // ifFalse → if / while 
         if (strcmp(q->op, "ifFalse") == 0) {
    CFInfo cf = analyse_ifFalse(i);
    consumed[i] = 1;

    // we try to inline the condition instead of using the temp variable 
    char cond_str[128];
    int cq = find_cond_quad(i, q->arg1);  // q->arg1 is the condition var e.g. t0 
    if (cq >= 0) {
        build_cond_str(cq, cond_str);
        consumed[cq] = 1;   // mark the condition quad consumed so it doesn't emit as assignment 
    } else {
        // fallback — use the temp variable as-is 
        safe_name(q->arg1, cond_str);
    }

    if (cf.kind == CF_WHILE) {
        write_indent(out);
        fprintf(out, "while %s:\n", cond_str);   // inlined condition 
        indent_level++;
        int body_end = cf.goto_pos - 1;
        consumed[cf.goto_pos] = 1;
        if (cf.false_label_pos >= 0) consumed[cf.false_label_pos] = 1;
        if (cf.end_label_pos   >= 0) consumed[cf.end_label_pos]   = 1;
        emit_range(out, i + 1, body_end);
        indent_level--;
    } else {
        // IF 
        write_indent(out);
        fprintf(out, "if %s:\n", cond_str);      // inlined condition 
        indent_level++;
        int true_end = (cf.goto_pos >= 0) ? cf.goto_pos - 1
                                           : cf.false_label_pos - 1;
        if (cf.goto_pos        >= 0) consumed[cf.goto_pos]        = 1;
        if (cf.false_label_pos >= 0) consumed[cf.false_label_pos] = 1;
        if (cf.end_label_pos   >= 0) consumed[cf.end_label_pos]   = 1;
        emit_range(out, i + 1, true_end);
        indent_level--;

        if (cf.has_else) {
            write_indent(out);
            fprintf(out, "else:\n");
            indent_level++;
            int else_end = (cf.end_label_pos >= 0) ? cf.end_label_pos - 1 : to;
            emit_range(out, cf.false_label_pos + 1, else_end);
            indent_level--;
        }
    }
    continue;
}
        // arg accumulation
        if (strcmp(q->op, "arg") == 0) {
            if (arg_cnt < 64) strncpy(arg_buf[arg_cnt++], a1, 63);
            consumed[i] = 1;
            continue;
        }

        // push_ptr (object for method call)
        if (strcmp(q->op, "push_ptr") == 0) {
            strncpy(current_obj, a1, 63);
            consumed[i] = 1;
            continue;
        }

        // call
        if (strcmp(q->op, "call") == 0) {
            write_indent(out);
            if (res[0]) fprintf(out, "%s = ", res);
            fprintf(out, "%s(", a1);
            for (int k = 0; k < arg_cnt; k++) {
                if (k) fprintf(out, ", ");
                fprintf(out, "%s", arg_buf[k]);
            }
            fprintf(out, ")\n");
            arg_cnt     = 0;
            consumed[i] = 1;
            continue;
        }

        // call_method 
        if (strcmp(q->op, "call_method") == 0) {
            write_indent(out);
            if (res[0]) fprintf(out, "%s = ", res);
            fprintf(out, "%s.%s(", current_obj[0] ? current_obj : "self", a1);
            for (int k = 0; k < arg_cnt; k++) {
                if (k) fprintf(out, ", ");
                fprintf(out, "%s", arg_buf[k]);
            }
            fprintf(out, ")\n");
            arg_cnt        = 0;
            current_obj[0] = '\0';
            consumed[i]    = 1;
            continue;
        }

        //call_constr (handled by new)
        if (strcmp(q->op, "call_constr") == 0) {
            consumed[i] = 1;
            continue;
        }

        //new
        if (strcmp(q->op, "new") == 0) {
            write_indent(out);
            // new  ClassName  ""  obj_var 
            fprintf(out, "%s = %s(", res, a1);
            for (int k = 0; k < arg_cnt; k++) {
                if (k) fprintf(out, ", ");
                fprintf(out, "%s", arg_buf[k]);
            }
            fprintf(out, ")\n");
            arg_cnt     = 0;
            consumed[i] = 1;
            continue;
        }

        //get_field
        if (strcmp(q->op, "get_field") == 0) {
            write_indent(out);
            fprintf(out, "%s = %s.%s\n", res, a1, a2);
            consumed[i] = 1;
            continue;
        }

        //set_field  obj  field  value
        if (strcmp(q->op, "set_field") == 0) {
            write_indent(out);
            fprintf(out, "%s.%s = %s\n", a1, a2, res);
            consumed[i] = 1;
            continue;
        }

        //return
        if (strcmp(q->op, "return") == 0) {
            write_indent(out);
            if (a1[0]) fprintf(out, "return %s\n", a1);
            else       fprintf(out, "return\n");
            consumed[i] = 1;
            continue;
        }

        //out → print
        if (strcmp(q->op, "out") == 0) {
            write_indent(out);
            fprintf(out, "print(%s)\n", a1);
            consumed[i] = 1;
            continue;
        }

        // in → input
        if (strcmp(q->op, "in") == 0) {
            write_indent(out);
            fprintf(out, "%s = int(input())\n", res);
            consumed[i] = 1;
            continue;
        }

        //simple assignment  result = arg1 
        if (strcmp(q->op, "=") == 0 && q->arg2[0] == '\0') {
            write_indent(out);
            fprintf(out, "%s = %s\n", res, a1);
            consumed[i] = 1;
            continue;
        }

        //array index
        if (strcmp(q->op, "[]") == 0) {
            write_indent(out);
            fprintf(out, "%s = %s[%s]\n", res, a1, a2);
            consumed[i] = 1;
            continue;
        }

        //unary NOT
        if (strcmp(q->op, "!") == 0) {
            write_indent(out);
            fprintf(out, "%s = not %s\n", res, a1);
            consumed[i] = 1;
            continue;
        }

        // binary ops
        static const char* ir_ops[] = {
            "+", "-", "*", "/", "%", "<<", ">>",
            "&", "|", "&&", "||", "==", ">", "<", NULL
        };
        static const char* py_ops[] = {
            "+", "-", "*", "//", "%", "<<", ">>",
            "&", "|", "and", "or", "==", ">", "<", NULL
        };

        int matched = 0;
        for (int k = 0; ir_ops[k]; k++) {
            if (strcmp(q->op, ir_ops[k]) == 0) {
                write_indent(out);
                fprintf(out, "%s = %s %s %s\n", res, a1, py_ops[k], a2);
                consumed[i] = 1;
                matched     = 1;
                break;
            }
        }
        if (matched) continue;

        write_indent(out);
        fprintf(out, "# [unhandled] op='%s' arg1='%s' arg2='%s' result='%s'\n",
                q->op, q->arg1, q->arg2, q->result);
        consumed[i] = 1;
    }
}

// public entry point
void transpile_to_python(FILE* out) {
    memset(consumed, 0, sizeof(consumed));
    arg_cnt        = 0;
    indent_level   = 0;
    current_obj[0] = '\0';

    collect_labels();

    fprintf(out, "# Generated by Pegasus transpiler\n\n");
    emit_range(out, 0, IR_idx - 1);

    // emit if __name__ == '__main__' guard if main() exists 
    for (int i = 0; i < IR_idx; i++) {
        if (strcmp(IR[i].op, "func") == 0 &&
            strcmp(IR[i].arg1, "main") == 0) {
            fprintf(out, "\nif __name__ == '__main__':\n    main()\n");
            break;
        }
    }
}
