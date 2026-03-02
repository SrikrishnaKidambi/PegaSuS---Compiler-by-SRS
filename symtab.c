#include "symtab.h"

SymTable* global_scope      = NULL;       // set in main() before yyparse()
SymTable* current_scope     = NULL;       // updated by parser as scopes open/close
DataType  current_decl_type = DT_UNKNOWN; // legacy — no longer used actively
//static Symbol* current_function = NULL;		// A global variable that is used for storing the function(a symbol right!!) so that we can know what is the expected 
						// return type

SymTable* entity_scopes[MAX_ENTITIES];
int entity_scope_count = 0;

void register_entity_scope(SymTable* scope){
	if(entity_scope_count < MAX_ENTITIES){
		entity_scopes[entity_scope_count++] = scope;
	}
}

SymTable* find_entity_scope(const char* entity_name){
	for(int i=0; i<entity_scope_count; i++){
		if(strcmp(entity_scopes[i]->name, entity_name) == 0){
			return entity_scopes[i];
		}
	}
	return NULL;
}

static unsigned int hash_fn(const char* s) {
    unsigned int h = 5381;
    while (*s)
        h = ((h << 5) + h) + (unsigned char)(*s++);
    return h % HASH_SIZE;
}

// ────────────────────────────────────────────────────────────
//  int    → 4 bytes  (standard 32-bit integer)
//  float  → 4 bytes  (standard 32-bit float)
//  char   → 1 byte   (single character)
//  string → 8 bytes  (pointer — 64-bit address)
//  bool   → 1 byte   (true/false fits in one byte)
//  void   → 0 bytes  (no storage — return type only)
//  entity → 8 bytes  (pointer to object on heap — 64-bit)
// ────────────────────────────────────────────────────────────
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


SymTable* create_scope(ScopeKind kind, const char* name, SymTable* parent) {
    SymTable* t    = calloc(1, sizeof(SymTable)); // zero-initialized
    t->kind        = kind;
    t->level       = parent ? parent->level + 1 : 0;
    t->parent      = parent;
    t->next_offset = 0;     // Important field: fresh offset counter for this scope
    strncpy(t->name, name, 63);
    return t;
}


void add_param(ParamNode** list, int* count, const char* name, DataType dt) {
    ParamNode* p = calloc(1, sizeof(ParamNode));
    strncpy(p->name, name, 63);
    p->datatype = dt;
    p->next     = NULL;

if (!*list) {
        *list = p;        // list was empty — this node IS the list
    } else {
        ParamNode* cur = *list;
        while (cur->next) cur = cur->next;  // walk to the last node
        cur->next = p;                       // append at the end
    }
    (*count)++;   // increment the counter
}


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

//for checking the access modifier

void semantic_error(const char *msg){
	printf("[Error]: %s\n",msg);
	exit(1);
}

void check_field_access(char* obj_name, char* field_name, int lineno) {
    char msg[256];

    // 1. lookup object variable
    Symbol* obj = lookup(current_scope, obj_name);
    if (!obj) {
        snprintf(msg, sizeof(msg),
                 "line %d: Undeclared object '%s'", lineno, obj_name);
        semantic_error(msg);
        return;
    }

    // 2. get class name from object
    char* class_name = obj->attr.object.entity_name;

    // 3. lookup class symbol
    Symbol* cls = lookup(global_scope, class_name);
    if (!cls || cls->kind != KIND_ENTITY) {
        snprintf(msg, sizeof(msg),
                 "line %d: Class '%s' not found", lineno, class_name);
        semantic_error(msg);
        return;
    }

    // 4. find field inside class
    NameNode* f = cls->attr.entity.fields_list;
    Symbol* field_sym = NULL;

    while (f) {
        if (strcmp(f->name, field_name) == 0) {
            field_sym = lookup(cls->attr.entity.scope, field_name);
            break;
        }
        f = f->next;
    }

    if (!field_sym || field_sym->kind != KIND_FIELD) {
        snprintf(msg, sizeof(msg),
                 "line %d: Field '%s' not found in class '%s'",
                 lineno, field_name, class_name);
        semantic_error(msg);
        return;
    }

    // 5. check private access
    if (field_sym->attr.field.access == ACC_PRIVATE) {
        if (strcmp(current_scope->name, class_name) != 0) {
            snprintf(msg, sizeof(msg),
                     "line %d: Private field '%s' of class '%s' is not accessible",
                     lineno, field_name, class_name);
            semantic_error(msg);
        }
    }
}

