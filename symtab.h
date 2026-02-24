// symtab.h
#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ─────────────────────────────────────────
//  ENUMS
// ─────────────────────────────────────────

// "type" field — what kind of construct is this symbol
typedef enum {
    KIND_VAR,
    KIND_ARRAY,
    KIND_FUNCTION,
    KIND_METHOD,
    KIND_FIELD,        // variable inside a class
    KIND_ENTITY,       // the class itself
    KIND_CONSTRUCTOR,
    KIND_PARAM
} SymKind;

// "datatype" field — the base data type
typedef enum {
    DT_INT,
    DT_FLOAT,
    DT_CHAR,
    DT_STRING,
    DT_BOOL,
    DT_VOID,
    DT_ENTITY,         // user-defined class type
    DT_UNKNOWN
} DataType;

// access modifier — only for methods and fields
typedef enum {
    ACC_NONE,
    ACC_PUBLIC,
    ACC_PRIVATE
} AccessMod;

// ─────────────────────────────────────────
//  PARAMETER NODE (for function param list)
// ─────────────────────────────────────────

typedef struct ParamNode {
    char          name[64];
    DataType      datatype;
    struct ParamNode* next;
} ParamNode;

// ─────────────────────────────────────────
//  NAME LIST NODE
//  used for fields list, methods list,
//  constructors list inside EntityInfo
// ─────────────────────────────────────────

typedef struct NameNode {
    char name[64];
    struct NameNode* next;
} NameNode;

// ─────────────────────────────────────────
//  CONSTRUCT-SPECIFIC ATTRIBUTE BLOCKS
// ─────────────────────────────────────────

// ── Array ────────────────────────────────
typedef struct {
    int is_initialized;   // 1 if has initializer list
    int dimensions;       // 1 or 2
    int dim1;             // size of dimension 1
    int dim2;             // size of dimension 2 (0 if 1-D)
} ArrayAttr;

// ── Function ─────────────────────────────
typedef struct {
    DataType   return_type;
    ParamNode* param_list;       // linked list of {name, type}
    int        param_count;
    char       entry_label[32];  // e.g. "func_add" for assembly
} FuncAttr;

// ── Method (extends FuncAttr with class info) ──
typedef struct {
    DataType   return_type;
    ParamNode* param_list;
    int        param_count;
    char       entry_label[32];
    char       belongs_to[64];   // class name
    AccessMod  access;
} MethodAttr;

// ── Constructor ──────────────────────────
typedef struct {
    ParamNode* param_list;
    int        param_count;
    char       belongs_to[64];
    char       entry_label[32];
} CtorAttr;

// ── Class Field (variable inside class) ──
typedef struct {
    char      belongs_to[64];
    AccessMod access;
} FieldAttr;

// ── Entity (the class itself) ────────────
typedef struct {
    char      class_name[64];
    NameNode* fields_list;        // names of fields
    NameNode* methods_list;       // names of methods
    NameNode* constructors_list;  // names of constructors
    int       class_size;         // total bytes for one instance
    char      parent_class[64];   // "" if no inheritance
} EntityAttr;

// ─────────────────────────────────────────
//  THE SYMBOL — one row in the table
// ─────────────────────────────────────────

typedef struct Symbol {
    // ── Common fields (every symbol has these) ──
    char      name[64];      // identifier name
    SymKind   kind;          // VAR, FUNCTION, ENTITY, etc.
    DataType  datatype;      // INT, FLOAT, ENTITY, etc.
    int       scope_level;   // 0=global, 1=function/class, 2=nested
    int       size;          // bytes this symbol occupies
    int       offset;  
    int var_initialized;

    // ── Construct-specific (only one active at a time) ──
    union {
        ArrayAttr  array;
        FuncAttr   func;
        MethodAttr method;
        CtorAttr   ctor;
        FieldAttr  field;
        EntityAttr entity;
    } attr;

    struct Symbol* next;     // chain in hash bucket
} Symbol;

// ─────────────────────────────────────────
//  SCOPE TABLE NODE
// ─────────────────────────────────────────

typedef enum {
    SCOPE_GLOBAL,
    SCOPE_FUNCTION,
    SCOPE_METHOD,
    SCOPE_CONSTRUCTOR,
    SCOPE_ENTITY,
    SCOPE_BLOCK
} ScopeKind;

#define HASH_SIZE 64

typedef struct SymTable {
    ScopeKind     kind;
    char          name[64];       // e.g. "global", "add", "Dog"
    int           level;          // nesting depth
    Symbol*       buckets[HASH_SIZE];
    int           symbol_count;
    int           next_offset;    // for assigning offsets
    struct SymTable* parent;
} SymTable;

// ─────────────────────────────────────────
//  API
// ─────────────────────────────────────────

SymTable* create_scope  (ScopeKind kind, const char* name, SymTable* parent);
Symbol*   insert_symbol (SymTable* tbl, const char* name, SymKind kind,
                         DataType dt, int line);
Symbol*   lookup_local  (SymTable* tbl, const char* name);
Symbol*   lookup        (SymTable* tbl, const char* name);

int       datatype_size (DataType dt);
void      add_param     (ParamNode** list, int* count,
                         const char* name, DataType dt);
void      add_name      (NameNode** list, const char* name);

void      print_table   (SymTable* tbl);
void      print_all     (SymTable* tbl);   // prints this scope + all children

// These are set by parser actions, declared here so symtab.c can use them
extern SymTable* global_scope;
extern SymTable* current_scope;

#endif
