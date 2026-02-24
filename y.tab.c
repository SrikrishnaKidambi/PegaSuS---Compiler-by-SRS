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

#line 124 "y.tab.c"

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
#line 54 "parser.y"

    int ival;
    float fval;
    char cval;
    char* sval;

#line 294 "y.tab.c"

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
  YYSYMBOL_access_var_decl = 71,           /* access_var_decl  */
  YYSYMBOL_access_modifier = 72,           /* access_modifier  */
  YYSYMBOL_object_decl = 73,               /* object_decl  */
  YYSYMBOL_arg_list_opt = 74,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 75,                  /* arg_list  */
  YYSYMBOL_block = 76,                     /* block  */
  YYSYMBOL_stmt_list = 77,                 /* stmt_list  */
  YYSYMBOL_var_decl = 78,                  /* var_decl  */
  YYSYMBOL_id_list = 79,                   /* id_list  */
  YYSYMBOL_type = 80,                      /* type  */
  YYSYMBOL_array_decl = 81,                /* array_decl  */
  YYSYMBOL_array_init = 82,                /* array_init  */
  YYSYMBOL_expr_list = 83,                 /* expr_list  */
  YYSYMBOL_function_decl = 84,             /* function_decl  */
  YYSYMBOL_85_7 = 85,                      /* $@7  */
  YYSYMBOL_86_8 = 86,                      /* $@8  */
  YYSYMBOL_func_type = 87,                 /* func_type  */
  YYSYMBOL_param_list_opt = 88,            /* param_list_opt  */
  YYSYMBOL_param_list = 89,                /* param_list  */
  YYSYMBOL_param = 90,                     /* param  */
  YYSYMBOL_return_stmt = 91,               /* return_stmt  */
  YYSYMBOL_expr_stmt = 92,                 /* expr_stmt  */
  YYSYMBOL_expression = 93,                /* expression  */
  YYSYMBOL_indexed_id = 94,                /* indexed_id  */
  YYSYMBOL_assignment = 95,                /* assignment  */
  YYSYMBOL_logic_expr = 96,                /* logic_expr  */
  YYSYMBOL_bitwise_expr = 97,              /* bitwise_expr  */
  YYSYMBOL_rel_expr = 98,                  /* rel_expr  */
  YYSYMBOL_arith_expr = 99,                /* arith_expr  */
  YYSYMBOL_term = 100,                     /* term  */
  YYSYMBOL_factor = 101,                   /* factor  */
  YYSYMBOL_if_stmt = 102,                  /* if_stmt  */
  YYSYMBOL_103_9 = 103,                    /* $@9  */
  YYSYMBOL_104_10 = 104,                   /* $@10  */
  YYSYMBOL_105_11 = 105,                   /* $@11  */
  YYSYMBOL_elif_list = 106,                /* elif_list  */
  YYSYMBOL_107_12 = 107,                   /* $@12  */
  YYSYMBOL_108_13 = 108,                   /* $@13  */
  YYSYMBOL_109_14 = 109,                   /* $@14  */
  YYSYMBOL_else_opt = 110,                 /* else_opt  */
  YYSYMBOL_for_stmt = 111,                 /* for_stmt  */
  YYSYMBOL_for_header = 112,               /* for_header  */
  YYSYMBOL_113_15 = 113,                   /* $@15  */
  YYSYMBOL_114_16 = 114,                   /* $@16  */
  YYSYMBOL_115_17 = 115,                   /* $@17  */
  YYSYMBOL_for_init = 116,                 /* for_init  */
  YYSYMBOL_var_decl_no_semi = 117,         /* var_decl_no_semi  */
  YYSYMBOL_io_stmt = 118                   /* io_stmt  */
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
#define YYLAST   418

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  63
/* YYNRULES -- Number of rules.  */
#define YYNRULES  143
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  296

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
       0,    85,    85,    86,    90,    91,    92,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   114,   113,   122,
     121,   132,   133,   137,   138,   139,   144,   143,   152,   151,
     163,   162,   172,   171,   182,   186,   187,   191,   196,   211,
     212,   216,   221,   228,   229,   230,   238,   239,   243,   244,
     249,   250,   254,   254,   254,   254,   254,   258,   259,   260,
     264,   268,   269,   274,   273,   282,   281,   301,   302,   306,
     307,   311,   312,   316,   323,   327,   331,   339,   340,   344,
     348,   358,   378,   380,   382,   384,   385,   386,   387,   391,
     392,   393,   394,   398,   399,   400,   404,   405,   406,   407,
     411,   412,   413,   417,   418,   419,   420,   424,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   443,   445,   447,
     442,   450,   459,   461,   463,   458,   465,   469,   470,   474,
     485,   494,   496,   498,   493,   503,   504,   505,   509,   510,
     514,   518,   522,   526
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
  "method_decl", "$@5", "$@6", "access_var_decl", "access_modifier",
  "object_decl", "arg_list_opt", "arg_list", "block", "stmt_list",
  "var_decl", "id_list", "type", "array_decl", "array_init", "expr_list",
  "function_decl", "$@7", "$@8", "func_type", "param_list_opt",
  "param_list", "param", "return_stmt", "expr_stmt", "expression",
  "indexed_id", "assignment", "logic_expr", "bitwise_expr", "rel_expr",
  "arith_expr", "term", "factor", "if_stmt", "$@9", "$@10", "$@11",
  "elif_list", "$@12", "$@13", "$@14", "else_opt", "for_stmt",
  "for_header", "$@15", "$@16", "$@17", "for_init", "var_decl_no_semi",
  "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-233)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-138)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -233,   110,  -233,   -31,  -233,  -233,  -233,  -233,  -233,  -233,
     -11,    15,  -233,  -233,    31,    10,   -14,    38,   336,   220,
    -233,    36,  -233,  -233,  -233,  -233,   363,  -233,  -233,  -233,
    -233,  -233,  -233,   -10,  -233,  -233,    46,  -233,  -233,    76,
      85,  -233,   -16,    27,  -233,   191,    50,  -233,  -233,  -233,
    -233,  -233,   150,   299,   336,    99,  -233,    43,   100,  -233,
     103,   106,    11,  -233,  -233,   260,   -10,   105,   336,   336,
     101,   304,   336,   336,    48,  -233,  -233,   109,   118,   116,
      47,   122,  -233,   336,   363,   363,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   132,   336,   134,   133,
    -233,   140,   137,  -233,     5,  -233,   336,  -233,   143,   144,
     136,  -233,  -233,  -233,  -233,   138,   146,   147,  -233,   148,
     162,   168,  -233,  -233,  -233,   -21,   165,   331,  -233,   163,
    -233,  -233,   158,  -233,   157,  -233,     1,     1,     1,    50,
      50,  -233,  -233,  -233,   140,  -233,   140,   170,  -233,  -233,
     178,   336,  -233,   216,   336,   336,  -233,   336,   187,   185,
     194,   198,   202,   206,   217,   166,   213,  -233,   221,   224,
    -233,   226,  -233,   336,   336,  -233,   228,   131,   222,  -233,
    -233,  -233,   336,   241,   242,   239,   336,   250,   252,   255,
     251,  -233,   275,   287,   140,  -233,   259,   266,  -233,  -233,
    -233,  -233,  -233,  -233,  -233,  -233,   275,  -233,   269,   336,
     274,   276,     0,  -233,  -233,   277,   282,    -9,   279,   283,
     286,  -233,   288,  -233,  -233,   289,   294,   296,    -6,  -233,
     297,  -233,  -233,  -233,   336,   290,   295,   336,  -233,   140,
     275,   140,   320,   336,  -233,   275,   332,   312,   315,   318,
    -233,   321,  -233,    -8,  -233,  -233,  -233,   325,   344,  -233,
     329,   334,  -233,  -233,  -233,   327,   345,  -233,   140,  -233,
     337,   140,   140,   352,   353,  -233,  -233,   336,  -233,  -233,
    -233,  -233,   275,   380,  -233,   354,   355,   356,   140,   140,
     140,  -233,  -233,  -233,   320,  -233
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,    52,    53,    54,    55,    56,    68,
       0,     0,   114,   115,     0,     0,     0,     0,     0,     0,
      78,   108,   113,   110,   111,   112,     0,     2,     4,     6,
      14,    15,     7,    67,     8,     5,     0,    13,     9,     0,
     109,    79,    88,    92,    95,    99,   102,   106,    10,    11,
      12,    16,     0,     0,     0,     0,    75,   108,     0,    17,
       0,     0,     0,    44,    47,     0,     0,     0,    40,     0,
       0,     0,     0,     0,   108,   109,    91,     0,     0,    51,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     136,     0,     0,   135,     0,    76,     0,    74,     0,     0,
       0,   116,    45,    43,    46,     0,     0,    39,    42,     0,
       0,     0,    82,    83,    84,     0,     0,     0,    48,     0,
      63,    85,    89,    90,    94,    93,    98,    96,    97,   100,
     101,   103,   104,   105,     0,   118,     0,   138,   129,   131,
       0,     0,    22,     0,     0,     0,   107,     0,    80,     0,
       0,     0,     0,     0,     0,   108,     0,    50,     0,     0,
     121,     0,   130,     0,     0,   142,     0,     0,     0,    86,
      87,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    70,     0,     0,   139,     0,     0,    35,    36,
      18,    26,    21,    23,    24,    25,     0,    20,     0,    40,
       0,     0,     0,    62,    57,     0,     0,     0,     0,     0,
      69,    72,     0,   119,   132,     0,     0,     0,     0,    81,
       0,   140,    58,    60,     0,     0,     0,     0,    73,     0,
       0,     0,   126,     0,   143,    70,     0,     0,     0,     0,
      61,     0,   141,     0,    64,    71,    66,     0,   128,   133,
       0,     0,    30,    34,    37,     0,     0,   122,     0,   120,
       0,     0,     0,     0,     0,    59,    38,     0,   127,   134,
      27,    29,    70,     0,   123,     0,     0,     0,     0,     0,
       0,    31,    33,   124,   126,   125
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -233,  -233,  -233,    -4,  -233,  -233,  -233,  -233,  -233,  -233,
    -233,  -233,  -233,  -233,  -233,  -233,  -233,  -233,   155,   151,
     -89,  -233,  -233,  -233,     3,  -233,  -233,  -233,  -233,  -233,
    -233,  -233,  -232,  -233,   145,  -233,  -233,   -15,     6,   -66,
     -25,    34,  -233,    18,    -5,    35,  -233,  -233,  -233,  -233,
      93,  -233,  -233,  -233,  -233,  -233,  -233,  -233,  -233,  -233,
    -233,  -233,  -233
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    27,    28,    29,   108,   109,   177,   202,   203,
     226,   227,   204,   273,   274,   205,   206,    30,   116,   117,
      31,    65,    32,    80,   218,    34,   187,   212,    35,   168,
     169,    36,   219,   220,   221,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    97,   171,   242,
     258,   277,   287,   294,   269,    49,   101,   174,   243,   270,
     102,   103,    50
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,    76,    51,    61,    33,   122,   123,   124,    77,    78,
     161,    55,   148,   260,   247,    64,    52,   131,   237,   162,
     266,    59,    66,    12,    13,    79,   157,    84,    85,   248,
     233,   155,    75,   150,   234,    17,   151,    18,   100,   104,
     122,   112,    53,    56,    51,    57,    22,    23,    24,    25,
     285,    91,    92,   118,   119,   170,    99,   172,    54,   132,
     133,   114,    67,    68,    60,    26,    81,    69,    66,    67,
      68,    70,    86,    87,    69,    68,    71,    72,    73,    69,
     128,   129,   145,   106,    72,    73,   139,   140,   179,   180,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    93,    94,    95,   223,   136,   137,   138,    82,
       2,     3,   166,     4,     5,     6,     7,     8,     9,    10,
     134,   135,    11,    12,    13,    83,    14,    15,   141,   142,
     143,    16,   105,   107,   111,    17,   176,    18,   110,    19,
     115,   120,   181,    20,   125,    21,    22,    23,    24,    25,
     254,    96,   256,   126,   198,   199,   127,   130,   195,   196,
     144,   200,   146,  -117,  -117,    26,   201,   208,   147,    19,
     149,   213,   152,   153,   156,  -117,   154,  -117,   155,   278,
     158,   157,   280,   281,   159,  -117,  -117,  -117,  -117,  -117,
      12,    13,   190,    68,   118,   160,   163,    69,   167,   291,
     292,   293,    85,    87,    18,  -117,   106,    72,    73,   228,
     173,   175,    74,    22,    23,    24,    25,   178,   182,   250,
     183,    62,   118,     4,     5,     6,     7,     8,   259,    10,
     184,   186,    11,    12,    13,   185,    14,    15,    88,    89,
      90,    91,    92,   188,   189,    17,   191,    18,   192,    19,
      63,   193,   207,    20,   194,    21,    22,    23,    24,    25,
     197,     3,   284,     4,     5,     6,     7,     8,   209,    10,
     210,   211,    11,    12,    13,    26,    14,    15,     4,     5,
       6,     7,     8,   214,   215,    17,   217,    18,   222,    19,
     113,   216,   224,    20,   225,    21,    22,    23,    24,    25,
      98,   229,     4,     5,     6,     7,     8,   231,   235,   232,
     236,   239,    12,    13,   238,    26,   241,    12,    13,   121,
     240,   245,   244,   246,    17,   249,    18,   251,   252,    17,
     257,    18,  -137,   261,    57,    22,    23,    24,    25,    57,
      22,    23,    24,    25,    12,    13,   164,   262,   263,    12,
      13,   264,   267,   265,    26,   268,    17,   271,    18,    26,
     275,    17,   272,    18,   230,   279,   165,    22,    23,    24,
      25,    57,    22,    23,    24,    25,    12,    13,   276,   282,
     283,   286,   288,   289,   290,   255,    26,   295,   253,     0,
      18,    26,     0,     0,     0,     0,     0,     0,    74,    22,
      23,    24,    25,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26
};

