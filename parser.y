%{
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;
extern int yylineno;
extern char* yytext;

typedef struct {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
} Quad;

Quad IR[10000];
int IR_idx = 0;
int tempVarCnt = 0;
int labelCnt = 0;

char* genVar();
char* getLabel();
void emit(char* op, char* arg1, char* arg2, char* result);
void emit_forinc(char* op, char* arg1, char* arg2, char* result);

int forDepth = -1;
int inForIncrement[10];
Quad forIncBuff[10][100];
int forIncIdx[10];

char* falseStack[100];
char* endStack[100];
int topPtr = -1;

static int if_cnt  = 0;
static int for_cnt = 0;

void pushIfLabels(char* falseLabel, char* endLabel) {
    topPtr++;
    falseStack[topPtr] = strdup(falseLabel);
    endStack[topPtr]   = strdup(endLabel);
}
char* topFalse() { return falseStack[topPtr]; }
char* topEnd()   { return endStack[topPtr];   }
void popIfLabels() {
    free(falseStack[topPtr]);
    free(endStack[topPtr]);
    topPtr--;
}

/*
 * insert_var_list:
 *   Called from var_decl rules to insert a comma-separated
 *   list of names stored in a single string "a,b,c".
 *   This avoids mid-rule actions in yacc which cause conflicts.
 */
void insert_var_list(char* names, DataType dt) {
    char buf[512];
    strncpy(buf, names, 511);
    char* tok = strtok(buf, ",");
    while (tok) {
        /* trim leading spaces */
        while (*tok == ' ') tok++;
        Symbol* sym = insert_symbol(current_scope, tok,
                                    KIND_VAR, dt, yylineno);
        if (sym) sym->is_initialized = 0;
        tok = strtok(NULL, ",");
    }
}
/* Returns 1 if the string is a numeric constant, 0 otherwise */
int isConstant(char* s) {
    if (!s || *s == '\0') return 0;
    char* end;
    strtod(s, &end);
    return (*end == '\0');
}

/* Folds two constants with a given operator. Returns heap string or NULL. */
char* foldConstants(char* op, char* arg1, char* arg2) {
    if (!isConstant(arg1) || !isConstant(arg2)) return NULL;

    double a = atof(arg1);
    double b = atof(arg2);
    double result;

    if      (strcmp(op, "+")  == 0) result = a + b;
    else if (strcmp(op, "-")  == 0) result = a - b;
    else if (strcmp(op, "*")  == 0) result = a * b;
    else if (strcmp(op, "/")  == 0) {
        if (b == 0) return NULL;   
        result = a / b;
    }
    else if (strcmp(op, "%")  == 0) {
        if ((int)b == 0) return NULL;
        result = (int)a % (int)b;
    }
    else if (strcmp(op, ">")  == 0) result = (a >  b);
    else if (strcmp(op, "<")  == 0) result = (a <  b);
    else if (strcmp(op, "==") == 0) result = (a == b);
    else return NULL;

    char* buf = malloc(32);
    if (result == (int)result)
        sprintf(buf, "%d", (int)result);
    else
        sprintf(buf, "%f", result);
    return buf;
}
%}

%union {
    int       ival;
    float     fval;
    char      cval;
    char*     sval;
    DataType  dtype;
    AccessMod access;
}

%token INT FP CHR STRING BOOL VOID
%token IF ELIF ELSE FOR TRUE FALSE FEED SHOW RETURN
%token SEQ1 SEQ2 FUNC ENTITY NEW PUBLIC PRIVATE THIS DOT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET SEMICOLON COMMA
%token <sval> IDENTIFIER STRING_LITERAL
%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <cval> CHAR_LITERAL

%type <dtype>  type func_type
%type <access> access_modifier
%type <sval>   expression arith_expr term factor assignment
%type <sval>   logic_expr rel_expr bitwise_expr indexed_id
%type <sval>   id_list   /* returns comma-separated name string */

%right ASSIGN ADD_ASSIGN SUB_ASSIGN
%left  OR
%left  AND
%left  BITOR
%left  BITAND
%left  EQ GT LT
%left  PLUS MINUS
%left  MUL DIV MOD
%right NOT

