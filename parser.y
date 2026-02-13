%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;

// Structure of IR code representation

typedef struct {
	char op[10],
	char arg1[20],
	char arg2[20],
	char result[20]
} Quad;

// Initialising the array for storing the Quadraples
Quad IR[10000];

// Index that points to the quadraple indicating representing the IR code
int IR_idx = 0;

// Variable that holds the count of number of temporary variables generated
int tempVarCnt = 0;

// Defining the function for generating new temporary variable
char* genVar();

// Function for emitting the IR code in the form of Quadruples
void emit(char* op, char* arg1, char* arg2, char* result);

%}

%union {
    int ival;
    float fval;
    char cval;
    char* sval;
}

%token INT FP CHR STRING BOOL VOID
%token IF ELIF ELSE FOR
%token TRUE FALSE
%token FEED SHOW
%token RETURN
%token SEQ1 SEQ2 FUNC

%token <sval> IDENTIFIER
%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <cval> CHAR_LITERAL
%token <sval> STRING_LITERAL

%token PLUS MINUS MUL DIV MOD
%token GT LT EQ
%token AND OR NOT
%token ASSIGN ADD_ASSIGN SUB_ASSIGN
%token SEMICOLON COMMA
%token LPAREN RPAREN
%token LBRACE RBRACE
%token LBRACKET RBRACKET

%left OR
%left AND
%left EQ GT LT
%left PLUS MINUS
%left MUL DIV MOD
%right NOT

%%

program
    : program element
    |			{ s = "hello"; }
    ;

element
    : statement
    | function_decl
    ;

statement
    : var_decl
    | array_decl
    | expr_stmt
    | if_stmt
    | for_stmt
    | io_stmt
    | return_stmt
    | block
    ;

block
    : LBRACE stmt_list RBRACE
    | LBRACE RBRACE
    ;

stmt_list
    : stmt_list statement
    | statement
    ;

var_decl
    : type id_list SEMICOLON
    | type IDENTIFIER ASSIGN expression SEMICOLON
    ;

id_list
    : id_list COMMA IDENTIFIER
    | IDENTIFIER
    ;

type
    : INT
    | FP
    | CHR
    | STRING
    | BOOL
    ;

array_decl
    : type SEQ1 IDENTIFIER ASSIGN array_init SEMICOLON
    | type SEQ1 IDENTIFIER LBRACKET INT_LITERAL RBRACKET SEMICOLON
    | type SEQ2 IDENTIFIER LBRACKET INT_LITERAL RBRACKET LBRACKET INT_LITERAL RBRACKET SEMICOLON
    ;

array_init
    : LBRACE expr_list RBRACE
    ;

expr_list
    : expr_list COMMA expression
    | expression
    ;

function_decl
    : func_type FUNC IDENTIFIER LPAREN param_list_opt RPAREN block
    ;

func_type
    : type
    | VOID
    ;

param_list_opt
    : param_list
    |
    ;

param_list
    : param_list COMMA param
    | param
    ;

param
    : type IDENTIFIER
    ;

return_stmt
    : RETURN expression SEMICOLON
    | RETURN SEMICOLON
    ;

expr_stmt
    : expression SEMICOLON
    | SEMICOLON
    ;

expression
    : assignment
    ;

indexed_id
    : IDENTIFIER LBRACKET expression RBRACKET
    ;

assignment
    : IDENTIFIER ASSIGN assignment
    | IDENTIFIER ADD_ASSIGN assignment
    | IDENTIFIER SUB_ASSIGN assignment
    | indexed_id ASSIGN assignment
    | logic_expr
    ;

logic_expr
    : logic_expr OR logic_expr
    | logic_expr AND logic_expr
    | NOT logic_expr
    | rel_expr
    ;

rel_expr
    : rel_expr GT arith_expr
    | rel_expr LT arith_expr
    | rel_expr EQ arith_expr
    | arith_expr
    ;

arith_expr
    : arith_expr PLUS term
    | arith_expr MINUS term
    | term
    ;

term
    : term MUL factor
    | term DIV factor
    | term MOD factor
    | factor
    ;

factor
    : IDENTIFIER
    | indexed_id
    | INT_LITERAL
    | FLOAT_LITERAL
    | CHAR_LITERAL
    | STRING_LITERAL
    | TRUE
    | FALSE
    | LPAREN expression RPAREN
    ;

if_stmt
    : IF LPAREN expression RPAREN block elif_list else_opt
    ;

elif_list
    : elif_list ELIF LPAREN expression RPAREN block
    |
    ;

else_opt
    : ELSE block
    |
    ;

for_stmt
    : FOR LPAREN expression SEMICOLON expression SEMICOLON expression RPAREN block
    ;

io_stmt
    : IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON
    | SHOW LPAREN expression RPAREN SEMICOLON
    ;

%%

/* Implementing the helper functions "genVar" and "emit" for
   generating IR code in the form of Quadruples
*/

char* genVar() {
	char newVar[20];
	sprintf(newVar, "t%d", tempVarCnt++);
	return strdup(newVar);
}

void emit(char* op, char* arg1, char* arg2, char* result) {
	strcpy(IR[IR_idx].op, op);
	strcpy(IR[IR_idx].arg1, arg1);
	strcpy(IR[IR_idx].arg2, arg2);
	strcpy(IR[IR_idx].result, result);
	IR_idx++;
}

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}

int main() {
    	yyin = stdin;
	char* s = "hi";	
    	// Starting the process of parsing the code. 
    	yyparse();
	printf("String s:%s\n", s);
    	printf("Parsing Successful\n");
    	return 0;
}

