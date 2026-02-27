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

#line 146 "y.tab.c"

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
#line 76 "parser.y"

    int       ival;
    float     fval;
    char      cval;
    char*     sval;
    DataType  dtype;
    AccessMod access;

#line 318 "y.tab.c"

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
  YYSYMBOL_array_init = 84,                /* array_init  */
  YYSYMBOL_expr_list = 85,                 /* expr_list  */
  YYSYMBOL_function_decl = 86,             /* function_decl  */
  YYSYMBOL_87_9 = 87,                      /* $@9  */
  YYSYMBOL_88_10 = 88,                     /* $@10  */
  YYSYMBOL_89_11 = 89,                     /* $@11  */
  YYSYMBOL_func_type = 90,                 /* func_type  */
  YYSYMBOL_param_list_opt = 91,            /* param_list_opt  */
  YYSYMBOL_param_list = 92,                /* param_list  */
  YYSYMBOL_param = 93,                     /* param  */
  YYSYMBOL_return_stmt = 94,               /* return_stmt  */
  YYSYMBOL_expr_stmt = 95,                 /* expr_stmt  */
  YYSYMBOL_expression = 96,                /* expression  */
  YYSYMBOL_indexed_id = 97,                /* indexed_id  */
  YYSYMBOL_assignment = 98,                /* assignment  */
  YYSYMBOL_logic_expr = 99,                /* logic_expr  */
  YYSYMBOL_bitwise_expr = 100,             /* bitwise_expr  */
  YYSYMBOL_rel_expr = 101,                 /* rel_expr  */
  YYSYMBOL_arith_expr = 102,               /* arith_expr  */
  YYSYMBOL_term = 103,                     /* term  */
  YYSYMBOL_factor = 104,                   /* factor  */
  YYSYMBOL_if_stmt = 105,                  /* if_stmt  */
  YYSYMBOL_106_12 = 106,                   /* $@12  */
  YYSYMBOL_107_13 = 107,                   /* $@13  */
  YYSYMBOL_108_14 = 108,                   /* $@14  */
  YYSYMBOL_109_15 = 109,                   /* $@15  */
  YYSYMBOL_elif_list = 110,                /* elif_list  */
  YYSYMBOL_111_16 = 111,                   /* $@16  */
  YYSYMBOL_112_17 = 112,                   /* $@17  */
  YYSYMBOL_113_18 = 113,                   /* $@18  */
  YYSYMBOL_114_19 = 114,                   /* $@19  */
  YYSYMBOL_else_opt = 115,                 /* else_opt  */
  YYSYMBOL_116_20 = 116,                   /* $@20  */
  YYSYMBOL_for_stmt = 117,                 /* for_stmt  */
  YYSYMBOL_118_21 = 118,                   /* $@21  */
  YYSYMBOL_for_header = 119,               /* for_header  */
  YYSYMBOL_120_22 = 120,                   /* $@22  */
  YYSYMBOL_121_23 = 121,                   /* $@23  */
  YYSYMBOL_122_24 = 122,                   /* $@24  */
  YYSYMBOL_for_init = 123,                 /* for_init  */
  YYSYMBOL_var_decl_no_semi = 124,         /* var_decl_no_semi  */
  YYSYMBOL_io_stmt = 125                   /* io_stmt  */
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
#define YYLAST   478

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  70
/* YYNRULES -- Number of rules.  */
#define YYNRULES  156
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  325

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
       0,   113,   113,   114,   118,   119,   120,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   145,   144,   179,
     178,   189,   190,   194,   195,   196,   204,   203,   229,   228,
     244,   243,   273,   272,   301,   300,   315,   329,   345,   346,
     353,   371,   382,   383,   387,   388,   396,   395,   405,   406,
     414,   415,   433,   440,   449,   456,   471,   478,   489,   490,
     491,   492,   493,   506,   523,   540,   553,   557,   558,   567,
     566,   593,   592,   616,   615,   626,   627,   631,   632,   636,
     637,   642,   663,   689,   690,   691,   699,   700,   704,   711,
     717,   728,   730,   732,   734,   736,   739,   742,   746,   748,
     750,   752,   756,   758,   760,   764,   766,   768,   770,   774,
     776,   778,   782,   784,   786,   788,   792,   794,   795,   796,
     797,   798,   799,   800,   801,   802,   810,   827,   829,   834,
     809,   838,   847,   864,   866,   871,   846,   873,   878,   877,
     889,   897,   896,   920,   929,   936,   943,   928,   948,   949,
     950,   954,   958,   971,   973,   975,   977
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
  "var_decl", "id_list", "type", "array_decl", "array_init", "expr_list",
  "function_decl", "$@9", "$@10", "$@11", "func_type", "param_list_opt",
  "param_list", "param", "return_stmt", "expr_stmt", "expression",
  "indexed_id", "assignment", "logic_expr", "bitwise_expr", "rel_expr",
  "arith_expr", "term", "factor", "if_stmt", "$@12", "$@13", "$@14",
  "$@15", "elif_list", "$@16", "$@17", "$@18", "$@19", "else_opt", "$@20",
  "for_stmt", "$@21", "for_header", "$@22", "$@23", "$@24", "for_init",
  "var_decl_no_semi", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-198)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-142)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -198,   193,  -198,   -28,  -198,  -198,  -198,  -198,  -198,  -198,
       4,    29,  -198,  -198,    35,     5,    -8,    22,    96,   237,
    -198,   419,  -198,  -198,  -198,  -198,   127,  -198,  -198,  -198,
    -198,  -198,  -198,    15,  -198,  -198,    45,  -198,  -198,    47,
      43,  -198,    57,    84,  -198,   123,   103,  -198,  -198,  -198,
    -198,  -198,   329,    89,    96,    58,  -198,   119,    65,  -198,
      80,    92,   109,  -198,   322,   107,   114,    96,    96,   139,
     361,    96,    96,   134,    -2,  -198,  -198,   146,   148,   155,
     158,   184,  -198,    96,   127,   127,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   160,    96,   197,   366,
      30,  -198,    96,  -198,   129,   192,   187,  -198,  -198,   416,
    -198,   282,    15,  -198,   195,   209,   211,  -198,   228,   393,
     225,  -198,  -198,  -198,  -198,   230,    -3,   232,   398,  -198,
    -198,  -198,   224,  -198,   223,  -198,   153,   153,   153,   103,
     103,  -198,  -198,  -198,   242,  -198,   242,   243,  -198,   242,
     244,  -198,   246,    96,  -198,   279,    96,  -198,  -198,   254,
      96,  -198,    96,   251,   249,   260,   261,  -198,   263,   272,
     265,   276,   436,   271,   278,   281,  -198,   285,  -198,   266,
    -198,  -198,  -198,   284,   113,   280,  -198,    10,  -198,  -198,
      96,   287,  -198,   294,   292,    96,   299,   301,   304,   306,
    -198,    10,   343,  -198,    96,    96,   317,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,  -198,    82,  -198,   311,   313,   324,
     316,  -198,   321,    96,   330,   345,    17,  -198,  -198,   331,
     353,    19,   354,   355,   242,  -198,   352,   356,   360,   363,
      -9,     1,  -198,  -198,   242,    10,  -198,   364,  -198,  -198,
    -198,    96,   357,   362,    96,   242,   242,  -198,  -198,  -198,
      10,   407,   374,   377,   379,   384,  -198,  -198,   386,  -198,
     390,  -198,    21,  -198,  -198,   414,    96,   399,   410,  -198,
    -198,  -198,  -198,  -198,   408,   411,   413,   415,  -198,   242,
     242,   422,   425,   428,  -198,  -198,  -198,  -198,  -198,   437,
    -198,  -198,    10,    10,   463,    96,   242,  -198,   438,   440,
     441,  -198,  -198,   242,   242,   242,   442,  -198,  -198,  -198,
    -198,   242,  -198,   414,  -198
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,    58,    59,    60,    61,    62,    76,
       0,     0,   123,   124,     0,     0,     0,     0,     0,     0,
      87,   117,   122,   119,   120,   121,     0,     2,     4,     6,
      14,    15,     7,    75,     8,     5,     0,    13,     9,     0,
     118,    88,    97,   101,   104,   108,   111,   115,    10,    11,
      12,    16,     0,     0,     0,     0,    84,   117,     0,    17,
       0,     0,     0,    48,     0,     0,     0,    43,     0,    57,
       0,     0,     0,     0,   117,   118,   100,     0,     0,    57,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   150,
       0,    85,     0,    83,     0,     0,     0,   125,    49,   117,
      51,     0,     0,    71,     0,     0,    42,    45,     0,     0,
       0,    91,    92,    93,    54,     0,     0,     0,     0,    52,
      69,    94,    98,    99,   103,   102,   107,   105,   106,   109,
     110,   112,   113,   114,     0,   127,     0,     0,   149,     0,
       0,   148,     0,     0,    22,     0,     0,    47,    50,     0,
       0,   116,     0,    89,     0,     0,     0,    56,     0,     0,
       0,     0,   117,     0,     0,     0,   131,     0,   143,   151,
     142,   144,   155,     0,     0,     0,    95,    78,    96,    44,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
      53,    78,     0,   128,     0,     0,     0,    38,    39,    18,
      26,    21,    23,    24,    25,     0,    20,     0,     0,     0,
      77,    80,     0,    43,     0,     0,     0,    68,    65,     0,
       0,     0,     0,     0,     0,   152,     0,     0,     0,     0,
       0,     0,    82,    81,     0,     0,    90,     0,   153,    63,
      66,     0,     0,     0,     0,     0,     0,   129,   145,   156,
      78,     0,     0,     0,     0,     0,    72,    79,     0,    67,
       0,   154,     0,    70,    74,   137,     0,     0,     0,    32,
      37,    30,    36,    40,     0,     0,     0,   140,   146,     0,
       0,     0,     0,     0,    64,    41,   132,   138,   130,     0,
      27,    29,    78,    78,     0,     0,     0,   147,     0,     0,
       0,   133,   139,     0,     0,     0,     0,    33,    31,    35,
     134,     0,   135,   137,   136
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -198,  -198,  -198,   -54,  -198,  -198,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,   248,
     218,  -137,  -198,  -198,  -198,   -31,     0,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,  -198,  -197,  -198,   229,  -198,  -198,
     -15,   -18,   -48,    -6,   130,  -198,    97,   132,   118,  -198,
    -198,  -198,  -198,  -198,   150,  -198,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    27,    28,    29,   104,   105,   184,   211,   212,
     238,   239,   213,   292,   291,   293,   214,   215,    30,   115,
     116,    31,    64,   111,    32,    73,   218,    34,   196,   226,
      35,   174,   159,   175,    36,   219,   220,   221,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      97,   177,   234,   275,   287,   305,   316,   321,   323,   298,
     306,    49,    99,   149,   205,   276,   299,   150,   151,    50
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,    33,    80,    61,   232,    51,    55,   176,    75,   178,
     110,   262,   180,     4,     5,     6,     7,     8,    12,    13,
      76,   264,   121,   122,   123,    67,   263,    59,   168,    68,
      17,    52,    18,    77,    78,   131,   265,   169,    56,   100,
      57,    22,    23,    24,    25,   217,   254,   250,    60,   285,
      79,   251,   117,   118,   121,   162,    53,   158,   152,   160,
      26,   153,    54,   277,   112,    81,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,   132,   133,
      82,    80,   145,    83,   148,     4,     5,     6,     7,     8,
      98,   101,  -141,  -141,  -141,  -141,  -141,   257,   103,   147,
      84,    85,  -141,  -141,   165,   308,   309,   266,   186,    12,
      13,   112,   188,   173,  -141,   106,  -141,   240,   273,   274,
     107,    17,  -141,    18,  -141,  -141,  -141,  -141,  -141,    86,
      87,    57,    22,    23,    24,    25,   207,   208,   183,   108,
      12,    13,   113,   209,  -141,    66,    67,   189,   210,   114,
      68,    26,   300,   301,    18,    93,    94,    95,   154,   102,
      71,    72,    74,    22,    23,    24,    25,   124,   125,   312,
      88,    89,    90,    91,    92,   222,   317,   318,   319,   119,
     227,   126,    26,   127,   322,   136,   137,   138,   144,   235,
     236,   129,   125,     2,     3,   128,     4,     5,     6,     7,
       8,     9,    10,    91,    92,    11,    12,    13,   117,    14,
      15,   141,   142,   143,    16,   241,   134,   135,    17,   130,
      18,   155,    19,   139,   140,   146,    20,   156,    21,    22,
      23,    24,    25,    12,    13,   160,   269,   161,    62,   117,
     -46,   -46,   -46,   -46,   -46,   162,   -46,    18,    26,   -46,
     -46,   -46,   166,   -46,   -46,    74,    22,    23,    24,    25,
     163,   288,   -46,   170,   -46,   167,   -46,    63,    85,    87,
     -46,    19,   -46,   -46,   -46,   -46,   -46,   181,   179,   182,
     185,   187,   190,     3,   191,     4,     5,     6,     7,     8,
     311,    10,   -46,   192,    11,    12,    13,   193,    14,    15,
     194,   195,   197,   198,   200,   201,   204,    17,   202,    18,
     216,    19,   157,   203,   223,    20,   206,   109,    22,    23,
      24,    25,   224,     3,   225,     4,     5,     6,     7,     8,
      96,    10,   228,   229,    11,    12,    13,    26,    14,    15,
     230,   231,  -126,  -126,   233,   237,   242,    17,   243,    18,
     245,    19,   244,   246,  -126,    20,  -126,   109,    22,    23,
      24,    25,   252,   248,  -126,  -126,  -126,  -126,  -126,     4,
       5,     6,     7,     8,    12,    13,   120,    26,   249,    12,
      13,   253,   255,   256,  -126,   258,    17,   260,    18,   259,
     261,    17,   268,    18,   270,   271,    57,    22,    23,    24,
      25,    57,    22,    23,    24,    25,    12,    13,   278,   279,
     280,    12,    13,   171,   281,   164,    26,   282,    17,   283,
      18,    26,   284,    17,   286,    18,   297,   289,    57,    22,
      23,    24,    25,   172,    22,    23,    24,    25,   290,    65,
     296,   294,    66,    67,   295,    66,    67,    68,    26,   302,
      68,    69,   303,    26,    69,   304,    70,    71,    72,    70,
      71,    72,   199,    67,   310,   307,   313,    68,   314,   315,
     320,   247,   272,   324,   267,     0,   102,    71,    72
};

