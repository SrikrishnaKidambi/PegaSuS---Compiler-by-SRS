// symtab.h
#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ─────────────────────────────────────────
//  ENUMS
// ─────────────────────────────────────────

typedef enum {
    KIND_VAR,
    KIND_ARRAY,
    KIND_FUNCTION,
    KIND_METHOD,
    KIND_FIELD,
    KIND_ENTITY,
    KIND_CONSTRUCTOR,
    KIND_PARAM,
    KIND_FOR,
    KIND_IF,
    KIND_ELIF,
    KIND_ELSE
} SymKind;

typedef enum {
    DT_INT,
    DT_FLOAT,
    DT_CHAR,
    DT_STRING,
    DT_BOOL,
    DT_VOID,
    DT_ENTITY,
    DT_UNKNOWN
} DataType;

typedef enum {
    ACC_NONE,
    ACC_PUBLIC,
    ACC_PRIVATE
} AccessMod;

// ─────────────────────────────────────────
//  LINKED LIST NODES
// ─────────────────────────────────────────

typedef struct ParamNode {
    char              name[64];
    DataType          datatype;
    struct ParamNode* next;
} ParamNode;

typedef struct NameNode {
    char             name[64];
    struct NameNode* next;
} NameNode;

// ─────────────────────────────────────────
//  CONSTRUCT-SPECIFIC ATTRIBUTE BLOCKS
// ─────────────────────────────────────────

typedef struct {
    int is_initialized;
    int dimensions;
    int dim1;
    int dim2;
} ArrayAttr;

typedef struct {
    DataType   return_type;
    ParamNode* param_list;
    int        param_count;
    char       entry_label[32];
} FuncAttr;

typedef struct {
    DataType   return_type;
    ParamNode* param_list;
    int        param_count;
    char       entry_label[32];
    char       belongs_to[64];
    AccessMod  access;
} MethodAttr;

typedef struct {
    ParamNode* param_list;
    int        param_count;
    char       belongs_to[64];
    char       entry_label[32];
} CtorAttr;

typedef struct {
    char      belongs_to[64];
    AccessMod access;
} FieldAttr;

typedef struct {
    char      class_name[64];
    NameNode* fields_list;
    NameNode* methods_list;
    NameNode* constructors_list;
    int       class_size;
    char      parent_class[64];
} EntityAttr;

typedef struct {
    char start_label[32];
    char end_label[32];
    char cond_label[32];
} ForAttr;

typedef struct {
    char false_label[32];
    char end_label[32];
    int  branch_index;   /* 0=if, 1=first elif, 2=second elif ... */
} IfAttr;

// ─────────────────────────────────────────
//  THE SYMBOL
// ─────────────────────────────────────────

typedef struct Symbol {
    char      name[64];
    SymKind   kind;
    DataType  datatype;
    int       scope_level;
    int       size;
    int       offset;
    int       is_initialized;   /* works for VAR and ARRAY */

    union {
        ArrayAttr  array;
        FuncAttr   func;
        MethodAttr method;
        CtorAttr   ctor;
        FieldAttr  field;
        EntityAttr entity;
        ForAttr    forstmt;
        IfAttr     ifstmt;
    } attr;

    struct Symbol* next;
} Symbol;

// ─────────────────────────────────────────
//  SCOPE TABLE
// ─────────────────────────────────────────

typedef enum {
    SCOPE_GLOBAL,
    SCOPE_FUNCTION,
    SCOPE_METHOD,
    SCOPE_CONSTRUCTOR,
    SCOPE_ENTITY,
    SCOPE_BLOCK,
    SCOPE_FOR,
    SCOPE_IF,
    SCOPE_ELIF,
    SCOPE_ELSE
} ScopeKind;

#define HASH_SIZE 64

typedef struct SymTable {
    ScopeKind        kind;
    char             name[64];
    int              level;
    Symbol*          buckets[HASH_SIZE];
    int              symbol_count;
    int              next_offset;
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

extern SymTable* global_scope;
extern SymTable* current_scope;

/* global used to pass DataType into id_list rules */
extern DataType  current_decl_type;

#endif /* SYMTAB_H */
