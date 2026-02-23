%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;

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

void pushIfLabels(char* falseLabel, char* endLabel) {
    topPtr++;
    falseStack[topPtr] = strdup(falseLabel);
    endStack[topPtr] = strdup(endLabel);
}

char* topFalse() { return falseStack[topPtr]; }
char* topEnd() { return endStack[topPtr]; }

void popIfLabels() {
    free(falseStack[topPtr]);
    free(endStack[topPtr]);
    topPtr--;
}
%}

%union {
    int ival;
    float fval;
    char cval;
    char* sval;
}

%token INT FP CHR STRING BOOL VOID
%token IF ELIF ELSE FOR TRUE FALSE FEED SHOW RETURN
%token SEQ1 SEQ2 FUNC ENTITY NEW PUBLIC PRIVATE THIS DOT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET SEMICOLON COMMA
%token <sval> IDENTIFIER STRING_LITERAL
%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <cval> CHAR_LITERAL

%right ASSIGN ADD_ASSIGN SUB_ASSIGN
%left OR
%left AND
%left BITOR
%left BITAND
%left EQ GT LT
%left PLUS MINUS
%left MUL DIV MOD
%right NOT

%type <sval> expression arith_expr term factor assignment logic_expr rel_expr bitwise_expr indexed_id

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
    ;

entity_decl
    : ENTITY IDENTIFIER
      {
        emit("entity",$2,"","");
      }
      LBRACE entity_body RBRACE
      {
        emit("end_entity",$2,"","");
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
       emit("constr",$1,"","");
      }
       LPAREN param_list_opt RPAREN block
       {
        emit("end_constr",$1,"","");
       }
    ;

method_decl
    : access_modifier type FUNC IDENTIFIER
      {
        emit("method",$4,"","");
      }
      LPAREN param_list_opt RPAREN block
      {
        emit("end_method",$4,"","");
      }
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
       {
         emit("new",$5,"",$2);
         emit("call_constr",$5,"",$2);
       }
    | type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list RPAREN SEMICOLON
       {
        char* t=genVar();
        emit("push_ptr",$4,"","");
        emit("call_method",$6,"",t);
        emit("=",t,"",$2);
       }
    ;

arg_list_opt
    : arg_list
    | /* empty */
    ;

arg_list
    : arg_list COMMA expression
      {
       emit("arg",$3,"","");
      }

    | expression
      {
       emit("arg",$1,"","");
      }
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
        { emit("=", $4, "", $2); }
    ;

id_list
    : id_list COMMA IDENTIFIER
    | IDENTIFIER
    ;

type
    : INT | FP | CHR | STRING | BOOL | IDENTIFIER
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
    : func_type FUNC IDENTIFIER
       {
         emit ("func",$3,"",""); 
       }
       LPAREN param_list_opt RPAREN block
        {
         emit("endfunc","","",""); 
        }
    ;

func_type
    : type
    | VOID
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
    : type IDENTIFIER
      {
        emit("param",$2,"","");
      }
    ;