static const yytype_int16 yycheck[] =
{
      15,     1,    33,    18,   201,    33,     1,   144,    26,   146,
      64,    20,   149,     3,     4,     5,     6,     7,    13,    14,
      26,    20,    70,    71,    72,    27,    35,    35,    31,    31,
      25,    27,    27,    18,    19,    83,    35,    40,    33,    54,
      35,    36,    37,    38,    39,    35,    27,    30,    26,    28,
      35,    34,    67,    68,   102,    34,    27,   111,    28,    40,
      55,    31,    27,   260,    64,    20,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    84,    85,
      33,   112,    97,    40,    99,     3,     4,     5,     6,     7,
       1,    33,     3,     4,     5,     6,     7,   234,    33,    99,
      43,    44,    13,    14,   119,   302,   303,   244,   156,    13,
      14,   111,   160,   128,    25,    35,    27,    35,   255,   256,
      28,    25,    33,    27,    35,    36,    37,    38,    39,    45,
      46,    35,    36,    37,    38,    39,    23,    24,   153,    30,
      13,    14,    35,    30,    55,    26,    27,   162,    35,    35,
      31,    55,   289,   290,    27,    52,    53,    54,    29,    40,
      41,    42,    35,    36,    37,    38,    39,    33,    34,   306,
      47,    48,    49,    50,    51,   190,   313,   314,   315,    40,
     195,    35,    55,    35,   321,    88,    89,    90,    28,   204,
     205,    33,    34,     0,     1,    40,     3,     4,     5,     6,
       7,     8,     9,    50,    51,    12,    13,    14,   223,    16,
      17,    93,    94,    95,    21,   215,    86,    87,    25,    35,
      27,    29,    29,    91,    92,    28,    33,    40,    35,    36,
      37,    38,    39,    13,    14,    40,   251,    28,     1,   254,
       3,     4,     5,     6,     7,    34,     9,    27,    55,    12,
      13,    14,    27,    16,    17,    35,    36,    37,    38,    39,
      32,   276,    25,    31,    27,    35,    29,    30,    44,    46,
      33,    29,    35,    36,    37,    38,    39,    33,    35,    33,
       1,    27,    31,     1,    35,     3,     4,     5,     6,     7,
     305,     9,    55,    33,    12,    13,    14,    36,    16,    17,
      37,    29,    37,    27,    33,    27,    40,    25,    27,    27,
      30,    29,    30,    28,    27,    33,    32,    35,    36,    37,
      38,    39,    28,     1,    32,     3,     4,     5,     6,     7,
       1,     9,    33,    32,    12,    13,    14,    55,    16,    17,
      36,    35,    13,    14,     1,    28,    35,    25,    35,    27,
      34,    29,    28,    32,    25,    33,    27,    35,    36,    37,
      38,    39,    31,    33,    35,    36,    37,    38,    39,     3,
       4,     5,     6,     7,    13,    14,    15,    55,    33,    13,
      14,    28,    28,    28,    55,    33,    25,    27,    27,    33,
      27,    25,    28,    27,    37,    33,    35,    36,    37,    38,
      39,    35,    36,    37,    38,    39,    13,    14,     1,    35,
      33,    13,    14,    15,    35,    22,    55,    33,    25,    33,
      27,    55,    32,    25,    10,    27,    11,    28,    35,    36,
      37,    38,    39,    35,    36,    37,    38,    39,    28,    20,
      27,    33,    26,    27,    33,    26,    27,    31,    55,    27,
      31,    35,    27,    55,    35,    27,    40,    41,    42,    40,
      41,    42,    26,    27,     1,    28,    28,    31,    28,    28,
      28,   223,   254,   323,   245,    -1,    40,    41,    42
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    57,     0,     1,     3,     4,     5,     6,     7,     8,
       9,    12,    13,    14,    16,    17,    21,    25,    27,    29,
      33,    35,    36,    37,    38,    39,    55,    58,    59,    60,
      74,    77,    80,    82,    83,    86,    90,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   117,
     125,    33,    27,    27,    27,     1,    33,    35,    96,    35,
      26,    96,     1,    30,    78,    20,    26,    27,    31,    35,
      40,    41,    42,    81,    35,    97,    99,    18,    19,    35,
      81,    20,    33,    40,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,     1,   106,     1,   118,
      96,    33,    40,    33,    61,    62,    35,    28,    30,    35,
      59,    79,    82,    35,    35,    75,    76,    96,    96,    40,
      15,    98,    98,    98,    33,    34,    35,    35,    40,    33,
      35,    98,    99,    99,   100,   100,   102,   102,   102,   103,
     103,   104,   104,   104,    28,    96,    28,    82,    96,   119,
     123,   124,    28,    31,    29,    29,    40,    30,    59,    88,
      40,    28,    34,    32,    22,    96,    27,    35,    31,    40,
      31,    15,    35,    96,    87,    89,    77,   107,    77,    35,
      77,    33,    33,    96,    63,     1,    98,    27,    98,    96,
      31,    35,    33,    36,    37,    29,    84,    37,    27,    26,
      33,    27,    27,    28,    40,   120,    32,    23,    24,    30,
      35,    64,    65,    68,    72,    73,    30,    35,    82,    91,
      92,    93,    96,    27,    28,    32,    85,    96,    33,    32,
      36,    35,    91,     1,   108,    96,    96,    28,    66,    67,
      35,    82,    35,    35,    28,    34,    32,    75,    33,    33,
      30,    34,    31,    28,    27,    28,    28,    77,    33,    33,
      27,    27,    20,    35,    20,    35,    77,    93,    28,    96,
      37,    33,    76,    77,    77,   109,   121,    91,     1,    35,
      33,    35,    33,    33,    32,    28,    10,   110,    96,    28,
      28,    70,    69,    71,    33,    33,    27,    11,   115,   122,
      77,    77,    27,    27,    27,   111,   116,    28,    91,    91,
       1,    96,    77,    28,    28,    28,   112,    77,    77,    77,
      28,   113,    77,   114,   110
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    57,    58,    58,    58,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    61,    60,    62,
      60,    63,    63,    64,    64,    64,    66,    65,    67,    65,
      69,    68,    70,    68,    71,    68,    72,    72,    73,    73,
      74,    74,    75,    75,    76,    76,    78,    77,    77,    77,
      79,    79,    80,    80,    80,    80,    81,    81,    82,    82,
      82,    82,    82,    83,    83,    83,    84,    85,    85,    87,
      86,    88,    86,    89,    86,    90,    90,    91,    91,    92,
      92,    93,    93,    94,    94,    94,    95,    95,    96,    97,
      97,    98,    98,    98,    98,    98,    98,    98,    99,    99,
      99,    99,   100,   100,   100,   101,   101,   101,   101,   102,
     102,   102,   103,   103,   103,   103,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   106,   107,   108,   109,
     105,   105,   111,   112,   113,   114,   110,   110,   116,   115,
     115,   118,   117,   117,   120,   121,   122,   119,   123,   123,
     123,   124,   124,   125,   125,   125,   125
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
       1,     1,     1,     7,    10,     6,     3,     3,     1,     0,
       8,     0,     8,     0,     8,     1,     1,     1,     0,     3,
       1,     2,     2,     3,     2,     3,     2,     1,     1,     4,
       7,     3,     3,     3,     3,     5,     5,     1,     3,     3,
       2,     1,     3,     3,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     0,     0,     0,
      11,     5,     0,     0,     0,     0,    10,     0,     0,     3,
       0,     0,     5,     5,     0,     0,     0,     9,     1,     1,
       0,     2,     4,     7,     8,     5,     8
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
#line 134 "parser.y"
        {
            printf("Invalid statement at line %d\n", yylineno);
            yyerrok;
        }
#line 1682 "y.tab.c"
    break;

  case 17: /* $@1: %empty  */
#line 145 "parser.y"
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
            current_scope = es;
        }