%%

program
    : program element
    | /* empty */
    ;

element
    : statement
    | function_decl
    | entity_decl
    ;

statement
    : var_decl
    | array_decl
    | expr_stmt
    | if_stmt
    | for_stmt
    | io_stmt
    | return_stmt
    | object_decl
    | block
    | error SEMICOLON
        {
            printf("Invalid statement at line %d\n", yylineno);
            yyerrok;
        }
    ;

/* ═══════════════════════════════════════════
   ENTITY
   ═══════════════════════════════════════════ */
entity_decl
    : ENTITY IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $2,
                                        KIND_ENTITY, DT_ENTITY, yylineno);
            if (sym) {
                strncpy(sym->attr.entity.class_name, $2, 63);
                sym->attr.entity.fields_list       = NULL;
                sym->attr.entity.methods_list      = NULL;
                sym->attr.entity.constructors_list = NULL;
                sym->attr.entity.class_size        = 0;
                sym->attr.entity.parent_class[0]   = '\0';
            }
            emit("entity", $2, "", "");
            SymTable* es = create_scope(SCOPE_ENTITY, $2, current_scope);
            current_scope = es;
        }
      LBRACE entity_body RBRACE
        {
            /* current_scope->next_offset is now the sum of ALL field sizes
               because only KIND_FIELD entries advanced next_offset.
               e.g.  entity Dog { int age; char x; }
                     → next_offset = 4 + 1 = 5
               Store this as class_size in the EntityAttr AND as sym->size
               in the global scope symbol so it prints correctly.          */
            int class_sz = current_scope->next_offset;
            Symbol* sym = lookup(current_scope->parent, current_scope->name);
            if (sym && sym->kind == KIND_ENTITY) {
                sym->attr.entity.class_size = class_sz;
                sym->size = class_sz;
                current_scope->parent->next_offset = sym->offset + class_sz;  
            }
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_entity", $2, "", "");
        }
    | ENTITY IDENTIFIER
        { emit("entity", $2, "", ""); }
      LBRACE error RBRACE
        {
            printf("Invalid entity body at line %d\n", yylineno);
            yyerrok;
            emit("end_entity", $2, "", "");
        }
    ;

entity_body
    : entity_body entity_member
    | /* empty */
    ;

entity_member
    : constructor_decl
    | method_decl
    | access_var_decl
    ;

/* ═══════════════════════════════════════════
   CONSTRUCTOR
   ═══════════════════════════════════════════ */
constructor_decl
    : IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $1,
                                        KIND_CONSTRUCTOR, DT_VOID, yylineno);
            if (sym) {
                strncpy(sym->attr.ctor.belongs_to, current_scope->name, 63);
                sym->attr.ctor.param_count = 0;
                sym->attr.ctor.param_list  = NULL;
                snprintf(sym->attr.ctor.entry_label, 32, "ctor_%s", $1);
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.constructors_list, $1);

            emit("constr", $1, "", "");
            SymTable* cs = create_scope(SCOPE_CONSTRUCTOR, $1, current_scope);
            current_scope = cs;
        }
      LPAREN param_list_opt RPAREN block
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_constr", $1, "", "");
        }
    | IDENTIFIER
        { emit("constr", $1, "", ""); }
      LPAREN error RPAREN block
        {
            printf("Invalid constructor parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_constr", $1, "", "");
        }
    ;

/* ═══════════════════════════════════════════
   METHOD
   ═══════════════════════════════════════════ */
