/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;
extern int yylineno;
extern char* yytext;

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

static int if_cnt  = 0;
static int for_cnt = 0;


// Utilities for handling array declaration along with initialization
static int arr2d_rows = 0;
static int arr2d_cols = 0;

DataType last_expr_type = DT_UNKNOWN;
DataType current_array_elem_type = DT_UNKNOWN;
int array_type_errors = 0;

// Utitilies for handling semantic errors in functions and function calls
static Symbol* current_function = NULL;         // A global variable that is used for storing the function(a symbol right!!) so that we can know what is the expected
                                                // return type
static DataType call_arg_types[64];	// Array for storing the type of argument in each function call
static int call_arg_count = 0;		// Store the number of arguments collected till now


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
/* Returns 1 if the string is a numeric constant, 0 otherwise */
int isConstant(char* s) {
    if (!s || *s == '\0') return 0;
    char* end;
    strtod(s, &end);
    return (*end == '\0');
}

/* Folds two constants with a given operator. Returns heap string or NULL. */
char* foldConstants(char* op, char* arg1, char* arg2) {
    if (!isConstant(arg1) || !isConstant(arg2)) return NULL;

    double a = atof(arg1);
    double b = atof(arg2);
    double result;

    if      (strcmp(op, "+")  == 0) result = a + b;
    else if (strcmp(op, "-")  == 0) result = a - b;
    else if (strcmp(op, "*")  == 0) result = a * b;
    else if (strcmp(op, "/")  == 0) {
        if (b == 0) return NULL;   
        result = a / b;
    }
    else if (strcmp(op, "%")  == 0) {
        if ((int)b == 0) return NULL;
        result = (int)a % (int)b;
    }
    else if (strcmp(op, ">")  == 0) result = (a >  b);
    else if (strcmp(op, "<")  == 0) result = (a <  b);
    else if (strcmp(op, "==") == 0) result = (a == b);
    else return NULL;

    char* buf = malloc(32);
    if (result == (int)result)
        sprintf(buf, "%d", (int)result);
    else
        sprintf(buf, "%f", result);
    return buf;
}

#line 201 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
    SEQ1 = 273,                    /* SEQ1  */
    SEQ2 = 274,                    /* SEQ2  */
    FUNC = 275,                    /* FUNC  */
    ENTITY = 276,                  /* ENTITY  */
    NEW = 277,                     /* NEW  */
    PUBLIC = 278,                  /* PUBLIC  */
    PRIVATE = 279,                 /* PRIVATE  */
    THIS = 280,                    /* THIS  */
    DOT = 281,                     /* DOT  */
    LPAREN = 282,                  /* LPAREN  */
    RPAREN = 283,                  /* RPAREN  */
    LBRACE = 284,                  /* LBRACE  */
    RBRACE = 285,                  /* RBRACE  */
    LBRACKET = 286,                /* LBRACKET  */
    RBRACKET = 287,                /* RBRACKET  */
    SEMICOLON = 288,               /* SEMICOLON  */
    COMMA = 289,                   /* COMMA  */
    IDENTIFIER = 290,              /* IDENTIFIER  */
    STRING_LITERAL = 291,          /* STRING_LITERAL  */
    INT_LITERAL = 292,             /* INT_LITERAL  */
    FLOAT_LITERAL = 293,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 294,            /* CHAR_LITERAL  */
    ASSIGN = 295,                  /* ASSIGN  */
    ADD_ASSIGN = 296,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 297,              /* SUB_ASSIGN  */
    OR = 298,                      /* OR  */
    AND = 299,                     /* AND  */
    BITOR = 300,                   /* BITOR  */
    BITAND = 301,                  /* BITAND  */
    EQ = 302,                      /* EQ  */
    GT = 303,                      /* GT  */
    LT = 304,                      /* LT  */
    PLUS = 305,                    /* PLUS  */
    MINUS = 306,                   /* MINUS  */
    MUL = 307,                     /* MUL  */
    DIV = 308,                     /* DIV  */
    MOD = 309,                     /* MOD  */
    NOT = 310                      /* NOT  */
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
#define SEQ1 273
#define SEQ2 274
#define FUNC 275
#define ENTITY 276
#define NEW 277
#define PUBLIC 278
#define PRIVATE 279
#define THIS 280
#define DOT 281
#define LPAREN 282
#define RPAREN 283
#define LBRACE 284
#define RBRACE 285
#define LBRACKET 286
#define RBRACKET 287
#define SEMICOLON 288
#define COMMA 289
#define IDENTIFIER 290
#define STRING_LITERAL 291
#define INT_LITERAL 292
#define FLOAT_LITERAL 293
#define CHAR_LITERAL 294
#define ASSIGN 295
#define ADD_ASSIGN 296
#define SUB_ASSIGN 297
#define OR 298
#define AND 299
#define BITOR 300
#define BITAND 301
#define EQ 302
#define GT 303
#define LT 304
#define PLUS 305
#define MINUS 306
#define MUL 307
#define DIV 308
#define MOD 309
#define NOT 310

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 131 "parser.y"

    int       ival;
    float     fval;
    char      cval;
    char*     sval;
    DataType  dtype;
    AccessMod access;

