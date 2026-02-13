/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT = 258,                     /* INT  */
    FP = 259,                      /* FP  */
    CHR = 260,                     /* CHR  */
    STRING = 261,                  /* STRING  */
    BOOL = 262,                    /* BOOL  */
    IF = 263,                      /* IF  */
    ELIF = 264,                    /* ELIF  */
    ELSE = 265,                    /* ELSE  */
    FOR = 266,                     /* FOR  */
    TRUE = 267,                    /* TRUE  */
    FALSE = 268,                   /* FALSE  */
    FEED = 269,                    /* FEED  */
    SHOW = 270,                    /* SHOW  */
    IDENTIFIER = 271,              /* IDENTIFIER  */
    INT_LITERAL = 272,             /* INT_LITERAL  */
    FLOAT_LITERAL = 273,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 274,            /* CHAR_LITERAL  */
    STRING_LITERAL = 275,          /* STRING_LITERAL  */
    PLUS = 276,                    /* PLUS  */
    MINUS = 277,                   /* MINUS  */
    MUL = 278,                     /* MUL  */
    DIV = 279,                     /* DIV  */
    MOD = 280,                     /* MOD  */
    GT = 281,                      /* GT  */
    LT = 282,                      /* LT  */
    EQ = 283,                      /* EQ  */
    AND = 284,                     /* AND  */
    OR = 285,                      /* OR  */
    NOT = 286,                     /* NOT  */
    ASSIGN = 287,                  /* ASSIGN  */
    ADD_ASSIGN = 288,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 289,              /* SUB_ASSIGN  */
    SEMICOLON = 290,               /* SEMICOLON  */
    COMMA = 291,                   /* COMMA  */
    LPAREN = 292,                  /* LPAREN  */
    RPAREN = 293,                  /* RPAREN  */
    LBRACE = 294,                  /* LBRACE  */
    RBRACE = 295                   /* RBRACE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define INT 258
#define FP 259
#define CHR 260
#define STRING 261
#define BOOL 262
#define IF 263
#define ELIF 264
#define ELSE 265
#define FOR 266
#define TRUE 267
#define FALSE 268
#define FEED 269
#define SHOW 270
#define IDENTIFIER 271
#define INT_LITERAL 272
#define FLOAT_LITERAL 273
#define CHAR_LITERAL 274
#define STRING_LITERAL 275
#define PLUS 276
#define MINUS 277
#define MUL 278
#define DIV 279
#define MOD 280
#define GT 281
#define LT 282
#define EQ 283
#define AND 284
#define OR 285
#define NOT 286
#define ASSIGN 287
#define ADD_ASSIGN 288
#define SUB_ASSIGN 289
#define SEMICOLON 290
#define COMMA 291
#define LPAREN 292
#define RPAREN 293
#define LBRACE 294
#define RBRACE 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 9 "parser.y"

    int ival;
    float fval;
    char cval;
    char* sval;

#line 154 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
