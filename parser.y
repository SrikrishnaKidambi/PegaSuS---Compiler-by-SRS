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


// Utilities for handling array declaration along with initialization
static int arr2d_rows = 0;
static int arr2d_cols = 0;

DataType last_expr_type = DT_UNKNOWN;
DataType current_array_elem_type = DT_UNKNOWN;
int array_type_errors = 0;

// Utitilies for handling semantic errors in functions and function calls
static Symbol* current_function = NULL;         // A global variable that is used for storing the function(a symbol right!!) so that we can know what is the expected
                                                // return type
static DataType call_arg_types[64];	// Array for storing the type of argument in each function call
static int call_arg_count = 0;		// Store the number of arguments collected till now


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
%type <ival>   expr_list array_init row_list 
%type <ival> array_init2d

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
            Symbol* sym = insert_symbol(global_scope, $2,
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
	    register_entity_scope(es);
	    if(sym){
		sym->attr.entity.scope = es;
	    }
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
	    Symbol* class_sym = lookup(current_scope,$5);
	    if(!class_sym || class_sym->kind != KIND_ENTITY){
		char buf[256];
	    	snprintf(buf, sizeof(buf),"line %d: Entity '%s' not found to instantiate",yylineno, $1);
	    	semantic_error(buf);
	    }
	    Symbol* obj = insert_symbol(current_scope,$2,KIND_OBJECT,DT_OBJECT,yylineno);
	    if(obj){
		strncpy(obj->attr.object.entity_name,class_sym->attr.entity.class_name,63); //supports max class size as 63
		obj->size = class_sym->attr.entity.class_size;
		current_scope->next_offset = obj->offset + obj->size;
	}
		
            emit("new", $5, "", $2);
            emit("call_constr", $5, "", $2);
        }
    | type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list RPAREN SEMICOLON
        {
	    	check_method_access($4,$6);
	    	Symbol* msym = lookup(current_scope, $6);
		if(msym && msym->kind == KIND_METHOD){
			if(msym->attr.method.return_type != $1){
				fprintf(stderr, "ERROR line %d: cannot assign result of method '%s' (returns %s) to '%s' (declared as %s).\n", yylineno, $6, dt_names[msym->attr.method.return_type], $2, dt_names[$1]);
			}
		}
            	char* t = genVar();
            	emit("push_ptr", $4, "", "");
            	emit("call_method", $6, "", t);
            	emit("=", t, "", $2);
        }
    ;

arg_list_opt
    : arg_list
    | /* empty */ 	{ call_arg_count = 0; }     // Set the number of arguments being passed to zero
    ;

arg_list
    : arg_list COMMA expression 
	{
		if(call_arg_count < 64){
			call_arg_types[call_arg_count++] = last_expr_type;
		} 
		emit("arg", $3, "", ""); 
	}
    | expression                
	{ 
		call_arg_count = 0;
		call_arg_types[call_arg_count++] = last_expr_type;
		emit("arg", $1, "", ""); 
	}
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
	    if(last_expr_type != DT_UNKNOWN && last_expr_type != $1){
		//printf("==== $1 = %d and last_expr_type = %d\n", $1, last_expr_type);
		fprintf(stderr, "ERROR hih line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, $2, dt_names[$1], dt_names[last_expr_type]);
	    }
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
		fprintf(stderr, "ERROR line %d: cannot initialize entity variable '%s' with value of type %s.\n", yylineno, $2, dt_names[last_expr_type]);
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
    | type SEQ1 IDENTIFIER ASSIGN 
	{
		current_array_elem_type = $1;
		array_type_errors = 0;
	}
	array_init SEMICOLON
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_ARRAY, $1, yylineno);
            if (sym) {
                sym->attr.array.dimensions = 1;
                sym->attr.array.is_initialized = 1;
		sym->attr.array.dim1 = $6;	// the token or symbol array_init returns the number of elements present in the list
		sym->attr.array.dim2 = 0;
		sym->size = datatype_size($1) * $6;
		current_scope->next_offset = sym->offset + sym->size;
            }
		current_array_elem_type = DT_UNKNOWN;
        }
     | type SEQ2 IDENTIFIER ASSIGN 
	{
		current_array_elem_type = $1;
		array_type_errors = 0;
		arr2d_rows = 0;
		arr2d_cols = 0;
	}
	array_init2d SEMICOLON
	{
		Symbol* sym = insert_symbol(current_scope, $3, KIND_ARRAY, $1, yylineno);
		if(sym) {
			sym->attr.array.dimensions = 2;
			sym->attr.array.dim1 = arr2d_rows;
			sym->attr.array.dim2 = arr2d_cols;
			sym->attr.array.is_initialized = 1;
			sym->size = datatype_size($1) * arr2d_rows * arr2d_cols;
			current_scope->next_offset = sym->offset + sym->size;
		}
		current_array_elem_type = DT_UNKNOWN;
	} 
    ;