#line 373 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_FP = 4,                         /* FP  */
  YYSYMBOL_CHR = 5,                        /* CHR  */
  YYSYMBOL_STRING = 6,                     /* STRING  */
  YYSYMBOL_BOOL = 7,                       /* BOOL  */
  YYSYMBOL_VOID = 8,                       /* VOID  */
  YYSYMBOL_IF = 9,                         /* IF  */
  YYSYMBOL_ELIF = 10,                      /* ELIF  */
  YYSYMBOL_ELSE = 11,                      /* ELSE  */
  YYSYMBOL_FOR = 12,                       /* FOR  */
  YYSYMBOL_TRUE = 13,                      /* TRUE  */
  YYSYMBOL_FALSE = 14,                     /* FALSE  */
  YYSYMBOL_FEED = 15,                      /* FEED  */
  YYSYMBOL_SHOW = 16,                      /* SHOW  */
  YYSYMBOL_RETURN = 17,                    /* RETURN  */
  YYSYMBOL_SEQ1 = 18,                      /* SEQ1  */
  YYSYMBOL_SEQ2 = 19,                      /* SEQ2  */
  YYSYMBOL_FUNC = 20,                      /* FUNC  */
  YYSYMBOL_ENTITY = 21,                    /* ENTITY  */
  YYSYMBOL_NEW = 22,                       /* NEW  */
  YYSYMBOL_PUBLIC = 23,                    /* PUBLIC  */
  YYSYMBOL_PRIVATE = 24,                   /* PRIVATE  */
  YYSYMBOL_THIS = 25,                      /* THIS  */
  YYSYMBOL_DOT = 26,                       /* DOT  */
  YYSYMBOL_LPAREN = 27,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 28,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 29,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 30,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 31,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 32,                  /* RBRACKET  */
  YYSYMBOL_SEMICOLON = 33,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 34,                     /* COMMA  */
  YYSYMBOL_IDENTIFIER = 35,                /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 36,            /* STRING_LITERAL  */
  YYSYMBOL_INT_LITERAL = 37,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 38,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 39,              /* CHAR_LITERAL  */
  YYSYMBOL_ASSIGN = 40,                    /* ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 41,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 42,                /* SUB_ASSIGN  */
  YYSYMBOL_OR = 43,                        /* OR  */
  YYSYMBOL_AND = 44,                       /* AND  */
  YYSYMBOL_BITOR = 45,                     /* BITOR  */
  YYSYMBOL_BITAND = 46,                    /* BITAND  */
  YYSYMBOL_EQ = 47,                        /* EQ  */
  YYSYMBOL_GT = 48,                        /* GT  */
  YYSYMBOL_LT = 49,                        /* LT  */
  YYSYMBOL_PLUS = 50,                      /* PLUS  */
  YYSYMBOL_MINUS = 51,                     /* MINUS  */
  YYSYMBOL_MUL = 52,                       /* MUL  */
  YYSYMBOL_DIV = 53,                       /* DIV  */
  YYSYMBOL_MOD = 54,                       /* MOD  */
  YYSYMBOL_NOT = 55,                       /* NOT  */
  YYSYMBOL_YYACCEPT = 56,                  /* $accept  */
  YYSYMBOL_program = 57,                   /* program  */
  YYSYMBOL_element = 58,                   /* element  */
  YYSYMBOL_statement = 59,                 /* statement  */
  YYSYMBOL_entity_decl = 60,               /* entity_decl  */
  YYSYMBOL_61_1 = 61,                      /* $@1  */
  YYSYMBOL_62_2 = 62,                      /* $@2  */
  YYSYMBOL_entity_body = 63,               /* entity_body  */
  YYSYMBOL_entity_member = 64,             /* entity_member  */
  YYSYMBOL_constructor_decl = 65,          /* constructor_decl  */
  YYSYMBOL_66_3 = 66,                      /* $@3  */
  YYSYMBOL_67_4 = 67,                      /* $@4  */
  YYSYMBOL_method_decl = 68,               /* method_decl  */
  YYSYMBOL_69_5 = 69,                      /* $@5  */
  YYSYMBOL_70_6 = 70,                      /* $@6  */
  YYSYMBOL_71_7 = 71,                      /* $@7  */
  YYSYMBOL_access_var_decl = 72,           /* access_var_decl  */
  YYSYMBOL_access_modifier = 73,           /* access_modifier  */
  YYSYMBOL_object_decl = 74,               /* object_decl  */
  YYSYMBOL_arg_list_opt = 75,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 76,                  /* arg_list  */
  YYSYMBOL_block = 77,                     /* block  */
  YYSYMBOL_78_8 = 78,                      /* $@8  */
  YYSYMBOL_stmt_list = 79,                 /* stmt_list  */
  YYSYMBOL_var_decl = 80,                  /* var_decl  */
  YYSYMBOL_id_list = 81,                   /* id_list  */
  YYSYMBOL_type = 82,                      /* type  */
  YYSYMBOL_array_decl = 83,                /* array_decl  */
  YYSYMBOL_84_9 = 84,                      /* $@9  */
  YYSYMBOL_85_10 = 85,                     /* $@10  */
  YYSYMBOL_array_init = 86,                /* array_init  */
  YYSYMBOL_array_init2d = 87,              /* array_init2d  */
  YYSYMBOL_row_list = 88,                  /* row_list  */
  YYSYMBOL_expr_list = 89,                 /* expr_list  */
  YYSYMBOL_function_decl = 90,             /* function_decl  */
  YYSYMBOL_91_11 = 91,                     /* $@11  */
  YYSYMBOL_92_12 = 92,                     /* $@12  */
  YYSYMBOL_93_13 = 93,                     /* $@13  */
  YYSYMBOL_func_type = 94,                 /* func_type  */
  YYSYMBOL_param_list_opt = 95,            /* param_list_opt  */
  YYSYMBOL_param_list = 96,                /* param_list  */
  YYSYMBOL_param = 97,                     /* param  */
  YYSYMBOL_return_stmt = 98,               /* return_stmt  */
  YYSYMBOL_expr_stmt = 99,                 /* expr_stmt  */
  YYSYMBOL_expression = 100,               /* expression  */
  YYSYMBOL_indexed_id = 101,               /* indexed_id  */
  YYSYMBOL_assignment = 102,               /* assignment  */
  YYSYMBOL_logic_expr = 103,               /* logic_expr  */
  YYSYMBOL_bitwise_expr = 104,             /* bitwise_expr  */
  YYSYMBOL_rel_expr = 105,                 /* rel_expr  */
  YYSYMBOL_arith_expr = 106,               /* arith_expr  */
  YYSYMBOL_term = 107,                     /* term  */
  YYSYMBOL_factor = 108,                   /* factor  */
  YYSYMBOL_if_stmt = 109,                  /* if_stmt  */
  YYSYMBOL_110_14 = 110,                   /* $@14  */
  YYSYMBOL_111_15 = 111,                   /* $@15  */
  YYSYMBOL_112_16 = 112,                   /* $@16  */
  YYSYMBOL_113_17 = 113,                   /* $@17  */
  YYSYMBOL_elif_list = 114,                /* elif_list  */
  YYSYMBOL_115_18 = 115,                   /* $@18  */
  YYSYMBOL_116_19 = 116,                   /* $@19  */
  YYSYMBOL_117_20 = 117,                   /* $@20  */
  YYSYMBOL_118_21 = 118,                   /* $@21  */
  YYSYMBOL_else_opt = 119,                 /* else_opt  */
  YYSYMBOL_120_22 = 120,                   /* $@22  */
  YYSYMBOL_for_stmt = 121,                 /* for_stmt  */
  YYSYMBOL_122_23 = 122,                   /* $@23  */
  YYSYMBOL_for_header = 123,               /* for_header  */
  YYSYMBOL_124_24 = 124,                   /* $@24  */
  YYSYMBOL_125_25 = 125,                   /* $@25  */
  YYSYMBOL_126_26 = 126,                   /* $@26  */
  YYSYMBOL_for_init = 127,                 /* for_init  */
  YYSYMBOL_var_decl_no_semi = 128,         /* var_decl_no_semi  */
  YYSYMBOL_io_stmt = 129                   /* io_stmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   504

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  74
/* YYNRULES -- Number of rules.  */
#define YYNRULES  163
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  342

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   310


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   170,   170,   171,   175,   176,   177,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   202,   201,   241,
     240,   251,   252,   256,   257,   258,   266,   265,   291,   290,
     306,   305,   335,   334,   363,   362,   377,   391,   407,   408,
     415,   433,   450,   451,   455,   462,   475,   474,   484,   485,
     493,   494,   512,   519,   532,   539,   555,   562,   573,   574,
     575,   576,   577,   590,   608,   626,   625,   645,   644,   667,
     671,   675,   681,   690,   700,   718,   717,   746,   745,   771,
     770,   781,   782,   786,   787,   791,   792,   797,   818,   844,
     858,   870,   878,   879,   883,   890,   896,   907,   920,   931,
     943,   945,   970,   996,  1000,  1002,  1004,  1006,  1010,  1012,
    1014,  1018,  1027,  1036,  1045,  1049,  1059,  1069,  1073,  1084,
    1095,  1106,  1110,  1141,  1210,  1217,  1218,  1219,  1220,  1221,
    1222,  1223,  1224,  1232,  1249,  1251,  1256,  1231,  1260,  1269,
    1286,  1288,  1293,  1268,  1295,  1300,  1299,  1311,  1319,  1318,
    1342,  1351,  1358,  1365,  1350,  1370,  1371,  1372,  1376,  1380,
    1396,  1404,  1406,  1408
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "FP", "CHR",
  "STRING", "BOOL", "VOID", "IF", "ELIF", "ELSE", "FOR", "TRUE", "FALSE",
  "FEED", "SHOW", "RETURN", "SEQ1", "SEQ2", "FUNC", "ENTITY", "NEW",
  "PUBLIC", "PRIVATE", "THIS", "DOT", "LPAREN", "RPAREN", "LBRACE",
  "RBRACE", "LBRACKET", "RBRACKET", "SEMICOLON", "COMMA", "IDENTIFIER",
  "STRING_LITERAL", "INT_LITERAL", "FLOAT_LITERAL", "CHAR_LITERAL",
  "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "OR", "AND", "BITOR", "BITAND",
  "EQ", "GT", "LT", "PLUS", "MINUS", "MUL", "DIV", "MOD", "NOT", "$accept",
  "program", "element", "statement", "entity_decl", "$@1", "$@2",
  "entity_body", "entity_member", "constructor_decl", "$@3", "$@4",
  "method_decl", "$@5", "$@6", "$@7", "access_var_decl", "access_modifier",
  "object_decl", "arg_list_opt", "arg_list", "block", "$@8", "stmt_list",
  "var_decl", "id_list", "type", "array_decl", "$@9", "$@10", "array_init",
  "array_init2d", "row_list", "expr_list", "function_decl", "$@11", "$@12",
  "$@13", "func_type", "param_list_opt", "param_list", "param",
  "return_stmt", "expr_stmt", "expression", "indexed_id", "assignment",
  "logic_expr", "bitwise_expr", "rel_expr", "arith_expr", "term", "factor",
  "if_stmt", "$@14", "$@15", "$@16", "$@17", "elif_list", "$@18", "$@19",
  "$@20", "$@21", "else_opt", "$@22", "for_stmt", "$@23", "for_header",
  "$@24", "$@25", "$@26", "for_init", "var_decl_no_semi", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-254)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-149)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -254,   196,  -254,     4,  -254,  -254,  -254,  -254,  -254,  -254,
      18,    27,  -254,  -254,    29,   344,    51,    49,   414,   253,
    -254,   462,  -254,  -254,  -254,  -254,   441,  -254,  -254,  -254,
    -254,  -254,  -254,    -5,  -254,  -254,    68,  -254,  -254,    61,
     100,  -254,    69,   106,  -254,   189,    84,  -254,  -254,  -254,
    -254,  -254,   377,    22,   414,    75,  -254,   118,   110,  -254,
      66,   166,   116,  -254,   337,   119,   163,   414,   414,   167,
      54,   414,   414,   123,    71,  -254,  -254,   171,   180,   176,
     135,   184,  -254,   414,   441,   441,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   192,   414,   194,   382,
      45,  -254,   414,  -254,   195,   201,   202,  -254,  -254,   144,
    -254,   297,    -5,  -254,   204,   213,   211,  -254,   214,   128,
     221,  -254,  -254,  -254,  -254,   215,   217,    -7,    11,   409,
    -254,  -254,  -254,   205,  -254,   207,  -254,   126,   126,   126,
      84,    84,  -254,  -254,  -254,   226,  -254,   226,   229,  -254,
     226,   228,  -254,   235,   414,  -254,   273,   414,  -254,  -254,
     248,   414,  -254,   414,   245,   242,   246,   251,  -254,  -254,
     244,  -254,   262,  -254,   278,   430,   279,   288,   289,  -254,
     290,  -254,   277,  -254,  -254,  -254,   287,     8,   291,  -254,
      13,  -254,  -254,   414,   293,  -254,   295,   296,   300,   299,
     308,   303,   312,  -254,    13,   324,  -254,   414,   414,   320,
    -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,   112,  -254,
     321,   325,   327,   329,  -254,   333,   414,   326,   328,   414,
     334,   353,   339,   360,   366,    -6,   369,   370,   226,  -254,
     367,   368,   376,   378,    30,    43,  -254,  -254,   226,    13,
    -254,   380,  -254,  -254,    10,  -254,  -254,   373,   414,    53,
    -254,   392,   414,   226,   226,  -254,  -254,  -254,    13,   405,
     391,   396,   395,   398,  -254,  -254,   400,  -254,   414,   379,
      73,  -254,   406,  -254,    46,  -254,  -254,   428,   414,   412,
     415,  -254,  -254,  -254,  -254,  -254,  -254,   425,  -254,   414,
     426,   433,   431,  -254,   226,   226,   435,   436,   438,  -254,
      76,  -254,  -254,  -254,  -254,   439,  -254,  -254,    13,    13,
     465,  -254,   414,   226,  -254,   445,   446,   447,  -254,  -254,
     226,   226,   226,   453,  -254,  -254,  -254,  -254,   226,  -254,
     428,  -254
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,    58,    59,    60,    61,    62,    82,
       0,     0,   130,   131,     0,     0,     0,     0,     0,     0,
      93,   124,   129,   126,   127,   128,     0,     2,     4,     6,
      14,    15,     7,    81,     8,     5,     0,    13,     9,     0,
     125,    94,   103,   107,   110,   114,   117,   121,    10,    11,
      12,    16,     0,     0,     0,     0,    90,   124,     0,    17,
       0,     0,     0,    48,     0,     0,     0,    43,     0,    57,
       0,     0,     0,     0,   124,   125,   106,     0,     0,    57,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   157,
       0,    91,     0,    89,     0,     0,     0,   132,    49,   124,
      51,     0,     0,    77,   123,     0,    42,    45,     0,     0,
       0,    97,    98,    99,    54,     0,     0,     0,     0,     0,
      52,    75,   100,   104,   105,   109,   108,   113,   111,   112,
     115,   116,   118,   119,   120,     0,   134,     0,     0,   156,
       0,     0,   155,     0,     0,    22,     0,     0,    47,    50,
       0,     0,   122,     0,    95,     0,     0,     0,    56,   123,
       0,    65,     0,    67,     0,   124,     0,     0,     0,   138,
       0,   150,   158,   149,   151,   162,     0,     0,     0,   101,
      84,   102,    44,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,    53,    84,     0,   135,     0,     0,     0,
      38,    39,    18,    26,    21,    23,    24,    25,     0,    20,
       0,     0,     0,    83,    86,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,   123,     0,     0,     0,   159,
       0,     0,     0,     0,     0,     0,    88,    87,     0,     0,
      96,     0,   160,    63,     0,    74,    66,     0,     0,     0,
      68,     0,     0,     0,     0,   136,   152,   163,    84,     0,
       0,     0,     0,     0,    78,    85,     0,    69,     0,     0,
       0,    70,     0,   161,     0,    76,    80,   144,     0,     0,
       0,    32,    37,    30,    36,    40,    73,     0,    72,     0,
       0,     0,   147,   153,     0,     0,     0,     0,     0,    64,
       0,    41,   139,   145,   137,     0,    27,    29,    84,    84,
       0,    71,     0,     0,   154,     0,     0,     0,   140,   146,
       0,     0,     0,     0,    33,    31,    35,   141,     0,   142,
     144,   143
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -254,  -254,  -254,   -49,  -254,  -254,  -254,  -254,  -254,  -254,
    -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,   257,
     222,  -143,  -254,  -254,  -254,   -27,     0,  -254,  -254,  -254,
    -254,  -254,  -254,  -253,  -254,  -254,  -254,  -254,  -254,  -196,
    -254,   236,  -254,  -254,   -15,    40,   -61,   -14,    95,  -254,
     138,    99,    79,  -254,  -254,  -254,  -254,  -254,   146,  -254,
    -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,
    -254,  -254,  -254,  -254
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    27,    28,    29,   104,   105,   187,   214,   215,
     242,   243,   216,   307,   306,   308,   217,   218,    30,   115,
     116,    31,    64,   111,    32,    73,   221,    34,   198,   200,
     230,   233,   259,   254,    35,   177,   160,   178,    36,   222,
     223,   224,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    97,   180,   238,   287,   302,   322,
     333,   338,   340,   314,   323,    49,    99,   150,   208,   288,
     315,   151,   152,    50
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,    33,   179,    61,   181,   280,    80,   183,   236,   121,
     122,   123,    76,    77,    78,   110,     4,     5,     6,     7,
       8,   262,   132,    98,   170,  -148,  -148,  -148,  -148,  -148,
      79,   210,   211,   171,   161,  -148,  -148,    51,   212,   100,
     277,   121,   172,   213,   278,    52,   310,  -148,   220,  -148,
     270,   173,   117,   118,    53,  -148,    54,  -148,  -148,  -148,
    -148,  -148,   159,   272,   112,   271,    75,    12,    13,   120,
     133,   134,   289,   153,   300,    60,   154,  -148,   273,    17,
     163,    18,   146,   281,   149,    80,    59,   282,    81,    57,
      22,    23,    24,    25,    82,   265,   189,   126,    67,   148,
     191,   106,    68,   298,   166,   274,   321,   278,   101,    26,
     278,   112,    84,    85,   176,     4,     5,     6,     7,     8,
     285,   286,   325,   326,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    93,    94,    95,   186,
      83,    12,    13,   103,    66,    67,   108,   244,   192,    68,
     165,    86,    87,    17,   113,    18,   124,   125,   102,    71,
      72,   316,   317,    57,    22,    23,    24,    25,   130,   125,
      66,    67,   142,   143,   144,    68,    91,    92,   225,    69,
     329,   135,   136,    26,    70,    71,    72,   334,   335,   336,
     140,   141,   239,   240,   107,   339,     2,     3,   114,     4,
       5,     6,     7,     8,     9,    10,   127,   119,    11,    12,
      13,   117,    14,    15,   255,   128,   129,    16,   245,   131,
     145,    17,   147,    18,   155,    19,   137,   138,   139,    20,
     156,    21,    22,    23,    24,    25,    88,    89,    90,    91,
      92,   162,   157,   255,   161,   163,   164,   117,   167,    85,
     168,    26,   169,    87,    62,    19,   -46,   -46,   -46,   -46,
     -46,   184,   -46,   296,   182,   -46,   -46,   -46,   185,   -46,
     -46,    12,    13,   303,   188,   190,   193,   194,   -46,   195,
     -46,   197,   -46,    63,   255,    18,   -46,   196,   -46,   -46,
     -46,   -46,   -46,    74,    22,    23,    24,    25,     3,   199,
       4,     5,     6,     7,     8,   201,    10,   328,   -46,    11,
      12,    13,   203,    14,    15,   204,   205,   207,   206,   209,
     226,   219,    17,   227,    18,   237,    19,   158,   228,   229,
      20,   231,   109,    22,    23,    24,    25,   232,     3,   234,
       4,     5,     6,     7,     8,    55,    10,   235,   241,    11,
      12,    13,    26,    14,    15,   248,   246,    12,    13,   252,
     247,   253,    17,   249,    18,   250,    19,   256,   258,    17,
      20,    18,   109,    22,    23,    24,    25,    56,    96,    57,
      22,    23,    24,    25,   257,     4,     5,     6,     7,     8,
    -133,  -133,    26,   260,   261,    12,    13,   263,   264,    26,
     266,   267,  -133,   268,  -133,   269,   290,    17,   276,    18,
     279,   297,  -133,  -133,  -133,  -133,  -133,    57,    22,    23,
      24,    25,    12,    13,   174,   283,   291,    12,    13,   292,
     293,   294,  -133,   295,    17,   299,    18,    26,   301,    17,
     304,    18,   313,   305,   175,    22,    23,    24,    25,    57,
      22,    23,    24,    25,    12,    13,   202,    67,   309,   311,
     312,    68,   318,   319,    26,   320,   327,   324,    18,    26,
     102,    71,    72,   330,   331,   332,    74,    22,    23,    24,
      25,   337,    65,   251,   284,   275,   341,     0,    66,    67,
       0,     0,     0,    68,     0,     0,    26,    69,     0,     0,
       0,     0,    70,    71,    72
};