#line 1702 "y.tab.c"
    break;

  case 18: /* entity_decl: ENTITY IDENTIFIER $@1 LBRACE entity_body RBRACE  */
#line 161 "parser.y"
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
                sym->size = class_sz;   /* ← THIS is what prints in SIZE column */
            }
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_entity", (yyvsp[-4].sval), "", "");
        }
#line 1724 "y.tab.c"
    break;

  case 19: /* $@2: %empty  */
#line 179 "parser.y"
        { emit("entity", (yyvsp[0].sval), "", ""); }
#line 1730 "y.tab.c"
    break;

  case 20: /* entity_decl: ENTITY IDENTIFIER $@2 LBRACE error RBRACE  */
#line 181 "parser.y"
        {
            printf("Invalid entity body at line %d\n", yylineno);
            yyerrok;
            emit("end_entity", (yyvsp[-4].sval), "", "");
        }
#line 1740 "y.tab.c"
    break;

  case 26: /* $@3: %empty  */
#line 204 "parser.y"
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
#line 1763 "y.tab.c"
    break;

  case 27: /* constructor_decl: IDENTIFIER $@3 LPAREN param_list_opt RPAREN block  */
#line 223 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_constr", (yyvsp[-5].sval), "", "");
        }
#line 1773 "y.tab.c"
    break;

  case 28: /* $@4: %empty  */