static const yytype_int16 yycheck[] =
{
      15,    26,    33,    18,     1,    71,    72,    73,    18,    19,
      31,     1,   101,   245,    20,    19,    27,    83,    27,    40,
      28,    35,    19,    13,    14,    35,    34,    43,    44,    35,
      30,    40,    26,    28,    34,    25,    31,    27,    53,    54,
     106,    30,    27,    33,    33,    35,    36,    37,    38,    39,
     282,    50,    51,    68,    69,   144,    53,   146,    27,    84,
      85,    65,    26,    27,    26,    55,    20,    31,    65,    26,
      27,    35,    45,    46,    31,    27,    40,    41,    42,    31,
      33,    34,    97,    40,    41,    42,    91,    92,   154,   155,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    52,    53,    54,   194,    88,    89,    90,    33,
       0,     1,   127,     3,     4,     5,     6,     7,     8,     9,
      86,    87,    12,    13,    14,    40,    16,    17,    93,    94,
      95,    21,    33,    33,    28,    25,   151,    27,    35,    29,
      35,    40,   157,    33,    35,    35,    36,    37,    38,    39,
     239,     1,   241,    35,    23,    24,    40,    35,   173,   174,
      28,    30,    28,    13,    14,    55,    35,   182,    35,    29,
      33,   186,    29,    29,    28,    25,    40,    27,    40,   268,
      32,    34,   271,   272,    22,    35,    36,    37,    38,    39,
      13,    14,    26,    27,   209,    27,    31,    31,    35,   288,
     289,   290,    44,    46,    27,    55,    40,    41,    42,   206,
      40,    33,    35,    36,    37,    38,    39,     1,    31,   234,
      35,     1,   237,     3,     4,     5,     6,     7,   243,     9,
      36,    29,    12,    13,    14,    37,    16,    17,    47,    48,
      49,    50,    51,    37,    27,    25,    33,    27,    27,    29,
      30,    27,    30,    33,    28,    35,    36,    37,    38,    39,
      32,     1,   277,     3,     4,     5,     6,     7,    27,     9,
      28,    32,    12,    13,    14,    55,    16,    17,     3,     4,
       5,     6,     7,    33,    32,    25,    35,    27,     1,    29,
      30,    36,    33,    33,    28,    35,    36,    37,    38,    39,
       1,    32,     3,     4,     5,     6,     7,    33,    31,    33,
      28,    28,    13,    14,    35,    55,    28,    13,    14,    15,
      34,    27,    33,    27,    25,    28,    27,    37,    33,    25,
      10,    27,    33,     1,    35,    36,    37,    38,    39,    35,
      36,    37,    38,    39,    13,    14,    15,    35,    33,    13,
      14,    33,    27,    32,    55,    11,    25,    28,    27,    55,
      33,    25,    28,    27,   209,    28,    35,    36,    37,    38,
      39,    35,    36,    37,    38,    39,    13,    14,    33,    27,
      27,     1,    28,    28,    28,   240,    55,   294,   237,    -1,
      27,    55,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    57,     0,     1,     3,     4,     5,     6,     7,     8,
       9,    12,    13,    14,    16,    17,    21,    25,    27,    29,
      33,    35,    36,    37,    38,    39,    55,    58,    59,    60,
      73,    76,    78,    80,    81,    84,    87,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   111,
     118,    33,    27,    27,    27,     1,    33,    35,    93,    35,
      26,    93,     1,    30,    59,    77,    80,    26,    27,    31,
      35,    40,    41,    42,    35,    94,    96,    18,    19,    35,
      79,    20,    33,    40,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,     1,   103,     1,    80,
      93,   112,   116,   117,    93,    33,    40,    33,    61,    62,
      35,    28,    30,    30,    59,    35,    74,    75,    93,    93,
      40,    15,    95,    95,    95,    35,    35,    40,    33,    34,
      35,    95,    96,    96,    97,    97,    99,    99,    99,   100,
     100,   101,   101,   101,    28,    93,    28,    35,    76,    33,
      28,    31,    29,    29,    40,    40,    28,    34,    32,    22,
      27,    31,    40,    31,    15,    35,    93,    35,    85,    86,
      76,   104,    76,    40,   113,    33,    93,    63,     1,    95,
      95,    93,    31,    35,    36,    37,    29,    82,    37,    27,
      26,    33,    27,    27,    28,    93,    93,    32,    23,    24,
      30,    35,    64,    65,    68,    71,    72,    30,    93,    27,
      28,    32,    83,    93,    33,    32,    36,    35,    80,    88,
      89,    90,     1,    76,    33,    28,    66,    67,    80,    32,
      74,    33,    33,    30,    34,    31,    28,    27,    35,    28,
      34,    28,   105,   114,    33,    27,    27,    20,    35,    28,
      93,    37,    33,    75,    76,    90,    76,    10,   106,    93,
      88,     1,    35,    33,    33,    32,    28,    27,    11,   110,
     115,    28,    28,    69,    70,    33,    33,   107,    76,    28,
      76,    76,    27,    27,    93,    88,     1,   108,    28,    28,
      28,    76,    76,    76,   109,   106
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    57,    58,    58,    58,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    61,    60,    62,
      60,    63,    63,    64,    64,    64,    66,    65,    67,    65,
      69,    68,    70,    68,    71,    72,    72,    73,    73,    74,
      74,    75,    75,    76,    76,    76,    77,    77,    78,    78,
      79,    79,    80,    80,    80,    80,    80,    81,    81,    81,
      82,    83,    83,    85,    84,    86,    84,    87,    87,    88,
      88,    89,    89,    90,    91,    91,    91,    92,    92,    93,
      94,    94,    95,    95,    95,    95,    95,    95,    95,    96,
      96,    96,    96,    97,    97,    97,    98,    98,    98,    98,
      99,    99,    99,   100,   100,   100,   100,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   103,   104,   105,
     102,   102,   107,   108,   109,   106,   106,   110,   110,   111,
     111,   113,   114,   115,   112,   116,   116,   116,   117,   117,
     118,   118,   118,   118
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     6,     0,
       6,     2,     0,     1,     1,     1,     0,     6,     0,     6,
       0,     9,     0,     9,     4,     1,     1,     9,    10,     1,
       0,     3,     1,     3,     2,     3,     2,     1,     3,     5,
       3,     1,     1,     1,     1,     1,     1,     6,     7,    10,
       3,     3,     1,     0,     8,     0,     8,     1,     1,     1,
       0,     3,     1,     2,     3,     2,     3,     2,     1,     1,
       4,     7,     3,     3,     3,     3,     5,     5,     1,     3,
       3,     2,     1,     3,     3,     1,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     0,     0,
      10,     5,     0,     0,     0,     9,     0,     2,     0,     4,
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
#line 106 "parser.y"
        {
		printf("Invalid statement at line %d\n", yylineno);
		yyerrok;
	}
#line 1627 "y.tab.c"
    break;

  case 17: /* $@1: %empty  */
#line 114 "parser.y"
      {
        emit("entity",(yyvsp[0].sval),"","");
      }
#line 1635 "y.tab.c"
    break;

  case 18: /* entity_decl: ENTITY IDENTIFIER $@1 LBRACE entity_body RBRACE  */
#line 118 "parser.y"
      {
        emit("end_entity",(yyvsp[-4].sval),"","");
      }
#line 1643 "y.tab.c"
    break;

  case 19: /* $@2: %empty  */
#line 122 "parser.y"
        { emit("entity",(yyvsp[0].sval),"",""); }
#line 1649 "y.tab.c"
    break;

  case 20: /* entity_decl: ENTITY IDENTIFIER $@2 LBRACE error RBRACE  */
#line 124 "parser.y"
        {
      	 printf("Invalid entity body at line %d\n", yylineno);
     	 yyerrok;
     	 emit("end_entity",(yyvsp[-4].sval),"","");
  	}
#line 1659 "y.tab.c"
    break;

  case 26: /* $@3: %empty  */
#line 144 "parser.y"
      {
       emit("constr",(yyvsp[0].sval),"","");
      }
#line 1667 "y.tab.c"
    break;

  case 27: /* constructor_decl: IDENTIFIER $@3 LPAREN param_list_opt RPAREN block  */
#line 148 "parser.y"
       {
        emit("end_constr",(yyvsp[-5].sval),"","");
       }
#line 1675 "y.tab.c"
    break;

  case 28: /* $@4: %empty  */
#line 152 "parser.y"
        { emit("constr", (yyvsp[0].sval), "",""); }
#line 1681 "y.tab.c"
    break;

  case 29: /* constructor_decl: IDENTIFIER $@4 LPAREN error RPAREN block  */
#line 154 "parser.y"
        {
      	printf("Invalid constructor parameters at line %d\n", yylineno);
      	yyerrok;
      	emit("end_constr",(yyvsp[-5].sval),"","");
  	}
#line 1691 "y.tab.c"
    break;

  case 30: /* $@5: %empty  */
#line 163 "parser.y"
      {
        emit("method",(yyvsp[0].sval),"","");
      }
#line 1699 "y.tab.c"
    break;

  case 31: /* method_decl: access_modifier type FUNC IDENTIFIER $@5 LPAREN param_list_opt RPAREN block  */
#line 167 "parser.y"
      {
        emit("end_method",(yyvsp[-5].sval),"","");
      }
#line 1707 "y.tab.c"
    break;

  case 32: /* $@6: %empty  */
#line 172 "parser.y"
        { emit("method", (yyvsp[0].sval), "", ""); }
#line 1713 "y.tab.c"
    break;

  case 33: /* method_decl: access_modifier type FUNC IDENTIFIER $@6 LPAREN error RPAREN block  */
#line 174 "parser.y"
        {
		printf("Invalid method parameters at line %d\n", yylineno);
		yyerrok;
		emit("end_method", (yyvsp[-5].sval), "","");
	}
#line 1723 "y.tab.c"
    break;

  case 37: /* object_decl: IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 192 "parser.y"
       {
         emit("new",(yyvsp[-4].sval),"",(yyvsp[-7].sval));
         emit("call_constr",(yyvsp[-4].sval),"",(yyvsp[-7].sval));
       }
#line 1732 "y.tab.c"
    break;

  case 38: /* object_decl: type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list RPAREN SEMICOLON  */
#line 197 "parser.y"
       {
        char* t=genVar();
        emit("push_ptr",(yyvsp[-6].sval),"","");
        emit("call_method",(yyvsp[-4].sval),"",t);
        emit("=",t,"",(yyvsp[-8].sval));
       }
#line 1743 "y.tab.c"
    break;

  case 41: /* arg_list: arg_list COMMA expression  */
#line 217 "parser.y"
      {
       emit("arg",(yyvsp[0].sval),"","");
      }
#line 1751 "y.tab.c"
    break;

  case 42: /* arg_list: expression  */
#line 222 "parser.y"
      {
       emit("arg",(yyvsp[0].sval),"","");
      }
#line 1759 "y.tab.c"
    break;

  case 45: /* block: LBRACE error RBRACE  */
#line 231 "parser.y"
        {
		printf("Error inside block at line %d\n", yylineno);
		yyerrok;
	}
#line 1768 "y.tab.c"
    break;

  case 49: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 245 "parser.y"
        { emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval)); }