static const yytype_int16 yycheck[] =
{
      15,     1,   145,    18,   147,   258,    33,   150,   204,    70,
      71,    72,    26,    18,    19,    64,     3,     4,     5,     6,
       7,    27,    83,     1,    31,     3,     4,     5,     6,     7,
      35,    23,    24,    40,    40,    13,    14,    33,    30,    54,
      30,   102,    31,    35,    34,    27,   299,    25,    35,    27,
      20,    40,    67,    68,    27,    33,    27,    35,    36,    37,
      38,    39,   111,    20,    64,    35,    26,    13,    14,    15,
      84,    85,   268,    28,    28,    26,    31,    55,    35,    25,
      34,    27,    97,    30,    99,   112,    35,    34,    20,    35,
      36,    37,    38,    39,    33,   238,   157,    26,    27,    99,
     161,    35,    31,    30,   119,   248,    30,    34,    33,    55,
      34,   111,    43,    44,   129,     3,     4,     5,     6,     7,
     263,   264,   318,   319,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    52,    53,    54,   154,
      40,    13,    14,    33,    26,    27,    30,    35,   163,    31,
      22,    45,    46,    25,    35,    27,    33,    34,    40,    41,
      42,   304,   305,    35,    36,    37,    38,    39,    33,    34,
      26,    27,    93,    94,    95,    31,    50,    51,   193,    35,
     323,    86,    87,    55,    40,    41,    42,   330,   331,   332,
      91,    92,   207,   208,    28,   338,     0,     1,    35,     3,
       4,     5,     6,     7,     8,     9,    35,    40,    12,    13,
      14,   226,    16,    17,   229,    35,    40,    21,   218,    35,
      28,    25,    28,    27,    29,    29,    88,    89,    90,    33,
      29,    35,    36,    37,    38,    39,    47,    48,    49,    50,
      51,    28,    40,   258,    40,    34,    32,   262,    27,    44,
      35,    55,    35,    46,     1,    29,     3,     4,     5,     6,
       7,    33,     9,   278,    35,    12,    13,    14,    33,    16,
      17,    13,    14,   288,     1,    27,    31,    35,    25,    33,
      27,    37,    29,    30,   299,    27,    33,    36,    35,    36,
      37,    38,    39,    35,    36,    37,    38,    39,     1,    37,
       3,     4,     5,     6,     7,    27,     9,   322,    55,    12,
      13,    14,    33,    16,    17,    27,    27,    40,    28,    32,
      27,    30,    25,    28,    27,     1,    29,    30,    32,    29,
      33,    32,    35,    36,    37,    38,    39,    29,     1,    36,
       3,     4,     5,     6,     7,     1,     9,    35,    28,    12,
      13,    14,    55,    16,    17,    28,    35,    13,    14,    33,
      35,    33,    25,    34,    27,    32,    29,    33,    29,    25,
      33,    27,    35,    36,    37,    38,    39,    33,     1,    35,
      36,    37,    38,    39,    31,     3,     4,     5,     6,     7,
      13,    14,    55,    33,    28,    13,    14,    28,    28,    55,
      33,    33,    25,    27,    27,    27,     1,    25,    28,    27,
      37,    32,    35,    36,    37,    38,    39,    35,    36,    37,
      38,    39,    13,    14,    15,    33,    35,    13,    14,    33,
      35,    33,    55,    33,    25,    29,    27,    55,    10,    25,
      28,    27,    11,    28,    35,    36,    37,    38,    39,    35,
      36,    37,    38,    39,    13,    14,    26,    27,    33,    33,
      27,    31,    27,    27,    55,    27,     1,    28,    27,    55,
      40,    41,    42,    28,    28,    28,    35,    36,    37,    38,
      39,    28,    20,   226,   262,   249,   340,    -1,    26,    27,
      -1,    -1,    -1,    31,    -1,    -1,    55,    35,    -1,    -1,
      -1,    -1,    40,    41,    42
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    57,     0,     1,     3,     4,     5,     6,     7,     8,
       9,    12,    13,    14,    16,    17,    21,    25,    27,    29,
      33,    35,    36,    37,    38,    39,    55,    58,    59,    60,
      74,    77,    80,    82,    83,    90,    94,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   121,
     129,    33,    27,    27,    27,     1,    33,    35,   100,    35,
      26,   100,     1,    30,    78,    20,    26,    27,    31,    35,
      40,    41,    42,    81,    35,   101,   103,    18,    19,    35,
      81,    20,    33,    40,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,     1,   110,     1,   122,
     100,    33,    40,    33,    61,    62,    35,    28,    30,    35,
      59,    79,    82,    35,    35,    75,    76,   100,   100,    40,
      15,   102,   102,   102,    33,    34,    26,    35,    35,    40,
      33,    35,   102,   103,   103,   104,   104,   106,   106,   106,
     107,   107,   108,   108,   108,    28,   100,    28,    82,   100,
     123,   127,   128,    28,    31,    29,    29,    40,    30,    59,
      92,    40,    28,    34,    32,    22,   100,    27,    35,    35,
      31,    40,    31,    40,    15,    35,   100,    91,    93,    77,
     111,    77,    35,    77,    33,    33,   100,    63,     1,   102,
      27,   102,   100,    31,    35,    33,    36,    37,    84,    37,
      85,    27,    26,    33,    27,    27,    28,    40,   124,    32,
      23,    24,    30,    35,    64,    65,    68,    72,    73,    30,
      35,    82,    95,    96,    97,   100,    27,    28,    32,    29,
      86,    32,    29,    87,    36,    35,    95,     1,   112,   100,
     100,    28,    66,    67,    35,    82,    35,    35,    28,    34,
      32,    75,    33,    33,    89,   100,    33,    31,    29,    88,
      33,    28,    27,    28,    28,    77,    33,    33,    27,    27,
      20,    35,    20,    35,    77,    97,    28,    30,    34,    37,
      89,    30,    34,    33,    76,    77,    77,   113,   125,    95,
       1,    35,    33,    35,    33,    33,   100,    32,    30,    29,
      28,    10,   114,   100,    28,    28,    70,    69,    71,    33,
      89,    33,    27,    11,   119,   126,    77,    77,    27,    27,
      27,    30,   115,   120,    28,    95,    95,     1,   100,    77,
      28,    28,    28,   116,    77,    77,    77,    28,   117,    77,
     118,   114
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    57,    58,    58,    58,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    61,    60,    62,
      60,    63,    63,    64,    64,    64,    66,    65,    67,    65,
      69,    68,    70,    68,    71,    68,    72,    72,    73,    73,
      74,    74,    75,    75,    76,    76,    78,    77,    77,    77,
      79,    79,    80,    80,    80,    80,    81,    81,    82,    82,
      82,    82,    82,    83,    83,    84,    83,    85,    83,    86,
      87,    88,    88,    89,    89,    91,    90,    92,    90,    93,
      90,    94,    94,    95,    95,    96,    96,    97,    97,    98,
      98,    98,    99,    99,   100,   101,   101,   102,   102,   102,
     102,   102,   102,   102,   103,   103,   103,   103,   104,   104,
     104,   105,   105,   105,   105,   106,   106,   106,   107,   107,
     107,   107,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   110,   111,   112,   113,   109,   109,   115,
     116,   117,   118,   114,   114,   120,   119,   119,   122,   121,
     121,   124,   125,   126,   123,   127,   127,   127,   128,   128,
     129,   129,   129,   129
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     6,     0,
       6,     2,     0,     1,     1,     1,     0,     6,     0,     6,
       0,     9,     0,     9,     0,     9,     4,     4,     1,     1,
       9,    10,     1,     0,     3,     1,     0,     4,     2,     3,
       2,     1,     3,     5,     3,     5,     3,     1,     1,     1,
       1,     1,     1,     7,    10,     0,     7,     0,     7,     3,
       3,     5,     3,     3,     1,     0,     8,     0,     8,     0,
       8,     1,     1,     1,     0,     3,     1,     2,     2,     3,
       2,     3,     2,     1,     1,     4,     7,     3,     3,     3,
       3,     5,     5,     1,     3,     3,     2,     1,     3,     3,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     4,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     0,     0,     0,    11,     5,     0,
       0,     0,     0,    10,     0,     0,     3,     0,     0,     5,
       5,     0,     0,     0,     9,     1,     1,     0,     2,     4,
       7,     8,     5,     8
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 16: /* statement: error SEMICOLON  */
#line 191 "parser.y"
        {
            printf("Invalid statement at line %d\n", yylineno);
            yyerrok;
        }
#line 1758 "y.tab.c"
    break;

  case 17: /* $@1: %empty  */
#line 202 "parser.y"
        {
            Symbol* sym = insert_symbol(global_scope, (yyvsp[0].sval),
                                        KIND_ENTITY, DT_ENTITY, yylineno);
            if (sym) {
                strncpy(sym->attr.entity.class_name, (yyvsp[0].sval), 63);
                sym->attr.entity.fields_list       = NULL;
                sym->attr.entity.methods_list      = NULL;
                sym->attr.entity.constructors_list = NULL;
                sym->attr.entity.class_size        = 0;
                sym->attr.entity.parent_class[0]   = '\0';
            }
            emit("entity", (yyvsp[0].sval), "", "");
            SymTable* es = create_scope(SCOPE_ENTITY, (yyvsp[0].sval), current_scope);
	    register_entity_scope(es);
	    if(sym){
		sym->attr.entity.scope = es;
	    }
            current_scope = es;
        }
#line 1782 "y.tab.c"
    break;

  case 18: /* entity_decl: ENTITY IDENTIFIER $@1 LBRACE entity_body RBRACE  */
#line 222 "parser.y"
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
            emit("end_entity", (yyvsp[-4].sval), "", "");
        }