method_decl
    /* primitive return type: public int func foo() */
    : access_modifier type FUNC IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $4,
                                        KIND_METHOD, $2, yylineno);
            if (sym) {
                sym->attr.method.return_type = $2;
                sym->attr.method.access      = $1;
                sym->attr.method.param_count = 0;
                sym->attr.method.param_list  = NULL;
                strncpy(sym->attr.method.belongs_to, current_scope->name, 63);
                snprintf(sym->attr.method.entry_label, 32,
                         "%s_%s", current_scope->name, $4);
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.methods_list, $4);

            emit("method", $4, "", "");
            SymTable* ms = create_scope(SCOPE_METHOD, $4, current_scope);
            current_scope = ms;
        }
      LPAREN param_list_opt RPAREN block
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_method", $4, "", "");
        }
    /* entity return type: public Dog func get() */
    | access_modifier IDENTIFIER FUNC IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $4,
                                        KIND_METHOD, DT_ENTITY, yylineno);
            if (sym) {
                sym->attr.method.return_type = DT_ENTITY;
                sym->attr.method.access      = $1;
                sym->attr.method.param_count = 0;
                sym->attr.method.param_list  = NULL;
                strncpy(sym->attr.method.belongs_to, current_scope->name, 63);
                snprintf(sym->attr.method.entry_label, 32,
                         "%s_%s", current_scope->name, $4);
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.methods_list, $4);

            emit("method", $4, "", "");
            SymTable* ms = create_scope(SCOPE_METHOD, $4, current_scope);
            current_scope = ms;
        }
      LPAREN param_list_opt RPAREN block
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_method", $4, "", "");
        }
    | access_modifier type FUNC IDENTIFIER
        { emit("method", $4, "", ""); }
      LPAREN error RPAREN block
        {
            printf("Invalid method parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_method", $4, "", "");
        }
    ;

/* ═══════════════════════════════════════════
   CLASS FIELD
   ═══════════════════════════════════════════ */
access_var_decl
    /* primitive type field: private int age; */
    : access_modifier type IDENTIFIER SEMICOLON
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_FIELD, $2, yylineno);
            if (sym) {
                sym->attr.field.access = $1;
                strncpy(sym->attr.field.belongs_to, current_scope->name, 63);
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.fields_list, $3);
        }
    /* entity-type field: private Dog myDog; */
    | access_modifier IDENTIFIER IDENTIFIER SEMICOLON
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_FIELD, DT_ENTITY, yylineno);
            if (sym) {
                sym->attr.field.access = $1;
                strncpy(sym->attr.field.belongs_to, current_scope->name, 63);
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.fields_list, $3);
        }
    ;

access_modifier
    : PUBLIC  { $$ = ACC_PUBLIC;  }
    | PRIVATE { $$ = ACC_PRIVATE; }
    ;

/* ═══════════════════════════════════════════
   OBJECT INSTANTIATION
   ═══════════════════════════════════════════ */
object_decl
    : IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON
        {
            emit("new", $5, "", $2);
            emit("call_constr", $5, "", $2);
        }
    | type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list RPAREN SEMICOLON
        {
            char* t = genVar();
            emit("push_ptr", $4, "", "");
            emit("call_method", $6, "", t);
            emit("=", t, "", $2);
        }
    ;

arg_list_opt
    : arg_list
    | /* empty */
    ;

arg_list
    : arg_list COMMA expression { emit("arg", $3, "", ""); }
    | expression                { emit("arg", $1, "", ""); }
    ;

/* ═══════════════════════════════════════════
   BLOCK
   ═══════════════════════════════════════════ */
block
    : LBRACE
        {
            SymTable* bs = create_scope(SCOPE_BLOCK, "block", current_scope);
            current_scope = bs;
        }
      stmt_list RBRACE
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
    | LBRACE RBRACE
    | LBRACE error RBRACE
        {
            printf("Error inside block at line %d\n", yylineno);
            yyerrok;
        }
    ;

stmt_list
    : stmt_list statement
    | statement
    ;

/* ═══════════════════════════════════════════
   VARIABLE DECLARATION

   KEY FIX: no mid-rule actions anywhere.
   id_list returns a comma-separated string
   of names upward via $$.
   insert_var_list() does the bulk insertion.

   Supported forms:
     int a;
     int a, b, c;
     int a = 10;
   ═══════════════════════════════════════════ */
