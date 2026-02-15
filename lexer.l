%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
%}

DIGIT [0-9]
LETTER [a-zA-Z_]
ID {LETTER}({LETTER}|{DIGIT})*
INT_CONST {DIGIT}+
FLOAT_CONST {DIGIT}+"."{DIGIT}+
CHAR_CONST \'(.|\\.)?\'
STRING_CONST \"([^\\\"]|\\.)*\"

%%

"int"       { return INT; }
"fp"        { return FP; }
"chr"       { return CHR; }
"string"    { return STRING; }
"bool"      { return BOOL; }
"void"      { return VOID; }
"if"        { return IF; }
"elif"      { return ELIF; }
"else"      { return ELSE; }
"for"       { return FOR; }
"True"      { return TRUE; }
"False"     { return FALSE; }
"feed"      { return FEED; }
"show"      { return SHOW; }
"return"    { return RETURN; }
"1seq"      { return SEQ1; }
"2seq"      { return SEQ2; }
"_func"     { return FUNC; }
"Entity"    { return ENTITY; }
"new"       { return NEW; }
"public"    { return PUBLIC; }
"private"   { return PRIVATE; }
"this"      { return THIS; }

"=="        { return EQ; }
">"         { return GT; }
"<"         { return LT; }
"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }
"+="        { return ADD_ASSIGN; }
"-="        { return SUB_ASSIGN; }
"="         { return ASSIGN; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
"%"         { return MOD; }
";"         { return SEMICOLON; }
","         { return COMMA; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
"."         { return DOT; }

{FLOAT_CONST}   { yylval.fval = atof(yytext); return FLOAT_LITERAL; }
{INT_CONST}     { yylval.ival = atoi(yytext); return INT_LITERAL; }
{CHAR_CONST}    { yylval.cval = yytext[1]; return CHAR_LITERAL; }
{STRING_CONST}  { yylval.sval = strdup(yytext); return STRING_LITERAL; }
{ID}            { yylval.sval = strdup(yytext); return IDENTIFIER; }

[ \t\r\n]+      ;
"//".*          ;
.               { printf("Lexical Error: %s\n", yytext); }

%%
int yywrap() { 
	printf("Done with token generation and parsing\n");
	return 1; 
}

int yywrap() {
    return 1;
}
