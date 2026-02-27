// ============================================================
// symtab.h  —  Symbol Table Header
// ============================================================
// This file defines ALL the data structures and declares ALL
// the functions that the symbol table uses.
// It is included by both symtab.c and parser.y
// ============================================================

#ifndef SYMTAB_H   // include guard — prevents double-inclusion
#define SYMTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ────────────────────────────────────────────────────────────
//  SECTION 1: ENUMERATIONS
//  Enums give human-readable names to integer constants.
//  The compiler replaces them with 0,1,2,3... internally.
// ────────────────────────────────────────────────────────────

// SymKind — what KIND of symbol is this table entry?
// Every symbol in the table has exactly one of these.
typedef enum {
    KIND_VAR,         // 0 — a plain variable:        int x;
    KIND_ARRAY,       // 1 — an array:                int[] arr[10];
    KIND_FUNCTION,    // 2 — a free function:          int func add(...)
    KIND_METHOD,      // 3 — a method inside a class:  public int func foo()
    KIND_FIELD,       // 4 — a data field in a class:  private int age;
    KIND_ENTITY,      // 5 — a class definition:       entity Dog { ... }
    KIND_CONSTRUCTOR, // 6 — a constructor:            Dog(int a, string n)
    KIND_PARAM,       // 7 — a function/method param:  int x  in  func f(int x)
    KIND_FOR,         // 8 — a for loop construct
    KIND_IF,          // 9 — an if statement construct
    KIND_ELIF,        // 10 — an elif branch
    KIND_ELSE         // 11 — an else branch
} SymKind;

// DataType — the BASE DATA TYPE of a symbol
// Used to know how many bytes to allocate and for type checking.
typedef enum {
    DT_INT,      // 0 — int      → 4 bytes
    DT_FLOAT,    // 1 — float    → 4 bytes
    DT_CHAR,     // 2 — char     → 1 byte
    DT_STRING,   // 3 — string   → 8 bytes (pointer)
    DT_BOOL,     // 4 — bool     → 1 byte
    DT_VOID,     // 5 — void     → 0 bytes (functions only)
    DT_ENTITY,   // 6 — class    → 8 bytes (pointer to object)
    DT_UNKNOWN   // 7 — error / not yet resolved
} DataType;

// AccessMod — access modifier for class members
// Only meaningful for KIND_FIELD and KIND_METHOD
typedef enum {
    ACC_NONE,    // 0 — not inside a class (irrelevant)
    ACC_PUBLIC,  // 1 — accessible from outside the class
    ACC_PRIVATE  // 2 — only accessible from inside the class
} AccessMod;

// ────────────────────────────────────────────────────────────
//  SECTION 2: LINKED LIST NODES
//  Used to store lists inside a symbol
//  (e.g. a function's parameter list, a class's field names)
// ────────────────────────────────────────────────────────────

// ParamNode — one node in a function/method parameter list
// Each parameter has a name and a data type.
// Example:  func add(int x, int y)
//   → ParamNode { name="x", datatype=DT_INT } → ParamNode { name="y", datatype=DT_INT } → NULL
typedef struct ParamNode {
    char              name[64];   // parameter name, e.g. "x"
    DataType          datatype;   // parameter type, e.g. DT_INT
    struct ParamNode* next;       // pointer to next param (linked list)
} ParamNode;

// NameNode — one node in a simple name list
// Used inside EntityAttr to store lists of field names, method names etc.
// Example: entity Dog has fields "age" and "name"
//   → NameNode { name="age" } → NameNode { name="name" } → NULL
typedef struct NameNode {
    char             name[64];   // e.g. "age", "bark"
    struct NameNode* next;       // pointer to next name
} NameNode;

// ────────────────────────────────────────────────────────────
//  SECTION 3: CONSTRUCT-SPECIFIC ATTRIBUTE STRUCTS
//  Each kind of symbol needs different extra information.
//  These structs hold that extra info.
//  Only ONE of them is active at a time (stored in a union).
// ────────────────────────────────────────────────────────────

// ArrayAttr — extra info for KIND_ARRAY symbols
typedef struct {
    int is_initialized; // 1 if declared with initializer:  int[] arr = {1,2,3};
                        // 0 if declared with size:         int[] arr[10];
    int dimensions;     // 1 for 1-D array, 2 for 2-D array
    int dim1;           // size of first dimension,  e.g. 10 in arr[10]
    int dim2;           // size of second dimension, e.g. 5  in arr[10][5], 0 if 1-D
} ArrayAttr;