var_decl
    /* int a;  or  int a, b, c; */
    : type id_list SEMICOLON
        {
            insert_var_list($2, $1);
            free($2);
        }

    /* int a = 10; */
    | type IDENTIFIER ASSIGN expression SEMICOLON
        {
            emit("=", $4, "", $2);
            Symbol* sym = insert_symbol(current_scope, $2,
                                        KIND_VAR, $1, yylineno);
            if (sym) sym->is_initialized = 1;
        }

    /* Dog d;  or  Dog d1, d2; — entity-typed vars */
    | IDENTIFIER id_list SEMICOLON
        {
            insert_var_list($2, DT_ENTITY);
            free($2);
        }

    /* Dog d = expr; */
    | IDENTIFIER IDENTIFIER ASSIGN expression SEMICOLON
        {
            emit("=", $4, "", $2);
            Symbol* sym = insert_symbol(current_scope, $2,
                                        KIND_VAR, DT_ENTITY, yylineno);
            if (sym) sym->is_initialized = 1;
        }
    ;

/*
 * id_list returns a heap-allocated comma-separated string
 * of all the variable names e.g. "a" or "a,b,c"
 * No symbol insertions happen here — done in var_decl action.
 */
id_list
    : id_list COMMA IDENTIFIER
        {
            char* buf = malloc(strlen($1) + strlen($3) + 2);
            sprintf(buf, "%s,%s", $1, $3);
            free($1);
            $$ = buf;
        }
    | IDENTIFIER
        {
            $$ = strdup($1);
        }
    ;

/* ═══════════════════════════════════════════
   TYPE — only keyword tokens, NO IDENTIFIER
   Entity types handled explicitly above
   ═══════════════════════════════════════════ */
type
    : INT    { $$ = DT_INT;    }
    | FP     { $$ = DT_FLOAT;  }
    | CHR    { $$ = DT_CHAR;   }
    | STRING { $$ = DT_STRING; }
    | BOOL   { $$ = DT_BOOL;   }
    ;

/* ═══════════════════════════════════════════
   ARRAY DECLARATION
   ═══════════════════════════════════════════ */
array_decl
    /* 1-D array with size:  int[] arr[10];
       OFFSET FIX:
         insert_symbol assigns offset = next_offset but does NOT
         advance next_offset (KIND_ARRAY excluded from the condition).
         We then set the REAL size = elem_size * dim1
         and manually advance next_offset by that real size.       */
    : type SEQ1 IDENTIFIER LBRACKET INT_LITERAL RBRACKET SEMICOLON
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_ARRAY, $1, yylineno);
            if (sym) {
                sym->attr.array.dimensions     = 1;
                sym->attr.array.dim1           = $5;
                sym->attr.array.dim2           = 0;
                sym->attr.array.is_initialized = 0;
                /* offset was already set by insert_symbol to the correct
                   position; now fix up size and advance next_offset      */
                sym->size = datatype_size($1) * $5;
                current_scope->next_offset = sym->offset + sym->size;
            }
        }
    /* 2-D array:  int[][] mat[3][4];
       Same pattern — fix size after insert, then advance counter.  */
    | type SEQ2 IDENTIFIER LBRACKET INT_LITERAL RBRACKET
                            LBRACKET INT_LITERAL RBRACKET SEMICOLON
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_ARRAY, $1, yylineno);
            if (sym) {
                sym->attr.array.dimensions     = 2;
                sym->attr.array.dim1           = $5;
                sym->attr.array.dim2           = $8;
                sym->attr.array.is_initialized = 0;
                sym->size = datatype_size($1) * $5 * $8;
                current_scope->next_offset = sym->offset + sym->size;
            }
        }
    /* 1-D array with initializer:  int[] arr = {1,2,3};
       Size unknown at parse time — left as element size,
       offset counter NOT advanced (size indeterminate).           */
    | type SEQ1 IDENTIFIER ASSIGN array_init SEMICOLON
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_ARRAY, $1, yylineno);
            if (sym) {
                sym->attr.array.dimensions     = 1;
                sym->attr.array.is_initialized = 1;
                /* size and offset will be determined at runtime */
            }
        }
    ;

array_init
    : LBRACE expr_list RBRACE
    ;

expr_list
    : expr_list COMMA expression
    | expression
    ;

/* ═══════════════════════════════════════════
   FUNCTION DECLARATION
   ═══════════════════════════════════════════ */