void check_method_access(char* obj_name, char* method_name, int lineno) {
    char msg[256];

    // 1. lookup object variable
    Symbol* obj = lookup(current_scope, obj_name);
    if (!obj) {
        snprintf(msg, sizeof(msg),
                 "line %d: Undeclared object '%s'", lineno, obj_name);
        semantic_error(msg);
        return;
    }

    // 2. get class name
    char* class_name = obj->attr.object.entity_name;

    // 3. lookup class symbol
    Symbol* cls = lookup(global_scope, class_name);
    if (!cls || cls->kind != KIND_ENTITY) {
        snprintf(msg, sizeof(msg),
                 "line %d: Class '%s' not found", lineno, class_name);
        semantic_error(msg);
        return;
    }

    // 4. find method inside class
    NameNode* m = cls->attr.entity.methods_list;
    Symbol* method_sym = NULL;

    while (m) {
        if (strcmp(m->name, method_name) == 0) {
            method_sym = lookup(cls->attr.entity.scope, method_name);
            break;
        }
        m = m->next;
    }

    if (!method_sym || method_sym->kind != KIND_METHOD) {
        snprintf(msg, sizeof(msg),
                 "line %d: Method '%s' not found in class '%s'",
                 lineno, method_name, class_name);
        semantic_error(msg);
        return;
    }

    // 5. check private access
    if (method_sym->attr.method.access == ACC_PRIVATE) {
        if (strcmp(current_scope->name, class_name) != 0) {
            snprintf(msg, sizeof(msg),
                     "line %d: Private method '%s' of class '%s' is not accessible",
                     lineno, method_name, class_name);
            semantic_error(msg);
        }
    }
}

	


Symbol* insert_symbol(SymTable* tbl, const char* name,
                      SymKind kind, DataType dt, int line) {

    // Only check the CURRENT scope, not parent scopes.
    // It's valid to have  int x;  at global level AND  int x;
    // inside a function — that's shadowing, not redeclaration.
    if (lookup_local(tbl, name)) {
        fprintf(stderr,
            "ERROR line %d: '%s' already declared in scope '%s'\n",
            line, name, tbl->name);
        return NULL;
    }

    Symbol* sym      = calloc(1, sizeof(Symbol));  // zero everything
    strncpy(sym->name, name, 63);
    sym->kind        = kind;
    sym->datatype    = dt;
    sym->scope_level = tbl->level;
    sym->size        = datatype_size(dt);
    if(strcmp(sym->name, "x") == 0){
	    printf("Found that the current offset is %d\n", tbl->next_offset);
    }
    sym->offset      = tbl->next_offset;
    sym->is_initialized = 0;

    /*if (kind == KIND_VAR   || kind == KIND_PARAM ||
        kind == KIND_ARRAY || kind == KIND_FIELD)*/
    //tbl->next_offset += sym->size;
    //sym->is_initialized = 0;   // default: not initialized

    switch (kind) {
        case KIND_VAR:
        case KIND_PARAM:
        case KIND_FIELD:
        case KIND_ARRAY:     /* corrected to full size by parser later */
            sym->size = datatype_size(dt);
            break;
        case KIND_ENTITY:
	case KIND_OBJECT:
	    sym->size = 0; // for now later we update
	    break;
        case KIND_FUNCTION:
        case KIND_METHOD:
        case KIND_CONSTRUCTOR:
        case KIND_FOR:
        case KIND_IF:
        case KIND_ELIF:
        case KIND_ELSE:
	    sym->size = 0;
	    break;
        default:
            sym->size = 0;   /* size set/updated by parser after body parsed */
            break;
    }

    sym->offset = tbl->next_offset;

    if(kind == KIND_VAR || kind == KIND_PARAM || kind == KIND_FIELD){
	    tbl->next_offset += sym->size; 
    }	// advance by exact byte count
    // KIND_ARRAY  → parser does: current_scope->next_offset = sym->offset + sym->size
    // Everything else → next_offset stays, offset stays as a marker only

    // Compute which bucket this name belongs to.
    // Insert at the FRONT of the bucket's linked list (O(1)).
    unsigned int h  = hash_fn(name);
    sym->next       = tbl->buckets[h];  // new node points to old head
    tbl->buckets[h] = sym;              // new node becomes new head
    tbl->symbol_count++;

    return sym;
}