#line 229 "parser.y"
        { emit("constr", (yyvsp[0].sval), "", ""); }
#line 1779 "y.tab.c"
    break;

  case 29: /* constructor_decl: IDENTIFIER $@4 LPAREN error RPAREN block  */
#line 231 "parser.y"
        {
            printf("Invalid constructor parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_constr", (yyvsp[-5].sval), "", "");
        }
#line 1789 "y.tab.c"
    break;

  case 30: /* $@5: %empty  */
#line 244 "parser.y"
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
#line 1815 "y.tab.c"
    break;

  case 31: /* method_decl: access_modifier type FUNC IDENTIFIER $@5 LPAREN param_list_opt RPAREN block  */
#line 266 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_method", (yyvsp[-5].sval), "", "");
        }
#line 1825 "y.tab.c"
    break;

  case 32: /* $@6: %empty  */
#line 273 "parser.y"
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
#line 1851 "y.tab.c"
    break;

  case 33: /* method_decl: access_modifier IDENTIFIER FUNC IDENTIFIER $@6 LPAREN param_list_opt RPAREN block  */
#line 295 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_method", (yyvsp[-5].sval), "", "");
        }
#line 1861 "y.tab.c"
    break;

  case 34: /* $@7: %empty  */
#line 301 "parser.y"
        { emit("method", (yyvsp[0].sval), "", ""); }