function_decl
    /* int func add(...)  /  void func main(...) */
    : func_type FUNC IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_FUNCTION, $1, yylineno);
            if (sym) {
                sym->attr.func.return_type = $1;
                sym->attr.func.param_count = 0;
                sym->attr.func.param_list  = NULL;
                snprintf(sym->attr.func.entry_label, 32, "func_%s", $3);
                /* size stays 0 — functions occupy no slot in parent scope */
            }
            emit("func", $3, "", "");
            SymTable* fs = create_scope(SCOPE_FUNCTION, $3, current_scope);
            current_scope = fs;
        }
      LPAREN param_list_opt RPAREN block
        {
            /* Do NOT write next_offset into sym->size.
               Function size in global scope = 0 always.
               The frame size is visible in the function's own scope table. */
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("endfunc", "", "", "");
        }

    /* Dog func create(...) — entity return type */
    | IDENTIFIER FUNC IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_FUNCTION, DT_ENTITY, yylineno);
            if (sym) {
                sym->attr.func.return_type = DT_ENTITY;
                sym->attr.func.param_count = 0;
                sym->attr.func.param_list  = NULL;
                snprintf(sym->attr.func.entry_label, 32, "func_%s", $3);
                /* size stays 0 */
            }
            emit("func", $3, "", "");
            SymTable* fs = create_scope(SCOPE_FUNCTION, $3, current_scope);
            current_scope = fs;
        }
      LPAREN param_list_opt RPAREN block
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("endfunc", "", "", "");
        }

    /* error recovery */
    | func_type FUNC IDENTIFIER
        { emit("func", $3, "", ""); }
      LPAREN error RPAREN block
        {
            printf("Invalid parameter list at line %d\n", yylineno);
            yyerrok;
            emit("endfunc", "", "", "");
        }
    ;

func_type
    : type { $$ = $1;      }
    | VOID { $$ = DT_VOID; }
    ;

param_list_opt
    : param_list
    | /* empty */
    ;

param_list
    : param_list COMMA param
    | param
    ;

param
    /* primitive-typed param: int x */
    : type IDENTIFIER
        {
            emit("param", $2, "", "");
            Symbol* sym = insert_symbol(current_scope, $2,
                                        KIND_PARAM, $1, yylineno);
            Symbol* owner = lookup(current_scope->parent,
                                   current_scope->name);
            if (owner) {
                if (owner->kind == KIND_FUNCTION)
                    add_param(&owner->attr.func.param_list,
                              &owner->attr.func.param_count, $2, $1);
                else if (owner->kind == KIND_METHOD)
                    add_param(&owner->attr.method.param_list,
                              &owner->attr.method.param_count, $2, $1);
                else if (owner->kind == KIND_CONSTRUCTOR)
                    add_param(&owner->attr.ctor.param_list,
                              &owner->attr.ctor.param_count, $2, $1);
            }
            (void)sym;
        }
    /* entity-typed param: Dog myDog */
    | IDENTIFIER IDENTIFIER
        {
            emit("param", $2, "", "");
            Symbol* sym = insert_symbol(current_scope, $2,
                                        KIND_PARAM, DT_ENTITY, yylineno);
            Symbol* owner = lookup(current_scope->parent,
                                   current_scope->name);
            if (owner) {
                if (owner->kind == KIND_FUNCTION)
                    add_param(&owner->attr.func.param_list,
                              &owner->attr.func.param_count, $2, DT_ENTITY);
                else if (owner->kind == KIND_METHOD)
                    add_param(&owner->attr.method.param_list,
                              &owner->attr.method.param_count, $2, DT_ENTITY);
                else if (owner->kind == KIND_CONSTRUCTOR)
                    add_param(&owner->attr.ctor.param_list,
                              &owner->attr.ctor.param_count, $2, DT_ENTITY);
            }
            (void)sym;
        }
    ;

/* ═══════════════════════════════════════════
   RETURN
   ═══════════════════════════════════════════ */
