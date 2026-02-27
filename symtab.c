// ============================================================
// symtab.c  —  Symbol Table Implementation
// ============================================================
// Contains the actual code for every function declared in
// symtab.h.  The parser calls these functions during parsing.
// ============================================================

#include "symtab.h"

// ────────────────────────────────────────────────────────────
//  GLOBAL VARIABLE DEFINITIONS
//  (declared extern in symtab.h, defined here)
// ────────────────────────────────────────────────────────────

SymTable* global_scope      = NULL;       // set in main() before yyparse()
SymTable* current_scope     = NULL;       // updated by parser as scopes open/close
DataType  current_decl_type = DT_UNKNOWN; // legacy — no longer used actively

// ────────────────────────────────────────────────────────────
//  SECTION 1: HASH FUNCTION
//
//  Purpose: given a symbol name (string), produce a number
//  0..63 that decides which bucket to put it in.
//
//  Algorithm: djb2 — simple, fast, good distribution.
//  h starts at 5381 (magic seed).
//  For each character c:  h = h*33 + c
//  (h << 5) + h  is the same as h*33 but faster.
//  Finally mod HASH_SIZE (64) to get a bucket index.
//
//  Example:
//    "x"   → some number mod 64 → e.g. 7
//    "age" → different number   → e.g. 42
// ────────────────────────────────────────────────────────────
static unsigned int hash_fn(const char* s) {
    unsigned int h = 5381;
    while (*s)
        h = ((h << 5) + h) + (unsigned char)(*s++);
    return h % HASH_SIZE;
}

// ────────────────────────────────────────────────────────────
//  SECTION 2: datatype_size
//
//  Purpose: given a DataType enum value, return how many
//  bytes that type occupies in memory.
//
//  Used by insert_symbol to set sym->size, and by the parser
//  when computing array sizes.
//
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
        case DT_ENTITY: return 8;
        default:        return 0;
    }
}

// ────────────────────────────────────────────────────────────
//  SECTION 3: create_scope
//
//  Purpose: create a brand new, empty scope table.
//  Called every time we enter a new lexical scope:
//    - entering a function body
//    - entering a class definition
//    - entering an if/for/block etc.
//
//  Parameters:
//    kind   — what type of scope (SCOPE_FUNCTION, SCOPE_IF etc.)
//    name   — human-readable name (function name, "block", etc.)
//    parent — the enclosing scope (NULL only for global)
//
//  What it does:
//    calloc(1, sizeof(SymTable)) — allocates zeroed memory
//    level = parent->level + 1  — nesting depth increases by 1
//    next_offset = 0            — offset counter starts fresh at 0
//    parent pointer stored      — so lookup() can walk upward
// ────────────────────────────────────────────────────────────
SymTable* create_scope(ScopeKind kind, const char* name, SymTable* parent) {
    SymTable* t    = calloc(1, sizeof(SymTable)); // zero-initialized
    t->kind        = kind;
    t->level       = parent ? parent->level + 1 : 0;
    t->parent      = parent;
    t->next_offset = 0;     // IMPORTANT: fresh offset counter for this scope
    strncpy(t->name, name, 63);
    return t;
}

// ────────────────────────────────────────────────────────────
//  SECTION 4: add_param
//
//  Purpose: add one parameter to the end of a parameter list.
//  Called from the parser's param rule when a function/method
//  parameter is encountered.
//
//  Example:  func add(int x, int y)
//    First call:  add_param(&list, &count, "x", DT_INT)
//    Second call: add_param(&list, &count, "y", DT_INT)
//    Result: list → {name="x",DT_INT} → {name="y",DT_INT} → NULL
//            count = 2
//
//  Uses a walk-to-end loop to append at the TAIL (preserves order).
// ────────────────────────────────────────────────────────────
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

// ────────────────────────────────────────────────────────────
//  SECTION 5: add_name
//
//  Purpose: add a name string to the end of a NameNode list.
//  Used to build a class's lists of field names, method names,
//  and constructor names inside EntityAttr.
//
//  Example: entity Dog { private int age; public string name; }
//    add_name(&fields_list, "age")
//    add_name(&fields_list, "name")
//    Result: fields_list → {name="age"} → {name="name"} → NULL
// ────────────────────────────────────────────────────────────
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

void check_field_access(char* obj_name, char* field_name) {
    // 1. lookup the object variable
    Symbol* obj = lookup(current_scope, obj_name);
    if (!obj)
        semantic_error("Undeclared object");

    // 2. lookup the field symbol means search in all scopes
    Symbol* field = lookup(global_scope, field_name);
    if (!field || field->kind != KIND_FIELD)
        semantic_error("Field not found");

    // 3. check private access
    if (field->attr.field.access == ACC_PRIVATE) {
        // current_scope->name holds current function/method/class name
        if (strcmp(current_scope->name, field->attr.field.belongs_to) != 0) {
            semantic_error("Private field access is not allowed");
        }
    }
}