#line 1805 "y.tab.c"
    break;

  case 19: /* $@2: %empty  */
#line 241 "parser.y"
        { emit("entity", (yyvsp[0].sval), "", ""); }
#line 1811 "y.tab.c"
    break;

  case 20: /* entity_decl: ENTITY IDENTIFIER $@2 LBRACE error RBRACE  */
#line 243 "parser.y"
        {
            printf("Invalid entity body at line %d\n", yylineno);
            yyerrok;
            emit("end_entity", (yyvsp[-4].sval), "", "");
        }
#line 1821 "y.tab.c"
    break;

  case 26: /* $@3: %empty  */
#line 266 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_CONSTRUCTOR, DT_VOID, yylineno);
            if (sym) {
                strncpy(sym->attr.ctor.belongs_to, current_scope->name, 63);
                sym->attr.ctor.param_count = 0;
                sym->attr.ctor.param_list  = NULL;
                snprintf(sym->attr.ctor.entry_label, 32, "ctor_%s", (yyvsp[0].sval));
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.constructors_list, (yyvsp[0].sval));

            emit("constr", (yyvsp[0].sval), "", "");
            SymTable* cs = create_scope(SCOPE_CONSTRUCTOR, (yyvsp[0].sval), current_scope);
            current_scope = cs;
        }
#line 1844 "y.tab.c"
    break;

  case 27: /* constructor_decl: IDENTIFIER $@3 LPAREN param_list_opt RPAREN block  */
#line 285 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_constr", (yyvsp[-5].sval), "", "");
        }
#line 1854 "y.tab.c"
    break;

  case 28: /* $@4: %empty  */
#line 291 "parser.y"
        { emit("constr", (yyvsp[0].sval), "", ""); }
#line 1860 "y.tab.c"
    break;

  case 29: /* constructor_decl: IDENTIFIER $@4 LPAREN error RPAREN block  */
#line 293 "parser.y"
        {
            printf("Invalid constructor parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_constr", (yyvsp[-5].sval), "", "");
        }
#line 1870 "y.tab.c"
    break;

  case 30: /* $@5: %empty  */
#line 306 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_METHOD, (yyvsp[-2].dtype), yylineno);
            if (sym) {
                sym->attr.method.return_type = (yyvsp[-2].dtype);
                sym->attr.method.access      = (yyvsp[-3].access);
                sym->attr.method.param_count = 0;
                sym->attr.method.param_list  = NULL;
                strncpy(sym->attr.method.belongs_to, current_scope->name, 63);
                snprintf(sym->attr.method.entry_label, 32,
                         "%s_%s", current_scope->name, (yyvsp[0].sval));
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.methods_list, (yyvsp[0].sval));

            emit("method", (yyvsp[0].sval), "", "");
            SymTable* ms = create_scope(SCOPE_METHOD, (yyvsp[0].sval), current_scope);
            current_scope = ms;
        }
#line 1896 "y.tab.c"
    break;

  case 31: /* method_decl: access_modifier type FUNC IDENTIFIER $@5 LPAREN param_list_opt RPAREN block  */
#line 328 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_method", (yyvsp[-5].sval), "", "");
        }
#line 1906 "y.tab.c"
    break;

  case 32: /* $@6: %empty  */
#line 335 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_METHOD, DT_ENTITY, yylineno);
            if (sym) {
                sym->attr.method.return_type = DT_ENTITY;
                sym->attr.method.access      = (yyvsp[-3].access);
                sym->attr.method.param_count = 0;
                sym->attr.method.param_list  = NULL;
                strncpy(sym->attr.method.belongs_to, current_scope->name, 63);
                snprintf(sym->attr.method.entry_label, 32,
                         "%s_%s", current_scope->name, (yyvsp[0].sval));
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.methods_list, (yyvsp[0].sval));

            emit("method", (yyvsp[0].sval), "", "");
            SymTable* ms = create_scope(SCOPE_METHOD, (yyvsp[0].sval), current_scope);
            current_scope = ms;
        }
#line 1932 "y.tab.c"
    break;

  case 33: /* method_decl: access_modifier IDENTIFIER FUNC IDENTIFIER $@6 LPAREN param_list_opt RPAREN block  */
#line 357 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_method", (yyvsp[-5].sval), "", "");
        }
#line 1942 "y.tab.c"
    break;

  case 34: /* $@7: %empty  */
#line 363 "parser.y"
        { emit("method", (yyvsp[0].sval), "", ""); }
#line 1948 "y.tab.c"
    break;

  case 35: /* method_decl: access_modifier type FUNC IDENTIFIER $@7 LPAREN error RPAREN block  */
#line 365 "parser.y"
        {
            printf("Invalid method parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_method", (yyvsp[-5].sval), "", "");
        }
#line 1958 "y.tab.c"
    break;

  case 36: /* access_var_decl: access_modifier type IDENTIFIER SEMICOLON  */
#line 378 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-1].sval),
                                        KIND_FIELD, (yyvsp[-2].dtype), yylineno);
            if (sym) {
                sym->attr.field.access = (yyvsp[-3].access);
                strncpy(sym->attr.field.belongs_to, current_scope->name, 63);
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.fields_list, (yyvsp[-1].sval));
        }
#line 1975 "y.tab.c"
    break;

  case 37: /* access_var_decl: access_modifier IDENTIFIER IDENTIFIER SEMICOLON  */
#line 392 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-1].sval),
                                        KIND_FIELD, DT_ENTITY, yylineno);
            if (sym) {
                sym->attr.field.access = (yyvsp[-3].access);
                strncpy(sym->attr.field.belongs_to, current_scope->name, 63);
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if (entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.fields_list, (yyvsp[-1].sval));
        }
#line 1992 "y.tab.c"
    break;

  case 38: /* access_modifier: PUBLIC  */