return_stmt
    : RETURN expression SEMICOLON { emit("return", $2, "", ""); }
    | RETURN SEMICOLON            { emit("return", "", "", ""); }
    | RETURN error SEMICOLON
        {
            printf("Invalid return statement at line %d\n", yylineno);
            yyerrok;
        }
    ;

expr_stmt
    : expression SEMICOLON
    | SEMICOLON
    ;

expression
    : assignment { $$ = $1; }
    ;

/* ═══════════════════════════════════════════
   INDEXED ACCESS
   ═══════════════════════════════════════════ */
indexed_id
    : IDENTIFIER LBRACKET expression RBRACKET
        {
            char* t1 = genVar(); emit("*",  $3, "type.width", t1);
            char* t2 = genVar(); emit("[]", $1, t1,           t2);
            $$ = t2;
        }
    | IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET
        {
            char* t1 = genVar(); emit("*",  $3, "array.cols", t1);
            char* t2 = genVar(); emit("+",  t1, $6,           t2);
            char* t3 = genVar(); emit("*",  t2, "type.width", t3);
            char* t4 = genVar(); emit("[]", $1, t3,           t4);
            $$ = t4;
        }
    ;

assignment
    : IDENTIFIER ASSIGN assignment
        { emit("=", $3, "", $1); $$ = strdup($1); }
    | IDENTIFIER ADD_ASSIGN assignment
        { char* t = genVar(); emit("+", $1, $3, t); emit("=", t, "", $1); $$ = t; }
    | IDENTIFIER SUB_ASSIGN assignment
        { char* t = genVar(); emit("-", $1, $3, t); emit("=", t, "", $1); $$ = t; }
    | indexed_id ASSIGN assignment
        { $$ = $3; }
    | THIS DOT IDENTIFIER ASSIGN assignment
        { emit("set_field", "this", $3, $5); $$ = $5; }
    | IDENTIFIER DOT IDENTIFIER ASSIGN assignment
        { emit("set_field", $1, $3, $5); $$ = $5; }
    | logic_expr { $$ = $1; }
    ;

logic_expr
    : logic_expr OR logic_expr
        { char* t = genVar(); emit("||", $1, $3, t); $$ = t; }
    | logic_expr AND logic_expr
        { char* t = genVar(); emit("&&", $1, $3, t); $$ = t; }
    | NOT logic_expr
        { char* t = genVar(); emit("!", $2, "", t); $$ = t; }
    | bitwise_expr { $$ = $1; }
    ;

bitwise_expr
    : bitwise_expr BITAND bitwise_expr
        { char* t = genVar(); emit("&", $1, $3, t); $$ = t; }
    | bitwise_expr BITOR bitwise_expr
        { char* t = genVar(); emit("|", $1, $3, t); $$ = t; }
    | rel_expr { $$ = $1; }
    ;

rel_expr
    : arith_expr GT arith_expr
        {
            char* folded = foldConstants(">", $1, $3);
            if (folded) {
                $$ = folded;
            } else {
                char* t = genVar(); emit(">", $1, $3, t); $$ = t;
            }
        }
    | arith_expr LT arith_expr
        {
            char* folded = foldConstants("<", $1, $3);
            if (folded) {
                $$ = folded;
            } else {
                char* t = genVar(); emit("<", $1, $3, t); $$ = t;
            }
        }
    | arith_expr EQ arith_expr
        {
            char* folded = foldConstants("==", $1, $3);
            if (folded) {
                $$ = folded;
            } else {
                char* t = genVar(); emit("==", $1, $3, t); $$ = t;
            }
        }
    | arith_expr { $$ = $1; }
    ;

arith_expr
    : arith_expr PLUS term
        {  char* folded = foldConstants("+", $1, $3);
            if (folded) {
                $$ = folded;                       
            } else {
                char* t = genVar();
                emit("+", $1, $3, t);
                $$ = t;
            } 
        }
    | arith_expr MINUS term
        {   char* folded = foldConstants("-", $1, $3);
            if (folded) {
                $$ = folded;
            } else {
                char* t = genVar();
                emit("-", $1, $3, t);
                $$ = t;
            } 
        }
    | term { $$ = $1; }
    ;