array_init
    : LBRACE expr_list RBRACE { $$ = $2; }
    ;

array_init2d:
	    LBRACE row_list RBRACE { $$ = $2; }
	;

row_list:
	row_list COMMA LBRACE expr_list RBRACE 
	{ 
		arr2d_rows++;
		arr2d_cols = $4;
		$$ = $1 + $4; 
	}
	| LBRACE expr_list RBRACE 	
	{ 
		arr2d_rows = 1;
		arr2d_cols = $2;
		$$ = $2; 
	}
	;

expr_list
    : expr_list COMMA expression 
	{
		if(current_array_elem_type != DT_UNKNOWN && last_expr_type != DT_UNKNOWN && last_expr_type != current_array_elem_type){
			{
				fprintf(stderr, "ERROR line %d: array initializer type mismatch - expected %s, got %s\n", yylineno, dt_names[current_array_elem_type], dt_names[last_expr_type]);
				array_type_errors++;
			} 
		}
		$$ = $1 + 1;
	}
    | expression
	{
                if(current_array_elem_type != DT_UNKNOWN && last_expr_type != DT_UNKNOWN && last_expr_type != current_array_elem_type){
                        {
                                fprintf(stderr, "ERROR line %d: array initializer type mismatch - expected %s, got %s\n", yylineno, dt_names[current_array_elem_type], dt_names[last_expr_type]);
                                array_type_errors++;
                        }
        	}
		$$ = 1;
	}
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
	    current_function = sym;	// Storing the current function we are in when we are entering the function declaration
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
	    current_function = NULL; 	// Clearing the variable when we are at the end of the current function declaration 
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
	    current_function = sym; 	// Storing the current function symbol being parsed to handle correct type of value(or variable) being returned
            emit("func", $3, "", "");
            SymTable* fs = create_scope(SCOPE_FUNCTION, $3, current_scope);
            current_scope = fs;
        }
      LPAREN param_list_opt RPAREN block
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
	    current_function = NULL;
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
    : RETURN expression SEMICOLON 
	{ 
		if(current_function) {
			DataType expected = current_function->attr.func.return_type;
			if(expected == DT_VOID){
				fprintf(stderr, "ERROR line %d: void function '%s' cannot return a value.\n", yylineno, current_function->name);
			}
			else if(last_expr_type != DT_UNKNOWN && last_expr_type != expected){
				fprintf(stderr, "ERROR line %d: function '%s' expected to return %s"
					" but returns %s", yylineno, current_function->name, dt_names[expected], dt_names[last_expr_type]); 
			}
		}
		emit("return", $2, "", ""); 
	}
    | RETURN SEMICOLON            
	{
		if(current_function){
			DataType expected = current_function->attr.func.return_type;
			if(expected != DT_VOID){
				fprintf(stderr, "ERROR line %d: Function '%s' is non-void (returns %s)"
					" but has empty return.\n", yylineno, current_function->name, dt_names[expected]);
			}
		} 
		emit("return", "", "", ""); 
	}

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
        {
		// Checking the type before assignment
		Symbol* lhs = lookup(current_scope, $1);
		if(lhs && last_expr_type != DT_UNKNOWN){
			if(lhs->datatype != last_expr_type){
				fprintf(stderr, "ERROR line %d: cannot assign %s to '%s' "
				"(declared as %s).\n", yylineno, dt_names[last_expr_type], $1, dt_names[lhs->datatype]);
			}
		} 
		emit("=", $3, "", $1); 
		$$ = strdup($1); 
	}
    | IDENTIFIER ADD_ASSIGN assignment
        { 
		Symbol* lhs = lookup(current_scope, $1);
		if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
			fprintf(stderr, "ERROR line %d: type mismatch in '+=' : '%s' is %s but RHS is %s.\n", yylineno, $1, dt_names[lhs->datatype], dt_names[last_expr_type]);
		}
		char* t = genVar(); 
		emit("+", $1, $3, t); 
		emit("=", t, "", $1); 
		$$ = t; 
	}
    | IDENTIFIER SUB_ASSIGN assignment
        { 	
		Symbol* lhs = lookup(current_scope, $1);
		if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
			fprintf(stderr, "ERROR line %d: type mismatch in '-=' : '%s' is %s but RHS is %s.\n", yylineno, $1, dt_names[lhs->datatype], dt_names[last_expr_type]);
		}
			
		char* t = genVar(); 
		emit("-", $1, $3, t); 
		emit("=", t, "", $1); 	
		$$ = t; 
	}
    | indexed_id ASSIGN assignment
        { $$ = $3; }
    | THIS DOT IDENTIFIER ASSIGN assignment
        { //check_field_access("this", $3); 
		//Symbol* method_sym = lookup(current_scope, current_scope->name);
		const char* entity_name = NULL;
		if(current_function && current_function->kind == KIND_METHOD){
			entity_name = current_function->attr.method.belongs_to;
		}
		if(entity_name){
			Symbol* entity_sym = lookup(global_scope, entity_name);
			if(entity_sym && entity_sym->kind == KIND_ENTITY){
				Symbol* field = lookup(global_scope, $3);
				while(field){
					if(field->kind == KIND_FIELD && strcmp(field->attr.field.belongs_to, entity_name) == 0){
						break;
					}
				field = field->next;
				}
				if(field && last_expr_type != DT_UNKNOWN && field->datatype != last_expr_type){
					fprintf(stderr, "ERROR line %d: Cannot assign %s to field '%s.%s' (declared as %s).\n", yylineno, dt_names[last_expr_type], entity_name, $3, dt_names[field->datatype]);
				}
			}
		}	
		emit("set_field", "this", $3, $5); 
		$$ = $5; 
	}
    | IDENTIFIER DOT IDENTIFIER ASSIGN assignment
        { 
		check_field_access($1,$3); 
		Symbol* obj = lookup(current_scope, $1);
		if(!obj || obj->kind != KIND_OBJECT){
			fprintf(stderr, "ERROR line %d: '%s' is not an object.\n", yylineno, $1);
		}
		else{
			const char* entity_name = obj->attr.object.entity_name;
			SymTable* ent_scope = find_entity_scope(entity_name);
			if(!ent_scope){
				fprintf(stderr, "ERROR line %d: Entity %s never defined.\n", yylineno, entity_name);
			}
			else{
				Symbol* field = lookup_local(ent_scope, $3);
				if(!field || field->kind != KIND_FIELD){
					fprintf(stderr, "ERROR line %d: '%s' is not a field of entity '%s'.\n", yylineno, $3, entity_name);
				}
				else if(last_expr_type != DT_UNKNOWN && field->datatype != last_expr_type){
					fprintf(stderr, "ERROR line %d: cannot assign %s to '%s.%s' (declared as %s).\n", yylineno, dt_names[last_expr_type], entity_name, $3, dt_names[field->datatype]);
				}
			}
		}
		emit("set_field", $1, $3, $5); 
		$$ = $5; 
	}
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
        { char* t = genVar(); emit(">",  $1, $3, t); $$ = t; }
    | arith_expr LT arith_expr
        { char* t = genVar(); emit("<",  $1, $3, t); $$ = t; }
    | arith_expr EQ arith_expr
        { char* t = genVar(); emit("==", $1, $3, t); $$ = t; }
    | arith_expr { $$ = $1; }
    ;