#line 407 "parser.y"
              { (yyval.access) = ACC_PUBLIC;  }
#line 1998 "y.tab.c"
    break;

  case 39: /* access_modifier: PRIVATE  */
#line 408 "parser.y"
              { (yyval.access) = ACC_PRIVATE; }
#line 2004 "y.tab.c"
    break;

  case 40: /* object_decl: IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 416 "parser.y"
        {
	    Symbol* class_sym = lookup(current_scope,(yyvsp[-4].sval));
	    if(!class_sym || class_sym->kind != KIND_ENTITY){
		char buf[256];
	    	snprintf(buf, sizeof(buf),"line %d: Entity '%s' not found to instantiate",yylineno, (yyvsp[-8].sval));
	    	semantic_error(buf);
	    }
	    Symbol* obj = insert_symbol(current_scope,(yyvsp[-7].sval),KIND_OBJECT,DT_OBJECT,yylineno);
	    if(obj){
		strncpy(obj->attr.object.entity_name,class_sym->attr.entity.class_name,63); //supports max class size as 63
		obj->size = class_sym->attr.entity.class_size;
		current_scope->next_offset = obj->offset + obj->size;
	}
		
            emit("new", (yyvsp[-4].sval), "", (yyvsp[-7].sval));
            emit("call_constr", (yyvsp[-4].sval), "", (yyvsp[-7].sval));
        }
#line 2026 "y.tab.c"
    break;

  case 41: /* object_decl: type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list RPAREN SEMICOLON  */
#line 434 "parser.y"
        {
	    	check_method_access((yyvsp[-6].sval),(yyvsp[-4].sval));
	    	Symbol* msym = lookup(current_scope, (yyvsp[-4].sval));
		if(msym && msym->kind == KIND_METHOD){
			if(msym->attr.method.return_type != (yyvsp[-9].dtype)){
				fprintf(stderr, "ERROR line %d: cannot assign result of method '%s' (returns %s) to '%s' (declared as %s).\n", yylineno, (yyvsp[-4].sval), dt_names[msym->attr.method.return_type], (yyvsp[-8].sval), dt_names[(yyvsp[-9].dtype)]);
			}
		}
            	char* t = genVar();
            	emit("push_ptr", (yyvsp[-6].sval), "", "");
            	emit("call_method", (yyvsp[-4].sval), "", t);
            	emit("=", t, "", (yyvsp[-8].sval));
        }
#line 2044 "y.tab.c"
    break;

  case 43: /* arg_list_opt: %empty  */
#line 451 "parser.y"
                        { call_arg_count = 0; }
#line 2050 "y.tab.c"
    break;

  case 44: /* arg_list: arg_list COMMA expression  */
#line 456 "parser.y"
        {
		if(call_arg_count < 64){
			call_arg_types[call_arg_count++] = last_expr_type;
		} 
		emit("arg", (yyvsp[0].sval), "", ""); 
	}
#line 2061 "y.tab.c"
    break;

  case 45: /* arg_list: expression  */
#line 463 "parser.y"
        { 
		call_arg_count = 0;
		call_arg_types[call_arg_count++] = last_expr_type;
		emit("arg", (yyvsp[0].sval), "", ""); 
	}
#line 2071 "y.tab.c"
    break;

  case 46: /* $@8: %empty  */
#line 475 "parser.y"
        {
            SymTable* bs = create_scope(SCOPE_BLOCK, "block", current_scope);
            current_scope = bs;
        }
#line 2080 "y.tab.c"
    break;

  case 47: /* block: LBRACE $@8 stmt_list RBRACE  */
#line 480 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 2089 "y.tab.c"
    break;

  case 49: /* block: LBRACE error RBRACE  */
#line 486 "parser.y"
        {
            printf("Error inside block at line %d\n", yylineno);
            yyerrok;
        }
#line 2098 "y.tab.c"
    break;

  case 52: /* var_decl: type id_list SEMICOLON  */
#line 513 "parser.y"
        {
            insert_var_list((yyvsp[-1].sval), (yyvsp[-2].dtype));
            free((yyvsp[-1].sval));
        }
#line 2107 "y.tab.c"
    break;

  case 53: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 520 "parser.y"
        {
	    if(last_expr_type != DT_UNKNOWN && last_expr_type != (yyvsp[-4].dtype)){
		//printf("==== $1 = %d and last_expr_type = %d\n", $1, last_expr_type);
		fprintf(stderr, "ERROR hih line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, (yyvsp[-3].sval), dt_names[(yyvsp[-4].dtype)], dt_names[last_expr_type]);
	    }
            emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval));
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
                                        KIND_VAR, (yyvsp[-4].dtype), yylineno);
            if (sym) sym->is_initialized = 1;
        }
#line 2122 "y.tab.c"
    break;

  case 54: /* var_decl: IDENTIFIER id_list SEMICOLON  */
#line 533 "parser.y"
        {
            insert_var_list((yyvsp[-1].sval), DT_ENTITY);
            free((yyvsp[-1].sval));
        }
#line 2131 "y.tab.c"
    break;

  case 55: /* var_decl: IDENTIFIER IDENTIFIER ASSIGN expression SEMICOLON  */
#line 540 "parser.y"
        {
		fprintf(stderr, "ERROR line %d: cannot initialize entity variable '%s' with value of type %s.\n", yylineno, (yyvsp[-3].sval), dt_names[last_expr_type]);
		emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval));
		Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
						KIND_VAR, DT_ENTITY, yylineno);
		if (sym) sym->is_initialized = 1;
        }
#line 2143 "y.tab.c"
    break;

  case 56: /* id_list: id_list COMMA IDENTIFIER  */
#line 556 "parser.y"
        {
            char* buf = malloc(strlen((yyvsp[-2].sval)) + strlen((yyvsp[0].sval)) + 2);
            sprintf(buf, "%s,%s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval));
            (yyval.sval) = buf;
        }
#line 2154 "y.tab.c"
    break;

  case 57: /* id_list: IDENTIFIER  */
#line 563 "parser.y"
        {
            (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 2162 "y.tab.c"
    break;

  case 58: /* type: INT  */
#line 573 "parser.y"
             { (yyval.dtype) = DT_INT;    }
#line 2168 "y.tab.c"
    break;

  case 59: /* type: FP  */
#line 574 "parser.y"
             { (yyval.dtype) = DT_FLOAT;  }
#line 2174 "y.tab.c"
    break;

  case 60: /* type: CHR  */
#line 575 "parser.y"
             { (yyval.dtype) = DT_CHAR;   }
#line 2180 "y.tab.c"
    break;

  case 61: /* type: STRING  */
#line 576 "parser.y"
             { (yyval.dtype) = DT_STRING; }
#line 2186 "y.tab.c"
    break;

  case 62: /* type: BOOL  */
#line 577 "parser.y"
             { (yyval.dtype) = DT_BOOL;   }
#line 2192 "y.tab.c"
    break;

  case 63: /* array_decl: type SEQ1 IDENTIFIER LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 591 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-4].sval),
                                        KIND_ARRAY, (yyvsp[-6].dtype), yylineno);
            if (sym) {
                sym->attr.array.dimensions     = 1;
                sym->attr.array.dim1           = (yyvsp[-2].ival);
                sym->attr.array.dim2           = 0;
                sym->attr.array.is_initialized = 0;
                /* offset was already set by insert_symbol to the correct
                   position; now fix up size and advance next_offset      */
                sym->size = datatype_size((yyvsp[-6].dtype)) * (yyvsp[-2].ival);
                current_scope->next_offset = sym->offset + sym->size;
		
            }
        }
#line 2212 "y.tab.c"
    break;

  case 64: /* array_decl: type SEQ2 IDENTIFIER LBRACKET INT_LITERAL RBRACKET LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 610 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-7].sval),
                                        KIND_ARRAY, (yyvsp[-9].dtype), yylineno);
            if (sym) {
                sym->attr.array.dimensions     = 2;
                sym->attr.array.dim1           = (yyvsp[-5].ival);
                sym->attr.array.dim2           = (yyvsp[-2].ival);
                sym->attr.array.is_initialized = 0;
                sym->size = datatype_size((yyvsp[-9].dtype)) * (yyvsp[-5].ival) * (yyvsp[-2].ival);
                current_scope->next_offset = sym->offset + sym->size;
            }
        }
#line 2229 "y.tab.c"
    break;

  case 65: /* $@9: %empty  */
#line 626 "parser.y"
        {
		current_array_elem_type = (yyvsp[-3].dtype);
		array_type_errors = 0;
	}
#line 2238 "y.tab.c"
    break;

  case 66: /* array_decl: type SEQ1 IDENTIFIER ASSIGN $@9 array_init SEMICOLON  */
#line 631 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-4].sval),
                                        KIND_ARRAY, (yyvsp[-6].dtype), yylineno);
            if (sym) {
                sym->attr.array.dimensions = 1;
                sym->attr.array.is_initialized = 1;
		sym->attr.array.dim1 = (yyvsp[-1].ival);	// the token or symbol array_init returns the number of elements present in the list
		sym->attr.array.dim2 = 0;
		sym->size = datatype_size((yyvsp[-6].dtype)) * (yyvsp[-1].ival);
		current_scope->next_offset = sym->offset + sym->size;
            }
		current_array_elem_type = DT_UNKNOWN;
        }
#line 2256 "y.tab.c"
    break;

  case 67: /* $@10: %empty  */
#line 645 "parser.y"
        {
		current_array_elem_type = (yyvsp[-3].dtype);
		array_type_errors = 0;
		arr2d_rows = 0;
		arr2d_cols = 0;
	}
#line 2267 "y.tab.c"
    break;

  case 68: /* array_decl: type SEQ2 IDENTIFIER ASSIGN $@10 array_init2d SEMICOLON  */
#line 652 "parser.y"
        {
		Symbol* sym = insert_symbol(current_scope, (yyvsp[-4].sval), KIND_ARRAY, (yyvsp[-6].dtype), yylineno);
		if(sym) {
			sym->attr.array.dimensions = 2;
			sym->attr.array.dim1 = arr2d_rows;
			sym->attr.array.dim2 = arr2d_cols;
			sym->attr.array.is_initialized = 1;
			sym->size = datatype_size((yyvsp[-6].dtype)) * arr2d_rows * arr2d_cols;
			current_scope->next_offset = sym->offset + sym->size;
		}
		current_array_elem_type = DT_UNKNOWN;
	}
#line 2284 "y.tab.c"
    break;

  case 69: /* array_init: LBRACE expr_list RBRACE  */