#line 1867 "y.tab.c"
    break;

  case 35: /* method_decl: access_modifier type FUNC IDENTIFIER $@7 LPAREN error RPAREN block  */
#line 303 "parser.y"
        {
            printf("Invalid method parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_method", (yyvsp[-5].sval), "", "");
        }
#line 1877 "y.tab.c"
    break;

  case 36: /* access_var_decl: access_modifier type IDENTIFIER SEMICOLON  */
#line 316 "parser.y"
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
#line 1894 "y.tab.c"
    break;

  case 37: /* access_var_decl: access_modifier IDENTIFIER IDENTIFIER SEMICOLON  */
#line 330 "parser.y"
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
#line 1911 "y.tab.c"
    break;

  case 38: /* access_modifier: PUBLIC  */
#line 345 "parser.y"
              { (yyval.access) = ACC_PUBLIC;  }
#line 1917 "y.tab.c"
    break;

  case 39: /* access_modifier: PRIVATE  */
#line 346 "parser.y"
              { (yyval.access) = ACC_PRIVATE; }
#line 1923 "y.tab.c"
    break;

  case 40: /* object_decl: IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 354 "parser.y"
        {
	    Symbol* class_sym = lookup(current_scope,(yyvsp[-4].sval));
	    if(!class_sym || class_sym != KIND_ENTITY){
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
#line 1945 "y.tab.c"
    break;

  case 41: /* object_decl: type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list RPAREN SEMICOLON  */
#line 372 "parser.y"
        {
	    check_method_access((yyvsp[-6].sval),(yyvsp[-4].sval));
            char* t = genVar();
            emit("push_ptr", (yyvsp[-6].sval), "", "");
            emit("call_method", (yyvsp[-4].sval), "", t);
            emit("=", t, "", (yyvsp[-8].sval));
        }
#line 1957 "y.tab.c"
    break;

  case 44: /* arg_list: arg_list COMMA expression  */
#line 387 "parser.y"
                                { emit("arg", (yyvsp[0].sval), "", ""); }
#line 1963 "y.tab.c"
    break;

  case 45: /* arg_list: expression  */
#line 388 "parser.y"
                                { emit("arg", (yyvsp[0].sval), "", ""); }
#line 1969 "y.tab.c"
    break;

  case 46: /* $@8: %empty  */
#line 396 "parser.y"
        {
            SymTable* bs = create_scope(SCOPE_BLOCK, "block", current_scope);
            current_scope = bs;
        }
#line 1978 "y.tab.c"
    break;

  case 47: /* block: LBRACE $@8 stmt_list RBRACE  */
#line 401 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 1987 "y.tab.c"
    break;

  case 49: /* block: LBRACE error RBRACE  */
#line 407 "parser.y"
        {
            printf("Error inside block at line %d\n", yylineno);
            yyerrok;
        }
#line 1996 "y.tab.c"
    break;

  case 52: /* var_decl: type id_list SEMICOLON  */
#line 434 "parser.y"
        {
            insert_var_list((yyvsp[-1].sval), (yyvsp[-2].dtype));
            free((yyvsp[-1].sval));
        }
#line 2005 "y.tab.c"
    break;

  case 53: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 441 "parser.y"
        {
            emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval));
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
                                        KIND_VAR, (yyvsp[-4].dtype), yylineno);
            if (sym) sym->is_initialized = 1;
        }
#line 2016 "y.tab.c"
    break;

  case 54: /* var_decl: IDENTIFIER id_list SEMICOLON  */
#line 450 "parser.y"
        {
            insert_var_list((yyvsp[-1].sval), DT_ENTITY);
            free((yyvsp[-1].sval));
        }
#line 2025 "y.tab.c"
    break;

  case 55: /* var_decl: IDENTIFIER IDENTIFIER ASSIGN expression SEMICOLON  */
#line 457 "parser.y"
        {
            emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval));
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
                                        KIND_VAR, DT_ENTITY, yylineno);
            if (sym) sym->is_initialized = 1;
        }
#line 2036 "y.tab.c"
    break;

  case 56: /* id_list: id_list COMMA IDENTIFIER  */
