// symtab.c
#include "symtab.h"

SymTable* global_scope       = NULL;
SymTable* current_scope      = NULL;
DataType  current_decl_type  = DT_UNKNOWN;

// ── djb2 hash ────────────────────────────
static unsigned int hash_fn(const char* s) {
    unsigned int h = 5381;
    while (*s) h = ((h << 5) + h) + (unsigned char)(*s++);
    return h % HASH_SIZE;
}

// ── Byte size of a primitive datatype ────
int datatype_size(DataType dt) {
    switch(dt) {
        case DT_INT:    return 4;
        case DT_FLOAT:  return 4;
        case DT_CHAR:   return 1;
        case DT_STRING: return 8;
        case DT_BOOL:   return 1;
        case DT_VOID:   return 0;
        //case DT_ENTITY: return 8;
        default:        return 0;
    }
}

// ── Create a new scope ───────────────────
SymTable* create_scope(ScopeKind kind, const char* name, SymTable* parent) {
    SymTable* t    = calloc(1, sizeof(SymTable));
    t->kind        = kind;
    t->level       = parent ? parent->level + 1 : 0;
    t->parent      = parent;
    t->next_offset = 0;
    strncpy(t->name, name, 63);
    return t;
}

// ── Append a parameter to a param list ───
void add_param(ParamNode** list, int* count, const char* name, DataType dt) {
    ParamNode* p = calloc(1, sizeof(ParamNode));
    strncpy(p->name, name, 63);
    p->datatype = dt;
    p->next     = NULL;
    if (!*list) {
        *list = p;
    } else {
        ParamNode* cur = *list;
        while (cur->next) cur = cur->next;
        cur->next = p;
    }
    (*count)++;
}

