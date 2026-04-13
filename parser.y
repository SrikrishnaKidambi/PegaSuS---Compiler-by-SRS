%{
#include "symtab.h"
#include "optimizer.h"
#include "asm_gen.h"
#include "transpiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;
extern int yylineno;
extern char* yytext;
// Add near your other globals at the top of parser.y
#ifndef QUAD_DEFINED
typedef struct {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
} Quad;
#define QUAD_DEFINED
#endif

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
static char arr_init_vals[256][32];
static int  arr_init_count = 0;

//store the idx for functions/methods and contructors (fix to issue pointed by Raghavendra)
static int pending_constr_ir_idx = -1;
static int pending_method_ir_idx = -1;
static int pending_func_ir_idx = -1;

DataType last_expr_type = DT_UNKNOWN;
DataType current_array_elem_type = DT_UNKNOWN;
int array_type_errors = 0;

// Utitilies for handling semantic errors in functions and function calls
static Symbol* current_function = NULL;         // A global variable that is used for storing the function(a symbol right!!) so that we can know what is the expected
                                                // return type
static DataType call_arg_types[64];	// Array for storing the type of argument in each function call
static int call_arg_count = 0;		// Store the number of arguments collected till now

// helper function for generating ir_name for a variable name in the loops 
static const char* ir_name_of(const char* name){
	Symbol* sym = lookup(current_scope, name);
	if(sym && sym->ir_name[0] != '\0'){
		return sym->ir_name;
	}
	return name;
}

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
%type <sval>   for_cond_opt     
%type <sval>   for_inc_opt      
%type <sval>   for_init_opt

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
constructor_decl
    : IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $1,
                                        KIND_CONSTRUCTOR, DT_VOID, yylineno);
            if(sym){
                strncpy(sym->attr.ctor.belongs_to, current_scope->name, 63);
                sym->attr.ctor.param_count = 0;
                sym->attr.ctor.param_list  = NULL;
                snprintf(sym->attr.ctor.entry_label, 32, "ctor_%s", $1);
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if(entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.constructors_list, $1);


            SymTable* cs = create_scope(SCOPE_CONSTRUCTOR, $1, current_scope);
            if(sym) sym->attr.ctor.scope = cs;
            current_scope = cs;
            emit("constr", $1, "", "");           // emit with original name as placeholder
            pending_constr_ir_idx = IR_idx - 1;  // record which quad to patch later
        }
      LPAREN param_list_opt RPAREN block
        {
            

            //Step 1: mangle the constructor symbol name in entity scope
            for(int i = 0; i < HASH_SIZE; i++){
                Symbol* s = current_scope->parent->buckets[i];
                while(s){
                    Symbol* next = s->next;
                    if(strcmp(s->name, $1) == 0 && s->kind == KIND_CONSTRUCTOR
                            && strchr(s->name, '_') == NULL){
                        char newName[80];
                        overloaded_ctor_name(newName, $1, s->attr.ctor.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, $1);
                    }
                    s = next;
                }
            }

            //step2: add mangled name to entity constructors list
            Symbol* entity_sym2 = lookup(current_scope->parent->parent,
                                         current_scope->parent->name);
            if(entity_sym2 && entity_sym2->kind == KIND_ENTITY){
                for(int i = 0; i < HASH_SIZE; i++){
                    for(Symbol* s = current_scope->parent->buckets[i]; s; s = s->next){
                        if(s->kind == KIND_CONSTRUCTOR &&
                           strncmp(s->name, $1, strlen($1)) == 0 &&
                           s->name[strlen($1)] == '_'){
                            if(!name_in_list(entity_sym2->attr.entity.constructors_list, s->name)){
                                add_name(&entity_sym2->attr.entity.constructors_list, s->name);
                            }
                        }
                    }
                }
            }

            //step 3: find the mangled name for this specific constructor by matching param count against current constructor scope
            char mangled_ir[80];
            strcpy(mangled_ir, $1);   /* fallback */
            for(int i = 0; i < HASH_SIZE; i++){
                for(Symbol* s = current_scope->parent->buckets[i]; s; s = s->next){
                    if(s->kind == KIND_CONSTRUCTOR &&
                       strncmp(s->name, $1, strlen($1)) == 0 &&
                       s->name[strlen($1)] == '_'){
                        int pc = 0;
                        for(ParamNode* p = s->attr.ctor.param_list; p; p = p->next) pc++;
                        int cur_pc = 0;
                        for(int b = 0; b < HASH_SIZE; b++)
                            for(Symbol* ps = current_scope->buckets[b]; ps; ps = ps->next)
                                if(ps->kind == KIND_PARAM) cur_pc++;
                        if(pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            //emit the mangled name
            if(pending_constr_ir_idx >= 0){
                strncpy(IR[pending_constr_ir_idx].arg1, mangled_ir, 19);
                pending_constr_ir_idx = -1;
            }

            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_constr", mangled_ir, "", "");
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

method_decl
    /* primitive return type: public int func foo() */
    : access_modifier type FUNC IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $4,
                                        KIND_METHOD, $2, yylineno);
            current_function = sym;
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
            //emit("method",...) moved to closing action

            SymTable* ms = create_scope(SCOPE_METHOD, $4, current_scope);
	    if(sym) sym->attr.method.scope = ms;
            current_scope = ms;

            emit("method", $4, "",""); //placeholder
            pending_method_ir_idx = IR_idx - 1;  // record index
        }
      LPAREN param_list_opt RPAREN block
        {
            
            for (int i = 0; i < HASH_SIZE; i++) {
                Symbol* s = current_scope->parent->buckets[i];
                while (s) {
                    Symbol* next = s->next;  /* save before rehash modifies list */
                    if (strcmp(s->name, $4) == 0 && s->kind == KIND_METHOD
                            && strchr(s->name, '$') == NULL) {
                        char newName[80];
                        overloaded_method_name(newName, $4, s->attr.method.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, $4);  /* fix: move to correct bucket */
                    }
                    s = next;
                }
            }

            /* add mangled name to entity methods list — no duplicates */
            Symbol* entity_sym2 = lookup(current_scope->parent->parent,
                                         current_scope->parent->name);
            if (entity_sym2 && entity_sym2->kind == KIND_ENTITY) {
                for (int i = 0; i < HASH_SIZE; i++) {
                    for (Symbol* s = current_scope->parent->buckets[i]; s; s = s->next) {
                        if (s->kind == KIND_METHOD &&
                            strncmp(s->name, $4, strlen($4)) == 0 &&
                            s->name[strlen($4)] == '_') {
                            if (!name_in_list(entity_sym2->attr.entity.methods_list, s->name)) {
                                add_name(&entity_sym2->attr.entity.methods_list, s->name);
                            }
                        }
                    }
                }
            }

            
            char mangled_ir[80];
            strcpy(mangled_ir, $4);  /* fallback to original name */
            for (int i = 0; i < HASH_SIZE; i++) {
                for (Symbol* s = current_scope->parent->buckets[i]; s; s = s->next) {
                    if (s->kind == KIND_METHOD &&
                        strncmp(s->name, $4, strlen($4)) == 0 &&
                        s->name[strlen($4)] == '_') {
                        /* match by param count against current method scope */
                        int pc = 0;
                        for (ParamNode* p = s->attr.method.param_list; p; p = p->next) pc++;
                        int cur_pc = 0;
                        for (int b = 0; b < HASH_SIZE; b++)
                            for (Symbol* ps = current_scope->buckets[b]; ps; ps = ps->next)
                                if (ps->kind == KIND_PARAM) cur_pc++;
                        if (pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            if(pending_method_ir_idx >= 0){
                strncpy(IR[pending_method_ir_idx].arg1, mangled_ir, 19);
                pending_method_ir_idx = -1;
            }
            
            print_table(current_scope);
            current_scope = current_scope->parent;
	    current_function = NULL;
            emit("end_method", mangled_ir, "", "");
        }

    /* entity return type: public Dog func get() */
    | access_modifier IDENTIFIER FUNC IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $4,
                                        KIND_METHOD, DT_ENTITY, yylineno);
            current_function = sym;
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
            
            SymTable* ms = create_scope(SCOPE_METHOD, $4, current_scope);
	    if(sym) sym->attr.method.scope = ms;
            current_scope = ms;
        }
      LPAREN param_list_opt RPAREN block
        {
            
            for (int i = 0; i < HASH_SIZE; i++) {
                Symbol* s = current_scope->parent->buckets[i];
                while (s) {
                    Symbol* next = s->next;  /* save before rehash modifies list */
                    if (strcmp(s->name, $4) == 0 && s->kind == KIND_METHOD
                            && strchr(s->name, '$') == NULL) {
                        char newName[80];
                        overloaded_method_name(newName, $4, s->attr.method.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, $4);  /* fix: move to correct bucket */
                    }
                    s = next;
                }
            }

            /* add mangled name to entity methods list — no duplicates */
            Symbol* entity_sym2 = lookup(current_scope->parent->parent,
                                         current_scope->parent->name);
            if (entity_sym2 && entity_sym2->kind == KIND_ENTITY) {
                for (int i = 0; i < HASH_SIZE; i++) {
                    for (Symbol* s = current_scope->parent->buckets[i]; s; s = s->next) {
                        if (s->kind == KIND_METHOD &&
                            strncmp(s->name, $4, strlen($4)) == 0 &&
                            s->name[strlen($4)] == '_') {
                            if (!name_in_list(entity_sym2->attr.entity.methods_list, s->name)) {
                                add_name(&entity_sym2->attr.entity.methods_list, s->name);
                            }
                        }
                    }
                }
            }

            
            char mangled_ir[80];
            strcpy(mangled_ir, $4);
            for (int i = 0; i < HASH_SIZE; i++) {
                for (Symbol* s = current_scope->parent->buckets[i]; s; s = s->next) {
                    if (s->kind == KIND_METHOD &&
                        strncmp(s->name, $4, strlen($4)) == 0 &&
                        s->name[strlen($4)] == '_') {
                        int pc = 0;
                        for (ParamNode* p = s->attr.method.param_list; p; p = p->next) pc++;
                        int cur_pc = 0;
                        for (int b = 0; b < HASH_SIZE; b++)
                            for (Symbol* ps = current_scope->buckets[b]; ps; ps = ps->next)
                                if (ps->kind == KIND_PARAM) cur_pc++;
                        if (pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            emit("method", mangled_ir, "", "");
            print_table(current_scope);
            current_scope = current_scope->parent;
	    current_function = NULL;
            emit("end_method", mangled_ir, "", "");
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

object_decl
    : IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON
        {
            Symbol* class_sym = lookup(current_scope, $5);
            if(!class_sym || class_sym->kind != KIND_ENTITY){
                char buf[256];
                snprintf(buf, sizeof(buf),
                    "line %d: Entity '%s' not found to instantiate", yylineno, $5);
                semantic_error(buf);
            }
            Symbol* obj = insert_symbol(current_scope, $2,
                                        KIND_OBJECT, DT_OBJECT, yylineno);
            if(obj){
                strncpy(obj->attr.object.entity_name,
                        class_sym->attr.entity.class_name, 63);
                obj->size = class_sym->attr.entity.class_size;
                current_scope->next_offset += obj->size;
            }

            //build mangled constructor name from arg types
            char mangled_ctor[80];
            strcpy(mangled_ctor, $5);
            strcat(mangled_ctor, "_");
            for(int i = 0; i < call_arg_count; i++){
                char code[2] = {dt_code(call_arg_types[i]), '\0'};
                strcat(mangled_ctor, code);
            }
            //verify if the constructor exists

            if(class_sym && class_sym->kind == KIND_ENTITY){
                SymTable* esc = find_entity_scope(class_sym->attr.entity.class_name);
                if(esc){
                    Symbol* ctor_sym = lookup_local(esc, mangled_ctor);
                    if(!ctor_sym || ctor_sym->kind != KIND_CONSTRUCTOR){
                        fprintf(stderr,
                            "ERROR line %d: No constructor '%s' matches"
                            " the given argument types.\n",
                            yylineno, mangled_ctor);
                    }
                }
            }

            emit("new", $5, "", $2);
            emit("push_ptr", $2, "", "");
            emit("call_constr", mangled_ctor, "", "");
            call_arg_count = 0;
        }
    | type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON
        {
            /* build mangled call name from arg types */
            char mangled_call[80];
            strcpy(mangled_call, $6);
            strcat(mangled_call, "_");
            for (int i = 0; i < call_arg_count; i++) {
                char code[2] = { dt_code(call_arg_types[i]), '\0' };
                strcat(mangled_call, code);
            }

            /*use lookup_local on entity scope directly
               instead of relying on check_method_access chain */
            Symbol* obj_sym = lookup(current_scope, $4);
            if(!obj_sym || obj_sym->kind != KIND_OBJECT){
                fprintf(stderr, "ERROR line %d: '%s' is not an object.\n", yylineno, $4);
            } else {
                SymTable* ent_scope = find_entity_scope(obj_sym->attr.object.entity_name);
                if(!ent_scope){
                    fprintf(stderr, "ERROR line %d: entity '%s' scope not found.\n",
                            yylineno, obj_sym->attr.object.entity_name);
                } else {
                    Symbol* method_sym = lookup_local(ent_scope, mangled_call);
                    if(!method_sym || method_sym->kind != KIND_METHOD){
                        fprintf(stderr, "ERROR line %d: Method '%s' not found in '%s'.\n",
                                yylineno, mangled_call, obj_sym->attr.object.entity_name);
                    } else {
                        /* check private access */
                        if(method_sym->attr.method.access == ACC_PRIVATE){
                            fprintf(stderr, "ERROR line %d: Method '%s' is private.\n",
                                    yylineno, mangled_call);
                        }
                        /* return type check uses mangled symbol directly */
                        if(method_sym->attr.method.return_type != $1){
                            fprintf(stderr,
                                "ERROR line %d: cannot assign result of '%s' (returns %s)"
                                " to '%s' (declared as %s).\n",
                                yylineno, mangled_call,
                                dt_names[method_sym->attr.method.return_type],
                                $2, dt_names[$1]);
                        }
                    }
                }
            }

            Symbol* sym = insert_symbol(current_scope, $2, KIND_VAR, $1, yylineno);
            if(sym){
                sym->is_initialized = 1;
            }
            char* t = genVar();
            emit("push_ptr", $4, "", "");
            emit("call_method", mangled_call, "", t);
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
		fprintf(stderr, "ERROR line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, $2, dt_names[$1], dt_names[last_expr_type]);
	    }
            emit("=", $4, "", $2);
            Symbol* sym = insert_symbol(current_scope, $2,
                                        KIND_VAR, $1, yylineno);
            if (sym){
		sym->is_initialized = 1;
		if(isConstant($4)){
			strncpy(sym->init_value, $4, 63); 
		}
		else{
			sym->init_value[0] = '\0';	// Populate the init value only when it is a constant else compute during the runtime
		}
	    }
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

type
    : INT    { $$ = DT_INT;    }
    | FP     { $$ = DT_FLOAT;  }
    | CHR    { $$ = DT_CHAR;   }
    | STRING { $$ = DT_STRING; }
    | BOOL   { $$ = DT_BOOL;   }
    ;

array_decl
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
                sym->attr.array.dim1 = $6;
                sym->attr.array.dim2 = 0;
                sym->size = datatype_size($1) * $6;
                current_scope->next_offset = sym->offset + sym->size;
                sym->attr.array.init_count = arr_init_count;
                for (int i = 0; i < arr_init_count; i++)
                    strncpy(sym->attr.array.init_values[i], arr_init_vals[i], 31);
            }
             /* ADD: emit IR for each element */
//            for (int i = 0; i < arr_init_count; i++) {
  //              char idx_str[16];
    //            sprintf(idx_str, "%d", i);
   //             char* t1 = genVar();
   //             char* t2 = genVar();
   //             emit("*",  idx_str,"type.width",t1);
   //             emit("[]", $3,t1,t2);
  //              emit("=",arr_init_vals[i],"",t2);
  //          }

            arr_init_count = 0; 
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

            sym->attr.array.init_count = arr_init_count;
			for (int i = 0; i < arr_init_count; i++)
			    strncpy(sym->attr.array.init_values[i], arr_init_vals[i], 31);
		}
 //       for (int i = 0; i < arr2d_rows; i++) {
//		    for (int j = 0; j < arr2d_cols; j++) {
//		        int linear = i * arr2d_cols + j;
//		        char idx_str[16];
//		        sprintf(idx_str, "%d", linear);
//		        char* t1 = genVar();
//		        char* t2 = genVar();
//		        emit("*",  idx_str,"type.width", t1);
//		        emit("[]", $3, t1, t2);
//		        emit("=",  arr_init_vals[linear],"",t2);
//		    }
//		}

		arr_init_count = 0;
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
        if (arr_init_count < 256)
            strncpy(arr_init_vals[arr_init_count++], $3, 31);
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
//            arr_init_count = 0;
	if (arr_init_count < 256)
            strncpy(arr_init_vals[arr_init_count++], $1, 31);
		$$ = 1;
	}
    ;

function_decl
    : func_type FUNC IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_FUNCTION, $1, yylineno);
            if(sym){
                sym->attr.func.return_type = $1;
                sym->attr.func.param_count = 0;
                sym->attr.func.param_list  = NULL;
                snprintf(sym->attr.func.entry_label, 32, "func_%s", $3);
            }
            current_function = sym;

            // emit func with original name as placeholder, record index
            emit("func", $3, "", "");
            pending_func_ir_idx = IR_idx - 1;

            SymTable* fs = create_scope(SCOPE_FUNCTION, $3, current_scope);
            if(sym) sym->attr.func.scope = fs;
            current_scope = fs;
        }
      LPAREN param_list_opt RPAREN block
        {
            // Step 1: mangle the symbol in global scope
            for(int i = 0; i < HASH_SIZE; i++){
                Symbol* s = current_scope->parent->buckets[i];
                while(s){
                    Symbol* next = s->next;
                    if(strcmp(s->name, $3) == 0 && s->kind == KIND_FUNCTION
                            && strchr(s->name, '_') == NULL){
                        char newName[80];
                        overloaded_method_name(newName, $3,
                                               s->attr.func.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, $3);
                    }
                    s = next;
                }
            }

            // Step 2: find mangled name matching current param count
            char mangled_ir[80];
            strcpy(mangled_ir, $3);
            for(int i = 0; i < HASH_SIZE; i++){
                for(Symbol* s = current_scope->parent->buckets[i];
                    s; s = s->next){
                    if(s->kind == KIND_FUNCTION &&
                       strncmp(s->name, $3, strlen($3)) == 0 &&
                       s->name[strlen($3)] == '_'){
                        int pc = 0;
                        for(ParamNode* p = s->attr.func.param_list;
                            p; p = p->next) pc++;
                        int cur_pc = 0;
                        for(int b = 0; b < HASH_SIZE; b++)
                            for(Symbol* ps = current_scope->buckets[b];
                                ps; ps = ps->next)
                                if(ps->kind == KIND_PARAM) cur_pc++;
                        if(pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            // Step 3: patch the placeholder func quad
            if(pending_func_ir_idx >= 0){
                strncpy(IR[pending_func_ir_idx].arg1, mangled_ir, 19);
                pending_func_ir_idx = -1;
            }

            print_table(current_scope);
            current_scope = current_scope->parent;
            current_function = NULL;
            emit("endfunc", "", "", "");
        }

    | IDENTIFIER FUNC IDENTIFIER
        {
            Symbol* sym = insert_symbol(current_scope, $3,
                                        KIND_FUNCTION, DT_ENTITY, yylineno);
            if(sym){
                sym->attr.func.return_type = DT_ENTITY;
                sym->attr.func.param_count = 0;
                sym->attr.func.param_list  = NULL;
                snprintf(sym->attr.func.entry_label, 32, "func_%s", $3);
            }
            current_function = sym;

            emit("func", $3, "", "");
            pending_func_ir_idx = IR_idx - 1;

            SymTable* fs = create_scope(SCOPE_FUNCTION, $3, current_scope);
            if(sym) sym->attr.func.scope = fs;
            current_scope = fs;
        }
      LPAREN param_list_opt RPAREN block
        {
            // same mangling steps
            for(int i = 0; i < HASH_SIZE; i++){
                Symbol* s = current_scope->parent->buckets[i];
                while(s){
                    Symbol* next = s->next;
                    if(strcmp(s->name, $3) == 0 && s->kind == KIND_FUNCTION
                            && strchr(s->name, '_') == NULL){
                        char newName[80];
                        overloaded_method_name(newName, $3,
                                               s->attr.func.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, $3);
                    }
                    s = next;
                }
            }

            char mangled_ir[80];
            strcpy(mangled_ir, $3);
            for(int i = 0; i < HASH_SIZE; i++){
                for(Symbol* s = current_scope->parent->buckets[i];
                    s; s = s->next){
                    if(s->kind == KIND_FUNCTION &&
                       strncmp(s->name, $3, strlen($3)) == 0 &&
                       s->name[strlen($3)] == '_'){
                        int pc = 0;
                        for(ParamNode* p = s->attr.func.param_list;
                            p; p = p->next) pc++;
                        int cur_pc = 0;
                        for(int b = 0; b < HASH_SIZE; b++)
                            for(Symbol* ps = current_scope->buckets[b];
                                ps; ps = ps->next)
                                if(ps->kind == KIND_PARAM) cur_pc++;
                        if(pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            if(pending_func_ir_idx >= 0){
                strncpy(IR[pending_func_ir_idx].arg1, mangled_ir, 19);
                pending_func_ir_idx = -1;
            }

            print_table(current_scope);
            current_scope = current_scope->parent;
            current_function = NULL;
            emit("endfunc", "", "", "");
        }

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
indexed_id
    : IDENTIFIER LBRACKET expression RBRACKET
        {
	    Symbol* asym = require_declared(current_scope, $1, yylineno);
	    char width_str[16];
	    if (asym && asym->kind == KIND_ARRAY)
		sprintf(width_str, "%d", datatype_size(asym->datatype));
	    else
		strcpy(width_str, "type.width");
            char* t1 = genVar(); emit("*",  $3, width_str, t1);
            char* t2 = genVar(); emit("[]", $1, t1,t2);
            $$ = t2;
        }
    | IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET
        {
            Symbol* asym = require_declared(current_scope, $1, yylineno);
	    char cols_str[16];
	    char width_str[16];
	    if(asym && asym->kind == KIND_ARRAY) {
		sprintf(cols_str, "%d", asym->attr.array.dim2);
		sprintf(width_str, "%d", datatype_size(asym->datatype));}
	    else{
		strcpy(cols_str, "array.cols");
		strcpy(width_str, "type.width");
   	    }
            char* t1 = genVar(); emit("*",  $3, cols_str, t1);
            char* t2 = genVar(); emit("+",  t1, $6,t2);
            char* t3 = genVar(); emit("*",  t2, width_str, t3);
            char* t4 = genVar(); emit("[]", $1, t3,t4);
            $$ = t4;
        }
    ;

assignment
    : IDENTIFIER ASSIGN assignment
        {
		// Checking the type before assignment
		Symbol* lhs = require_declared(current_scope, $1, yylineno);
		if(lhs && last_expr_type != DT_UNKNOWN){
			if(lhs->datatype != last_expr_type){
				fprintf(stderr, "ERROR line %d: cannot assign %s to '%s' "
				"(declared as %s).\n", yylineno, dt_names[last_expr_type], $1, dt_names[lhs->datatype]);
			}
		} 
		emit("=", $3, "", ir_name_of($1)); 
		$$ = strdup($1); 
	}
    | IDENTIFIER ADD_ASSIGN assignment
        { 
		Symbol* lhs = require_declared(current_scope, $1, yylineno);
		if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
			fprintf(stderr, "ERROR line %d: type mismatch in '+=' : '%s' is %s but RHS is %s.\n", yylineno, $1, dt_names[lhs->datatype], dt_names[last_expr_type]);
		}
		char* t = genVar(); 
		const char* irn = ir_name_of($1);
		emit("+", irn, $3, t); 
		emit("=", t, "", irn); 
		$$ = t; 
	}
    | IDENTIFIER SUB_ASSIGN assignment
        { 	
		Symbol* lhs = require_declared(current_scope, $1, yylineno);
		if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
			fprintf(stderr, "ERROR line %d: type mismatch in '-=' : '%s' is %s but RHS is %s.\n", yylineno, $1, dt_names[lhs->datatype], dt_names[last_expr_type]);
		}
			
		char* t = genVar(); 
		const char* irn = ir_name_of($1);
		emit("-", irn, $3, t); 
		emit("=", t, "", irn); 	
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
				SymTable* esc = find_entity_scope(entity_name);
				Symbol* field = esc ? lookup_local(esc, $3):NULL;
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
		check_field_access($1,$3,yylineno); 
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
        {
          char* t = genVar(); emit(">", $1, $3, t); $$ = t;
            
        }
    | arith_expr LT arith_expr
        {
                char* t = genVar(); emit("<", $1, $3, t); $$ = t;
        }
    | arith_expr EQ arith_expr
        {
                char* t = genVar(); emit("==", $1, $3, t); $$ = t;
        }
    | arith_expr { $$ = $1; }
    ;

arith_expr
    : arith_expr PLUS term
       {
                char* t = genVar();
                emit("+", $1, $3, t);
                $$ = t;
             
        }
    | arith_expr MINUS term
        {  
                char* t = genVar();
                emit("-", $1, $3, t);
                $$ = t;
        }
    | term { $$ = $1; }
    ;

term
    : term MUL factor
        {
                char* t = genVar();
                emit("*", $1, $3, t);
                $$ = t;
        }
    | term DIV factor
        {
                char* t = genVar();
                emit("/", $1, $3, t);
                $$ = t;
        }
    | term MOD factor
        {
                char* t = genVar();
                emit("%", $1, $3, t);
                $$ = t;
        }
    | factor { $$ = $1; }
    ;

factor
    : MINUS factor
        {
            char* t = genVar();
            emit("-", "0", $2, t);
            $$ = t;
            last_expr_type = DT_INT;
        }
    | IDENTIFIER LPAREN arg_list_opt RPAREN
    {
        char* t = genVar();

        // Build mangled name from collected arg types
        char mangled_call[80];
        strcpy(mangled_call, $1);
        strcat(mangled_call, "_");
        for(int i = 0; i < call_arg_count; i++){
            char code[2] = {dt_code(call_arg_types[i]), '\0'};
            strcat(mangled_call, code);
        }

        // Try mangled name first, fall back to original name
        Symbol* fsym = lookup(current_scope, mangled_call);
        if(!fsym){
            fsym = lookup(current_scope, $1);
        }

        if(!fsym){
            fprintf(stderr,
                "ERROR line %d: call to undeclared function '%s'.\n",
                yylineno, $1);
            last_expr_type = DT_UNKNOWN;
        }
        else if(fsym->kind != KIND_FUNCTION && fsym->kind != KIND_METHOD){
            fprintf(stderr,
                "ERROR line %d: '%s' is not a function.\n", yylineno, $1);
            last_expr_type = DT_UNKNOWN;
        }
        else{
            ParamNode* ep = (fsym->kind == KIND_FUNCTION)
                            ? fsym->attr.func.param_list
                            : fsym->attr.method.param_list;
            int expected_count = (fsym->kind == KIND_FUNCTION)
                                  ? fsym->attr.func.param_count
                                  : fsym->attr.method.param_count;
            if(call_arg_count != expected_count){
                fprintf(stderr,
                    "ERROR line %d: function '%s' expects %d arg(s),"
                    " got %d.\n",
                    yylineno, $1, expected_count, call_arg_count);
            }
            else{
                for(int i = 0; i < call_arg_count && ep;
                    i++, ep = ep->next){
                    if(call_arg_types[i] != DT_UNKNOWN &&
                       call_arg_types[i] != ep->datatype){
                        fprintf(stderr,
                            "ERROR line %d: function '%s' argument %d"
                            " - expected %s, but got %s.\n",
                            yylineno, $1, i+1,
                            dt_names[ep->datatype],
                            dt_names[call_arg_types[i]]);
                    }
                }
            }
            last_expr_type = (fsym->kind == KIND_FUNCTION)
                              ? fsym->attr.func.return_type
                              : fsym->attr.method.return_type;
        }

        // emit with mangled name if overloaded, original otherwise
        const char* emit_name = (fsym && strchr(fsym->name, '_'))
                                 ? fsym->name : $1;
        emit("call", emit_name, "", t);
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
            /* get entity name safely */
            const char* entity_name = obj->attr.object.entity_name;
            if (!entity_name || entity_name[0] == '\0') {
                fprintf(stderr,
                    "ERROR line %d: object '%s' has no entity type.\n",
                    yylineno, $1);
            }
            else {
                /* look up the entity symbol in global scope */
                Symbol* cls = lookup(global_scope, entity_name);
                if (!cls || cls->kind != KIND_ENTITY) {
                    fprintf(stderr,
                        "ERROR line %d: entity '%s' not defined.\n",
                        yylineno, entity_name);
                }
                else {
                    /* get the entity's scope (stored in EntityAttr) */
                    SymTable* escope = cls->attr.entity.scope;
                    if (!escope) {
                        fprintf(stderr,
                            "ERROR line %d: entity '%s' has no scope.\n",
                            yylineno, entity_name);
                    }
                    else {
                        /* look up the field inside entity scope */
                        Symbol* field = lookup_local(escope, $3);
                        if (!field || field->kind != KIND_FIELD) {
                            fprintf(stderr,
                                "ERROR line %d: '%s' is not a field of '%s'.\n",
                                yylineno, $3, entity_name);
                        }
                        else {
                            //check private access
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
		Symbol* s = require_declared(current_scope, $1, yylineno);
		last_expr_type = s ? s->datatype : DT_UNKNOWN;
		$$ = strdup(ir_name_of($1)); 
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
    | ELSE IF LPAREN           /* ← ADD THIS: treat "else if" same as "elif" */
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
        { emit("ifFalse", $5, "", topFalse()); }
      RPAREN
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
      block
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
      elif_list else_opt
    | /* empty */
    ;

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
    : for_init_opt SEMICOLON
        {
            char* b = getLabel();
            char* e = getLabel();
            pushIfLabels(b, e);
            emit("label", "", "", b);
        }
      for_cond_opt SEMICOLON
        {
            emit("ifFalse", $4 ? $4 : "1", "", topEnd());
            forDepth++;
            forIncIdx[forDepth]      = 0;
            inForIncrement[forDepth] = 1;
        }
      for_inc_opt
        { inForIncrement[forDepth] = 0; }
      RPAREN
    ;

for_init_opt
    : var_decl_no_semi	{ $$ = NULL; }
    | expression	{ $$ = $1; }
    | /* empty */   { $$ = NULL; }
    ;

for_cond_opt
    : expression    { $$ = $1; }
    | /* empty */   { $$ = strdup("1"); }   /* empty condition = always true */
    ;

for_inc_opt
    : expression	{ $$ = $1; }
    | /* empty */	{ $$ = NULL; }
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
            	//emit("=", $4, "", $2);
           	Symbol* sym = insert_symbol(current_scope, $2,
                                        KIND_VAR, $1, yylineno);
            	if (sym) sym->is_initialized = 1;
		emit("=", $4, "", ir_name_of($2));
        }
    ;

io_stmt
    : IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON
        {
		Symbol* var = require_declared(current_scope, $1, yylineno);
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

int main(int argc, char* argv[]) {
    	global_scope  = create_scope(SCOPE_GLOBAL, "global", NULL);
    	current_scope = global_scope;


        //check --oalloc flag
	    // Parse the -S flag (stats mode)
	    int compare_mode = 0;
	    for(int i = 1; i < argc; i++){
	    	if(strcmp(argv[i], "-S") == 0){
	    		compare_mode = 1;
	    		printf("Mode: Comparision (standard + optimal both generated)\n");
	    	}
            else if(strcmp(argv[i],"--oalloc")==0){
                use_optimized_regalloc=1;
                printf("Register allocation: OPTIMIZED\n");
            }
	    }	

    	//global_scope  = create_scope(SCOPE_GLOBAL, "global", NULL);
    	//current_scope = global_scope;
    	yyin = stdin;
    	yyparse();

    	printf("\n========== GLOBAL SCOPE ==========\n");
    	print_table(global_scope);
         
int opt_level = 3;   //default=O3
int do_python=0;
for (int i = 1; i < argc; i++) {
    if      (strcmp(argv[i], "-O0") == 0) opt_level = 0;
    else if (strcmp(argv[i], "-O1") == 0) opt_level = 1;
    else if (strcmp(argv[i], "-O2") == 0) opt_level = 2;
    else if (strcmp(argv[i], "-O3") == 0) opt_level = 3;
    else if (strcmp(argv[i], "-py") == 0) do_python = 1;
}
printf("Running Optimizations (-%c%d)\n", 'O', opt_level);

//O1
if (opt_level >= 1) {
    algebraic_simplification();
    constant_folding();
    constant_propagation();
    copy_propagation(opt_level);
    //second pass to clean up copies exposed by propagation 
    constant_folding();
    constant_propagation();
}

//O2
if (opt_level >= 2) {
    common_subexpression_elimination();
    dead_code_elimination();
}

//O3
if (opt_level >= 3) {
    strength_reduction();
    loop_invariant_code_motion();
    induction_variable_elimination();
    dead_code_elimination();   // final cleanup after loop optimizations
}
       printf("\n========== IR Code Visualization Section ==========\n");
	print_original_IR();
	print_opt_IR();

    /*printf("\nParsing Successful\nGenerated quadruple table:\n");
    printf("%-15s %-15s %-15s %-15s\n", "OP", "ARG1", "ARG2", "RESULT");
    for (int i = 0; i < IR_idx; i++)
        printf("%-15s %-15s %-15s %-15s\n",
               IR[i].op, IR[i].arg1, IR[i].arg2, IR[i].result);*/
    	FILE *asm_file = fopen("output.s", "w");
    	if (!asm_file) {
        	perror("Failed to open assembly file");
       		 return 1;
    	}
    	asmSetOutput(asm_file);
	use_template_matching = 1;	// Turning on the optimal instruction selection
    	printf("\nGenerating RISC-V Assembly...\n");
    	generateASM(); 

    fclose(asm_file);
    printf("Assembly code saved to 'output.s'\n");
   
 if (do_python) {
    FILE* py_file = fopen("output.py", "w");
    if (!py_file) {
        perror("Failed to open output.py");
    } else {
        printf("\nTranspiling to Python...\n");
        transpile_to_python(py_file);
        fclose(py_file);
        printf("Python code saved to 'output.py'\n");
    }
}

	if(compare_mode){
		FILE* std_file = fopen("output_standard.s", "w");
		if(!std_file){
			perror("Failed to open output_standard.s");
			return 1;
		}
		asmSetOutput(std_file);
		use_template_matching = 0;	// turn off the optimal ins selection
		printf("Generating Assembly code using standard if/else chain");
		generateASM();
		fclose(std_file);
		printf("Standard Assembly code saved to 'output_standard.s' file\n");
	
		// Now compare the code from two files using a function
			
	}
   	return 0;
}