arith_expr
    : arith_expr PLUS term
        { char* t = genVar(); emit("+", $1, $3, t); $$ = t; }
    | arith_expr MINUS term
        { char* t = genVar(); emit("-", $1, $3, t); $$ = t; }
    | term { $$ = $1; }
    ;

term
    : term MUL factor
        { char* t = genVar(); emit("*", $1, $3, t); $$ = t; }
    | term DIV factor
        { char* t = genVar(); emit("/", $1, $3, t); $$ = t; }
    | term MOD factor
        { char* t = genVar(); emit("%", $1, $3, t); $$ = t; }
    | factor { $$ = $1; }
    ;

factor
    : IDENTIFIER LPAREN arg_list_opt RPAREN
        { 
		char* t = genVar(); 
		Symbol* fsym = lookup(current_scope, $1);
		if(!fsym) {
			fprintf(stderr, 
				"ERROR line %d: call to undeclared function '%s'.\n",yylineno, $1);
			last_expr_type = DT_UNKNOWN;
		}
		else if(fsym->kind != KIND_FUNCTION && fsym->kind != KIND_METHOD){
			fprintf(stderr, "ERROR line %d: '%s' is not a function.\n", yylineno, $1);
			last_expr_type = DT_UNKNOWN;
		}
		else{
			ParamNode* ep = (fsym->kind == KIND_FUNCTION) ? fsym->attr.func.param_list : fsym->attr.method.param_list;
			int expected_count = (fsym->kind == KIND_FUNCTION) ? fsym->attr.func.param_count : fsym->attr.method.param_count;
			if(call_arg_count != expected_count) {
				fprintf(stderr, "ERROR line %d:, function '%s' expects %d arg(s), got %d.\n", yylineno, $1, expected_count, call_arg_count);
			}
			else{
				for(int i = 0; i < call_arg_count && ep; i++, ep=ep->next) {
					if(call_arg_types[i] != DT_UNKNOWN && call_arg_types[i] != ep->datatype) {
						fprintf(stderr, "ERROR line %d: function '%s' argument %d - expected %s, but got %s.\n", yylineno, $1, i+1, dt_names[ep->datatype], dt_names[call_arg_types[i]]);
					}
				}
			}
			last_expr_type = (fsym->kind == KIND_FUNCTION) ? fsym->attr.func.return_type : fsym->attr.method.return_type;
		}
		emit("call", $1, "", t);
		$$ = t; 
	}
    | IDENTIFIER DOT IDENTIFIER
    {
        char* t = genVar();
        last_expr_type = DT_UNKNOWN;  /* safe default */

        /* Step 1: look up the object variable */
        Symbol* obj = lookup(current_scope, $1);
        if (!obj) {
            fprintf(stderr,
                "ERROR line %d: undeclared identifier '%s'.\n",
                yylineno, $1);
        }
        else if (obj->kind != KIND_OBJECT) {
            fprintf(stderr,
                "ERROR line %d: '%s' is not an object.\n",
                yylineno, $1);
        }
        else {
            /* Step 2: get entity name safely */
            const char* entity_name = obj->attr.object.entity_name;
            if (!entity_name || entity_name[0] == '\0') {
                fprintf(stderr,
                    "ERROR line %d: object '%s' has no entity type.\n",
                    yylineno, $1);
            }
            else {
                /* Step 3: look up the entity symbol in global scope */
                Symbol* cls = lookup(global_scope, entity_name);
                if (!cls || cls->kind != KIND_ENTITY) {
                    fprintf(stderr,
                        "ERROR line %d: entity '%s' not defined.\n",
                        yylineno, entity_name);
                }
                else {
                    /* Step 4: get the entity's scope (stored in EntityAttr) */
                    SymTable* escope = cls->attr.entity.scope;
                    if (!escope) {
                        fprintf(stderr,
                            "ERROR line %d: entity '%s' has no scope.\n",
                            yylineno, entity_name);
                    }
                    else {
                        /* Step 5: look up the field inside entity scope */
                        Symbol* field = lookup_local(escope, $3);
                        if (!field || field->kind != KIND_FIELD) {
                            fprintf(stderr,
                                "ERROR line %d: '%s' is not a field of '%s'.\n",
                                yylineno, $3, entity_name);
                        }
                        else {
                            /* Step 6: check private access */
                            if (field->attr.field.access == ACC_PRIVATE &&
                                strcmp(current_scope->name, entity_name) != 0) {
                                fprintf(stderr,
                                    "ERROR line %d: field '%s' of '%s' is private.\n",
                                    yylineno, $3, entity_name);
                            }
                            else {
                                last_expr_type = field->datatype;
                            }
                        }
                    }
                }
            }
        }

        emit("get_field", $1, $3, t);
        $$ = t;
    }
    | IDENTIFIER      

	{ 
		Symbol* s = lookup(current_scope, $1);
		last_expr_type = s ? s->datatype : DT_UNKNOWN;
		$$ = strdup($1); 
	}
    | indexed_id      { $$ = $1; }
    | INT_LITERAL     { char b[20]; sprintf(b, "%d",   $1); $$ = strdup(b); last_expr_type = DT_INT; }
    | FLOAT_LITERAL   { char b[20]; sprintf(b, "%f",   $1); $$ = strdup(b);  last_expr_type = DT_FLOAT; }
    | CHAR_LITERAL    { char b[20]; sprintf(b, "'%c'", $1); $$ = strdup(b); last_expr_type = DT_CHAR; }
    | STRING_LITERAL  { $$ = strdup($1);  last_expr_type = DT_STRING; }
    | TRUE            { $$ = strdup("1"); last_expr_type = DT_BOOL; }
    | FALSE           { $$ = strdup("0"); last_expr_type = DT_BOOL; }
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
	    	if(last_expr_type != DT_UNKNOWN && last_expr_type != $1){
			fprintf(stderr, "ERROR line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, $2, dt_names[$1], dt_names[last_expr_type]);
		}
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
        {
		Symbol* var = lookup(current_scope, $1);
		if(!var){
			fprintf(stderr, "ERROR line %d: assignment to undeclared variable '%s'.\n", yylineno, $1);
		}
		emit("in", "", "", $1); 
	}
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

