#include "transpiler.h"
#include "quad.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


extern Quad IR[];
extern int  IR_idx;

static int indent_level = 0;

static void write_indent(FILE* out) {
    for (int i = 0; i < indent_level; i++)
        fprintf(out, "    ");
}

/*
 * Strip trailing _<digits> suffix from a name.
 * e.g. l_0 -> l, r_1 -> r, foo_23 -> foo
 * but  add_ii, r_ii, foo_bar -> unchanged (suffix must be all digits)
 */
static void strip_numeric_suffix(const char* in, char* out_buf) {
    strcpy(out_buf, in);
    char* underscore = strrchr(out_buf, '_');
    if (!underscore) return;
    char* after = underscore + 1;
    if (*after == '\0') return;          /* trailing underscore, no suffix */
    for (char* p = after; *p; p++) {
        if (*p < '0' || *p > '9') return; /* non-digit found -> leave alone */
    }
    /* all chars after last '_' are digits -> truncate */
    *underscore = '\0';
}

static void safe_name(const char* in, char* out_buf) {
    static const char* kw[] = {
        "in", "is", "or", "and", "not", "for", "if",
        "else", "elif", "while", "def", "return", "class",
        "pass", "True", "False", "None", "lambda", "yield",
        "import", "from", "global", "del", "with", "as",
        "try", "except", "finally", "raise", NULL
    };

    /* first strip numeric suffix */
    char stripped[64];
    strip_numeric_suffix(in, stripped);

    strcpy(out_buf, stripped);
    for (int i = 0; kw[i]; i++) {
        if (strcmp(stripped, kw[i]) == 0) {
            sprintf(out_buf, "_%s", stripped);
            return;
        }
    }
}

static void resolve_this(const char* name, char* buf) {
    if (strcmp(name, "this") == 0 ||
        strcmp(name, "self") == 0 ){
        strcpy(buf, "self");
    } else {
        safe_name(name, buf);
    }
}

#define MAX_LABELS 256

typedef struct {
    char name[32];
    int  quad_idx;
} LabelInfo;

static LabelInfo labels[MAX_LABELS];
static int       label_cnt = 0;

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

static int label_pos(const char* name) {
    for (int i = 0; i < label_cnt; i++)
        if (strcmp(labels[i].name, name) == 0)
            return labels[i].quad_idx;
    return -1;
}

static int is_back_edge(int goto_idx, const char* target) {
    int lp = label_pos(target);
    return (lp >= 0 && lp < goto_idx);
}

static char arg_buf[64][64];
static int  arg_cnt = 0;
static char current_obj[64] = "";
static int  consumed[10000];

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

    for (int k = if_pos + 1; k < flp; k++) {
        if (strcmp(IR[k].op, "goto") == 0)
            cf.goto_pos = k;
    }

    if (cf.goto_pos >= 0 && is_back_edge(cf.goto_pos, IR[cf.goto_pos].result)) {
        cf.kind          = CF_WHILE;
        cf.end_label_pos = label_pos(IR[cf.goto_pos].result);
        cf.has_else      = 0;
    } else {
        cf.kind = CF_IF;
        if (cf.goto_pos >= 0) {
            const char* end_lbl = IR[cf.goto_pos].result;
            cf.end_label_pos    = label_pos(end_lbl);
            cf.has_else         = (cf.end_label_pos > cf.false_label_pos + 1);
        } else {
            cf.end_label_pos = flp;
            cf.has_else      = 0;
        }
    }
    return cf;
}

static void emit_range(FILE* out, int from, int to);

