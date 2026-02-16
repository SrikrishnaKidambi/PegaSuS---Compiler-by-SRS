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
void emit_forinc(char* op,char* arg1, char* arg2, char* result);
// Function for generating new labels
char* getLabel();

// variable that tracks the number of labels generated
int labelCnt = 0;

int forDepth = -1;
int inForIncrement[10];
Quad forIncBuff[10][100];
int forIncIdx[10];

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

%token INT FP CHR STRING BOOL VOID
%token IF ELIF ELSE FOR
%token TRUE FALSE
%token FEED SHOW
%token RETURN
%token SEQ1 SEQ2 FUNC


%token ENTITY NEW PUBLIC PRIVATE THIS DOT


%token <sval> IDENTIFIER
%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <cval> CHAR_LITERAL
%token <sval> STRING_LITERAL
//operators
%token PLUS MINUS MUL DIV MOD
%token GT LT EQ
%token AND OR NOT
%token BITAND BITOR
%token ASSIGN ADD_ASSIGN SUB_ASSIGN
//seperators and brackets
%token SEMICOLON COMMA
%token LPAREN RPAREN
%token LBRACE RBRACE
%token LBRACKET RBRACKET
//precedence and associativity setting

%type <sval> expression arith_expr term factor assignment indexed_id logic_expr rel_expr bitwise_expr

%left OR
%left AND
%left BITOR
%left BITAND
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
    | entity_decl
    | object_decl
    ;

entity_decl
    : ENTITY IDENTIFIER LBRACE entity_body RBRACE
    ;

entity_body
    : entity_body entity_member
    |
    ;

entity_member
    : constructor_decl
    | method_decl
    | access_var_decl
    ;

constructor_decl
    : IDENTIFIER LPAREN param_list_opt RPAREN block
    ;

method_decl
    : access_modifier type FUNC IDENTIFIER LPAREN param_list_opt RPAREN block   
    ;

access_var_decl
    : access_modifier type IDENTIFIER SEMICOLON
    ;

access_modifier
    : PUBLIC
    | PRIVATE
    ;

object_decl
    : IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON
    ;

arg_list_opt
    : arg_list
    |
    ;

arg_list
    : arg_list COMMA expression
    | expression
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
	{
		//no need to emit anything here and later stage add code for symbol table here
	}
    | type IDENTIFIER ASSIGN expression SEMICOLON
	{
		//code for symbol table comes here
		emit("=",$4,"",$2);
	}
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
    | IDENTIFIER
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
		emit("=", $3, "", $1);
		$$ = strdup($1);
	}
    | IDENTIFIER ADD_ASSIGN assignment
	{
		char* temp = genVar();
		emit("+", $1, $3, temp);
		emit("=", temp, "", $1);
		$$ = temp;
	}

    | IDENTIFIER SUB_ASSIGN assignment
        {
                char* temp = genVar();
                emit("-", $1, $3, temp);
                emit("=", temp, "", $1);
		$$ = temp;
        }
    | indexed_id ASSIGN assignment
    | THIS DOT IDENTIFIER ASSIGN assignment
        {
                emit("", $5, "", $3);  
        }
    | IDENTIFIER DOT IDENTIFIER ASSIGN assignment
        {
                emit("", $5, "", $3);
        }
    | logic_expr
	{
		$$ = $1;
	}
    ;

logic_expr
    : logic_expr OR logic_expr
	{
		char* temp = genVar();
		emit("||",$1,$3,temp); //temp = $1 || $2
		$$ = temp;
	}
    | logic_expr AND logic_expr
	{
                char* temp = genVar();
                emit("&&",$1,$3,temp); //temp = $1 && $2
                $$ = temp;
        }
    | NOT logic_expr
	{
                char* temp = genVar();
                emit("!",$2,"",temp); //temp = !$2
                $$ = temp;
        }
    | bitwise_expr
	{
		$$ = $1;
	}
    ;

bitwise_expr
    : bitwise_expr BITAND bitwise_expr
	{
		char* temp = genVar();
		emit("&", $1, $3 , temp); //temp = $1 & $3
		$$ = temp;
	}
    | bitwise_expr BITOR bitwise_expr
	{
		char* temp = genVar();
		emit("|",$1,$3, temp);
		$$ = temp;
	}
    | rel_expr
	{
		$$ = $1;
	}
    ;
rel_expr
    : arith_expr GT arith_expr
	{
		char* temp = genVar();
		emit(">", $1, $3, temp); //temp = $1 > $3
		$$ = temp;
	}
    | arith_expr LT arith_expr
	{
		char* temp = genVar();
		emit("<",$1 , $3, temp); //temp = $1 < $3
		$$ = temp;
	}
    | arith_expr EQ arith_expr
	{
		char* temp = genVar();
		emit("==",$1, $3, temp); //temp = $1 == $3
		$$ = temp;
	}
    | arith_expr
	{
		$$ = $1;
	}
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
		// emit("label", "", "", topFalse());
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
		
		//emit("goto", "", "", topEnd());
		emit("label", "", "", prevFalseLabel);
		falseStack[topPtr] = nextFalseLabel;
	}
      expression RPAREN
      block
	{
		emit("goto", "", "", topEnd());
	}
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
    : FOR LPAREN 
	expression 
	{
		char* Lbegin = getLabel();
		char* Lend = getLabel();
		
		pushIfLabels(Lbegin,Lend);
		
		emit("label","","",Lbegin);
	}		
	SEMICOLON 
	expression 
	{
		emit("ifFalse",$6,"",topEnd());
	}
	SEMICOLON 
	{
		forDepth++;
		forIncIdx[forDepth] = 0;
		inForIncrement[forDepth]=1;
	}
	expression
	{
		inForIncrement[forDepth]=0;
	}
	RPAREN block
	{
		for(int i=0;i<forIncIdx[forDepth];i++){
			strcpy(IR[IR_idx].op,forIncBuff[forDepth][i].op);
			strcpy(IR[IR_idx].arg1, forIncBuff[forDepth][i].arg1);
                	strcpy(IR[IR_idx].arg2, forIncBuff[forDepth][i].arg2);
                	strcpy(IR[IR_idx].result, forIncBuff[forDepth][i].result);
                	IR_idx++;
            	}
            	forDepth--;
		emit("goto","","",topFalse());
		emit("label","","",topEnd());
		popIfLabels();
	}
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
void emit_forinc(char* op, char* arg1, char* arg2, char* result){
	strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].op,op);
	strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].arg1,arg1);
	strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].arg2, arg2);
    	strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].result, result);
	forIncIdx[forDepth]++;
}

void emit(char* op, char* arg1, char* arg2, char* result) {
//	printf("EMIT called: %s %s %s %s  (flag=%d)\n",op,arg1,arg2,result,inForIncrement);	
//	fflush(stdout);
	if(forDepth >= 0 && inForIncrement[forDepth]){
		emit_forinc(op,arg1,arg2,result);
		return;
	}

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
    yyparse();
    //inForIncrement=0;
    printf("Parsing Successful\n");
    printf("Generated quadruple table:\n");

    printf("%-10s %-10s %-10s %-10s\n", "OP", "ARG1", "ARG2", "RESULT");
 //   printf("%d",IR_idx);
    for(int i = 0; i < IR_idx; i++){
        printf("%-10s %-10s %-10s %-10s\n",
               IR[i].op,
               IR[i].arg1,
               IR[i].arg2,
               IR[i].result);
    }

    return 0;
}