Symbol* lookup_local(SymTable* tbl, const char* name) {
    unsigned int h = hash_fn(name);
    for (Symbol* s = tbl->buckets[h]; s; s = s->next)
        if (strcmp(s->name, name) == 0) return s;
    return NULL;
}

Symbol* require_declared(SymTable* scope, const char* name, int lineno){
	Symbol* sym = lookup(scope, name);
	if(!sym){
		char buf[256];
		fprintf(stderr, "ERROR line %d: '%s' used but not declared.\n", lineno, name);
	}
	return sym;
}


Symbol* lookup(SymTable* tbl, const char* name) {
    for (SymTable* t = tbl; t; t = t->parent) {
        Symbol* s = lookup_local(t, name);
        if (s) return s;
    }
    return NULL;
}



// String arrays for printing enum values as text
static const char* kind_names[] = {
    "VAR","ARRAY","FUNCTION","METHOD",
    "FIELD","ENTITY","CONSTRUCTOR","PARAM",
    "FOR","IF","ELIF","ELSE", "OBJECT"
};

const char* dt_names[] = {
    "int","float","char","string","bool",
    "void","entity", "object", "unknown"
};

static const char* acc_names[] = { "---", "public", "private" };

// Convert ScopeKind enum to a display string
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

    // Collect all symbols from all 64 hash buckets into a flat array
    // (hash table doesn't store in insertion order — but that's fine)
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
        char extra[512] = "";  // build the EXTRA column text here

        switch (s->kind) {

            case KIND_FUNCTION: {
                // Show: return type, parameter list, entry label
                char params[256] = "";
                for (ParamNode* p = s->attr.func.param_list; p; p = p->next) {
                    char tmp[80];
                    snprintf(tmp, sizeof(tmp), "%s:%s", p->name, dt_names[p->datatype]);
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
                // Show: access modifier, class name, return type, param list
                char params[256] = "";
                for (ParamNode* p = s->attr.method.param_list; p; p = p->next) {
                    char tmp[80];
                    snprintf(tmp, sizeof(tmp), "%s:%s", p->name, dt_names[p->datatype]);
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
                // Show: access modifier and class name
                snprintf(extra, 512, "%s cls=%s",
                         acc_names[s->attr.field.access],
                         s->attr.field.belongs_to);
                break;

            case KIND_ARRAY:
                // Show: dimensions, sizes, initialized flag
                snprintf(extra, 512, "dims=%d [%d][%d] init=%d",
                        s->attr.array.dimensions,
                        s->attr.array.dim1,
                        s->attr.array.dim2,
                        s->attr.array.is_initialized);
                break;

            case KIND_ENTITY: {
                // Show: class size, list of field names, method names, constructor names
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
                // Show: class name, param count, entry label
                snprintf(extra, 512, "cls=%s params=%d lbl=%s",
                         s->attr.ctor.belongs_to,
                         s->attr.ctor.param_count,
                         s->attr.ctor.entry_label);
                break;
	    case KIND_OBJECT:
    		snprintf(extra, 512, "dt=%s class=%s size=%d offset=%d",
             		dt_names[s->datatype],
             		s->attr.object.entity_name,
             		s->size,
             		s->offset);
    		break;
            case KIND_PARAM:
            case KIND_VAR:
                // Show: whether it was initialized at declaration
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

        // Truncate extra to 30 chars for the table column
        char extra_short[32];
        snprintf(extra_short, 32, "%.30s", extra);

        printf("| %-12s | %-11s | %-7s | %-5d | %-4d | %-8s |\n",
               s->name,
               kind_names[s->kind],
               dt_names[s->datatype],
               s->size,
               s->offset,
               extra_short);

        // If extra is too long for the column, print it on a second line
        if (strlen(extra) > 30) {
            printf("|   >> %-59s|\n", extra);
        }

        if (s->kind == KIND_ARRAY && s->attr.array.is_initialized && s->attr.array.init_count > 0) {
            printf("|   >> init_vals: ");
            for (int vi = 0; vi < s->attr.array.init_count; vi++)
                printf("[%d]=%s ", vi, s->attr.array.init_values[vi]);
            printf("\n");
        }
    }
    printf("+--------------+-------------+---------+-------+------+----------+\n");
}
