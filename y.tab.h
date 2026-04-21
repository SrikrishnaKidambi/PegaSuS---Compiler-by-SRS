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
<<<<<<< HEAD
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
    EXTENDS = 284,                 /* EXTENDS  */
    LINEFREQ = 285,                /* LINEFREQ  */
    LPAREN = 286,                  /* LPAREN  */
    RPAREN = 287,                  /* RPAREN  */
    LBRACE = 288,                  /* LBRACE  */
    RBRACE = 289,                  /* RBRACE  */
    LBRACKET = 290,                /* LBRACKET  */
    RBRACKET = 291,                /* RBRACKET  */
    SEMICOLON = 292,               /* SEMICOLON  */
    COMMA = 293,                   /* COMMA  */
    IDENTIFIER = 294,              /* IDENTIFIER  */
    STRING_LITERAL = 295,          /* STRING_LITERAL  */
    INT_LITERAL = 296,             /* INT_LITERAL  */
    FLOAT_LITERAL = 297,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 298,            /* CHAR_LITERAL  */
    ASSIGN = 299,                  /* ASSIGN  */
    ADD_ASSIGN = 300,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 301,              /* SUB_ASSIGN  */
    OR = 302,                      /* OR  */
    AND = 303,                     /* AND  */
    BITOR = 304,                   /* BITOR  */
    BITAND = 305,                  /* BITAND  */
    EQ = 306,                      /* EQ  */
    GT = 307,                      /* GT  */
    LT = 308,                      /* LT  */
    PLUS = 309,                    /* PLUS  */
    MINUS = 310,                   /* MINUS  */
    MUL = 311,                     /* MUL  */
    DIV = 312,                     /* DIV  */
    MOD = 313,                     /* MOD  */
    NOT = 314                      /* NOT  */
=======
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
    LPAREN = 287,                  /* LPAREN  */
    RPAREN = 288,                  /* RPAREN  */
    LBRACE = 289,                  /* LBRACE  */
    RBRACE = 290,                  /* RBRACE  */
    LBRACKET = 291,                /* LBRACKET  */
    RBRACKET = 292,                /* RBRACKET  */
    SEMICOLON = 293,               /* SEMICOLON  */
    COMMA = 294,                   /* COMMA  */
    IDENTIFIER = 295,              /* IDENTIFIER  */
    STRING_LITERAL = 296,          /* STRING_LITERAL  */
    INT_LITERAL = 297,             /* INT_LITERAL  */
    FLOAT_LITERAL = 298,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 299,            /* CHAR_LITERAL  */
    ASSIGN = 300,                  /* ASSIGN  */
    ADD_ASSIGN = 301,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 302,              /* SUB_ASSIGN  */
    OR = 303,                      /* OR  */
    AND = 304,                     /* AND  */
    BITOR = 305,                   /* BITOR  */
    BITAND = 306,                  /* BITAND  */
    EQ = 307,                      /* EQ  */
    GT = 308,                      /* GT  */
    LT = 309,                      /* LT  */
    PLUS = 310,                    /* PLUS  */
    MINUS = 311,                   /* MINUS  */
    MUL = 312,                     /* MUL  */
    DIV = 313,                     /* DIV  */
    MOD = 314,                     /* MOD  */
    NOT = 315                      /* NOT  */
>>>>>>> 7f9aa65265fe0c183bce4a99f541ad9d12b7d46f
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
<<<<<<< HEAD
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
#define EXTENDS 284
#define LINEFREQ 285
#define LPAREN 286
#define RPAREN 287
#define LBRACE 288
#define RBRACE 289
#define LBRACKET 290
#define RBRACKET 291
#define SEMICOLON 292
#define COMMA 293
#define IDENTIFIER 294
#define STRING_LITERAL 295
#define INT_LITERAL 296
#define FLOAT_LITERAL 297
#define CHAR_LITERAL 298
#define ASSIGN 299
#define ADD_ASSIGN 300
#define SUB_ASSIGN 301
#define OR 302
#define AND 303
#define BITOR 304
#define BITAND 305
#define EQ 306
#define GT 307
#define LT 308
#define PLUS 309
#define MINUS 310
#define MUL 311
#define DIV 312
#define MOD 313
#define NOT 314
=======
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
#define LPAREN 287
#define RPAREN 288
#define LBRACE 289
#define RBRACE 290
#define LBRACKET 291
#define RBRACKET 292
#define SEMICOLON 293
#define COMMA 294
#define IDENTIFIER 295
#define STRING_LITERAL 296
#define INT_LITERAL 297
#define FLOAT_LITERAL 298
#define CHAR_LITERAL 299
#define ASSIGN 300
#define ADD_ASSIGN 301
#define SUB_ASSIGN 302
#define OR 303
#define AND 304
#define BITOR 305
#define BITAND 306
#define EQ 307
#define GT 308
#define LT 309
#define PLUS 310
#define MINUS 311
#define MUL 312
#define DIV 313
#define MOD 314
#define NOT 315
>>>>>>> 7f9aa65265fe0c183bce4a99f541ad9d12b7d46f

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
    

<<<<<<< HEAD
#line 195 "y.tab.h"
=======
#line 197 "y.tab.h"
>>>>>>> 7f9aa65265fe0c183bce4a99f541ad9d12b7d46f

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