#line 1774 "y.tab.c"
    break;

  case 63: /* $@7: %empty  */
#line 274 "parser.y"
       {
         emit ("func",(yyvsp[0].sval),"",""); 
       }
#line 1782 "y.tab.c"
    break;

  case 64: /* function_decl: func_type FUNC IDENTIFIER $@7 LPAREN param_list_opt RPAREN block  */
#line 278 "parser.y"
        {
         emit("endfunc","","",""); 
        }
#line 1790 "y.tab.c"
    break;

  case 65: /* $@8: %empty  */
#line 282 "parser.y"
        { emit("func",(yyvsp[0].sval),"",""); }
#line 1796 "y.tab.c"
    break;

  case 66: /* function_decl: func_type FUNC IDENTIFIER $@8 LPAREN error RPAREN block  */
#line 284 "parser.y"
        {
		printf("Invalid parameter list at line %d\n",yylineno);
		yyerrok;
		emit("endfunc","","","");
	}
#line 1806 "y.tab.c"
    break;

  case 73: /* param: type IDENTIFIER  */
#line 317 "parser.y"
      {
        emit("param",(yyvsp[0].sval),"","");
      }
#line 1814 "y.tab.c"
    break;

  case 74: /* return_stmt: RETURN expression SEMICOLON  */