// ── Append a name to a NameNode list ─────
void add_name(NameNode** list, const char* name) {
    NameNode* n = calloc(1, sizeof(NameNode));
    strncpy(n->name, name, 63);
    n->next = NULL;
    if (!*list) {
        *list = n;
    } else {
        NameNode* cur = *list;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
}

// ── Insert a symbol into a scope ─────────
Symbol* insert_symbol(SymTable* tbl, const char* name,
                      SymKind kind, DataType dt, int line) {
    if (lookup_local(tbl, name)) {
        fprintf(stderr,
            "ERROR line %d: '%s' already declared in scope '%s'\n",
            line, name, tbl->name);
        return NULL;
    }
    Symbol* sym      = calloc(1, sizeof(Symbol));
    strncpy(sym->name, name, 63);
    sym->kind        = kind;
    sym->datatype    = dt;
    sym->scope_level = tbl->level;
    sym->size        = datatype_size(dt);
    if(dt == DT_INT){
	 //printf("Parent scope of %s is %s with code:%d\n", sym->name, tbl->parent->name, tbl->kind);
    }	 
    sym->offset      = tbl->next_offset;
    sym->is_initialized = 0;

    /*if (kind == KIND_VAR   || kind == KIND_PARAM ||
        kind == KIND_ARRAY || kind == KIND_FIELD)*/
    tbl->next_offset += sym->size;

    unsigned int h    = hash_fn(name);
    sym->next         = tbl->buckets[h];
    tbl->buckets[h]   = sym;
    tbl->symbol_count++;
    return sym;
}

// ── Lookup in this scope only ─────────────
Symbol* lookup_local(SymTable* tbl, const char* name) {
    unsigned int h = hash_fn(name);
    for (Symbol* s = tbl->buckets[h]; s; s = s->next)
        if (strcmp(s->name, name) == 0) return s;
    return NULL;
}

// ── Lookup walking up scope chain ─────────
Symbol* lookup(SymTable* tbl, const char* name) {
    for (SymTable* t = tbl; t; t = t->parent) {
        Symbol* s = lookup_local(t, name);
        if (s) return s;
    }
    return NULL;
}

// ─────────────────────────────────────────
//  PRINTING
// ─────────────────────────────────────────

static const char* kind_names[] = {
    "VAR","ARRAY","FUNCTION","METHOD",
    "FIELD","ENTITY","CONSTRUCTOR","PARAM",
    "FOR","IF","ELIF","ELSE"
};

static const char* dt_names[] = {
    "int","float","char","string","bool",
    "void","entity","unknown"
};

static const char* acc_names[] = { "---", "public", "private" };

static const char* scope_kind_name(ScopeKind k) {
    switch(k) {
        case SCOPE_GLOBAL:      return "GLOBAL";
        case SCOPE_FUNCTION:    return "FUNCTION";
        case SCOPE_METHOD:      return "METHOD";
        case SCOPE_CONSTRUCTOR: return "CONSTRUCTOR";
        case SCOPE_ENTITY:      return "ENTITY";
        case SCOPE_BLOCK:       return "BLOCK";
        case SCOPE_FOR:         return "FOR";
        case SCOPE_IF:          return "IF";
        case SCOPE_ELIF:        return "ELIF";
        case SCOPE_ELSE:        return "ELSE";
        default:                return "UNKNOWN";
    }
}

void print_table(SymTable* tbl) {
    printf("\n");
    printf("+-----------------------------------------------------------------+\n");
    printf("|  SCOPE: %-15s  kind=%-12s  level=%-2d            |\n",
           tbl->name,
           scope_kind_name(tbl->kind),
           tbl->level);
    printf("+--------------+-------------+---------+-------+------+----------+\n");
    printf("| %-12s | %-11s | %-7s | %-5s | %-4s | %-8s |\n",
           "NAME", "KIND", "DTYPE", "SIZE", "OFF", "EXTRA");
    printf("+--------------+-------------+---------+-------+------+----------+\n");

    /* collect all symbols */
    Symbol* all[1000];
    int cnt = 0;
    for (int b = 0; b < HASH_SIZE; b++)
        for (Symbol* s = tbl->buckets[b]; s; s = s->next)
            all[cnt++] = s;

    if (cnt == 0) {
        printf("|  (empty)                                                        |\n");
        printf("+--------------+-------------+---------+-------+------+----------+\n");
        return;
    }

    for (int i = 0; i < cnt; i++) {
        Symbol* s = all[i];
        char extra[512] = "";

        switch (s->kind) {
            case KIND_FUNCTION: {
                char params[256] = "";
                for (ParamNode* p = s->attr.func.param_list; p; p = p->next) {
                    char tmp[80];
                    snprintf(tmp, sizeof(tmp), "%s:%s",
                             p->name, dt_names[p->datatype]);
                    if (params[0]) strncat(params, ",", sizeof(params)-strlen(params)-1);
                    strncat(params, tmp, sizeof(params)-strlen(params)-1);
                }
                snprintf(extra, 512, "ret=%s p=[%s] lbl=%s",
                         dt_names[s->attr.func.return_type],
                         params,
                         s->attr.func.entry_label);
                break;
            }
            case KIND_METHOD: {
                char params[256] = "";
                for (ParamNode* p = s->attr.method.param_list; p; p = p->next) {
                    char tmp[80];
                    snprintf(tmp, sizeof(tmp), "%s:%s",
                             p->name, dt_names[p->datatype]);
                    if (params[0]) strncat(params, ",", sizeof(params)-strlen(params)-1);
                    strncat(params, tmp, sizeof(params)-strlen(params)-1);
                }
                snprintf(extra, 512, "%s cls=%s ret=%s p=[%s]",
                         acc_names[s->attr.method.access],
                         s->attr.method.belongs_to,
                         dt_names[s->attr.method.return_type],
                         params);
                break;
            }
            case KIND_FIELD:
                snprintf(extra, 512, "%s cls=%s",
                         acc_names[s->attr.field.access],
                         s->attr.field.belongs_to);
                break;
            case KIND_ARRAY:
                snprintf(extra, 512, "dims=%d [%d][%d] init=%d",
                         s->attr.array.dimensions,
                         s->attr.array.dim1,
                         s->attr.array.dim2,
                         s->attr.array.is_initialized);
                break;
            case KIND_ENTITY: {
                char fl[256]="", ml[256]="", cl[256]="";
                for (NameNode* n = s->attr.entity.fields_list; n; n = n->next) {
                    if (fl[0]) strncat(fl, ",", sizeof(fl)-strlen(fl)-1);
                    strncat(fl, n->name, sizeof(fl)-strlen(fl)-1);
                }
                for (NameNode* n = s->attr.entity.methods_list; n; n = n->next) {
                    if (ml[0]) strncat(ml, ",", sizeof(ml)-strlen(ml)-1);
                    strncat(ml, n->name, sizeof(ml)-strlen(ml)-1);
                }
                for (NameNode* n = s->attr.entity.constructors_list; n; n = n->next) {
                    if (cl[0]) strncat(cl, ",", sizeof(cl)-strlen(cl)-1);
                    strncat(cl, n->name, sizeof(cl)-strlen(cl)-1);
                }
                snprintf(extra, 512, "sz=%d F:[%s] M:[%s] C:[%s]",
                         s->attr.entity.class_size, fl, ml, cl);
                break;
            }
            case KIND_CONSTRUCTOR:
                snprintf(extra, 512, "cls=%s params=%d lbl=%s",
                         s->attr.ctor.belongs_to,
                         s->attr.ctor.param_count,
                         s->attr.ctor.entry_label);
                break;
            case KIND_PARAM:
                snprintf(extra, 512, "init=%d", s->is_initialized);
                break;
            case KIND_VAR:
                snprintf(extra, 512, "init=%d", s->is_initialized);
                break;
            case KIND_FOR:
                snprintf(extra, 512, "start=%s end=%s",
                         s->attr.forstmt.start_label,
                         s->attr.forstmt.end_label);
                break;
            case KIND_IF:
            case KIND_ELIF:
                snprintf(extra, 512, "false=%s end=%s",
                         s->attr.ifstmt.false_label,
                         s->attr.ifstmt.end_label);
                break;
            default:
                break;
        }

        /* truncate extra for display */
        char extra_short[32];
        snprintf(extra_short, 32, "%.30s", extra);

        printf("| %-12s | %-11s | %-7s | %-5d | %-4d | %-8s |\n",
               s->name,
               kind_names[s->kind],
               dt_names[s->datatype],
               s->size,
               s->offset,
               extra_short);

        /* print full extra on next line if it's long */
        if (strlen(extra) > 30) {
            printf("|   >> %-59s|\n", extra);
        }
    }
    printf("+--------------+-------------+---------+-------+------+----------+\n");
}