void check_method_access(char* obj_name, char* method_name) {
    // 1. lookup the object variable
    Symbol* obj = lookup(current_scope, obj_name);
    if (!obj)
        semantic_error("Undeclared object");

    // 2. lookup the method symbol
    Symbol* method = lookup(global_scope, method_name);
    if (!method || method->kind != KIND_METHOD)
        semantic_error("Method not found");

    // 3. check private access
    if (method->attr.method.access == ACC_PRIVATE) {
        if (strcmp(current_scope->name, method->attr.method.belongs_to) != 0) {
            semantic_error("Private method access is not allowed");
        }
    }
}
// ────────────────────────────────────────────────────────────
//  SECTION 6: insert_symbol
//
//  Purpose: create a new Symbol and insert it into a scope.
//  Called from parser actions whenever a new identifier is
//  declared (variable, func/tion, parameter, field, etc.)
//
//  ── OFFSET CALCULATION (THE KEY LOGIC) ──────────────────
//
//  Offset means: how many bytes from the START of this scope's
//  memory block is this symbol located?
//
//  The scope's next_offset counter tracks the next available
//  byte position.  Each time we insert a symbol:
//    1. sym->offset = tbl->next_offset   (assign current position)
//    2. tbl->next_offset += sym->size    (advance past this symbol)
//
//  Example: func add(int x, int y) { int result; }
//
//    insert "x"      (KIND_PARAM, DT_INT, size=4)
//      sym->offset = 0          next_offset was 0
//      next_offset becomes 4
//
//    insert "y"      (KIND_PARAM, DT_INT, size=4)
//      sym->offset = 4          next_offset was 4
//      next_offset becomes 8
//
//    insert "result" (KIND_VAR,   DT_INT, size=4)
//      sym->offset = 8          next_offset was 8
//      next_offset becomes 12
//
//  Final next_offset = 12 = total frame size for this function.
//
//  ── WHICH KINDS ADVANCE THE OFFSET? ────────────────────
//
//  Only symbols that actually occupy MEMORY advance the offset:
//    KIND_VAR    — local variable, takes space in stack frame
//    KIND_PARAM  — parameter, takes space in stack frame
//    KIND_FIELD  — class field, takes space in object layout
//
//  The following do NOT advance the offset:
//    KIND_FUNCTION    — functions have their own separate scope
//    KIND_METHOD      — same — they don't live inside the caller
//    KIND_ENTITY      — class definition, not a memory location
//    KIND_CONSTRUCTOR — same as function
//    KIND_FOR         — control flow, not a memory allocation
//    KIND_IF/ELIF     — same
//
//  ── ARRAYS ARE SPECIAL (THE BUG FIX) ───────────────────
//
//  Arrays are NOT handled here — they are NOT in the if-condition
//  below.  Why?  Because when insert_symbol is called for an array,
//  we only know the element type (e.g. DT_INT = 4 bytes), but we
//  don't yet know the dimensions (those come from the grammar rule).
//
//  So for arrays:
//    insert_symbol is called → offset assigned, next_offset NOT advanced
//    Parser action then sets sym->size = element_size * rows * cols
//    Parser action then advances current_scope->next_offset += sym->size
//
//  This is why arrays are EXCLUDED from the offset-advancing condition.
//
//  ── REDECLARATION CHECK ─────────────────────────────────
//
//  Before inserting, we call lookup_local to check if this name
//  already exists in THIS scope (not parent scopes).
//  If it does, we print an error and return NULL.
//  The parser must handle NULL return gracefully.
// ────────────────────────────────────────────────────────────
Symbol* insert_symbol(SymTable* tbl, const char* name,
                      SymKind kind, DataType dt, int line) {

    // ── Step 1: Redeclaration check ─────────────────────────
    // Only check the CURRENT scope, not parent scopes.
    // It's valid to have  int x;  at global level AND  int x;
    // inside a function — that's shadowing, not redeclaration.
    if (lookup_local(tbl, name)) {
        fprintf(stderr,
            "ERROR line %d: '%s' already declared in scope '%s'\n",
            line, name, tbl->name);
        return NULL;
    }

    // ── Step 2: Allocate and fill the Symbol ────────────────
    Symbol* sym      = calloc(1, sizeof(Symbol));  // zero everything
    strncpy(sym->name, name, 63);
    sym->kind        = kind;
    sym->datatype    = dt;
    sym->scope_level = tbl->level;
    sym->is_initialized = 0;   // default: not initialized

    // ── Step 3: Assign size ─────────────────────────────────
    //
    //  SIZE RULES:
    //
    //  KIND_VAR / KIND_PARAM / KIND_FIELD:
    //    size = datatype_size(dt)   e.g. int→4, char→1
    //    This is the exact number of bytes the variable occupies.
    //
    //  KIND_ARRAY:
    //    size = datatype_size(dt) initially (one element).
    //    The parser overwrites this AFTER insert_symbol returns,
    //    once it knows dim1 (and dim2).
    //    e.g.  int[] arr[10]  →  size = 4*10 = 40
    //
    //  KIND_ENTITY (class definition in global scope):
    //    size = 0 now.
    //    After the entire class body is parsed, the parser sets:
    //      sym->size = entity_scope->next_offset
    //    which equals the sum of ALL field sizes inside the class.
    //    e.g. entity Dog { int age; char x; }  →  size = 4+1 = 5
    //
    //  KIND_FUNCTION / KIND_METHOD / KIND_CONSTRUCTOR:
    //    size = 0 always.
    //    Functions do not occupy a slot in the enclosing scope's
    //    memory layout — they have their own separate stack frame.
    //    (The function's OWN scope's next_offset becomes the frame
    //     size, stored back onto the function symbol after parsing.)
    //
    //  KIND_IF / KIND_ELIF / KIND_FOR:
    //    size = 0 — these are control-flow markers, not memory.

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
        default:
            sym->size = 0;   /* size set/updated by parser after body parsed */
            break;
    }

    // ── Step 4: Assign offset and advance counter ───────────
    // offset = byte position of this symbol inside the enclosing scope.
    // next_offset is advanced ONLY for symbols that genuinely occupy
    // memory in a stack frame (VAR, PARAM) or object layout (FIELD).
    //
    // ARRAY: offset assigned here but next_offset NOT yet advanced —
    //   the parser fixes the size then advances next_offset manually.
    //
    // ENTITY / FUNCTION / METHOD / CONSTRUCTOR / IF / FOR:
    //   offset = 0, next_offset NOT advanced — they occupy no slot
    //   in the enclosing scope's memory block.
    sym->offset = tbl->next_offset;

    if (kind == KIND_VAR   ||
        kind == KIND_PARAM ||
	kind == KIND_OBJECT||
        kind == KIND_FIELD) {
        tbl->next_offset += sym->size;  // advance by exact byte count
    }
    // KIND_ARRAY  → parser does: current_scope->next_offset = sym->offset + sym->size
    // Everything else → next_offset stays, offset stays as a marker only

    // ── Step 5: Insert into hash table ──────────────────────
    // Compute which bucket this name belongs to.
    // Insert at the FRONT of the bucket's linked list (O(1)).
    unsigned int h  = hash_fn(name);
    sym->next       = tbl->buckets[h];  // new node points to old head
    tbl->buckets[h] = sym;              // new node becomes new head
    tbl->symbol_count++;

    return sym;
}