#line 667 "parser.y"
                              { (yyval.ival) = (yyvsp[-1].ival); }
#line 2290 "y.tab.c"
    break;

  case 70: /* array_init2d: LBRACE row_list RBRACE  */
#line 671 "parser.y"
                                   { (yyval.ival) = (yyvsp[-1].ival); }
#line 2296 "y.tab.c"
    break;

  case 71: /* row_list: row_list COMMA LBRACE expr_list RBRACE  */
#line 676 "parser.y"
        { 
		arr2d_rows++;
		arr2d_cols = (yyvsp[-1].ival);
		(yyval.ival) = (yyvsp[-4].ival) + (yyvsp[-1].ival); 
	}
#line 2306 "y.tab.c"
    break;

  case 72: /* row_list: LBRACE expr_list RBRACE  */
#line 682 "parser.y"
        { 
		arr2d_rows = 1;
		arr2d_cols = (yyvsp[-1].ival);
		(yyval.ival) = (yyvsp[-1].ival); 
	}
#line 2316 "y.tab.c"
    break;

  case 73: /* expr_list: expr_list COMMA expression  */
#line 691 "parser.y"
        {
		if(current_array_elem_type != DT_UNKNOWN && last_expr_type != DT_UNKNOWN && last_expr_type != current_array_elem_type){
			{
				fprintf(stderr, "ERROR line %d: array initializer type mismatch - expected %s, got %s\n", yylineno, dt_names[current_array_elem_type], dt_names[last_expr_type]);
				array_type_errors++;
			} 
		}
		(yyval.ival) = (yyvsp[-2].ival) + 1;
	}
#line 2330 "y.tab.c"
    break;

  case 74: /* expr_list: expression  */
#line 701 "parser.y"
        {
                if(current_array_elem_type != DT_UNKNOWN && last_expr_type != DT_UNKNOWN && last_expr_type != current_array_elem_type){
                        {
                                fprintf(stderr, "ERROR line %d: array initializer type mismatch - expected %s, got %s\n", yylineno, dt_names[current_array_elem_type], dt_names[last_expr_type]);
                                array_type_errors++;
                        }
        	}
		(yyval.ival) = 1;
	}
#line 2344 "y.tab.c"
    break;

  case 75: /* $@11: %empty  */
#line 718 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_FUNCTION, (yyvsp[-2].dtype), yylineno);
            if (sym) {
                sym->attr.func.return_type = (yyvsp[-2].dtype);
                sym->attr.func.param_count = 0;
                sym->attr.func.param_list  = NULL;
                snprintf(sym->attr.func.entry_label, 32, "func_%s", (yyvsp[0].sval));
                /* size stays 0 — functions occupy no slot in parent scope */
            }
	    current_function = sym;	// Storing the current function we are in when we are entering the function declaration
            emit("func", (yyvsp[0].sval), "", "");
            SymTable* fs = create_scope(SCOPE_FUNCTION, (yyvsp[0].sval), current_scope);
            current_scope = fs;
        }
#line 2364 "y.tab.c"
    break;

  case 76: /* function_decl: func_type FUNC IDENTIFIER $@11 LPAREN param_list_opt RPAREN block  */
#line 734 "parser.y"
        {
            /* Do NOT write next_offset into sym->size.
               Function size in global scope = 0 always.
               The frame size is visible in the function's own scope table. */
            print_table(current_scope);
            current_scope = current_scope->parent;
	    current_function = NULL; 	// Clearing the variable when we are at the end of the current function declaration 
            emit("endfunc", "", "", "");
        }
#line 2378 "y.tab.c"
    break;

  case 77: /* $@12: %empty  */
#line 746 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_FUNCTION, DT_ENTITY, yylineno);
            if (sym) {
                sym->attr.func.return_type = DT_ENTITY;
                sym->attr.func.param_count = 0;
                sym->attr.func.param_list  = NULL;
                snprintf(sym->attr.func.entry_label, 32, "func_%s", (yyvsp[0].sval));
                /* size stays 0 */
            }
	    current_function = sym; 	// Storing the current function symbol being parsed to handle correct type of value(or variable) being returned
            emit("func", (yyvsp[0].sval), "", "");
            SymTable* fs = create_scope(SCOPE_FUNCTION, (yyvsp[0].sval), current_scope);
            current_scope = fs;
        }
#line 2398 "y.tab.c"
    break;

  case 78: /* function_decl: IDENTIFIER FUNC IDENTIFIER $@12 LPAREN param_list_opt RPAREN block  */
#line 762 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
	    current_function = NULL;
            emit("endfunc", "", "", "");
        }
#line 2409 "y.tab.c"
    break;

  case 79: /* $@13: %empty  */
#line 771 "parser.y"
        { emit("func", (yyvsp[0].sval), "", ""); }
#line 2415 "y.tab.c"
    break;

  case 80: /* function_decl: func_type FUNC IDENTIFIER $@13 LPAREN error RPAREN block  */
#line 773 "parser.y"
        {
            printf("Invalid parameter list at line %d\n", yylineno);
            yyerrok;
            emit("endfunc", "", "", "");
        }
#line 2425 "y.tab.c"
    break;

  case 81: /* func_type: type  */
#line 781 "parser.y"
           { (yyval.dtype) = (yyvsp[0].dtype);      }
#line 2431 "y.tab.c"
    break;

  case 82: /* func_type: VOID  */
#line 782 "parser.y"
           { (yyval.dtype) = DT_VOID; }
#line 2437 "y.tab.c"
    break;

  case 87: /* param: type IDENTIFIER  */
#line 798 "parser.y"
        {
            emit("param", (yyvsp[0].sval), "", "");
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_PARAM, (yyvsp[-1].dtype), yylineno);
            Symbol* owner = lookup(current_scope->parent,
                                   current_scope->name);
            if (owner) {
                if (owner->kind == KIND_FUNCTION)
                    add_param(&owner->attr.func.param_list,
                              &owner->attr.func.param_count, (yyvsp[0].sval), (yyvsp[-1].dtype));
                else if (owner->kind == KIND_METHOD)
                    add_param(&owner->attr.method.param_list,
                              &owner->attr.method.param_count, (yyvsp[0].sval), (yyvsp[-1].dtype));
                else if (owner->kind == KIND_CONSTRUCTOR)
                    add_param(&owner->attr.ctor.param_list,
                              &owner->attr.ctor.param_count, (yyvsp[0].sval), (yyvsp[-1].dtype));
            }
            (void)sym;
        }
#line 2461 "y.tab.c"
    break;

  case 88: /* param: IDENTIFIER IDENTIFIER  */
#line 819 "parser.y"
        {
            emit("param", (yyvsp[0].sval), "", "");
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_PARAM, DT_ENTITY, yylineno);
            Symbol* owner = lookup(current_scope->parent,
                                   current_scope->name);
            if (owner) {
                if (owner->kind == KIND_FUNCTION)
                    add_param(&owner->attr.func.param_list,
                              &owner->attr.func.param_count, (yyvsp[0].sval), DT_ENTITY);
                else if (owner->kind == KIND_METHOD)
                    add_param(&owner->attr.method.param_list,
                              &owner->attr.method.param_count, (yyvsp[0].sval), DT_ENTITY);
                else if (owner->kind == KIND_CONSTRUCTOR)
                    add_param(&owner->attr.ctor.param_list,
                              &owner->attr.ctor.param_count, (yyvsp[0].sval), DT_ENTITY);
            }
            (void)sym;
        }
#line 2485 "y.tab.c"
    break;

  case 89: /* return_stmt: RETURN expression SEMICOLON  */
#line 845 "parser.y"
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
		emit("return", (yyvsp[-1].sval), "", ""); 
	}
#line 2503 "y.tab.c"
    break;

  case 90: /* return_stmt: RETURN SEMICOLON  */
#line 859 "parser.y"
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
#line 2518 "y.tab.c"
    break;

  case 91: /* return_stmt: RETURN error SEMICOLON  */
#line 871 "parser.y"
        {
            printf("Invalid return statement at line %d\n", yylineno);
            yyerrok;
        }
#line 2527 "y.tab.c"
    break;

  case 94: /* expression: assignment  */
#line 883 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2533 "y.tab.c"
    break;

  case 95: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET  */
#line 891 "parser.y"
        {
            char* t1 = genVar(); emit("*",  (yyvsp[-1].sval), "type.width", t1);
            char* t2 = genVar(); emit("[]", (yyvsp[-3].sval), t1,           t2);
            (yyval.sval) = t2;
        }
#line 2543 "y.tab.c"
    break;

  case 96: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET  */
#line 897 "parser.y"
        {
            char* t1 = genVar(); emit("*",  (yyvsp[-4].sval), "array.cols", t1);
            char* t2 = genVar(); emit("+",  t1, (yyvsp[-1].sval),           t2);
            char* t3 = genVar(); emit("*",  t2, "type.width", t3);
            char* t4 = genVar(); emit("[]", (yyvsp[-6].sval), t3,           t4);
            (yyval.sval) = t4;
        }
#line 2555 "y.tab.c"
    break;

  case 97: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 908 "parser.y"
        {
		// Checking the type before assignment
		Symbol* lhs = lookup(current_scope, (yyvsp[-2].sval));
		if(lhs && last_expr_type != DT_UNKNOWN){
			if(lhs->datatype != last_expr_type){
				fprintf(stderr, "ERROR line %d: cannot assign %s to '%s' "
				"(declared as %s).\n", yylineno, dt_names[last_expr_type], (yyvsp[-2].sval), dt_names[lhs->datatype]);
			}
		} 
		emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval)); 
		(yyval.sval) = strdup((yyvsp[-2].sval)); 
	}
#line 2572 "y.tab.c"
    break;

  case 98: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 921 "parser.y"
        { 
		Symbol* lhs = lookup(current_scope, (yyvsp[-2].sval));
		if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
			fprintf(stderr, "ERROR line %d: type mismatch in '+=' : '%s' is %s but RHS is %s.\n", yylineno, (yyvsp[-2].sval), dt_names[lhs->datatype], dt_names[last_expr_type]);
		}
		char* t = genVar(); 
		emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t); 
		emit("=", t, "", (yyvsp[-2].sval)); 
		(yyval.sval) = t; 
	}