#line 472 "parser.y"
        {
            char* buf = malloc(strlen((yyvsp[-2].sval)) + strlen((yyvsp[0].sval)) + 2);
            sprintf(buf, "%s,%s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval));
            (yyval.sval) = buf;
        }
#line 2047 "y.tab.c"
    break;

  case 57: /* id_list: IDENTIFIER  */
#line 479 "parser.y"
        {
            (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 2055 "y.tab.c"
    break;

  case 58: /* type: INT  */
#line 489 "parser.y"
             { (yyval.dtype) = DT_INT;    }
#line 2061 "y.tab.c"
    break;

  case 59: /* type: FP  */
#line 490 "parser.y"
             { (yyval.dtype) = DT_FLOAT;  }
#line 2067 "y.tab.c"
    break;

  case 60: /* type: CHR  */
#line 491 "parser.y"
             { (yyval.dtype) = DT_CHAR;   }
#line 2073 "y.tab.c"
    break;

  case 61: /* type: STRING  */
#line 492 "parser.y"
             { (yyval.dtype) = DT_STRING; }
#line 2079 "y.tab.c"
    break;

  case 62: /* type: BOOL  */
#line 493 "parser.y"
             { (yyval.dtype) = DT_BOOL;   }
#line 2085 "y.tab.c"
    break;

  case 63: /* array_decl: type SEQ1 IDENTIFIER LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 507 "parser.y"
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
#line 2104 "y.tab.c"
    break;

  case 64: /* array_decl: type SEQ2 IDENTIFIER LBRACKET INT_LITERAL RBRACKET LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 525 "parser.y"
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
#line 2121 "y.tab.c"
    break;

  case 65: /* array_decl: type SEQ1 IDENTIFIER ASSIGN array_init SEMICOLON  */
#line 541 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
                                        KIND_ARRAY, (yyvsp[-5].dtype), yylineno);
            if (sym) {
                sym->attr.array.dimensions     = 1;
                sym->attr.array.is_initialized = 1;
                /* size and offset will be determined at runtime */
            }
        }
#line 2135 "y.tab.c"
    break;

  case 69: /* $@9: %empty  */
#line 567 "parser.y"
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
            emit("func", (yyvsp[0].sval), "", "");
            SymTable* fs = create_scope(SCOPE_FUNCTION, (yyvsp[0].sval), current_scope);
            current_scope = fs;
        }
#line 2154 "y.tab.c"
    break;

  case 70: /* function_decl: func_type FUNC IDENTIFIER $@9 LPAREN param_list_opt RPAREN block  */
#line 582 "parser.y"
        {
            /* Do NOT write next_offset into sym->size.
               Function size in global scope = 0 always.
               The frame size is visible in the function's own scope table. */
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("endfunc", "", "", "");
        }
#line 2167 "y.tab.c"
    break;

  case 71: /* $@10: %empty  */
#line 593 "parser.y"
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
            emit("func", (yyvsp[0].sval), "", "");
            SymTable* fs = create_scope(SCOPE_FUNCTION, (yyvsp[0].sval), current_scope);
            current_scope = fs;
        }
#line 2186 "y.tab.c"
    break;

  case 72: /* function_decl: IDENTIFIER FUNC IDENTIFIER $@10 LPAREN param_list_opt RPAREN block  */
#line 608 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("endfunc", "", "", "");
        }
#line 2196 "y.tab.c"
    break;

  case 73: /* $@11: %empty  */
#line 616 "parser.y"
        { emit("func", (yyvsp[0].sval), "", ""); }
#line 2202 "y.tab.c"
    break;

  case 74: /* function_decl: func_type FUNC IDENTIFIER $@11 LPAREN error RPAREN block  */
#line 618 "parser.y"
        {
            printf("Invalid parameter list at line %d\n", yylineno);
            yyerrok;
            emit("endfunc", "", "", "");
        }
#line 2212 "y.tab.c"
    break;

  case 75: /* func_type: type  */
#line 626 "parser.y"
           { (yyval.dtype) = (yyvsp[0].dtype);      }
#line 2218 "y.tab.c"
    break;

  case 76: /* func_type: VOID  */
#line 627 "parser.y"
           { (yyval.dtype) = DT_VOID; }
#line 2224 "y.tab.c"
    break;

  case 81: /* param: type IDENTIFIER  */
#line 643 "parser.y"
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
#line 2248 "y.tab.c"
    break;

  case 82: /* param: IDENTIFIER IDENTIFIER  */
#line 664 "parser.y"
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
#line 2272 "y.tab.c"
    break;

  case 83: /* return_stmt: RETURN expression SEMICOLON  */
#line 689 "parser.y"
                                  { emit("return", (yyvsp[-1].sval), "", ""); }
#line 2278 "y.tab.c"
    break;

  case 84: /* return_stmt: RETURN SEMICOLON  */
#line 690 "parser.y"
                                  { emit("return", "", "", ""); }
#line 2284 "y.tab.c"
    break;

  case 85: /* return_stmt: RETURN error SEMICOLON  */
#line 692 "parser.y"
        {
            printf("Invalid return statement at line %d\n", yylineno);
            yyerrok;
        }
#line 2293 "y.tab.c"
    break;

  case 88: /* expression: assignment  */
#line 704 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2299 "y.tab.c"
    break;

  case 89: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET  */
#line 712 "parser.y"
        {
            char* t1 = genVar(); emit("*",  (yyvsp[-1].sval), "type.width", t1);
            char* t2 = genVar(); emit("[]", (yyvsp[-3].sval), t1,           t2);
            (yyval.sval) = t2;
        }
#line 2309 "y.tab.c"
    break;

  case 90: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET  */
#line 718 "parser.y"
        {
            char* t1 = genVar(); emit("*",  (yyvsp[-4].sval), "array.cols", t1);
            char* t2 = genVar(); emit("+",  t1, (yyvsp[-1].sval),           t2);
            char* t3 = genVar(); emit("*",  t2, "type.width", t3);
            char* t4 = genVar(); emit("[]", (yyvsp[-6].sval), t3,           t4);
            (yyval.sval) = t4;
        }