// ────────────────────────────────────────────────────────────
//  SECTION 7: lookup_local
//
//  Purpose: search for a symbol by name in ONE scope only.
//  Does NOT look at parent scopes.
//
//  How it works:
//    1. Hash the name to find which bucket to look in.
//    2. Walk the linked list in that bucket.
//    3. Return the Symbol if name matches, NULL otherwise.
//
//  Used by:
//    - insert_symbol (to check for redeclaration)
//    - lookup (as a helper, called for each scope level)
// ────────────────────────────────────────────────────────────
Symbol* lookup_local(SymTable* tbl, const char* name) {
    unsigned int h = hash_fn(name);
    for (Symbol* s = tbl->buckets[h]; s; s = s->next)
        if (strcmp(s->name, name) == 0) return s;
    return NULL;
}

// ────────────────────────────────────────────────────────────
//  SECTION 8: lookup
//
//  Purpose: search for a symbol walking UP the scope chain.
//  This implements LEXICAL SCOPING — inner scopes can see
//  variables from outer scopes.
//
//  How it works:
//    Start at the given scope (usually current_scope).
//    Try lookup_local.  If found, return it.
//    If not found, move to parent scope and try again.
//    Keep going until we reach the global scope (parent=NULL).
//    If still not found, return NULL.
//
//  Example:
//    int x = 5;                     // in global scope
//    int func add(int a) {
//        int b = x + a;             // x is not in add's scope,
//    }                              // so lookup walks up to global and finds it
//
//  Used by the parser to:
//    - find the owning function/class when adding params
//    - check if a variable is declared before use
//    - find entity symbols to update their field/method counts
// ────────────────────────────────────────────────────────────
Symbol* lookup(SymTable* tbl, const char* name) {
    for (SymTable* t = tbl; t; t = t->parent) {
        Symbol* s = lookup_local(t, name);
        if (s) return s;
    }
    return NULL;
}

// ────────────────────────────────────────────────────────────
//  SECTION 9: print_table
//
//  Purpose: print all symbols in ONE scope as a formatted table.
//  Called by parser actions just before a scope is closed/popped.
//
//  Format:
//  +--------------+-------------+---------+-------+------+----------+
//  | NAME         | KIND        | DTYPE   | SIZE  | OFF  | EXTRA    |
//  +--------------+-------------+---------+-------+------+----------+
//  | x            | PARAM       | int     | 4     | 0    | init=0   |
//  | y            | PARAM       | int     | 4     | 4    | init=0   |
//  | result       | VAR         | int     | 4     | 8    | init=1   |
//  +--------------+-------------+---------+-------+------+----------+
//
//  EXTRA column shows kind-specific info (params list, access, etc.)
//  If extra is longer than 30 chars, a second line is printed.
// ────────────────────────────────────────────────────────────

// String arrays for printing enum values as text
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
    }
    printf("+--------------+-------------+---------+-------+------+----------+\n");
}