// FuncAttr — extra info for KIND_FUNCTION symbols
typedef struct {
    DataType   return_type;      // what type this function returns, e.g. DT_INT
    ParamNode* param_list;       // linked list of parameters
    int        param_count;      // how many parameters (length of param_list)
    char       entry_label[32];  // assembly label for this function, e.g. "func_add"
} FuncAttr;

// MethodAttr — extra info for KIND_METHOD symbols
// Same as FuncAttr but also knows which class it belongs to and its access level
typedef struct {
    DataType   return_type;      // return type of this method
    ParamNode* param_list;       // parameter list
    int        param_count;      // number of parameters
    char       entry_label[32];  // assembly label, e.g. "Dog_bark"
    char       belongs_to[64];   // name of the class that owns this method, e.g. "Dog"
    AccessMod  access;           // ACC_PUBLIC or ACC_PRIVATE
} MethodAttr;

// CtorAttr — extra info for KIND_CONSTRUCTOR symbols
typedef struct {
    ParamNode* param_list;       // parameter list
    int        param_count;      // number of parameters
    char       belongs_to[64];   // name of the class, e.g. "Dog"
    char       entry_label[32];  // assembly label, e.g. "ctor_Dog"
} CtorAttr;

// FieldAttr — extra info for KIND_FIELD symbols (class data members)
typedef struct {
    char      belongs_to[64];   // name of the class that owns this field, e.g. "Dog"
    AccessMod access;           // ACC_PUBLIC or ACC_PRIVATE
} FieldAttr;

// EntityAttr — extra info for KIND_ENTITY symbols (the class itself)
typedef struct {
    char      class_name[64];        // name of the class, e.g. "Dog"
    NameNode* fields_list;           // linked list of all field names in this class
    NameNode* methods_list;          // linked list of all method names in this class
    NameNode* constructors_list;     // linked list of all constructor names
    int       class_size;            // total bytes needed for ONE object instance
                                     // = sum of all field sizes
    char      parent_class[64];      // name of parent class if inheritance (stretch goal)
                                     // empty string "" if no parent
} EntityAttr;

// ForAttr — extra info for KIND_FOR symbols
typedef struct {
    char start_label[32];  // label at the start of the loop (condition check)
    char end_label[32];    // label after the loop ends
    char cond_label[32];   // label for the condition expression
} ForAttr;

// IfAttr — extra info for KIND_IF and KIND_ELIF symbols
typedef struct {
    char false_label[32];  // label to jump to when condition is false
    char end_label[32];    // label at the very end of the entire if/elif/else chain
    int  branch_index;     // 0=if, 1=first elif, 2=second elif, etc.
} IfAttr;

// ────────────────────────────────────────────────────────────
//  SECTION 4: THE SYMBOL STRUCT
//  This is ONE ROW in the symbol table.
//  Every identifier in the source code becomes one Symbol.
// ────────────────────────────────────────────────────────────

typedef struct Symbol {
    // ── COMMON FIELDS — every symbol has all of these ──────
    char      name[64];       // the identifier name as written in source, e.g. "x", "Dog", "add"
    SymKind   kind;           // what kind: VAR, FUNCTION, ENTITY, etc.
    DataType  datatype;       // base type: int, float, entity, etc.
    int       scope_level;    // nesting depth: 0=global, 1=inside function, 2=inside if, etc.
    int       size;           // bytes this symbol occupies in memory
                              //   int  → 4,  float → 4,  char → 1
                              //   int[10] → 40,  int[3][4] → 48
                              //   function → frame size (set after parsing body)
    int       offset;         // byte offset FROM THE START OF THE ENCLOSING SCOPE
                              //   e.g. in  func add(int x, int y) { int result; }
                              //   x      → offset 0   (first param,  4 bytes)
                              //   y      → offset 4   (second param, 4 bytes)
                              //   result → offset 8   (local var,    4 bytes)
                              //   This is used to generate assembly instructions like:
                              //   [bp+0] = x,  [bp+4] = y,  [bp+8] = result
    int       is_initialized; // 1 if assigned a value at declaration time
                              //   int a = 5;  → is_initialized = 1
                              //   int a;      → is_initialized = 0

    // ── CONSTRUCT-SPECIFIC FIELDS — only ONE active at a time ──
    // A union shares the same memory block for all members.
    // If kind==KIND_FUNCTION, use attr.func
    // If kind==KIND_ARRAY,    use attr.array
    // If kind==KIND_ENTITY,   use attr.entity   etc.
    union {
        ArrayAttr  array;    // used when kind == KIND_ARRAY
        FuncAttr   func;     // used when kind == KIND_FUNCTION
        MethodAttr method;   // used when kind == KIND_METHOD
        CtorAttr   ctor;     // used when kind == KIND_CONSTRUCTOR
        FieldAttr  field;    // used when kind == KIND_FIELD
        EntityAttr entity;   // used when kind == KIND_ENTITY
        ForAttr    forstmt;  // used when kind == KIND_FOR
        IfAttr     ifstmt;   // used when kind == KIND_IF or KIND_ELIF
    } attr;

    // Linked list pointer — used internally by the hash table
    // When two symbols hash to the same bucket, they chain together
    struct Symbol* next;
} Symbol;