#line 324 "parser.y"
      { 
       emit("return",(yyvsp[-1].sval),"","");
      }
#line 1822 "y.tab.c"
    break;

  case 75: /* return_stmt: RETURN SEMICOLON  */
#line 328 "parser.y"
      {
        emit("return","","","");
      }
#line 1830 "y.tab.c"
    break;

  case 76: /* return_stmt: RETURN error SEMICOLON  */
#line 332 "parser.y"
        {
		printf("Invalid Return statement at line %d\n");
		yyerrok;
	}
#line 1839 "y.tab.c"
    break;

  case 79: /* expression: assignment  */
#line 344 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1845 "y.tab.c"
    break;

  case 80: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET  */
#line 349 "parser.y"
        {
		char* t1 = genVar();
		emit("*",(yyvsp[-1].sval),"type.width",t1);
		
		char* t2 = genVar();
		emit("[]",(yyvsp[-3].sval),t1,t2);
		
		(yyval.sval) = t2;
	}
#line 1859 "y.tab.c"
    break;

  case 81: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET  */
#line 359 "parser.y"
        {
		char* t1 = genVar();
		emit("*",(yyvsp[-4].sval),"array.cols",t1);
		
		char* t2 = genVar();
		emit("+",t1,(yyvsp[-1].sval),t2);

		char* t3 = genVar();
		emit("*",t2,"type.width",t3);
		
		char* t4 = genVar();
		emit("[]",(yyvsp[-6].sval),t3,t4);

		(yyval.sval) = t4;
	}