#line 2587 "y.tab.c"
    break;

  case 99: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 932 "parser.y"
        { 	
		Symbol* lhs = lookup(current_scope, (yyvsp[-2].sval));
		if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
			fprintf(stderr, "ERROR line %d: type mismatch in '-=' : '%s' is %s but RHS is %s.\n", yylineno, (yyvsp[-2].sval), dt_names[lhs->datatype], dt_names[last_expr_type]);
		}
			
		char* t = genVar(); 
		emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t); 
		emit("=", t, "", (yyvsp[-2].sval)); 	
		(yyval.sval) = t; 
	}
#line 2603 "y.tab.c"
    break;

  case 100: /* assignment: indexed_id ASSIGN assignment  */
#line 944 "parser.y"
        { (yyval.sval) = (yyvsp[0].sval); }
#line 2609 "y.tab.c"
    break;

  case 101: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 946 "parser.y"
        { //check_field_access("this", $3); 
		//Symbol* method_sym = lookup(current_scope, current_scope->name);
		const char* entity_name = NULL;
		if(current_function && current_function->kind == KIND_METHOD){
			entity_name = current_function->attr.method.belongs_to;
		}
		if(entity_name){
			Symbol* entity_sym = lookup(global_scope, entity_name);
			if(entity_sym && entity_sym->kind == KIND_ENTITY){
				Symbol* field = lookup(global_scope, (yyvsp[-2].sval));
				while(field){
					if(field->kind == KIND_FIELD && strcmp(field->attr.field.belongs_to, entity_name) == 0){
						break;
					}
				field = field->next;
				}
				if(field && last_expr_type != DT_UNKNOWN && field->datatype != last_expr_type){
					fprintf(stderr, "ERROR line %d: Cannot assign %s to field '%s.%s' (declared as %s).\n", yylineno, dt_names[last_expr_type], entity_name, (yyvsp[-2].sval), dt_names[field->datatype]);
				}
			}
		}	
		emit("set_field", "this", (yyvsp[-2].sval), (yyvsp[0].sval)); 
		(yyval.sval) = (yyvsp[0].sval); 
	}