static int find_cond_quad(int if_pos, const char* cond_var) {
    static const char* relops[] = {
        "<", ">", "==", "&&", "||", "!", "<=", ">=", NULL
    };
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

static void build_cond_str(int cq, char* buf) {
    char a1[64], a2[64];
    safe_name(IR[cq].arg1, a1);
    safe_name(IR[cq].arg2, a2);
    const char* op = IR[cq].op;

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

/*
 * USE-COUNT PASS
 */
#define MAX_VARS 512

typedef struct {
    char name[32];
    int  use_count;
    int  def_quad;
} VarInfo;

static VarInfo var_table[MAX_VARS];
static int     var_cnt = 0;

static int find_or_add_var(const char* name) {
    for (int i = 0; i < var_cnt; i++)
        if (strcmp(var_table[i].name, name) == 0) return i;
    if (var_cnt >= MAX_VARS) return -1;
    strncpy(var_table[var_cnt].name, name, 31);
    var_table[var_cnt].use_count = 0;
    var_table[var_cnt].def_quad  = -1;
    return var_cnt++;
}

static void build_use_counts(void) {
    var_cnt = 0;
    for (int i = 0; i < IR_idx; i++) {
        if (IR[i].result[0]) {
            int vi = find_or_add_var(IR[i].result);
            if (vi >= 0 && var_table[vi].def_quad < 0)
                var_table[vi].def_quad = i;
        }
        if (IR[i].arg1[0]) {
            int vi = find_or_add_var(IR[i].arg1);
            if (vi >= 0) var_table[vi].use_count++;
        }
        if (IR[i].arg2[0]) {
            int vi = find_or_add_var(IR[i].arg2);
            if (vi >= 0) var_table[vi].use_count++;
        }
    }
}

static int get_use_count(const char* name) {
    for (int i = 0; i < var_cnt; i++)
        if (strcmp(var_table[i].name, name) == 0)
            return var_table[i].use_count;
    return 0;
}

/*
 * SUBSTITUTION TABLE
 */
#define MAX_ALIASES 512

typedef struct {
    char from[32];
    char to[32];
} Alias;

static Alias aliases[MAX_ALIASES];
static int   alias_cnt = 0;

static void add_alias(const char* from, const char* to) {
    if (alias_cnt >= MAX_ALIASES) return;
    strncpy(aliases[alias_cnt].from, from, 31);
    strncpy(aliases[alias_cnt].to,   to,   31);
    alias_cnt++;
}

static const char* resolve_alias(const char* name) {
    for (int i = alias_cnt - 1; i >= 0; i--)
        if (strcmp(aliases[i].from, name) == 0)
            return aliases[i].to;
    return name;
}

/*
 * PRE-PASS
 */
static int dead[10000];

static int is_temp(const char* name) {
    return (name[0] == 't' && name[1] >= '0' && name[1] <= '9');
}

static void pre_pass(void) {
    memset(dead, 0, sizeof(dead));
    alias_cnt = 0;
    build_use_counts();

    for (int i = 0; i < IR_idx - 1; i++) {
        Quad* q  = &IR[i];
        Quad* qn = &IR[i + 1];

        if (!q->result[0]) continue;

        if (strcmp(q->op, "=") == 0 && q->arg2[0] == '\0') {
            if (is_temp(q->result) && get_use_count(q->result) == 0) {
                dead[i] = 1;
                continue;
            }
            if (is_temp(q->result)) {
                add_alias(q->result, q->arg1);
                dead[i] = 1;
                continue;
            }
        }
        if (strcmp(q->op, "array_init") == 0) continue;

        if (get_use_count(q->result) == 0 && is_temp(q->result)) {
            if (strcmp(qn->op,   q->op)   == 0 &&
                strcmp(qn->arg1, q->arg1) == 0 &&
                strcmp(qn->arg2, q->arg2) == 0) {
                add_alias(q->result, qn->result);
                dead[i] = 1;
            } else {
                dead[i] = 1;
            }
        }
    }
    if (IR_idx > 0) {
        int last = IR_idx - 1;
        Quad* q = &IR[last];
        if (q->result[0] && get_use_count(q->result) == 0 && is_temp(q->result))
            dead[last] = 1;
    }
}

static void rname(const char* raw, char* buf) {
    const char* resolved = resolve_alias(raw);
    resolve_this(resolved, buf);
}

static void emit_range(FILE* out, int from, int to) {
    for (int i = from; i <= to; i++) {
        if (i < 0 || i >= IR_idx) continue;
        if (consumed[i])          continue;
        if (dead[i])              continue;

        Quad* q = &IR[i];
        char a1[64], a2[64], res[64];
        rname(q->arg1,   a1);
        rname(q->arg2,   a2);
        rname(q->result, res);

        if (strcmp(q->op, "func") == 0) {
            fprintf(out, "\n");
            write_indent(out);
            fprintf(out, "def %s(", a1);
            int pi = i + 1, first = 1;
            while (pi <= to && strcmp(IR[pi].op, "param") == 0) {
                char pn[64]; resolve_this(IR[pi].arg1, pn);
                if (!first) fprintf(out, ", ");
                fprintf(out, "%s", pn);
                first = 0;
                consumed[pi++] = 1;
            }
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

        if (strcmp(q->op, "entity") == 0) {
            fprintf(out, "\n");
            write_indent(out);
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

        if (strcmp(q->op, "constr") == 0) {
            fprintf(out, "\n");
            write_indent(out);
            fprintf(out, "def __init__(self");
            int pi = i + 1;
            while (pi <= to && strcmp(IR[pi].op, "param") == 0) {
                char pn[64]; resolve_this(IR[pi].arg1, pn);
                if (strcmp(pn, "self") != 0)
                    fprintf(out, ", %s", pn);
                consumed[pi++] = 1;
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

        if (strcmp(q->op, "method") == 0) {
            fprintf(out, "\n");
            write_indent(out);
            fprintf(out, "def %s(self", a1);
            int pi = i + 1;
            while (pi <= to && strcmp(IR[pi].op, "param") == 0) {
                char pn[64]; resolve_this(IR[pi].arg1, pn);
                if (strcmp(pn, "self") != 0)
                    fprintf(out, ", %s", pn);
                consumed[pi++] = 1;
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

        if (strcmp(q->op, "param") == 0) {
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "label") == 0 ||
            strcmp(q->op, "goto")  == 0) {
            consumed[i] = 1;
            continue;
        }
        if (strcmp(q->op, "array_init") == 0) {
            write_indent(out);
            /* a1 = "1,2,3,4,5"  res = "arr" */
            fprintf(out, "%s = [%s]\n", res, a1);
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "ifFalse") == 0) {
            CFInfo cf = analyse_ifFalse(i);
            consumed[i] = 1;

            char cond_str[128];
            int cq = find_cond_quad(i, q->arg1);
            if (cq >= 0) {
                build_cond_str(cq, cond_str);
                consumed[cq] = 1;
            } else {
                safe_name(q->arg1, cond_str);
            }

            if (cf.kind == CF_WHILE) {
                write_indent(out);
                fprintf(out, "while %s:\n", cond_str);
                indent_level++;
                int body_end = cf.goto_pos - 1;
                consumed[cf.goto_pos] = 1;
                if (cf.false_label_pos >= 0) consumed[cf.false_label_pos] = 1;
                if (cf.end_label_pos   >= 0) consumed[cf.end_label_pos]   = 1;
                emit_range(out, i + 1, body_end);
                indent_level--;
            } else {
                write_indent(out);
                fprintf(out, "if %s:\n", cond_str);
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
                    int else_end = (cf.end_label_pos >= 0)
                                   ? cf.end_label_pos - 1 : to;
                    emit_range(out, cf.false_label_pos + 1, else_end);
                    indent_level--;
                }
            }
            continue;
        }

        if (strcmp(q->op, "arg") == 0) {
            if (arg_cnt < 64) strncpy(arg_buf[arg_cnt++], a1, 63);
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "push_ptr") == 0) {
                if (strcmp(q->arg1, "0") == 0 || strcmp(q->arg1, "this") == 0){
        strncpy(current_obj, "self", 63);
                }
    else{
        strncpy(current_obj, a1, 63);
    }
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "call") == 0) {
            write_indent(out);
            if (res[0] && strcmp(res, "_") != 0)
                fprintf(out, "%s = ", res);
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

        if (strcmp(q->op, "call_method") == 0) {
            write_indent(out);
            if (res[0] && strcmp(res, "_") != 0)
                fprintf(out, "%s = ", res);
            const char* obj = (current_obj[0]) ? current_obj : "self";
            fprintf(out, "%s.%s(", obj, a1);
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

        if (strcmp(q->op, "call_constr") == 0) {
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "new") == 0) {
            write_indent(out);
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

        if (strcmp(q->op, "get_field") == 0) {
            write_indent(out);
            fprintf(out, "%s = %s.%s\n", res, a1, a2);
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "set_field") == 0) {
            write_indent(out);
            fprintf(out, "%s.%s = %s\n", a1, a2, res);
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "self_set") == 0) {
            write_indent(out);
            fprintf(out, "self.%s = %s\n", a1, res);
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "return") == 0) {
            write_indent(out);
            if (a1[0]) fprintf(out, "return %s\n", a1);
            else       fprintf(out, "return\n");
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "out") == 0) {
            write_indent(out);
            fprintf(out, "print(%s)\n", a1);
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "in") == 0) {
            write_indent(out);
            fprintf(out, "%s = int(input())\n", res);
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "=") == 0 && q->arg2[0] == '\0') {
            write_indent(out);
            fprintf(out, "%s = %s\n", res, a1);
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "[]") == 0) {
            write_indent(out);
            fprintf(out, "%s = %s[%s]\n", res, a1, a2);
            consumed[i] = 1;
            continue;
        }

        if (strcmp(q->op, "!") == 0) {
            write_indent(out);
            fprintf(out, "%s = not %s\n", res, a1);
            consumed[i] = 1;
            continue;
        }

        static const char* ir_ops[] = {
            "+", "-", "*", "/", "%", "<<", ">>",
            "&", "|", "&&", "||", "==", "!=",
            ">", "<", ">=", "<=", NULL
        };
        static const char* py_ops[] = {
            "+", "-", "*", "//", "%", "<<", ">>",
            "&", "|", "and", "or", "==", "!=",
            ">", "<", ">=", "<=", NULL
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

void transpile_to_python(FILE* out) {
    memset(consumed, 0, sizeof(consumed));
    arg_cnt        = 0;
    indent_level   = 0;
    current_obj[0] = '\0';

    collect_labels();
    pre_pass();

    fprintf(out, "# Generated by Pegasus transpiler\n\n");
    emit_range(out, 0, IR_idx - 1);

    for (int i = 0; i < IR_idx; i++) {
        if (strcmp(IR[i].op, "func")  == 0 &&
            strcmp(IR[i].arg1, "main") == 0) {
            fprintf(out, "\nif __name__ == '__main__':\n    main()\n");
            break;
        }
    }
}