#line 1879 "y.tab.c"
    break;

  case 82: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 379 "parser.y"
        { emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval)); (yyval.sval) = strdup((yyvsp[-2].sval)); }
#line 1885 "y.tab.c"
    break;

  case 83: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 381 "parser.y"
        { char* t = genVar(); emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t); emit("=", t, "", (yyvsp[-2].sval)); (yyval.sval) = t; }
#line 1891 "y.tab.c"
    break;

  case 84: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 383 "parser.y"
        { char* t = genVar(); emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t); emit("=", t, "", (yyvsp[-2].sval)); (yyval.sval) = t; }
#line 1897 "y.tab.c"
    break;

  case 85: /* assignment: indexed_id ASSIGN assignment  */
#line 384 "parser.y"
                                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1903 "y.tab.c"
    break;

  case 86: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 385 "parser.y"
                                            { emit("set_field", "this", (yyvsp[-2].sval), (yyvsp[0].sval)); (yyval.sval) = (yyvsp[0].sval); }
#line 1909 "y.tab.c"
    break;

  case 87: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 386 "parser.y"
                                                  { emit("set_field", (yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); (yyval.sval) = (yyvsp[0].sval); }
#line 1915 "y.tab.c"
    break;

  case 88: /* assignment: logic_expr  */
#line 387 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1921 "y.tab.c"
    break;

  case 89: /* logic_expr: logic_expr OR logic_expr  */
#line 391 "parser.y"
                               { char* t = genVar(); emit("||", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1927 "y.tab.c"
    break;

  case 90: /* logic_expr: logic_expr AND logic_expr  */
#line 392 "parser.y"
                                { char* t = genVar(); emit("&&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1933 "y.tab.c"
    break;

  case 91: /* logic_expr: NOT logic_expr  */
#line 393 "parser.y"
                     { char* t = genVar(); emit("!", (yyvsp[0].sval), "", t); (yyval.sval) = t; }
#line 1939 "y.tab.c"
    break;

  case 92: /* logic_expr: bitwise_expr  */
#line 394 "parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1945 "y.tab.c"
    break;

  case 93: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 398 "parser.y"
                                       { char* t = genVar(); emit("&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1951 "y.tab.c"
    break;

  case 94: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 399 "parser.y"
                                      { char* t = genVar(); emit("|", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1957 "y.tab.c"
    break;

  case 95: /* bitwise_expr: rel_expr  */
#line 400 "parser.y"
               { (yyval.sval) = (yyvsp[0].sval); }
#line 1963 "y.tab.c"
    break;

  case 96: /* rel_expr: arith_expr GT arith_expr  */
#line 404 "parser.y"
                               { char* t = genVar(); emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1969 "y.tab.c"
    break;

  case 97: /* rel_expr: arith_expr LT arith_expr  */
#line 405 "parser.y"
                               { char* t = genVar(); emit("<", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1975 "y.tab.c"
    break;

  case 98: /* rel_expr: arith_expr EQ arith_expr  */
#line 406 "parser.y"
                               { char* t = genVar(); emit("==", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1981 "y.tab.c"
    break;

  case 99: /* rel_expr: arith_expr  */
#line 407 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1987 "y.tab.c"
    break;

  case 100: /* arith_expr: arith_expr PLUS term  */
#line 411 "parser.y"
                           { char* t = genVar(); emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1993 "y.tab.c"
    break;

  case 101: /* arith_expr: arith_expr MINUS term  */
#line 412 "parser.y"
                            { char* t = genVar(); emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1999 "y.tab.c"
    break;

  case 102: /* arith_expr: term  */
#line 413 "parser.y"
           { (yyval.sval) = (yyvsp[0].sval); }
#line 2005 "y.tab.c"
    break;

  case 103: /* term: term MUL factor  */
#line 417 "parser.y"
                      { char* t = genVar(); emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2011 "y.tab.c"
    break;

  case 104: /* term: term DIV factor  */
#line 418 "parser.y"
                      { char* t = genVar(); emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2017 "y.tab.c"
    break;

  case 105: /* term: term MOD factor  */
#line 419 "parser.y"
                      { char* t = genVar(); emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 2023 "y.tab.c"
    break;

  case 106: /* term: factor  */
#line 420 "parser.y"
             { (yyval.sval) = (yyvsp[0].sval); }
#line 2029 "y.tab.c"
    break;

  case 107: /* factor: IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 425 "parser.y"
      {
        char* t=genVar();
        emit("call",(yyvsp[-3].sval),"",t);
        (yyval.sval)=t;
      }
#line 2039 "y.tab.c"
    break;

  case 108: /* factor: IDENTIFIER  */
#line 430 "parser.y"
                 { (yyval.sval) = strdup((yyvsp[0].sval)); }
#line 2045 "y.tab.c"
    break;

  case 109: /* factor: indexed_id  */
#line 431 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2051 "y.tab.c"
    break;

  case 110: /* factor: INT_LITERAL  */
#line 432 "parser.y"
                  { char b[20]; sprintf(b, "%d", (yyvsp[0].ival)); (yyval.sval) = strdup(b); }
#line 2057 "y.tab.c"
    break;

  case 111: /* factor: FLOAT_LITERAL  */
#line 433 "parser.y"
                    { char b[20]; sprintf(b, "%f", (yyvsp[0].fval)); (yyval.sval) = strdup(b); }
#line 2063 "y.tab.c"
    break;

  case 112: /* factor: CHAR_LITERAL  */
#line 434 "parser.y"
                   { char b[20]; sprintf(b, "'%c'", (yyvsp[0].cval)); (yyval.sval) = strdup(b); }
#line 2069 "y.tab.c"
    break;

  case 113: /* factor: STRING_LITERAL  */
#line 435 "parser.y"
                     { (yyval.sval) = strdup((yyvsp[0].sval)); }
#line 2075 "y.tab.c"
    break;

  case 114: /* factor: TRUE  */
#line 436 "parser.y"
           { (yyval.sval) = strdup("1"); }
#line 2081 "y.tab.c"
    break;

  case 115: /* factor: FALSE  */
#line 437 "parser.y"
            { (yyval.sval) = strdup("0"); }
#line 2087 "y.tab.c"
    break;

  case 116: /* factor: LPAREN expression RPAREN  */
#line 438 "parser.y"
                               { (yyval.sval) = (yyvsp[-1].sval); }
#line 2093 "y.tab.c"
    break;

  case 117: /* $@9: %empty  */
#line 443 "parser.y"
        { char* f = getLabel(); char* e = getLabel(); pushIfLabels(f, e); }
#line 2099 "y.tab.c"
    break;

  case 118: /* $@10: %empty  */
#line 445 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 2105 "y.tab.c"
    break;

  case 119: /* $@11: %empty  */
#line 447 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 2111 "y.tab.c"
    break;

  case 120: /* if_stmt: IF LPAREN $@9 expression $@10 RPAREN block $@11 elif_list else_opt  */
#line 449 "parser.y"
        { emit("label", "", "", topEnd()); popIfLabels(); }
#line 2117 "y.tab.c"
    break;

  case 121: /* if_stmt: IF LPAREN error RPAREN block  */
#line 451 "parser.y"
        {
		printf("Invalid IF condition at line %d\n", yylineno);
		yyerrok;
	}
#line 2126 "y.tab.c"
    break;

  case 122: /* $@12: %empty  */
#line 459 "parser.y"
        { char* n = getLabel(); free(falseStack[topPtr]); falseStack[topPtr] = strdup(n); }
#line 2132 "y.tab.c"
    break;

  case 123: /* $@13: %empty  */
#line 461 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 2138 "y.tab.c"
    break;

  case 124: /* $@14: %empty  */
#line 463 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 2144 "y.tab.c"
    break;

  case 129: /* for_stmt: FOR LPAREN for_header block  */
#line 475 "parser.y"
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
#line 2159 "y.tab.c"
    break;

  case 130: /* for_stmt: FOR LPAREN error RPAREN block  */
#line 486 "parser.y"
        {
		printf("Invalid FOR header at line %d\n", yylineno);
		yyerrok;
	}
#line 2168 "y.tab.c"
    break;

  case 131: /* $@15: %empty  */
#line 494 "parser.y"
        { char* b = getLabel(); char* e = getLabel(); pushIfLabels(b, e); emit("label", "", "", b); }
#line 2174 "y.tab.c"
    break;

  case 132: /* $@16: %empty  */
#line 496 "parser.y"
        { emit("ifFalse", (yyvsp[-1].sval), "", topEnd()); forDepth++; forIncIdx[forDepth] = 0; inForIncrement[forDepth] = 1; }
#line 2180 "y.tab.c"
    break;

  case 133: /* $@17: %empty  */
#line 498 "parser.y"
        { inForIncrement[forDepth] = 0; }
#line 2186 "y.tab.c"
    break;

  case 139: /* var_decl_no_semi: type IDENTIFIER ASSIGN expression  */
#line 510 "parser.y"
                                        { emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval)); }
#line 2192 "y.tab.c"
    break;

  case 140: /* io_stmt: IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 515 "parser.y"
      {
        emit("in","","",(yyvsp[-6].sval));
      }
#line 2200 "y.tab.c"
    break;

  case 141: /* io_stmt: type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 519 "parser.y"
      {
        emit("in","","",(yyvsp[-6].sval));
      }
#line 2208 "y.tab.c"
    break;

  case 142: /* io_stmt: SHOW LPAREN expression RPAREN SEMICOLON  */
#line 523 "parser.y"
      {
       emit("out",(yyvsp[-2].sval),"","");
      }
#line 2216 "y.tab.c"
    break;

  case 143: /* io_stmt: SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON  */
#line 527 "parser.y"
     {
       char* t=genVar();
       emit("[]",(yyvsp[-5].sval),(yyvsp[-3].sval),t);
       emit("out",t,"","");
     }
#line 2226 "y.tab.c"
    break;


#line 2230 "y.tab.c"

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

#line 534 "parser.y"


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

void yyerror(const char *s) {
	//errorCount++;
	fprintf(stderr, "\nSyntax Error at line %d near '%s'\n", yylineno, yytext);
}

int main() {
    yyin = stdin;
    yyparse();
    printf("Parsing Successful\nGenerated quadruple table:\n");
    printf("%-15s %-15s %-15s %-15s\n", "OP", "ARG1", "ARG2", "RESULT");
    for(int i = 0; i < IR_idx; i++)
        printf("%-15s %-15s %-15s %-15s\n", IR[i].op, IR[i].arg1, IR[i].arg2, IR[i].result);
    return 0;
}