return_stmt
    : RETURN expression SEMICOLON
      { 
       emit("return",$2,"","");
      }
    | RETURN SEMICOLON
      {
        emit("return","","","");
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
    : IDENTIFIER LBRACKET expression RBRACKET { $$ = strdup($1); }
    ;

assignment
    : IDENTIFIER ASSIGN assignment
        { emit("=", $3, "", $1); $$ = strdup($1); }
    | IDENTIFIER ADD_ASSIGN assignment
        { char* t = genVar(); emit("+", $1, $3, t); emit("=", t, "", $1); $$ = t; }
    | IDENTIFIER SUB_ASSIGN assignment
        { char* t = genVar(); emit("-", $1, $3, t); emit("=", t, "", $1); $$ = t; }
    | indexed_id ASSIGN assignment { $$ = $3; }
    | THIS DOT IDENTIFIER ASSIGN assignment { emit("set_field", "this", $3, $5); $$ = $5; }
    | IDENTIFIER DOT IDENTIFIER ASSIGN assignment { emit("set_field", $1, $3, $5); $$ = $5; }
    | logic_expr { $$ = $1; }
    ;

logic_expr
    : logic_expr OR logic_expr { char* t = genVar(); emit("||", $1, $3, t); $$ = t; }
    | logic_expr AND logic_expr { char* t = genVar(); emit("&&", $1, $3, t); $$ = t; }
    | NOT logic_expr { char* t = genVar(); emit("!", $2, "", t); $$ = t; }
    | bitwise_expr { $$ = $1; }
    ;

bitwise_expr
    : bitwise_expr BITAND bitwise_expr { char* t = genVar(); emit("&", $1, $3, t); $$ = t; }
    | bitwise_expr BITOR bitwise_expr { char* t = genVar(); emit("|", $1, $3, t); $$ = t; }
    | rel_expr { $$ = $1; }
    ;

rel_expr
    : arith_expr GT arith_expr { char* t = genVar(); emit(">", $1, $3, t); $$ = t; }
    | arith_expr LT arith_expr { char* t = genVar(); emit("<", $1, $3, t); $$ = t; }
    | arith_expr EQ arith_expr { char* t = genVar(); emit("==", $1, $3, t); $$ = t; }
    | arith_expr { $$ = $1; }
    ;

arith_expr
    : arith_expr PLUS term { char* t = genVar(); emit("+", $1, $3, t); $$ = t; }
    | arith_expr MINUS term { char* t = genVar(); emit("-", $1, $3, t); $$ = t; }
    | term { $$ = $1; }
    ;

term
    : term MUL factor { char* t = genVar(); emit("*", $1, $3, t); $$ = t; }
    | term DIV factor { char* t = genVar(); emit("/", $1, $3, t); $$ = t; }
    | term MOD factor { char* t = genVar(); emit("%", $1, $3, t); $$ = t; }
    | factor { $$ = $1; }
    ;

factor
    : IDENTIFIER LPAREN arg_list_opt RPAREN
      {
        char* t=genVar();
        emit("call",$1,"",t);
        $$=t;
      } 
    | IDENTIFIER { $$ = strdup($1); }
    | indexed_id { $$ = $1; }
    | INT_LITERAL { char b[20]; sprintf(b, "%d", $1); $$ = strdup(b); }
    | FLOAT_LITERAL { char b[20]; sprintf(b, "%f", $1); $$ = strdup(b); }
    | CHAR_LITERAL { char b[20]; sprintf(b, "'%c'", $1); $$ = strdup(b); }
    | STRING_LITERAL { $$ = strdup($1); }
    | TRUE { $$ = strdup("1"); }
    | FALSE { $$ = strdup("0"); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

if_stmt
    : IF LPAREN 
        { char* f = getLabel(); char* e = getLabel(); pushIfLabels(f, e); }
      expression 
        { emit("ifFalse", $4, "", topFalse()); }
      RPAREN block
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
      elif_list else_opt
        { emit("label", "", "", topEnd()); popIfLabels(); }
    ;

elif_list
    : ELIF LPAREN 
        { char* n = getLabel(); free(falseStack[topPtr]); falseStack[topPtr] = strdup(n); }
      expression 
        { emit("ifFalse", $4, "", topFalse()); }
      RPAREN block
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
      elif_list
    | /* empty */
    ;

else_opt
    : ELSE block
    | /* empty */
    ;

for_stmt
    : FOR LPAREN for_header block
        {
            for(int i=0; i<forIncIdx[forDepth]; i++) {
                emit(forIncBuff[forDepth][i].op, forIncBuff[forDepth][i].arg1, 
                     forIncBuff[forDepth][i].arg2, forIncBuff[forDepth][i].result);
            }
            forDepth--;
            emit("goto", "", "", topFalse());
            emit("label", "", "", topEnd());
            popIfLabels();
        }
    ;

for_header
    : for_init SEMICOLON 
        { char* b = getLabel(); char* e = getLabel(); pushIfLabels(b, e); emit("label", "", "", b); }
      expression SEMICOLON
        { emit("ifFalse", $4, "", topEnd()); forDepth++; forIncIdx[forDepth] = 0; inForIncrement[forDepth] = 1; }
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
    | type IDENTIFIER ASSIGN expression { emit("=", $4, "", $2); }
    ;

io_stmt
    : IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON
      {
        emit("in","","",$1);
      }
    | type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON
      {
        emit("in","","",$2);
      }
    | SHOW LPAREN expression RPAREN SEMICOLON
      {
       emit("out",$3,"","");
      }
    | SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON
     {
       char* t=genVar();
       emit("[]",$3,$5,t);
       emit("out",t,"","");
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
    strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].op, op);
    strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].arg1, arg1);
    strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].arg2, arg2);
    strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].result, result);
    forIncIdx[forDepth]++;
}

void emit(char* op, char* arg1, char* arg2, char* result) {
    if(forDepth >= 0 && inForIncrement[forDepth]){
        emit_forinc(op, arg1, arg2, result);
        return;
      }
     memset(&IR[IR_idx], 0, sizeof(Quad));  

    strcpy(IR[IR_idx].op, op ? op : "");
    strcpy(IR[IR_idx].arg1, arg1);
    strcpy(IR[IR_idx].arg2, arg2);
    strcpy(IR[IR_idx].result, result);
    IR_idx++;
}

void yyerror(const char *s) { fprintf(stderr, "Syntax Error: %s\n", s); }

int main() {
    yyin = stdin;
    yyparse();
    printf("Parsing Successful\nGenerated quadruple table:\n");
    printf("%-15s %-15s %-15s %-15s\n", "OP", "ARG1", "ARG2", "RESULT");
    for(int i = 0; i < IR_idx; i++)
        printf("%-15s %-15s %-15s %-15s\n", IR[i].op, IR[i].arg1, IR[i].arg2, IR[i].result);
    return 0;
}
