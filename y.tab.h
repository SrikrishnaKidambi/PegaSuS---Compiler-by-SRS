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
    VOID = 263,                    /* VOID  */
    IF = 264,                      /* IF  */
    ELIF = 265,                    /* ELIF  */
    ELSE = 266,                    /* ELSE  */
    FOR = 267,                     /* FOR  */
    TRUE = 268,                    /* TRUE  */
    FALSE = 269,                   /* FALSE  */
    FEED = 270,                    /* FEED  */
    SHOW = 271,                    /* SHOW  */
    RETURN = 272,                  /* RETURN  */
    BREAK = 273,                   /* BREAK  */
    CONTINUE = 274,                /* CONTINUE  */
    SEQ1 = 275,                    /* SEQ1  */
    SEQ2 = 276,                    /* SEQ2  */
    FUNC = 277,                    /* FUNC  */
    ENTITY = 278,                  /* ENTITY  */
    NEW = 279,                     /* NEW  */
    PUBLIC = 280,                  /* PUBLIC  */
    PRIVATE = 281,                 /* PRIVATE  */
    THIS = 282,                    /* THIS  */
    DOT = 283,                     /* DOT  */
    LPAREN = 284,                  /* LPAREN  */
    RPAREN = 285,                  /* RPAREN  */
    LBRACE = 286,                  /* LBRACE  */
    RBRACE = 287,                  /* RBRACE  */
    LBRACKET = 288,                /* LBRACKET  */
    RBRACKET = 289,                /* RBRACKET  */
    SEMICOLON = 290,               /* SEMICOLON  */
    COMMA = 291,                   /* COMMA  */
    IDENTIFIER = 292,              /* IDENTIFIER  */
    STRING_LITERAL = 293,          /* STRING_LITERAL  */
    INT_LITERAL = 294,             /* INT_LITERAL  */
    FLOAT_LITERAL = 295,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 296,            /* CHAR_LITERAL  */
    ASSIGN = 297,                  /* ASSIGN  */
    ADD_ASSIGN = 298,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 299,              /* SUB_ASSIGN  */
    OR = 300,                      /* OR  */
    AND = 301,                     /* AND  */
    BITOR = 302,                   /* BITOR  */
    BITAND = 303,                  /* BITAND  */
    EQ = 304,                      /* EQ  */
    GT = 305,                      /* GT  */
    LT = 306,                      /* LT  */
    PLUS = 307,                    /* PLUS  */
    MINUS = 308,                   /* MINUS  */
    MUL = 309,                     /* MUL  */
    DIV = 310,                     /* DIV  */
    MOD = 311,                     /* MOD  */
    NOT = 312                      /* NOT  */
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
#define VOID 263
#define IF 264
#define ELIF 265
#define ELSE 266
#define FOR 267
#define TRUE 268
#define FALSE 269
#define FEED 270
#define SHOW 271
#define RETURN 272
#define BREAK 273
#define CONTINUE 274
#define SEQ1 275
#define SEQ2 276
#define FUNC 277
#define ENTITY 278
#define NEW 279
#define PUBLIC 280
#define PRIVATE 281
#define THIS 282
#define DOT 283
#define LPAREN 284
#define RPAREN 285
#define LBRACE 286
#define RBRACE 287
#define LBRACKET 288
#define RBRACKET 289
#define SEMICOLON 290
#define COMMA 291
#define IDENTIFIER 292
#define STRING_LITERAL 293
#define INT_LITERAL 294
#define FLOAT_LITERAL 295
#define CHAR_LITERAL 296
#define ASSIGN 297
#define ADD_ASSIGN 298
#define SUB_ASSIGN 299
#define OR 300
#define AND 301
#define BITOR 302
#define BITAND 303
#define EQ 304
#define GT 305
#define LT 306
#define PLUS 307
#define MINUS 308
#define MUL 309
#define DIV 310
#define MOD 311
#define NOT 312

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 127 "parser.y"

        int       ival;
        float     fval;
        char      cval;
        char*     sval;
        DataType  dtype;
        AccessMod access;
    

#line 191 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