#line 2321 "y.tab.c"
    break;

  case 91: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 729 "parser.y"
        { emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval)); (yyval.sval) = strdup((yyvsp[-2].sval)); }
#line 2327 "y.tab.c"
    break;

  case 92: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 731 "parser.y"
        { char* t = genVar(); emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t); emit("=", t, "", (yyvsp[-2].sval)); (yyval.sval) = t; }
#line 2333 "y.tab.c"
    break;

  case 93: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 733 "parser.y"
        { char* t = genVar(); emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t); emit("=", t, "", (yyvsp[-2].sval)); (yyval.sval) = t; }
#line 2339 "y.tab.c"
    break;

  case 94: /* assignment: indexed_id ASSIGN assignment  */
#line 735 "parser.y"
        { (yyval.sval) = (yyvsp[0].sval); }
#line 2345 "y.tab.c"
    break;

  case 95: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 737 "parser.y"
        { //check_field_access("this", $3); 
	emit("set_field", "this", (yyvsp[-2].sval), (yyvsp[0].sval)); (yyval.sval) = (yyvsp[0].sval); }
#line 2352 "y.tab.c"
    break;

  case 96: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 740 "parser.y"
        { check_field_access((yyvsp[-4].sval),(yyvsp[-2].sval)); 
	emit("set_field", (yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); (yyval.sval) = (yyvsp[0].sval); }
#line 2359 "y.tab.c"
    break;

  case 97: /* assignment: logic_expr  */
#line 742 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2365 "y.tab.c"
    break;

  case 98: /* logic_expr: logic_expr OR logic_expr  */
#line 747 "parser.y"
        { char* t = genVar(); emit("||", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2371 "y.tab.c"
    break;

  case 99: /* logic_expr: logic_expr AND logic_expr  */
#line 749 "parser.y"
        { char* t = genVar(); emit("&&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2377 "y.tab.c"
    break;

  case 100: /* logic_expr: NOT logic_expr  */
#line 751 "parser.y"
        { char* t = genVar(); emit("!", (yyvsp[0].sval), "", t); (yyval.sval) = t; }
#line 2383 "y.tab.c"
    break;

  case 101: /* logic_expr: bitwise_expr  */
#line 752 "parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 2389 "y.tab.c"
    break;

  case 102: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 757 "parser.y"
        { char* t = genVar(); emit("&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2395 "y.tab.c"
    break;

  case 103: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 759 "parser.y"
        { char* t = genVar(); emit("|", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2401 "y.tab.c"
    break;

  case 104: /* bitwise_expr: rel_expr  */
#line 760 "parser.y"
               { (yyval.sval) = (yyvsp[0].sval); }
#line 2407 "y.tab.c"
    break;

  case 105: /* rel_expr: arith_expr GT arith_expr  */
#line 765 "parser.y"
        { char* t = genVar(); emit(">",  (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2413 "y.tab.c"
    break;

  case 106: /* rel_expr: arith_expr LT arith_expr  */
#line 767 "parser.y"
        { char* t = genVar(); emit("<",  (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2419 "y.tab.c"
    break;

  case 107: /* rel_expr: arith_expr EQ arith_expr  */
#line 769 "parser.y"
        { char* t = genVar(); emit("==", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2425 "y.tab.c"
    break;

  case 108: /* rel_expr: arith_expr  */
#line 770 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2431 "y.tab.c"
    break;

  case 109: /* arith_expr: arith_expr PLUS term  */
#line 775 "parser.y"
        { char* t = genVar(); emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2437 "y.tab.c"
    break;

  case 110: /* arith_expr: arith_expr MINUS term  */
#line 777 "parser.y"
        { char* t = genVar(); emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2443 "y.tab.c"
    break;

  case 111: /* arith_expr: term  */
#line 778 "parser.y"
           { (yyval.sval) = (yyvsp[0].sval); }
#line 2449 "y.tab.c"
    break;

  case 112: /* term: term MUL factor  */
#line 783 "parser.y"
        { char* t = genVar(); emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2455 "y.tab.c"
    break;

  case 113: /* term: term DIV factor  */
#line 785 "parser.y"
        { char* t = genVar(); emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2461 "y.tab.c"
    break;

  case 114: /* term: term MOD factor  */
#line 787 "parser.y"
        { char* t = genVar(); emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2467 "y.tab.c"
    break;

  case 115: /* term: factor  */
#line 788 "parser.y"
             { (yyval.sval) = (yyvsp[0].sval); }
#line 2473 "y.tab.c"
    break;

  case 116: /* factor: IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 793 "parser.y"
        { char* t = genVar(); emit("call", (yyvsp[-3].sval), "", t); (yyval.sval) = t; }
#line 2479 "y.tab.c"
    break;

  case 117: /* factor: IDENTIFIER  */
#line 794 "parser.y"
                      { (yyval.sval) = strdup((yyvsp[0].sval)); }
#line 2485 "y.tab.c"
    break;

  case 118: /* factor: indexed_id  */
#line 795 "parser.y"
                      { (yyval.sval) = (yyvsp[0].sval); }
#line 2491 "y.tab.c"
    break;

  case 119: /* factor: INT_LITERAL  */
#line 796 "parser.y"
                      { char b[20]; sprintf(b, "%d",   (yyvsp[0].ival)); (yyval.sval) = strdup(b); }
#line 2497 "y.tab.c"
    break;

  case 120: /* factor: FLOAT_LITERAL  */
#line 797 "parser.y"
                      { char b[20]; sprintf(b, "%f",   (yyvsp[0].fval)); (yyval.sval) = strdup(b); }
#line 2503 "y.tab.c"
    break;

  case 121: /* factor: CHAR_LITERAL  */
#line 798 "parser.y"
                      { char b[20]; sprintf(b, "'%c'", (yyvsp[0].cval)); (yyval.sval) = strdup(b); }
#line 2509 "y.tab.c"
    break;

  case 122: /* factor: STRING_LITERAL  */
#line 799 "parser.y"
                      { (yyval.sval) = strdup((yyvsp[0].sval)); }
#line 2515 "y.tab.c"
    break;

  case 123: /* factor: TRUE  */
#line 800 "parser.y"
                      { (yyval.sval) = strdup("1"); }
#line 2521 "y.tab.c"
    break;

  case 124: /* factor: FALSE  */
#line 801 "parser.y"
                      { (yyval.sval) = strdup("0"); }
#line 2527 "y.tab.c"
    break;

  case 125: /* factor: LPAREN expression RPAREN  */
#line 802 "parser.y"
                               { (yyval.sval) = (yyvsp[-1].sval); }
#line 2533 "y.tab.c"
    break;

  case 126: /* $@12: %empty  */
#line 810 "parser.y"
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
#line 2554 "y.tab.c"
    break;

  case 127: /* $@13: %empty  */
#line 827 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 2560 "y.tab.c"
    break;

  case 128: /* $@14: %empty  */
#line 829 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 2569 "y.tab.c"
    break;

  case 129: /* $@15: %empty  */
#line 834 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 2575 "y.tab.c"
    break;

  case 130: /* if_stmt: IF LPAREN $@12 expression $@13 RPAREN $@14 block $@15 elif_list else_opt  */
#line 836 "parser.y"
        { emit("label", "", "", topEnd()); popIfLabels(); }
#line 2581 "y.tab.c"
    break;

  case 131: /* if_stmt: IF LPAREN error RPAREN block  */
#line 839 "parser.y"
        {
            printf("Invalid IF condition at line %d\n", yylineno);
            yyerrok;
        }
#line 2590 "y.tab.c"
    break;

  case 132: /* $@16: %empty  */
#line 847 "parser.y"
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
#line 2611 "y.tab.c"
    break;

  case 133: /* $@17: %empty  */
#line 864 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 2617 "y.tab.c"
    break;

  case 134: /* $@18: %empty  */
#line 866 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 2626 "y.tab.c"
    break;

  case 135: /* $@19: %empty  */
#line 871 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 2632 "y.tab.c"
    break;

  case 138: /* $@20: %empty  */
#line 878 "parser.y"
        {
            char else_name[32];
            snprintf(else_name, 32, "else_%d", if_cnt++);
            SymTable* es = create_scope(SCOPE_ELSE, else_name, current_scope);
            current_scope = es;
        }
#line 2643 "y.tab.c"
    break;

  case 139: /* else_opt: ELSE $@20 block  */
#line 885 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 2652 "y.tab.c"
    break;

  case 141: /* $@21: %empty  */
#line 897 "parser.y"
        {
            char for_name[32];
            snprintf(for_name, 32, "for_%d", for_cnt++);
            SymTable* fs = create_scope(SCOPE_FOR, for_name, current_scope);
            current_scope = fs;
        }
#line 2663 "y.tab.c"
    break;

  case 142: /* for_stmt: FOR LPAREN $@21 for_header block  */
#line 904 "parser.y"
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
#line 2683 "y.tab.c"
    break;

  case 143: /* for_stmt: FOR LPAREN error RPAREN block  */
#line 921 "parser.y"
        {
            printf("Invalid FOR header at line %d\n", yylineno);
            yyerrok;
        }
#line 2692 "y.tab.c"
    break;

  case 144: /* $@22: %empty  */
#line 929 "parser.y"
        {
            char* b = getLabel();
            char* e = getLabel();
            pushIfLabels(b, e);
            emit("label", "", "", b);
        }
#line 2703 "y.tab.c"
    break;

  case 145: /* $@23: %empty  */
#line 936 "parser.y"
        {
            emit("ifFalse", (yyvsp[-1].sval), "", topEnd());
            forDepth++;
            forIncIdx[forDepth]      = 0;
            inForIncrement[forDepth] = 1;
        }
#line 2714 "y.tab.c"
    break;

  case 146: /* $@24: %empty  */
#line 943 "parser.y"
        { inForIncrement[forDepth] = 0; }
#line 2720 "y.tab.c"
    break;

  case 151: /* var_decl_no_semi: type IDENTIFIER  */
#line 955 "parser.y"
        {
            insert_symbol(current_scope, (yyvsp[0].sval), KIND_VAR, (yyvsp[-1].dtype), yylineno);
        }
#line 2728 "y.tab.c"
    break;

  case 152: /* var_decl_no_semi: type IDENTIFIER ASSIGN expression  */
#line 959 "parser.y"
        {
            emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval));
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-2].sval),
                                        KIND_VAR, (yyvsp[-3].dtype), yylineno);
            if (sym) sym->is_initialized = 1;
        }
#line 2739 "y.tab.c"
    break;

  case 153: /* io_stmt: IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 972 "parser.y"
        { emit("in", "", "", (yyvsp[-6].sval)); }
#line 2745 "y.tab.c"
    break;

  case 154: /* io_stmt: type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 974 "parser.y"
        { emit("in", "", "", (yyvsp[-6].sval)); }
#line 2751 "y.tab.c"
    break;

  case 155: /* io_stmt: SHOW LPAREN expression RPAREN SEMICOLON  */
#line 976 "parser.y"
        { emit("out", (yyvsp[-2].sval), "", ""); }
#line 2757 "y.tab.c"
    break;

  case 156: /* io_stmt: SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON  */
#line 978 "parser.y"
        {
            char* t = genVar();
            emit("[]",  (yyvsp[-5].sval), (yyvsp[-3].sval), t);
            emit("out", t,  "", "");
        }
#line 2767 "y.tab.c"
    break;


#line 2771 "y.tab.c"

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

#line 985 "parser.y"


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