// ────────────────────────────────────────────────────────────
//  SECTION 5: THE SCOPE TABLE STRUCT
//  One SymTable = one scope (one level of nesting).
//  Scopes are arranged in a tree via the parent pointer.
//
//  Example for:
//    int func add(int x) { int y; }
//
//  global scope
//    └── add's function scope  (parent = global)
//          └── block scope     (parent = add's scope)
// ────────────────────────────────────────────────────────────

// ScopeKind — what type of scope is this?
typedef enum {
    SCOPE_GLOBAL,       // the outermost scope — functions and entities live here
    SCOPE_FUNCTION,     // inside a function body
    SCOPE_METHOD,       // inside a class method body
    SCOPE_CONSTRUCTOR,  // inside a constructor body
    SCOPE_ENTITY,       // inside a class definition (fields, methods go here)
    SCOPE_BLOCK,        // anonymous block  { ... }  inside a function
    SCOPE_FOR,          // inside a for loop
    SCOPE_IF,           // inside an if condition
    SCOPE_ELIF,         // inside an elif condition
    SCOPE_ELSE          // inside an else block
} ScopeKind;

// HASH_SIZE — number of buckets in the hash table
// 64 buckets means up to 64 different hash values.
// Symbols that hash to the same bucket form a linked list.
#define HASH_SIZE 64

typedef struct SymTable {
    ScopeKind        kind;              // what kind of scope this is
    char             name[64];          // human-readable name, e.g. "global", "add", "Dog"
    int              level;             // nesting depth: global=0, function=1, block inside function=2
    Symbol*          buckets[HASH_SIZE];// the hash table — array of 64 linked list heads
                                        // to find a symbol: compute hash → look in that bucket
    int              symbol_count;      // total number of symbols in this scope
    int              next_offset;       // THE OFFSET COUNTER
                                        // starts at 0 when scope is created
                                        // advances by each symbol's size as symbols are inserted
                                        // at end of scope = total bytes needed (frame size)
    struct SymTable* parent;            // pointer to the enclosing scope
                                        // NULL only for the global scope
} SymTable;

// ────────────────────────────────────────────────────────────
//  SECTION 6: FUNCTION DECLARATIONS (the API)
//  These are implemented in symtab.c
// ────────────────────────────────────────────────────────────

SymTable* create_scope  (ScopeKind kind, const char* name, SymTable* parent);
                         // allocates and initializes a new SymTable
                         // sets level = parent->level + 1 (or 0 for global)
                         // sets next_offset = 0

Symbol*   insert_symbol (SymTable* tbl, const char* name, SymKind kind,
                         DataType dt, int line);
                         // creates a new Symbol and adds it to tbl
                         // sets offset = tbl->next_offset  THEN advances next_offset by size
                         // returns NULL if name already exists (redeclaration error)

Symbol*   lookup_local  (SymTable* tbl, const char* name);
                         // searches ONLY in tbl (current scope)
                         // returns the Symbol if found, NULL if not

Symbol*   lookup        (SymTable* tbl, const char* name);
                         // searches tbl, then tbl->parent, then tbl->parent->parent etc.
                         // walks the entire scope chain upward
                         // returns the first match found, NULL if nowhere

int       datatype_size (DataType dt);
                         // returns byte size: DT_INT→4, DT_CHAR→1, etc.

void      add_param     (ParamNode** list, int* count, const char* name, DataType dt);
                         // appends a new ParamNode to the end of *list
                         // increments *count

void      add_name      (NameNode** list, const char* name);
                         // appends a new NameNode to the end of *list

void      print_table   (SymTable* tbl);
                         // prints the scope table in a formatted box

// ────────────────────────────────────────────────────────────
//  SECTION 7: GLOBAL VARIABLES
//  Declared here, defined in symtab.c
// ────────────────────────────────────────────────────────────

extern SymTable* global_scope;    // points to the top-level (global) scope
                                  // created in main() before parsing starts

extern SymTable* current_scope;   // points to whatever scope we are currently inside
                                  // changes as we enter/leave functions, blocks, etc.
                                  // parser actions always use current_scope

extern DataType  current_decl_type; // NOT USED ANYMORE — kept for compatibility
                                    // was used for mid-rule actions (now removed)

extern const char* dt_names[];
#endif /* SYMTAB_H */