term
    : term MUL factor
        {
            char* folded = foldConstants("*", $1, $3);
            if (folded) {
                $$ = folded;
            } else {
                char* t = genVar();
                emit("*", $1, $3, t);
                $$ = t;
            }
        }
    | term DIV factor
        {
            char* folded = foldConstants("/", $1, $3);
            if (folded) {
                $$ = folded;
            } else {
                char* t = genVar();
                emit("/", $1, $3, t);
                $$ = t;
            }
        }
    | term MOD factor
        {
            char* folded = foldConstants("%", $1, $3);
            if (folded) {
                $$ = folded;
            } else {
                char* t = genVar();
                emit("%", $1, $3, t);
                $$ = t;
            }
        }
    | factor { $$ = $1; }
    ;

factor
    : IDENTIFIER LPAREN arg_list_opt RPAREN
        { char* t = genVar(); emit("call", $1, "", t); $$ = t; }
    | IDENTIFIER      { $$ = strdup($1); }
    | indexed_id      { $$ = $1; }
    | INT_LITERAL     { char b[20]; sprintf(b, "%d",   $1); $$ = strdup(b); }
    | FLOAT_LITERAL   { char b[20]; sprintf(b, "%f",   $1); $$ = strdup(b); }
    | CHAR_LITERAL    { char b[20]; sprintf(b, "'%c'", $1); $$ = strdup(b); }
    | STRING_LITERAL  { $$ = strdup($1); }
    | TRUE            { $$ = strdup("1"); }
    | FALSE           { $$ = strdup("0"); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

/* ═══════════════════════════════════════════
   IF / ELIF / ELSE
   ═══════════════════════════════════════════ */
if_stmt
    : IF LPAREN
        {
            char* f = getLabel();
            char* e = getLabel();
            pushIfLabels(f, e);

            char if_name[32];
            snprintf(if_name, 32, "if_%d", if_cnt++);
            Symbol* sym = insert_symbol(current_scope, if_name,
                                        KIND_IF, DT_VOID, yylineno);
            if (sym) {
                strncpy(sym->attr.ifstmt.false_label, f, 31);
                strncpy(sym->attr.ifstmt.end_label,   e, 31);
            }
            SymTable* is = create_scope(SCOPE_IF, if_name, current_scope);
            current_scope = is;
        }
      expression
        { emit("ifFalse", $4, "", topFalse()); }
      RPAREN
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
      block
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
      elif_list else_opt
        { emit("label", "", "", topEnd()); popIfLabels(); }

    | IF LPAREN error RPAREN block
        {
            printf("Invalid IF condition at line %d\n", yylineno);
            yyerrok;
        }
    ;

elif_list
    : ELIF LPAREN
        {
            char* n = getLabel();
            free(falseStack[topPtr]);
            falseStack[topPtr] = strdup(n);

            char elif_name[32];
            snprintf(elif_name, 32, "elif_%d", if_cnt++);
            Symbol* sym = insert_symbol(current_scope, elif_name,
                                        KIND_ELIF, DT_VOID, yylineno);
            if (sym) {
                strncpy(sym->attr.ifstmt.false_label, n, 31);
                strncpy(sym->attr.ifstmt.end_label, topEnd(), 31);
            }
            SymTable* es = create_scope(SCOPE_ELIF, elif_name, current_scope);
            current_scope = es;
        }
      expression
        { emit("ifFalse", $4, "", topFalse()); }
      RPAREN
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
      block
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
      elif_list
    | /* empty */
    ;

else_opt
    : ELSE
        {
            char else_name[32];
            snprintf(else_name, 32, "else_%d", if_cnt++);
            SymTable* es = create_scope(SCOPE_ELSE, else_name, current_scope);
            current_scope = es;
        }
      block
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
    | /* empty */
    ;

/* ═══════════════════════════════════════════
   FOR LOOP
   ═══════════════════════════════════════════ */
for_stmt
    : FOR LPAREN
        {
            char for_name[32];
            snprintf(for_name, 32, "for_%d", for_cnt++);
            SymTable* fs = create_scope(SCOPE_FOR, for_name, current_scope);
            current_scope = fs;
        }
      for_header block
        {
            for (int i = 0; i < forIncIdx[forDepth]; i++) {
                emit(forIncBuff[forDepth][i].op,
                     forIncBuff[forDepth][i].arg1,
                     forIncBuff[forDepth][i].arg2,
                     forIncBuff[forDepth][i].result);
            }
            forDepth--;
            emit("goto",  "", "", topFalse());
            emit("label", "", "", topEnd());
            popIfLabels();

            print_table(current_scope);
            current_scope = current_scope->parent;
        }

    | FOR LPAREN error RPAREN block
        {
            printf("Invalid FOR header at line %d\n", yylineno);
            yyerrok;
        }
    ;

for_header
    : for_init SEMICOLON
        {
            char* b = getLabel();
            char* e = getLabel();
            pushIfLabels(b, e);
            emit("label", "", "", b);
        }
      expression SEMICOLON
        {
            emit("ifFalse", $4, "", topEnd());
            forDepth++;
            forIncIdx[forDepth]      = 0;
            inForIncrement[forDepth] = 1;
        }
      expression
        { inForIncrement[forDepth] = 0; }
      RPAREN
    ;

for_init
    : var_decl_no_semi
    | expression
    | /* empty */
    ;

var_decl_no_semi
    : type IDENTIFIER
        {
            insert_symbol(current_scope, $2, KIND_VAR, $1, yylineno);
        }
    | type IDENTIFIER ASSIGN expression
        {
            emit("=", $4, "", $2);
            Symbol* sym = insert_symbol(current_scope, $2,
                                        KIND_VAR, $1, yylineno);
            if (sym) sym->is_initialized = 1;
        }
    ;

/* ═══════════════════════════════════════════
   I/O
   ═══════════════════════════════════════════ */
io_stmt
    : IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON
        { emit("in", "", "", $1); }
    | type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON
        { emit("in", "", "", $2); }
    | SHOW LPAREN expression RPAREN SEMICOLON
        { emit("out", $3, "", ""); }
    | SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON
        {
            char* t = genVar();
            emit("[]",  $3, $5, t);
            emit("out", t,  "", "");
        }
    ;

%%

char* genVar() {
    char newVar[20];
    sprintf(newVar, "t%d", tempVarCnt++);
    return strdup(newVar);
}

char* getLabel() {
    char newLabel[20];
    sprintf(newLabel, "L%d", labelCnt++);
    return strdup(newLabel);
}

void emit_forinc(char* op, char* arg1, char* arg2, char* result) {
    strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].op,     op);
    strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].arg1,   arg1);
    strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].arg2,   arg2);
    strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].result, result);
    forIncIdx[forDepth]++;
}