#line 2638 "y.tab.c"
    break;

  case 102: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 971 "parser.y"
        { 
		check_field_access((yyvsp[-4].sval),(yyvsp[-2].sval)); 
		Symbol* obj = lookup(current_scope, (yyvsp[-4].sval));
		if(!obj || obj->kind != KIND_OBJECT){
			fprintf(stderr, "ERROR line %d: '%s' is not an object.\n", yylineno, (yyvsp[-4].sval));
		}
		else{
			const char* entity_name = obj->attr.object.entity_name;
			SymTable* ent_scope = find_entity_scope(entity_name);
			if(!ent_scope){
				fprintf(stderr, "ERROR line %d: Entity %s never defined.\n", yylineno, entity_name);
			}
			else{
				Symbol* field = lookup_local(ent_scope, (yyvsp[-2].sval));
				if(!field || field->kind != KIND_FIELD){
					fprintf(stderr, "ERROR line %d: '%s' is not a field of entity '%s'.\n", yylineno, (yyvsp[-2].sval), entity_name);
				}
				else if(last_expr_type != DT_UNKNOWN && field->datatype != last_expr_type){
					fprintf(stderr, "ERROR line %d: cannot assign %s to '%s.%s' (declared as %s).\n", yylineno, dt_names[last_expr_type], entity_name, (yyvsp[-2].sval), dt_names[field->datatype]);
				}
			}
		}
		emit("set_field", (yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); 
		(yyval.sval) = (yyvsp[0].sval); 
	}
#line 2668 "y.tab.c"
    break;

  case 103: /* assignment: logic_expr  */
#line 996 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2674 "y.tab.c"
    break;

  case 104: /* logic_expr: logic_expr OR logic_expr  */
#line 1001 "parser.y"
        { char* t = genVar(); emit("||", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2680 "y.tab.c"
    break;

  case 105: /* logic_expr: logic_expr AND logic_expr  */
#line 1003 "parser.y"
        { char* t = genVar(); emit("&&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2686 "y.tab.c"
    break;

  case 106: /* logic_expr: NOT logic_expr  */
#line 1005 "parser.y"
        { char* t = genVar(); emit("!", (yyvsp[0].sval), "", t); (yyval.sval) = t; }
#line 2692 "y.tab.c"
    break;

  case 107: /* logic_expr: bitwise_expr  */
#line 1006 "parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 2698 "y.tab.c"
    break;

  case 108: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 1011 "parser.y"
        { char* t = genVar(); emit("&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2704 "y.tab.c"
    break;

  case 109: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 1013 "parser.y"
        { char* t = genVar(); emit("|", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2710 "y.tab.c"
    break;

  case 110: /* bitwise_expr: rel_expr  */
#line 1014 "parser.y"
               { (yyval.sval) = (yyvsp[0].sval); }
#line 2716 "y.tab.c"
    break;

  case 111: /* rel_expr: arith_expr GT arith_expr  */
#line 1019 "parser.y"
        {
            char* folded = foldConstants(">", (yyvsp[-2].sval), (yyvsp[0].sval));
            if (folded) {
                (yyval.sval) = folded;
            } else {
                char* t = genVar(); emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
            }
        }
#line 2729 "y.tab.c"
    break;

  case 112: /* rel_expr: arith_expr LT arith_expr  */
#line 1028 "parser.y"
        {
            char* folded = foldConstants("<", (yyvsp[-2].sval), (yyvsp[0].sval));
            if (folded) {
                (yyval.sval) = folded;
            } else {
                char* t = genVar(); emit("<", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
            }
        }
#line 2742 "y.tab.c"
    break;

  case 113: /* rel_expr: arith_expr EQ arith_expr  */
#line 1037 "parser.y"
        {
            char* folded = foldConstants("==", (yyvsp[-2].sval), (yyvsp[0].sval));
            if (folded) {
                (yyval.sval) = folded;
            } else {
                char* t = genVar(); emit("==", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
            }
        }
#line 2755 "y.tab.c"
    break;

  case 114: /* rel_expr: arith_expr  */
#line 1045 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2761 "y.tab.c"
    break;

  case 115: /* arith_expr: arith_expr PLUS term  */
#line 1050 "parser.y"
        {  char* folded = foldConstants("+", (yyvsp[-2].sval), (yyvsp[0].sval));
            if (folded) {
                (yyval.sval) = folded;                       
            } else {
                char* t = genVar();
                emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
            } 
        }
#line 2775 "y.tab.c"
    break;

  case 116: /* arith_expr: arith_expr MINUS term  */
#line 1060 "parser.y"
        {   char* folded = foldConstants("-", (yyvsp[-2].sval), (yyvsp[0].sval));
            if (folded) {
                (yyval.sval) = folded;
            } else {
                char* t = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
            } 
        }
#line 2789 "y.tab.c"
    break;

  case 117: /* arith_expr: term  */
#line 1069 "parser.y"
           { (yyval.sval) = (yyvsp[0].sval); }
#line 2795 "y.tab.c"
    break;

  case 118: /* term: term MUL factor  */
#line 1074 "parser.y"
        {
            char* folded = foldConstants("*", (yyvsp[-2].sval), (yyvsp[0].sval));
            if (folded) {
                (yyval.sval) = folded;
            } else {
                char* t = genVar();
                emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
            }
        }
#line 2810 "y.tab.c"
    break;

  case 119: /* term: term DIV factor  */
#line 1085 "parser.y"
        {
            char* folded = foldConstants("/", (yyvsp[-2].sval), (yyvsp[0].sval));
            if (folded) {
                (yyval.sval) = folded;
            } else {
                char* t = genVar();
                emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
            }
        }
#line 2825 "y.tab.c"
    break;

  case 120: /* term: term MOD factor  */
#line 1096 "parser.y"
        {
            char* folded = foldConstants("%", (yyvsp[-2].sval), (yyvsp[0].sval));
            if (folded) {
                (yyval.sval) = folded;
            } else {
                char* t = genVar();
                emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
            }
        }
#line 2840 "y.tab.c"
    break;

  case 121: /* term: factor  */
#line 1106 "parser.y"
             { (yyval.sval) = (yyvsp[0].sval); }
#line 2846 "y.tab.c"
    break;

  case 122: /* factor: IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 1111 "parser.y"
        { 
		char* t = genVar(); 
		Symbol* fsym = lookup(current_scope, (yyvsp[-3].sval));
		if(!fsym) {
			fprintf(stderr, 
				"ERROR line %d: call to undeclared function '%s'.\n",yylineno, (yyvsp[-3].sval));
			last_expr_type = DT_UNKNOWN;
		}
		else if(fsym->kind != KIND_FUNCTION && fsym->kind != KIND_METHOD){
			fprintf(stderr, "ERROR line %d: '%s' is not a function.\n", yylineno, (yyvsp[-3].sval));
			last_expr_type = DT_UNKNOWN;
		}
		else{
			ParamNode* ep = (fsym->kind == KIND_FUNCTION) ? fsym->attr.func.param_list : fsym->attr.method.param_list;
			int expected_count = (fsym->kind == KIND_FUNCTION) ? fsym->attr.func.param_count : fsym->attr.method.param_count;
			if(call_arg_count != expected_count) {
				fprintf(stderr, "ERROR line %d:, function '%s' expects %d arg(s), got %d.\n", yylineno, (yyvsp[-3].sval), expected_count, call_arg_count);
			}
			else{
				for(int i = 0; i < call_arg_count && ep; i++, ep=ep->next) {
					if(call_arg_types[i] != DT_UNKNOWN && call_arg_types[i] != ep->datatype) {
						fprintf(stderr, "ERROR line %d: function '%s' argument %d - expected %s, but got %s.\n", yylineno, (yyvsp[-3].sval), i+1, dt_names[ep->datatype], dt_names[call_arg_types[i]]);
					}
				}
			}
			last_expr_type = (fsym->kind == KIND_FUNCTION) ? fsym->attr.func.return_type : fsym->attr.method.return_type;
		}
		emit("call", (yyvsp[-3].sval), "", t);
		(yyval.sval) = t; 
	}
#line 2881 "y.tab.c"
    break;

  case 123: /* factor: IDENTIFIER DOT IDENTIFIER  */
#line 1142 "parser.y"
    {
        char* t = genVar();
        last_expr_type = DT_UNKNOWN;  /* safe default */

        /* Step 1: look up the object variable */
        Symbol* obj = lookup(current_scope, (yyvsp[-2].sval));
        if (!obj) {
            fprintf(stderr,
                "ERROR line %d: undeclared identifier '%s'.\n",
                yylineno, (yyvsp[-2].sval));
        }
        else if (obj->kind != KIND_OBJECT) {
            fprintf(stderr,
                "ERROR line %d: '%s' is not an object.\n",
                yylineno, (yyvsp[-2].sval));
        }
        else {
            /* Step 2: get entity name safely */
            const char* entity_name = obj->attr.object.entity_name;
            if (!entity_name || entity_name[0] == '\0') {
                fprintf(stderr,
                    "ERROR line %d: object '%s' has no entity type.\n",
                    yylineno, (yyvsp[-2].sval));
            }
            else {
                /* Step 3: look up the entity symbol in global scope */
                Symbol* cls = lookup(global_scope, entity_name);
                if (!cls || cls->kind != KIND_ENTITY) {
                    fprintf(stderr,
                        "ERROR line %d: entity '%s' not defined.\n",
                        yylineno, entity_name);
                }
                else {
                    /* Step 4: get the entity's scope (stored in EntityAttr) */
                    SymTable* escope = cls->attr.entity.scope;
                    if (!escope) {
                        fprintf(stderr,
                            "ERROR line %d: entity '%s' has no scope.\n",
                            yylineno, entity_name);
                    }
                    else {
                        /* Step 5: look up the field inside entity scope */
                        Symbol* field = lookup_local(escope, (yyvsp[0].sval));
                        if (!field || field->kind != KIND_FIELD) {
                            fprintf(stderr,
                                "ERROR line %d: '%s' is not a field of '%s'.\n",
                                yylineno, (yyvsp[0].sval), entity_name);
                        }
                        else {
                            /* Step 6: check private access */
                            if (field->attr.field.access == ACC_PRIVATE &&
                                strcmp(current_scope->name, entity_name) != 0) {
                                fprintf(stderr,
                                    "ERROR line %d: field '%s' of '%s' is private.\n",
                                    yylineno, (yyvsp[0].sval), entity_name);
                            }
                            else {
                                last_expr_type = field->datatype;
                            }
                        }
                    }
                }
            }
        }

        emit("get_field", (yyvsp[-2].sval), (yyvsp[0].sval), t);
        (yyval.sval) = t;
    }
#line 2954 "y.tab.c"
    break;

  case 124: /* factor: IDENTIFIER  */
#line 1212 "parser.y"
        { 
		Symbol* s = lookup(current_scope, (yyvsp[0].sval));
		last_expr_type = s ? s->datatype : DT_UNKNOWN;
		(yyval.sval) = strdup((yyvsp[0].sval)); 
	}
#line 2964 "y.tab.c"
    break;

  case 125: /* factor: indexed_id  */
#line 1217 "parser.y"
                      { (yyval.sval) = (yyvsp[0].sval); }
#line 2970 "y.tab.c"
    break;

  case 126: /* factor: INT_LITERAL  */
#line 1218 "parser.y"
                      { char b[20]; sprintf(b, "%d",   (yyvsp[0].ival)); (yyval.sval) = strdup(b); last_expr_type = DT_INT; }
#line 2976 "y.tab.c"
    break;

  case 127: /* factor: FLOAT_LITERAL  */
#line 1219 "parser.y"
                      { char b[20]; sprintf(b, "%f",   (yyvsp[0].fval)); (yyval.sval) = strdup(b);  last_expr_type = DT_FLOAT; }
#line 2982 "y.tab.c"
    break;

  case 128: /* factor: CHAR_LITERAL  */
#line 1220 "parser.y"
                      { char b[20]; sprintf(b, "'%c'", (yyvsp[0].cval)); (yyval.sval) = strdup(b); last_expr_type = DT_CHAR; }
#line 2988 "y.tab.c"
    break;

  case 129: /* factor: STRING_LITERAL  */
#line 1221 "parser.y"
                      { (yyval.sval) = strdup((yyvsp[0].sval));  last_expr_type = DT_STRING; }
#line 2994 "y.tab.c"
    break;

  case 130: /* factor: TRUE  */
#line 1222 "parser.y"
                      { (yyval.sval) = strdup("1"); last_expr_type = DT_BOOL; }
#line 3000 "y.tab.c"
    break;

  case 131: /* factor: FALSE  */
#line 1223 "parser.y"
                      { (yyval.sval) = strdup("0"); last_expr_type = DT_BOOL; }
#line 3006 "y.tab.c"
    break;

  case 132: /* factor: LPAREN expression RPAREN  */
#line 1224 "parser.y"
                               { (yyval.sval) = (yyvsp[-1].sval); }
#line 3012 "y.tab.c"
    break;

  case 133: /* $@14: %empty  */
#line 1232 "parser.y"
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
#line 3033 "y.tab.c"
    break;

  case 134: /* $@15: %empty  */
#line 1249 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3039 "y.tab.c"
    break;

  case 135: /* $@16: %empty  */
#line 1251 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3048 "y.tab.c"
    break;

  case 136: /* $@17: %empty  */
#line 1256 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3054 "y.tab.c"
    break;

  case 137: /* if_stmt: IF LPAREN $@14 expression $@15 RPAREN $@16 block $@17 elif_list else_opt  */
#line 1258 "parser.y"
        { emit("label", "", "", topEnd()); popIfLabels(); }
#line 3060 "y.tab.c"
    break;

  case 138: /* if_stmt: IF LPAREN error RPAREN block  */
#line 1261 "parser.y"
        {
            printf("Invalid IF condition at line %d\n", yylineno);
            yyerrok;
        }
#line 3069 "y.tab.c"
    break;

  case 139: /* $@18: %empty  */
#line 1269 "parser.y"
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
#line 3090 "y.tab.c"
    break;

  case 140: /* $@19: %empty  */
#line 1286 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3096 "y.tab.c"
    break;

  case 141: /* $@20: %empty  */
#line 1288 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3105 "y.tab.c"
    break;

  case 142: /* $@21: %empty  */
#line 1293 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3111 "y.tab.c"
    break;

  case 145: /* $@22: %empty  */
#line 1300 "parser.y"
        {
            char else_name[32];
            snprintf(else_name, 32, "else_%d", if_cnt++);
            SymTable* es = create_scope(SCOPE_ELSE, else_name, current_scope);
            current_scope = es;
        }
#line 3122 "y.tab.c"
    break;

  case 146: /* else_opt: ELSE $@22 block  */
#line 1307 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3131 "y.tab.c"
    break;

  case 148: /* $@23: %empty  */
#line 1319 "parser.y"
        {
            char for_name[32];
            snprintf(for_name, 32, "for_%d", for_cnt++);
            SymTable* fs = create_scope(SCOPE_FOR, for_name, current_scope);
            current_scope = fs;
        }
#line 3142 "y.tab.c"
    break;

  case 149: /* for_stmt: FOR LPAREN $@23 for_header block  */
#line 1326 "parser.y"
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
#line 3162 "y.tab.c"
    break;

  case 150: /* for_stmt: FOR LPAREN error RPAREN block  */
#line 1343 "parser.y"
        {
            printf("Invalid FOR header at line %d\n", yylineno);
            yyerrok;
        }
#line 3171 "y.tab.c"
    break;

  case 151: /* $@24: %empty  */
#line 1351 "parser.y"
        {
            char* b = getLabel();
            char* e = getLabel();
            pushIfLabels(b, e);
            emit("label", "", "", b);
        }
#line 3182 "y.tab.c"
    break;

  case 152: /* $@25: %empty  */
#line 1358 "parser.y"
        {
            emit("ifFalse", (yyvsp[-1].sval), "", topEnd());
            forDepth++;
            forIncIdx[forDepth]      = 0;
            inForIncrement[forDepth] = 1;
        }
#line 3193 "y.tab.c"
    break;

  case 153: /* $@26: %empty  */
#line 1365 "parser.y"
        { inForIncrement[forDepth] = 0; }
#line 3199 "y.tab.c"
    break;

  case 158: /* var_decl_no_semi: type IDENTIFIER  */
#line 1377 "parser.y"
        {
            insert_symbol(current_scope, (yyvsp[0].sval), KIND_VAR, (yyvsp[-1].dtype), yylineno);
        }
#line 3207 "y.tab.c"
    break;

  case 159: /* var_decl_no_semi: type IDENTIFIER ASSIGN expression  */
#line 1381 "parser.y"
        {
	    	if(last_expr_type != DT_UNKNOWN && last_expr_type != (yyvsp[-3].dtype)){
			fprintf(stderr, "ERROR line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, (yyvsp[-2].sval), dt_names[(yyvsp[-3].dtype)], dt_names[last_expr_type]);
		}
            	emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval));
           	Symbol* sym = insert_symbol(current_scope, (yyvsp[-2].sval),
                                        KIND_VAR, (yyvsp[-3].dtype), yylineno);
            	if (sym) sym->is_initialized = 1;
        }
#line 3221 "y.tab.c"
    break;

  case 160: /* io_stmt: IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 1397 "parser.y"
        {
		Symbol* var = lookup(current_scope, (yyvsp[-6].sval));
		if(!var){
			fprintf(stderr, "ERROR line %d: assignment to undeclared variable '%s'.\n", yylineno, (yyvsp[-6].sval));
		}
		emit("in", "", "", (yyvsp[-6].sval)); 
	}
#line 3233 "y.tab.c"
    break;

  case 161: /* io_stmt: type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 1405 "parser.y"
        { emit("in", "", "", (yyvsp[-6].sval)); }
#line 3239 "y.tab.c"
    break;

  case 162: /* io_stmt: SHOW LPAREN expression RPAREN SEMICOLON  */
#line 1407 "parser.y"
        { emit("out", (yyvsp[-2].sval), "", ""); }
#line 3245 "y.tab.c"
    break;

  case 163: /* io_stmt: SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON  */
#line 1409 "parser.y"
        {
            char* t = genVar();
            emit("[]",  (yyvsp[-5].sval), (yyvsp[-3].sval), t);
            emit("out", t,  "", "");
        }
#line 3255 "y.tab.c"
    break;


#line 3259 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1416 "parser.y"


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

int main() {
    global_scope  = create_scope(SCOPE_GLOBAL, "global", NULL);
    current_scope = global_scope;

    yyin = stdin;
    yyparse();

    printf("\n========== GLOBAL SCOPE ==========\n");
    print_table(global_scope);

    printf("\nParsing Successful\nGenerated quadruple table:\n");
    printf("%-15s %-15s %-15s %-15s\n", "OP", "ARG1", "ARG2", "RESULT");
    for (int i = 0; i < IR_idx; i++)
        printf("%-15s %-15s %-15s %-15s\n",
               IR[i].op, IR[i].arg1, IR[i].arg2, IR[i].result);
    return 0;
}

