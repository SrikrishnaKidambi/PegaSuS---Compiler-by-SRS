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
    SNAPSHOT = 258,                /* SNAPSHOT  */
    REWIND = 259,                  /* REWIND  */
    INT = 260,                     /* INT  */
    FP = 261,                      /* FP  */
    CHR = 262,                     /* CHR  */
    STRING = 263,                  /* STRING  */
    BOOL = 264,                    /* BOOL  */
    VOID = 265,                    /* VOID  */
    IF = 266,                      /* IF  */
    ELIF = 267,                    /* ELIF  */
    ELSE = 268,                    /* ELSE  */
    FOR = 269,                     /* FOR  */
    TRUE = 270,                    /* TRUE  */
    FALSE = 271,                   /* FALSE  */
    FEED = 272,                    /* FEED  */
    SHOW = 273,                    /* SHOW  */
    RETURN = 274,                  /* RETURN  */
    BREAK = 275,                   /* BREAK  */
    CONTINUE = 276,                /* CONTINUE  */
    SEQ1 = 277,                    /* SEQ1  */
    SEQ2 = 278,                    /* SEQ2  */
    FUNC = 279,                    /* FUNC  */
    ENTITY = 280,                  /* ENTITY  */
    NEW = 281,                     /* NEW  */
    PUBLIC = 282,                  /* PUBLIC  */
    PRIVATE = 283,                 /* PRIVATE  */
    THIS = 284,                    /* THIS  */
    DOT = 285,                     /* DOT  */
    EXTENDS = 286,                 /* EXTENDS  */
    LINEFREQ = 287,                /* LINEFREQ  */
    LPAREN = 288,                  /* LPAREN  */
    RPAREN = 289,                  /* RPAREN  */
    LBRACE = 290,                  /* LBRACE  */
    RBRACE = 291,                  /* RBRACE  */
    LBRACKET = 292,                /* LBRACKET  */
    RBRACKET = 293,                /* RBRACKET  */
    SEMICOLON = 294,               /* SEMICOLON  */
    COMMA = 295,                   /* COMMA  */
    IDENTIFIER = 296,              /* IDENTIFIER  */
    STRING_LITERAL = 297,          /* STRING_LITERAL  */
    INT_LITERAL = 298,             /* INT_LITERAL  */
    FLOAT_LITERAL = 299,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 300,            /* CHAR_LITERAL  */
    ASSIGN = 301,                  /* ASSIGN  */
    ADD_ASSIGN = 302,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 303,              /* SUB_ASSIGN  */
    OR = 304,                      /* OR  */
    AND = 305,                     /* AND  */
    BITOR = 306,                   /* BITOR  */
    BITAND = 307,                  /* BITAND  */
    EQ = 308,                      /* EQ  */
    GT = 309,                      /* GT  */
    LT = 310,                      /* LT  */
    PLUS = 311,                    /* PLUS  */
    MINUS = 312,                   /* MINUS  */
    MUL = 313,                     /* MUL  */
    DIV = 314,                     /* DIV  */
    MOD = 315,                     /* MOD  */
    NOT = 316                      /* NOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define SNAPSHOT 258
#define REWIND 259
#define INT 260
#define FP 261
#define CHR 262
#define STRING 263
#define BOOL 264
#define VOID 265
#define IF 266
#define ELIF 267
#define ELSE 268
#define FOR 269
#define TRUE 270
#define FALSE 271
#define FEED 272
#define SHOW 273
#define RETURN 274
#define BREAK 275
#define CONTINUE 276
#define SEQ1 277
#define SEQ2 278
#define FUNC 279
#define ENTITY 280
#define NEW 281
#define PUBLIC 282
#define PRIVATE 283
#define THIS 284
#define DOT 285
#define EXTENDS 286
#define LINEFREQ 287
#define LPAREN 288
#define RPAREN 289
#define LBRACE 290
#define RBRACE 291
#define LBRACKET 292
#define RBRACKET 293
#define SEMICOLON 294
#define COMMA 295
#define IDENTIFIER 296
#define STRING_LITERAL 297
#define INT_LITERAL 298
#define FLOAT_LITERAL 299
#define CHAR_LITERAL 300
#define ASSIGN 301
#define ADD_ASSIGN 302
#define SUB_ASSIGN 303
#define OR 304
#define AND 305
#define BITOR 306
#define BITAND 307
#define EQ 308
#define GT 309
#define LT 310
#define PLUS 311
#define MINUS 312
#define MUL 313
#define DIV 314
#define MOD 315
#define NOT 316

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 140 "parser.y"

        int       ival;
        float     fval;
        char      cval;
        char*     sval;
        DataType  dtype;
        AccessMod access;
    

#line 199 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
