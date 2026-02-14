%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;

// Structure of IR code representation

typedef struct {
	char op[10];
	char arg1[20];
	char arg2[20];
	char result[20];
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

// Function for generating new labels
char* getLabel();

// variable that tracks the number of labels generated
int labelCnt = 0;

char* falseStack[100];
char* endStack[100];
int topPtr = -1;
void pushIfLabels(char* falseLabel, char* endLabel) {
	topPtr++;
	falseStack[topPtr] = falseLabel;
	endStack[topPtr] = endLabel;
}

char* topFalse() {
	return falseStack[topPtr];
}

char* topEnd() {
	return endStack[topPtr];
}

void popIfLabels() {
	topPtr--;
}

%}

%union {
    	int ival;
    	float fval;
    	char cval;
    	char* sval;
    	char* place;
}
// carry the syntax based tokens and not value based tokens
%token INT FP CHR STRING BOOL VOID
%token IF ELIF ELSE FOR
%token TRUE FALSE
%token FEED SHOW
%token RETURN
%token SEQ1 SEQ2 FUNC
// carry value based tokens
%token <sval> IDENTIFIER
%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <cval> CHAR_LITERAL
%token <sval> STRING_LITERAL
//operators
%token PLUS MINUS MUL DIV MOD
%token GT LT EQ
%token AND OR NOT
%token ASSIGN ADD_ASSIGN SUB_ASSIGN
//seperators and brackets
%token SEMICOLON COMMA
%token LPAREN RPAREN
%token LBRACE RBRACE
%token LBRACKET RBRACKET
//precedence and associativity setting

%type <sval> expression arith_expr term factor assignment indexed_id logic_expr

%left OR
%left AND
%left EQ GT LT
%left PLUS MINUS
%left MUL DIV MOD
%right NOT

%%

program
    : program element
    |
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
    | SEMICOLON  // this is like do nothing statement
    ;

expression
    : assignment
    ;

indexed_id
    : IDENTIFIER LBRACKET expression RBRACKET
    ;

assignment
    : IDENTIFIER ASSIGN assignment
	{
		emit("", $3, "", $1);
		$$ = strdup($1);
	}
    | IDENTIFIER ADD_ASSIGN assignment
	{
		char* temp = genVar();
		emit("+", $1, $3, temp);
		emit("", temp, "", $1);
	}

    | IDENTIFIER SUB_ASSIGN assignment
	{
                char* temp = genVar();
                emit("-", $1, $3, temp);
                emit("", temp, "", $1);
        }
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
	{
		char* temp = genVar();
		emit("+", $1, $3, temp);
		$$ = temp;
	}
    | arith_expr MINUS term
	{
		char* temp = genVar();
		emit("-", $1, $3, temp);
		$$ = temp;
	}
    | term
	{
		$$ = $1;
	}
    ;

term
    : term MUL factor
	{
		char* temp = genVar();
		emit("*", $1, $3, temp);
		$$ = temp;
	}
    | term DIV factor
	{
		char* temp = genVar();
		emit("/", $1, $3, temp);
		$$ = temp;
	}
    | term MOD factor
	{
		char* temp = genVar();
		emit("%", $1, $3, temp);
		$$ = temp;
	}
    | factor
	{
		$$ = $1;
	}
    ;

factor
    : IDENTIFIER
	{
		$$ = strdup($1);
	}
    | indexed_id
    | INT_LITERAL
	{
		char buffer[20];
		sprintf(buffer, "%d", $1);
		$$ = strdup(buffer);
	}	
    | FLOAT_LITERAL
	{
		char buffer[20];
		sprintf(buffer, "%f", $1);
		$$ = strdup(buffer);
	}
    | CHAR_LITERAL
	{
		char buffer[20];
		sprintf(buffer, "'%c'", $1);
		$$ = strdup(buffer);
	}
    | STRING_LITERAL
	{
		$$ = strdup($1);
	}
    |TRUE
	{
                char buffer[20];
                sprintf(buffer, "%d", 1);
                $$ = strdup(buffer);
        }
    | FALSE
	{
                char buffer[20];
                sprintf(buffer, "%d", 0);
                $$ = strdup(buffer);
        }	
    | LPAREN expression RPAREN
	{
		$$ = $2;
	}
    ;

if_stmt
    : IF LPAREN 
	{
		char* falseLabel = getLabel();
		char* Lend = getLabel();
		pushIfLabels(falseLabel, Lend);
	}
      expression 
      RPAREN 
      block
	{
		emit("goto", "", "", topEnd());
		emit("label", "", "", topFalse());
	}
      elif_list
      else_opt
	{
		emit("label", "", "", topEnd());
		popIfLabels();
	}
    ;

elif_list
    : ELIF LPAREN 
	{
		char* prevFalseLabel = topFalse();
		char* nextFalseLabel = getLabel();
		
		emit("goto", "", "", topEnd());
		emit("label", "", "", prevFalseLabel);
		
		falseStack[topPtr] = nextFalseLabel;
	}
      expression RPAREN
      block
      elif_list
    |
    ;

else_opt
    : ELSE 
	{
		emit("label", "", "", topFalse());
	}
      block
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

char* getLabel() {
	char newLabel[20];
	sprintf(newLabel, "L%d", labelCnt++);
	return strdup(newLabel);
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
    	printf("Parsing Successful\n");
	printf("Generated three address code:\n");
	for(int i = 0; i < IR_idx; i++){
		printf("%s = %s %s %s\n", IR[i].result, IR[i].arg1, IR[i].op, IR[i].arg2);
	}
    	return 0;
}