void emit(char* op, char* arg1, char* arg2, char* result) {
    if (forDepth >= 0 && inForIncrement[forDepth]) {
        emit_forinc(op, arg1, arg2, result);
        return;
    }
    memset(&IR[IR_idx], 0, sizeof(Quad));
    strcpy(IR[IR_idx].op,     op ? op : "");
    strcpy(IR[IR_idx].arg1,   arg1);
    strcpy(IR[IR_idx].arg2,   arg2);
    strcpy(IR[IR_idx].result, result);
    IR_idx++;
}

void yyerror(const char *s) {
    fprintf(stderr, "\nSyntax Error at line %d near '%s': %s\n",
            yylineno, yytext, s);
}

int main() {
    global_scope  = create_scope(SCOPE_GLOBAL, "global", NULL);
    current_scope = global_scope;

    yyin = stdin;
    yyparse();

    printf("\n========== GLOBAL SCOPE ==========\n");
    print_table(global_scope);

    printf("\nParsing Successful\nGenerated quadruple table:\n");
    printf("%-15s %-15s %-15s %-15s\n", "OP", "ARG1", "ARG2", "RESULT");
    for (int i = 0; i < IR_idx; i++)
        printf("%-15s %-15s %-15s %-15s\n",
               IR[i].op, IR[i].arg1, IR[i].arg2, IR[i].result);
    return 0;
}

