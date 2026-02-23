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

#line 122 "y.tab.c"

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
#line 52 "parser.y"

    int ival;
    float fval;
    char cval;
    char* sval;

#line 292 "y.tab.c"

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
  YYSYMBOL_entity_body = 62,               /* entity_body  */
  YYSYMBOL_entity_member = 63,             /* entity_member  */
  YYSYMBOL_constructor_decl = 64,          /* constructor_decl  */
  YYSYMBOL_65_2 = 65,                      /* $@2  */
  YYSYMBOL_method_decl = 66,               /* method_decl  */
  YYSYMBOL_67_3 = 67,                      /* $@3  */
  YYSYMBOL_access_var_decl = 68,           /* access_var_decl  */
  YYSYMBOL_access_modifier = 69,           /* access_modifier  */
  YYSYMBOL_object_decl = 70,               /* object_decl  */
  YYSYMBOL_arg_list_opt = 71,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 72,                  /* arg_list  */
  YYSYMBOL_block = 73,                     /* block  */
  YYSYMBOL_stmt_list = 74,                 /* stmt_list  */
  YYSYMBOL_var_decl = 75,                  /* var_decl  */
  YYSYMBOL_id_list = 76,                   /* id_list  */
  YYSYMBOL_type = 77,                      /* type  */
  YYSYMBOL_array_decl = 78,                /* array_decl  */
  YYSYMBOL_array_init = 79,                /* array_init  */
  YYSYMBOL_expr_list = 80,                 /* expr_list  */
  YYSYMBOL_function_decl = 81,             /* function_decl  */
  YYSYMBOL_82_4 = 82,                      /* $@4  */
  YYSYMBOL_func_type = 83,                 /* func_type  */
  YYSYMBOL_param_list_opt = 84,            /* param_list_opt  */
  YYSYMBOL_param_list = 85,                /* param_list  */
  YYSYMBOL_param = 86,                     /* param  */
  YYSYMBOL_return_stmt = 87,               /* return_stmt  */
  YYSYMBOL_expr_stmt = 88,                 /* expr_stmt  */
  YYSYMBOL_expression = 89,                /* expression  */
  YYSYMBOL_indexed_id = 90,                /* indexed_id  */
  YYSYMBOL_assignment = 91,                /* assignment  */
  YYSYMBOL_logic_expr = 92,                /* logic_expr  */
  YYSYMBOL_bitwise_expr = 93,              /* bitwise_expr  */
  YYSYMBOL_rel_expr = 94,                  /* rel_expr  */
  YYSYMBOL_arith_expr = 95,                /* arith_expr  */
  YYSYMBOL_term = 96,                      /* term  */
  YYSYMBOL_factor = 97,                    /* factor  */
  YYSYMBOL_if_stmt = 98,                   /* if_stmt  */
  YYSYMBOL_99_5 = 99,                      /* $@5  */
  YYSYMBOL_100_6 = 100,                    /* $@6  */
  YYSYMBOL_101_7 = 101,                    /* $@7  */
  YYSYMBOL_elif_list = 102,                /* elif_list  */
  YYSYMBOL_103_8 = 103,                    /* $@8  */
  YYSYMBOL_104_9 = 104,                    /* $@9  */
  YYSYMBOL_105_10 = 105,                   /* $@10  */
  YYSYMBOL_else_opt = 106,                 /* else_opt  */
  YYSYMBOL_for_stmt = 107,                 /* for_stmt  */
  YYSYMBOL_for_header = 108,               /* for_header  */
  YYSYMBOL_109_11 = 109,                   /* $@11  */
  YYSYMBOL_110_12 = 110,                   /* $@12  */
  YYSYMBOL_111_13 = 111,                   /* $@13  */
  YYSYMBOL_for_init = 112,                 /* for_init  */
  YYSYMBOL_var_decl_no_semi = 113,         /* var_decl_no_semi  */
  YYSYMBOL_io_stmt = 114                   /* io_stmt  */
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
<<<<<<< HEAD
#define YYLAST   409
=======
#define YYLAST   376
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
<<<<<<< HEAD
#define YYNRULES  128
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  268
=======
#define YYNRULES  130
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  264
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936

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
<<<<<<< HEAD
       0,   120,   120,   121,   125,   126,   127,   128,   132,   136,
     137,   141,   142,   143,   147,   151,   155,   159,   160,   164,
     165,   169,   170,   174,   175,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   191,   192,   196,   197,   201,   205,
     213,   214,   218,   219,   220,   221,   222,   223,   227,   228,
     229,   233,   237,   238,   242,   246,   247,   251,   252,   256,
     257,   261,   265,   266,   270,   271,   275,   279,   289,   309,
     314,   322,   329,   330,   334,   338,   345,   351,   357,   363,
     370,   376,   382,   388,   394,   400,   406,   413,   419,   425,
     432,   438,   444,   450,   457,   461,   462,   468,   474,   480,
     484,   490,   496,   504,   510,   515,   503,   529,   538,   543,
     528,   547,   552,   551,   556,   563,   573,   577,   583,   561,
     602,   612,   616,   622,   600,   642,   643,   644,   645
=======
       0,    83,    83,    84,    88,    89,    90,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   107,   106,   117,   118,
     122,   123,   124,   129,   128,   140,   139,   150,   154,   155,
     159,   164,   174,   175,   179,   184,   191,   192,   196,   197,
     201,   202,   207,   208,   212,   212,   212,   212,   212,   212,
     216,   217,   218,   222,   226,   227,   232,   231,   242,   243,
     247,   248,   252,   253,   257,   264,   268,   275,   276,   280,
     284,   288,   290,   292,   294,   295,   296,   297,   301,   302,
     303,   304,   308,   309,   310,   314,   315,   316,   317,   321,
     322,   323,   327,   328,   329,   330,   334,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   353,   355,   357,   352,
     364,   366,   368,   363,   370,   374,   375,   379,   394,   396,
     398,   393,   403,   404,   405,   409,   410,   414,   418,   422,
     426
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
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
  "program", "element", "statement", "entity_decl", "$@1", "entity_body",
  "entity_member", "constructor_decl", "$@2", "method_decl", "$@3",
  "access_var_decl", "access_modifier", "object_decl", "arg_list_opt",
  "arg_list", "block", "stmt_list", "var_decl", "id_list", "type",
  "array_decl", "array_init", "expr_list", "function_decl", "$@4",
  "func_type", "param_list_opt", "param_list", "param", "return_stmt",
  "expr_stmt", "expression", "indexed_id", "assignment", "logic_expr",
  "bitwise_expr", "rel_expr", "arith_expr", "term", "factor", "if_stmt",
  "$@5", "$@6", "$@7", "elif_list", "$@8", "$@9", "$@10", "else_opt",
  "for_stmt", "for_header", "$@11", "$@12", "$@13", "for_init",
  "var_decl_no_semi", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

<<<<<<< HEAD
#define YYPACT_NINF (-181)
=======
#define YYPACT_NINF (-206)
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-50)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
<<<<<<< HEAD
    -181,   118,  -181,  -181,  -181,  -181,  -181,  -181,  -181,   -41,
     -35,  -181,  -181,     4,   297,    35,    33,   254,  -181,  -181,
    -181,  -181,     5,  -181,   359,   176,  -181,  -181,  -181,  -181,
    -181,  -181,   -11,  -181,  -181,    11,  -181,  -181,    22,    28,
    -181,    42,    54,  -181,   -10,    32,  -181,  -181,  -181,  -181,
    -181,   264,   359,    18,  -181,    31,    67,   106,   111,    40,
     328,   359,   359,   359,    88,  -181,  -181,    93,  -181,  -181,
    -181,   234,   -11,   125,   131,   114,    68,   134,  -181,   359,
       5,     5,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,   359,   290,   359,  -181,    -1,   359,  -181,  -181,
     120,   124,   141,   121,  -181,  -181,  -181,   129,  -181,  -181,
    -181,   -40,   119,   335,  -181,   159,   137,  -181,  -181,   151,
    -181,   152,    32,    32,    96,    96,    96,  -181,  -181,  -181,
    -181,  -181,   146,   148,   359,    16,   359,   359,   172,   169,
     154,   157,   182,   183,   162,   110,   166,  -181,   171,   164,
     168,   359,  -181,   165,  -181,  -181,   167,  -181,  -181,  -181,
    -181,  -181,   171,  -181,  -181,   173,   174,   359,   359,   179,
     175,   177,   188,   195,  -181,  -181,   206,   184,   185,  -181,
     190,   359,  -181,   193,   171,    -3,   359,   197,   194,    -8,
    -181,  -181,   204,   199,   203,   -24,  -181,   190,   171,  -181,
    -181,   207,   208,   209,   230,   210,   215,   226,  -181,  -181,
    -181,   359,  -181,  -181,   251,   235,   359,  -181,  -181,   275,
     240,  -181,  -181,   190,   246,  -181,   242,   359,  -181,   243,
    -181,     7,   247,   291,  -181,   359,  -181,   171,  -181,  -181,
     257,   259,  -181,  -181,  -181,   359,  -181,   258,  -181,  -181,
     359,   190,  -181,   261,   190,  -181,  -181,   262,   190,  -181,
     272,   190,  -181,   190,  -181,  -181,   275,  -181
=======
    -206,   104,  -206,  -206,  -206,  -206,  -206,  -206,  -206,     3,
       9,  -206,  -206,    15,   243,    37,    57,   280,   179,  -206,
     334,  -206,  -206,  -206,  -206,   307,  -206,  -206,  -206,  -206,
    -206,  -206,    21,  -206,  -206,   102,  -206,  -206,    86,    84,
    -206,   -37,    31,  -206,    51,    82,  -206,  -206,  -206,  -206,
    -206,     8,   280,  -206,    27,    93,  -206,    95,   100,  -206,
    -206,   216,    21,   103,   280,   280,   107,   248,   280,   280,
      30,  -206,  -206,   109,   123,   125,    81,   139,  -206,   280,
     307,   307,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   280,    39,   140,  -206,   148,   145,  -206,    75,
     280,  -206,   150,   141,  -206,  -206,  -206,   147,   152,   155,
    -206,   162,   175,   171,  -206,  -206,  -206,    33,   169,   275,
    -206,   166,  -206,  -206,   159,  -206,   164,  -206,    99,    99,
      99,    82,    82,  -206,  -206,  -206,  -206,   165,  -206,  -206,
     178,   280,  -206,   280,   280,  -206,   280,  -206,   189,   177,
     190,   197,   194,   208,   126,   204,  -206,   211,   212,   280,
     280,  -206,   207,   242,  -206,  -206,  -206,   215,   219,   218,
     280,   225,   227,   224,   209,  -206,    20,   148,  -206,   231,
     220,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,    20,
     280,   234,   236,    41,  -206,  -206,   260,   246,   -11,  -206,
     257,   267,   262,  -206,  -206,  -206,   264,   272,    -3,   273,
    -206,  -206,  -206,   280,   269,   271,   280,  -206,   148,    20,
     298,   280,  -206,    20,   274,   289,   290,  -206,   292,  -206,
       0,  -206,  -206,   300,   317,  -206,   301,  -206,  -206,  -206,
     299,   303,  -206,   148,  -206,   305,   148,   304,  -206,  -206,
     280,  -206,  -206,  -206,    20,  -206,   309,   310,   148,   148,
    -206,  -206,   298,  -206
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
<<<<<<< HEAD
       3,     0,     1,    42,    43,    44,    45,    46,    56,     0,
       0,   100,   101,     0,     0,     0,     0,    94,    96,    97,
      98,    99,     0,    65,     0,     0,     2,     6,     7,     4,
      33,    25,    55,    26,     5,     0,    31,    27,     0,    95,
      66,    75,    79,    82,    86,    89,    93,    28,    29,    30,
     103,     0,     0,    94,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    78,     0,    35,    32,
      37,     0,     0,     0,     0,    41,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,   115,     0,     0,    62,    10,
       0,     0,     0,     0,    69,    70,    71,     0,   102,    34,
      36,     0,     0,     0,    38,     0,     0,    72,    77,    76,
      80,    81,    87,    88,    83,    84,    85,    90,    91,    92,
     104,   120,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,     0,     0,     0,    94,     0,    40,    58,     0,
       0,     0,   127,     0,    17,    18,     0,     8,     9,    11,
      12,    13,     0,    73,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,    47,     0,     0,    57,    60,
       0,     0,   116,     0,    58,     0,    22,     0,     0,     0,
      53,    48,     0,     0,     0,     0,    61,     0,     0,   105,
     121,     0,     0,     0,     0,     0,     0,    21,    24,   125,
      68,     0,    51,    49,     0,     0,     0,    54,    59,   111,
       0,   117,   128,     0,     0,    16,     0,     0,    52,     0,
     126,     0,     0,   114,   122,     0,    14,    58,    19,    23,
       0,     0,   107,   112,   106,     0,   118,     0,    50,    20,
       0,     0,   123,     0,     0,   108,   113,     0,     0,    15,
       0,     0,   119,     0,   124,   109,   111,   110
=======
       3,     0,     1,    44,    45,    46,    47,    48,    59,     0,
       0,   103,   104,     0,     0,     0,     0,     0,     0,    68,
      97,   102,    99,   100,   101,     0,     2,     4,     6,    14,
      15,     7,    58,     8,     5,     0,    13,     9,     0,    98,
      69,    77,    81,    84,    88,    91,    95,    10,    11,    12,
     106,   124,     0,    66,    97,     0,    16,     0,     0,    37,
      39,     0,     0,     0,    33,     0,     0,     0,     0,     0,
      97,    98,    80,     0,     0,    43,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,   123,     0,     0,   122,     0,
       0,    65,     0,     0,   105,    36,    38,     0,     0,    32,
      35,     0,     0,     0,    71,    72,    73,     0,     0,     0,
      40,     0,    56,    74,    78,    79,    83,    82,    87,    85,
      86,    89,    90,    92,    93,    94,   107,   125,   117,   118,
       0,     0,    19,     0,     0,    96,     0,    70,     0,     0,
       0,     0,     0,     0,    97,     0,    42,     0,     0,     0,
       0,   129,     0,     0,    75,    76,    34,     0,     0,     0,
       0,     0,     0,     0,     0,    41,    61,     0,   126,     0,
       0,    28,    29,    17,    23,    18,    20,    21,    22,     0,
      33,     0,     0,     0,    55,    50,     0,     0,     0,    49,
       0,     0,    60,    63,   108,   119,     0,     0,     0,     0,
     127,    51,    53,     0,     0,     0,     0,    64,     0,     0,
     114,     0,   130,    61,     0,     0,     0,    54,     0,   128,
       0,    57,    62,     0,   116,   120,     0,    25,    27,    30,
       0,     0,   110,     0,   109,     0,     0,     0,    52,    31,
       0,   115,   121,    24,    61,   111,     0,     0,     0,     0,
      26,   112,   114,   113
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
<<<<<<< HEAD
    -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,
     329,  -181,   113,   -19,  -177,  -181,  -181,  -181,     0,  -181,
    -181,  -181,  -181,  -181,  -180,  -181,   133,  -181,  -181,   -14,
      20,   -49,   -20,    58,  -181,     8,    66,    24,  -181,  -181,
    -181,  -181,    72,  -181,  -181,  -181,  -181,  -181,  -181,  -181,
    -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181
=======
    -206,  -206,  -206,   -13,  -206,  -206,  -206,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,   151,   124,   -95,  -206,  -206,
    -206,     1,  -206,  -206,  -206,  -206,  -206,  -206,  -205,  -206,
     136,  -206,  -206,   -14,     6,   -59,   -21,    72,  -206,    76,
      85,    80,  -206,  -206,  -206,  -206,    94,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
<<<<<<< HEAD
       0,     1,    26,    27,   135,   158,   159,   160,   161,   162,
      69,   206,   207,    29,    30,    71,    31,    76,   176,    33,
     169,   189,    34,    35,   177,   178,   179,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    92,
     149,   219,   233,   250,   260,   266,   244,   251,    48,   132,
     201,   235,   253,   150,   220,   245,   257,    49
=======
       0,     1,    26,    27,    28,   102,   163,   185,   186,   207,
     187,   247,   188,   189,    29,   108,   109,    30,    61,    31,
      76,   200,    33,   171,   193,    34,   157,    35,   201,   202,
     203,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    92,   158,   220,   234,   250,   257,   262,
     244,    48,    96,   160,   221,   245,    97,    98,    49
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
<<<<<<< HEAD
      55,    32,    66,   199,   203,   141,    70,    73,    74,    50,
      67,   104,   105,   106,   142,    51,    75,   204,    11,    12,
     217,   137,    84,    85,   205,    72,   216,    86,    87,    88,
     117,    77,    64,    18,    19,    20,    21,    95,    96,   154,
     155,   211,    65,   156,    58,   212,   236,    22,   104,   107,
     133,    94,   110,   134,    52,    24,   227,   247,   241,    57,
     118,   119,    56,    97,    61,    62,    89,    90,    91,   157,
      78,    72,    63,    79,   256,    11,    12,   259,   130,    98,
     131,   262,    80,    81,   264,   102,   265,   163,   164,    64,
      18,    19,    20,    21,   124,   125,   126,    82,    83,   146,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    24,   127,   128,   129,   114,   115,     2,    99,
     153,     3,     4,     5,     6,     7,     8,     9,    84,    85,
      10,    11,    12,   100,    13,    14,   173,   182,   101,    15,
     120,   121,    63,    16,   108,    17,    18,    19,    20,    21,
     122,   123,   111,   188,   190,    97,    61,    62,   112,   113,
      22,   116,   185,   138,    63,   136,    23,   200,    24,   137,
      25,   139,   208,   143,     3,     4,     5,     6,     7,     3,
       4,     5,     6,     7,   140,     9,   147,   148,    10,    11,
      12,    80,    13,    14,   151,    82,   152,   228,   175,   165,
     166,    16,   208,    17,    18,    19,    20,    21,   167,   168,
     170,   171,   172,   239,   174,   180,   181,   184,    22,   194,
     183,   246,   195,   186,    23,   187,    24,   191,    25,    68,
     192,   252,   193,   196,   198,   197,   255,     3,     4,     5,
       6,     7,    25,     9,   202,   209,    10,    11,    12,   210,
      13,    14,   213,   214,   215,   221,   222,   224,   225,    16,
     223,    17,    18,    19,    20,    21,   226,     3,     4,     5,
       6,     7,   -47,   -47,   -47,   227,    22,    11,    12,   229,
      58,    59,    23,   230,    24,   232,    25,   109,   234,    16,
     238,    93,    18,    19,    20,    21,   237,   242,   240,    60,
      61,    62,   243,   -47,   -47,   248,    22,   249,    63,   254,
      11,    12,   258,   261,    24,   -47,    58,   -47,   -47,   -47,
     -47,   -47,    16,   263,    53,    18,    19,    20,    21,   231,
      28,   218,   -47,     0,     0,    97,    61,    62,   267,    22,
     -47,    11,    12,   103,    63,    54,     0,    24,    11,    12,
     144,     0,     0,    16,     0,    53,    18,    19,    20,    21,
      16,     0,   145,    18,    19,    20,    21,     0,     0,     0,
      22,     0,    11,    12,     0,     0,     0,    22,    24,     0,
       0,     0,     0,     0,    16,    24,    53,    18,    19,    20,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    24
=======
      55,   138,    32,    58,    72,    60,    80,    81,   114,   115,
     116,     3,     4,     5,     6,     7,   216,   224,   236,    62,
     123,    11,    12,     3,     4,     5,     6,     7,   241,   144,
      50,    71,   225,    16,   146,    17,    51,    95,    99,    73,
      74,   114,    52,    93,    21,    22,    23,    24,   106,   256,
     110,   111,    94,    63,    64,   199,    75,    64,    65,   124,
     125,    65,    62,    25,   150,    63,    64,   100,    68,    69,
      65,   212,    56,   151,   -49,   213,    82,    83,   136,   100,
      68,    69,   204,    57,   164,   165,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    84,    85,
      86,    87,    88,   140,     2,   155,   141,     3,     4,     5,
       6,     7,     8,     9,   120,   121,    10,    11,    12,    78,
      13,    14,    77,   231,    79,    15,   101,   162,   104,    16,
     103,    17,   166,    18,    89,    90,    91,    19,   107,    20,
      21,    22,    23,    24,   117,   178,   179,   112,   251,    87,
      88,   253,   174,    64,   126,   127,   194,    65,   118,    25,
     128,   129,   130,   260,   261,   119,   100,    68,    69,   133,
     134,   135,   131,   132,   122,   137,   110,    18,   139,   142,
     145,   143,     3,     4,     5,     6,     7,   144,     9,   146,
     208,    10,    11,    12,   147,    13,    14,   148,   149,   227,
     152,   156,   110,    81,    16,   159,    17,   235,    18,    59,
      83,   161,    19,   168,    20,    21,    22,    23,    24,     3,
       4,     5,     6,     7,   167,     9,   170,   169,    10,    11,
      12,   172,    13,    14,    25,   173,   255,   175,   176,   180,
     177,    16,   190,    17,   198,    18,   105,   191,   206,    19,
     192,    20,    21,    22,    23,    24,    11,    12,   195,   196,
     197,    11,    12,   113,   205,   181,   182,   210,    16,   211,
      17,    25,   183,    16,   215,    17,    53,   184,    54,    21,
      22,    23,    24,    54,    21,    22,    23,    24,    11,    12,
     153,   214,   217,    11,    12,   218,   219,   222,    25,   223,
      16,   226,    17,    25,   229,    16,   228,    17,   233,   237,
     154,    21,    22,    23,    24,    54,    21,    22,    23,    24,
      11,    12,   238,   239,   240,    11,    12,   242,   243,   246,
      25,   254,   248,   252,    17,    25,   249,   258,   259,    17,
     230,   209,    70,    21,    22,    23,    24,    70,    21,    22,
      23,    24,   -49,   -49,   -49,   232,   263,     0,     0,     0,
      63,    64,    25,     0,     0,    65,     0,     0,     0,    66,
       0,     0,     0,     0,    67,    68,    69
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
};

static const yytype_int16 yycheck[] =
{
<<<<<<< HEAD
      14,     1,    22,   180,   184,    45,    25,    18,    19,    50,
      24,    60,    61,    62,    54,    50,    27,    20,    13,    14,
     197,    45,    32,    33,    27,    25,    50,    37,    38,    39,
      79,    20,    27,    28,    29,    30,    31,    51,    52,    23,
      24,    49,    22,    27,    26,    53,   223,    42,    97,    63,
      51,    51,    71,    54,    50,    50,    49,   237,    51,    26,
      80,    81,    27,    45,    46,    47,    34,    35,    36,    53,
      48,    71,    54,    45,   251,    13,    14,   254,    92,    48,
      94,   258,    40,    41,   261,    45,   263,   136,   137,    27,
      28,    29,    30,    31,    86,    87,    88,    43,    44,   113,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    50,    89,    90,    91,    48,    49,     0,    52,
     134,     3,     4,     5,     6,     7,     8,     9,    32,    33,
      12,    13,    14,    27,    16,    17,    26,   151,    27,    21,
      82,    83,    54,    25,    51,    27,    28,    29,    30,    31,
      84,    85,    27,   167,   168,    45,    46,    47,    27,    45,
      42,    27,   162,    22,    54,    45,    48,   181,    50,    45,
      52,    50,   186,    54,     3,     4,     5,     6,     7,     3,
       4,     5,     6,     7,    55,     9,    27,    50,    12,    13,
      14,    40,    16,    17,    48,    43,    48,   211,    27,    27,
      31,    25,   216,    27,    28,    29,    30,    31,    54,    52,
      28,    28,    50,   227,    48,    51,    48,    50,    42,    31,
      55,   235,    27,    50,    48,    51,    50,    48,    52,    53,
      55,   245,    55,    27,    49,    51,   250,     3,     4,     5,
       6,     7,    52,     9,    51,    48,    12,    13,    14,    55,
      16,    17,    48,    54,    51,    48,    48,    27,    48,    25,
      51,    27,    28,    29,    30,    31,    51,     3,     4,     5,
       6,     7,    18,    19,    20,    49,    42,    13,    14,    28,
      26,    27,    48,    48,    50,    10,    52,    53,    48,    25,
      48,    27,    28,    29,    30,    31,    50,    50,    55,    45,
      46,    47,    11,    13,    14,    48,    42,    48,    54,    51,
      13,    14,    51,    51,    50,    25,    26,    27,    28,    29,
      30,    31,    25,    51,    27,    28,    29,    30,    31,   216,
       1,   198,    42,    -1,    -1,    45,    46,    47,   266,    42,
      50,    13,    14,    15,    54,    48,    -1,    50,    13,    14,
      15,    -1,    -1,    25,    -1,    27,    28,    29,    30,    31,
      25,    -1,    27,    28,    29,    30,    31,    -1,    -1,    -1,
      42,    -1,    13,    14,    -1,    -1,    -1,    42,    50,    -1,
      -1,    -1,    -1,    -1,    25,    50,    27,    28,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50
=======
      14,    96,     1,    17,    25,    18,    43,    44,    67,    68,
      69,     3,     4,     5,     6,     7,    27,    20,   223,    18,
      79,    13,    14,     3,     4,     5,     6,     7,    28,    40,
      27,    25,    35,    25,    34,    27,    27,    51,    52,    18,
      19,   100,    27,    35,    36,    37,    38,    39,    61,   254,
      64,    65,    51,    26,    27,    35,    35,    27,    31,    80,
      81,    31,    61,    55,    31,    26,    27,    40,    41,    42,
      31,    30,    35,    40,    35,    34,    45,    46,    92,    40,
      41,    42,   177,    26,   143,   144,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    47,    48,
      49,    50,    51,    28,     0,   119,    31,     3,     4,     5,
       6,     7,     8,     9,    33,    34,    12,    13,    14,    33,
      16,    17,    20,   218,    40,    21,    33,   141,    28,    25,
      35,    27,   146,    29,    52,    53,    54,    33,    35,    35,
      36,    37,    38,    39,    35,   159,   160,    40,   243,    50,
      51,   246,    26,    27,    82,    83,   170,    31,    35,    55,
      84,    85,    86,   258,   259,    40,    40,    41,    42,    89,
      90,    91,    87,    88,    35,    35,   190,    29,    33,    29,
      28,    40,     3,     4,     5,     6,     7,    40,     9,    34,
     189,    12,    13,    14,    32,    16,    17,    22,    27,   213,
      31,    35,   216,    44,    25,    40,    27,   221,    29,    30,
      46,    33,    33,    36,    35,    36,    37,    38,    39,     3,
       4,     5,     6,     7,    35,     9,    29,    37,    12,    13,
      14,    37,    16,    17,    55,    27,   250,    33,    27,    32,
      28,    25,    27,    27,    35,    29,    30,    28,    28,    33,
      32,    35,    36,    37,    38,    39,    13,    14,    33,    32,
      36,    13,    14,    15,    33,    23,    24,    33,    25,    33,
      27,    55,    30,    25,    28,    27,    33,    35,    35,    36,
      37,    38,    39,    35,    36,    37,    38,    39,    13,    14,
      15,    31,    35,    13,    14,    28,    34,    33,    55,    27,
      25,    28,    27,    55,    33,    25,    37,    27,    10,    35,
      35,    36,    37,    38,    39,    35,    36,    37,    38,    39,
      13,    14,    33,    33,    32,    13,    14,    27,    11,    28,
      55,    27,    33,    28,    27,    55,    33,    28,    28,    27,
     216,   190,    35,    36,    37,    38,    39,    35,    36,    37,
      38,    39,    18,    19,    20,   219,   262,    -1,    -1,    -1,
      26,    27,    55,    -1,    -1,    31,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    42
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    57,     0,     3,     4,     5,     6,     7,     8,     9,
<<<<<<< HEAD
      12,    13,    14,    16,    17,    21,    25,    27,    28,    29,
      30,    31,    42,    48,    50,    52,    58,    59,    66,    69,
      70,    72,    74,    75,    78,    79,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,   104,   113,
      50,    50,    50,    27,    48,    85,    27,    26,    26,    27,
      45,    46,    47,    54,    27,    86,    88,    85,    53,    66,
      69,    71,    74,    18,    19,    27,    73,    20,    48,    45,
      40,    41,    43,    44,    32,    33,    37,    38,    39,    34,
      35,    36,    95,    27,    74,    85,    85,    45,    48,    52,
      27,    27,    45,    15,    87,    87,    87,    85,    51,    53,
      69,    27,    27,    45,    48,    49,    27,    87,    88,    88,
      89,    89,    92,    92,    91,    91,    91,    93,    93,    93,
      85,    85,   105,    51,    54,    60,    45,    45,    22,    50,
      55,    45,    54,    54,    15,    27,    85,    27,    50,    96,
     109,    48,    48,    85,    23,    24,    27,    53,    61,    62,
      63,    64,    65,    87,    87,    27,    31,    54,    52,    76,
      28,    28,    50,    26,    48,    27,    74,    80,    81,    82,
      51,    48,    85,    55,    50,    74,    50,    51,    85,    77,
      85,    48,    55,    55,    31,    27,    27,    51,    49,    70,
      85,   106,    51,    80,    20,    27,    67,    68,    85,    48,
      55,    49,    53,    48,    54,    51,    50,    70,    82,    97,
     110,    48,    48,    51,    27,    48,    51,    49,    85,    28,
      48,    68,    10,    98,    48,   107,    70,    50,    48,    85,
      55,    51,    50,    11,   102,   111,    85,    80,    48,    48,
      99,   103,    85,   108,    51,    85,    70,   112,    51,    70,
     100,    51,    70,    51,    70,    70,   101,    98
=======
      12,    13,    14,    16,    17,    21,    25,    27,    29,    33,
      35,    36,    37,    38,    39,    55,    58,    59,    60,    70,
      73,    75,    77,    78,    81,    83,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,   107,   114,
      27,    27,    27,    33,    35,    89,    35,    26,    89,    30,
      59,    74,    77,    26,    27,    31,    35,    40,    41,    42,
      35,    90,    92,    18,    19,    35,    76,    20,    33,    40,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    99,    35,    77,    89,   108,   112,   113,    89,
      40,    33,    61,    35,    28,    30,    59,    35,    71,    72,
      89,    89,    40,    15,    91,    91,    91,    35,    35,    40,
      33,    34,    35,    91,    92,    92,    93,    93,    95,    95,
      95,    96,    96,    97,    97,    97,    89,    35,    73,    33,
      28,    31,    29,    40,    40,    28,    34,    32,    22,    27,
      31,    40,    31,    15,    35,    89,    35,    82,   100,    40,
     109,    33,    89,    62,    91,    91,    89,    35,    36,    37,
      29,    79,    37,    27,    26,    33,    27,    28,    89,    89,
      32,    23,    24,    30,    35,    63,    64,    66,    68,    69,
      27,    28,    32,    80,    89,    33,    32,    36,    35,    35,
      77,    84,    85,    86,    73,    33,    28,    65,    77,    71,
      33,    33,    30,    34,    31,    28,    27,    35,    28,    34,
     101,   110,    33,    27,    20,    35,    28,    89,    37,    33,
      72,    73,    86,    10,   102,    89,    84,    35,    33,    33,
      32,    28,    27,    11,   106,   111,    28,    67,    33,    33,
     103,    73,    28,    73,    27,    89,    84,   104,    28,    28,
      73,    73,   105,   102
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
<<<<<<< HEAD
       0,    56,    57,    57,    58,    58,    58,    58,    59,    60,
      60,    61,    61,    61,    62,    63,    64,    65,    65,    66,
      66,    67,    67,    68,    68,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    70,    70,    71,    71,    72,    72,
      73,    73,    74,    74,    74,    74,    74,    74,    75,    75,
      75,    76,    77,    77,    78,    79,    79,    80,    80,    81,
      81,    82,    83,    83,    84,    84,    85,    86,    86,    87,
      87,    87,    87,    87,    87,    87,    88,    88,    88,    88,
      89,    89,    89,    90,    90,    90,    90,    91,    91,    91,
      92,    92,    92,    92,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    95,    96,    97,    94,    99,   100,   101,
      98,    98,   103,   102,   102,   105,   106,   107,   108,   104,
     109,   110,   111,   112,   104,   113,   113,   113,   113
=======
       0,    56,    57,    57,    58,    58,    58,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    61,    60,    62,    62,
      63,    63,    63,    65,    64,    67,    66,    68,    69,    69,
      70,    70,    71,    71,    72,    72,    73,    73,    74,    74,
      75,    75,    76,    76,    77,    77,    77,    77,    77,    77,
      78,    78,    78,    79,    80,    80,    82,    81,    83,    83,
      84,    84,    85,    85,    86,    87,    87,    88,    88,    89,
      90,    91,    91,    91,    91,    91,    91,    91,    92,    92,
      92,    92,    93,    93,    93,    94,    94,    94,    94,    95,
      95,    95,    96,    96,    96,    96,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    99,   100,   101,    98,
     103,   104,   105,   102,   102,   106,   106,   107,   109,   110,
     111,   108,   112,   112,   112,   113,   113,   114,   114,   114,
     114
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
<<<<<<< HEAD
       0,     2,     2,     0,     1,     1,     1,     1,     5,     2,
       0,     1,     1,     1,     5,     8,     4,     1,     1,     9,
      10,     1,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     2,     1,     3,     5,
       3,     1,     1,     1,     1,     1,     1,     1,     6,     7,
      10,     3,     3,     1,     7,     1,     1,     1,     0,     3,
       1,     2,     3,     2,     2,     1,     1,     4,     7,     3,
       3,     3,     3,     5,     5,     1,     3,     3,     2,     1,
       3,     3,     1,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     0,     0,    10,     0,     0,     0,
       9,     0,     0,     3,     0,     0,     0,     0,     0,    13,
       0,     0,     0,     0,    14,     7,     8,     5,     8
=======
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     6,     2,     0,
       1,     1,     1,     0,     6,     0,     9,     4,     1,     1,
       9,    10,     1,     0,     3,     1,     3,     2,     2,     1,
       3,     5,     3,     1,     1,     1,     1,     1,     1,     1,
       6,     7,    10,     3,     3,     1,     0,     8,     1,     1,
       1,     0,     3,     1,     2,     3,     2,     2,     1,     1,
       4,     3,     3,     3,     3,     5,     5,     1,     3,     3,
       2,     1,     3,     3,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     0,     0,    10,
       0,     0,     0,     9,     0,     2,     0,     4,     0,     0,
       0,     9,     1,     1,     0,     2,     4,     7,     8,     5,
       8
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
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
<<<<<<< HEAD
  case 38: /* var_decl: type id_list SEMICOLON  */
#line 202 "parser.y"
        {
                //no need to emit anything here and later stage add code for symbol table here
        }
#line 1616 "y.tab.c"
    break;

  case 39: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 206 "parser.y"
        {
                //code for symbol table comes here
                emit("=",(yyvsp[-1].sval),"",(yyvsp[-3].sval));
        }
#line 1625 "y.tab.c"
    break;

  case 67: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET  */
#line 280 "parser.y"
        {
		char* t1 = genVar();
		emit("*",(yyvsp[-1].sval),"type.width",t1);
		
		char* t2 = genVar();
		emit("[]",(yyvsp[-3].sval),t1,t2);
		
		(yyval.sval) = t2;
	}
#line 1639 "y.tab.c"
    break;

  case 68: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET  */
#line 290 "parser.y"
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
#line 1659 "y.tab.c"
    break;

  case 69: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 310 "parser.y"
        {
                emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval));
                (yyval.sval) = strdup((yyvsp[-2].sval));
        }
#line 1668 "y.tab.c"
    break;

  case 70: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 315 "parser.y"
        {
                char* temp = genVar();
                emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                emit("=", temp, "", (yyvsp[-2].sval));
                (yyval.sval) = temp;
        }
#line 1679 "y.tab.c"
    break;

  case 71: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 323 "parser.y"
        {
                char* temp = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                emit("=", temp, "", (yyvsp[-2].sval));
                (yyval.sval) = temp;
        }
#line 1690 "y.tab.c"
    break;

  case 73: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 331 "parser.y"
        {
                emit("", (yyvsp[0].sval), "", (yyvsp[-2].sval));
        }
#line 1698 "y.tab.c"
    break;

  case 74: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 335 "parser.y"
        {
                emit("", (yyvsp[0].sval), "", (yyvsp[-2].sval));
        }
#line 1706 "y.tab.c"
    break;

  case 75: /* assignment: logic_expr  */
#line 339 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1714 "y.tab.c"
    break;

  case 76: /* logic_expr: logic_expr OR logic_expr  */
#line 346 "parser.y"
        {
                char* temp = genVar();
                emit("||",(yyvsp[-2].sval),(yyvsp[0].sval),temp); //temp = $1 || $2
                (yyval.sval) = temp;
        }
#line 1724 "y.tab.c"
    break;

  case 77: /* logic_expr: logic_expr AND logic_expr  */
#line 352 "parser.y"
        {
                char* temp = genVar();
                emit("&&",(yyvsp[-2].sval),(yyvsp[0].sval),temp); //temp = $1 && $2
                (yyval.sval) = temp;
        }
#line 1734 "y.tab.c"
    break;

  case 78: /* logic_expr: NOT logic_expr  */
#line 358 "parser.y"
        {
                char* temp = genVar();
                emit("!",(yyvsp[0].sval),"",temp); //temp = !$2
                (yyval.sval) = temp;
        }
#line 1744 "y.tab.c"
    break;

  case 79: /* logic_expr: bitwise_expr  */
#line 364 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1752 "y.tab.c"
    break;

  case 80: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 371 "parser.y"
        {
                char* temp = genVar();
                emit("&", (yyvsp[-2].sval), (yyvsp[0].sval) , temp); //temp = $1 & $3
                (yyval.sval) = temp;
        }
#line 1762 "y.tab.c"
    break;

  case 81: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 377 "parser.y"
        {
                char* temp = genVar();
                emit("|",(yyvsp[-2].sval),(yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1772 "y.tab.c"
    break;

  case 82: /* bitwise_expr: rel_expr  */
#line 383 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1780 "y.tab.c"
    break;

  case 83: /* rel_expr: arith_expr GT arith_expr  */
#line 389 "parser.y"
        {
                char* temp = genVar();
                emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), temp); //temp = $1 > $3
                (yyval.sval) = temp;
        }
#line 1790 "y.tab.c"
    break;

  case 84: /* rel_expr: arith_expr LT arith_expr  */
#line 395 "parser.y"
        {
                char* temp = genVar();
                emit("<",(yyvsp[-2].sval) , (yyvsp[0].sval), temp); //temp = $1 < $3
                (yyval.sval) = temp;
        }
#line 1800 "y.tab.c"
    break;

  case 85: /* rel_expr: arith_expr EQ arith_expr  */
#line 401 "parser.y"
        {
                char* temp = genVar();
                emit("==",(yyvsp[-2].sval), (yyvsp[0].sval), temp); //temp = $1 == $3
                (yyval.sval) = temp;
        }
#line 1810 "y.tab.c"
    break;

  case 86: /* rel_expr: arith_expr  */
#line 407 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1818 "y.tab.c"
    break;

  case 87: /* arith_expr: arith_expr PLUS term  */
#line 414 "parser.y"
        {
                char* temp = genVar();
                emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1828 "y.tab.c"
    break;

  case 88: /* arith_expr: arith_expr MINUS term  */
#line 420 "parser.y"
        {
                char* temp = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1838 "y.tab.c"
    break;

  case 89: /* arith_expr: term  */
#line 426 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1846 "y.tab.c"
    break;

  case 90: /* term: term MUL factor  */
#line 433 "parser.y"
        {
                char* temp = genVar();
                emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1856 "y.tab.c"
    break;

  case 91: /* term: term DIV factor  */
#line 439 "parser.y"
        {
                char* temp = genVar();
                emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1866 "y.tab.c"
    break;

  case 92: /* term: term MOD factor  */
#line 445 "parser.y"
        {
                char* temp = genVar();
                emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1876 "y.tab.c"
    break;

  case 93: /* term: factor  */
#line 451 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1884 "y.tab.c"
    break;

  case 94: /* factor: IDENTIFIER  */
#line 458 "parser.y"
        {
                (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 1892 "y.tab.c"
    break;

  case 96: /* factor: INT_LITERAL  */
#line 463 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", (yyvsp[0].ival));
                (yyval.sval) = strdup(buffer);
        }
#line 1902 "y.tab.c"
    break;

  case 97: /* factor: FLOAT_LITERAL  */
#line 469 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%f", (yyvsp[0].fval));
                (yyval.sval) = strdup(buffer);
        }
#line 1912 "y.tab.c"
    break;

  case 98: /* factor: CHAR_LITERAL  */
#line 475 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "'%c'", (yyvsp[0].cval));
                (yyval.sval) = strdup(buffer);
        }
#line 1922 "y.tab.c"
    break;

  case 99: /* factor: STRING_LITERAL  */
#line 481 "parser.y"
        {
                (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 1930 "y.tab.c"
    break;

  case 100: /* factor: TRUE  */
#line 485 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", 1);
                (yyval.sval) = strdup(buffer);
        }
#line 1940 "y.tab.c"
    break;

  case 101: /* factor: FALSE  */
#line 491 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", 0);
                (yyval.sval) = strdup(buffer);
        }
#line 1950 "y.tab.c"
    break;

  case 102: /* factor: LPAREN expression RPAREN  */
#line 497 "parser.y"
        {
                (yyval.sval) = (yyvsp[-1].sval);
        }
#line 1958 "y.tab.c"
    break;

  case 103: /* $@1: %empty  */
#line 504 "parser.y"
        {
                char* falseLabel = getLabel();
                char* Lend = getLabel();
                pushIfLabels(falseLabel, Lend);
        }
#line 1968 "y.tab.c"
    break;

  case 104: /* $@2: %empty  */
#line 510 "parser.y"
        {
		emit("ifFalse",(yyvsp[0].sval), "",topFalse());
	}
#line 1976 "y.tab.c"
    break;

  case 105: /* $@3: %empty  */
#line 515 "parser.y"
        {
		emit("goto", "", "", topEnd());
		emit("label", "", "", topFalse());
	}
#line 1985 "y.tab.c"
    break;

  case 106: /* if_stmt: IF LPAREN $@1 expression $@2 RPAREN block $@3 elif_list else_opt  */
#line 521 "parser.y"
        {
                emit("label", "", "", topEnd());
                popIfLabels();
        }
#line 1994 "y.tab.c"
    break;

  case 107: /* $@4: %empty  */
#line 529 "parser.y"
        {
		char* prevFalseLabel = topFalse();
		char* nextFalseLabel = getLabel();
		
		//emit("goto", "", "", topEnd());
		//emit("label", "", "", prevFalseLabel);
		falseStack[topPtr] = nextFalseLabel;
	}
#line 2007 "y.tab.c"
    break;

  case 108: /* $@5: %empty  */
#line 538 "parser.y"
        {
		emit("ifFalse",(yyvsp[0].sval),"",topFalse());
	}
#line 2015 "y.tab.c"
    break;

  case 109: /* $@6: %empty  */
#line 543 "parser.y"
        {
		emit("goto", "", "", topEnd());
	}
#line 2023 "y.tab.c"
    break;

  case 112: /* $@7: %empty  */
#line 552 "parser.y"
=======
  case 16: /* $@1: %empty  */
#line 107 "parser.y"
      {
        emit("entity",(yyvsp[0].sval),"","");
      }
#line 1597 "y.tab.c"
    break;

  case 17: /* entity_decl: ENTITY IDENTIFIER $@1 LBRACE entity_body RBRACE  */
#line 111 "parser.y"
      {
        emit("end_entity",(yyvsp[-4].sval),"","");
      }
#line 1605 "y.tab.c"
    break;

  case 23: /* $@2: %empty  */
#line 129 "parser.y"
      {
       emit("constr",(yyvsp[0].sval),"","");
      }
#line 1613 "y.tab.c"
    break;

  case 24: /* constructor_decl: IDENTIFIER $@2 LPAREN param_list_opt RPAREN block  */
#line 133 "parser.y"
       {
        emit("end_constr",(yyvsp[-5].sval),"","");
       }
#line 1621 "y.tab.c"
    break;

  case 25: /* $@3: %empty  */
#line 140 "parser.y"
      {
        emit("method",(yyvsp[0].sval),"","");
      }
#line 1629 "y.tab.c"
    break;

  case 26: /* method_decl: access_modifier type FUNC IDENTIFIER $@3 LPAREN param_list_opt RPAREN block  */
#line 144 "parser.y"
      {
        emit("end_method",(yyvsp[-5].sval),"","");
      }
#line 1637 "y.tab.c"
    break;

  case 30: /* object_decl: IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 160 "parser.y"
       {
         emit("new",(yyvsp[-4].sval),"",(yyvsp[-7].sval));
         emit("call_constr",(yyvsp[-4].sval),"",(yyvsp[-7].sval));
       }
#line 1646 "y.tab.c"
    break;

  case 31: /* object_decl: type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list RPAREN SEMICOLON  */
#line 165 "parser.y"
       {
        char* t=genVar();
        emit("push_ptr",(yyvsp[-6].sval),"","");
        emit("call_method",(yyvsp[-4].sval),"",t);
        emit("=",t,"",(yyvsp[-8].sval));
       }
#line 1657 "y.tab.c"
    break;

  case 34: /* arg_list: arg_list COMMA expression  */
#line 180 "parser.y"
      {
       emit("arg",(yyvsp[0].sval),"","");
      }
#line 1665 "y.tab.c"
    break;

  case 35: /* arg_list: expression  */
#line 185 "parser.y"
      {
       emit("arg",(yyvsp[0].sval),"","");
      }
#line 1673 "y.tab.c"
    break;

  case 41: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 203 "parser.y"
        { emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval)); }
#line 1679 "y.tab.c"
    break;

  case 56: /* $@4: %empty  */
#line 232 "parser.y"
       {
         emit ("func",(yyvsp[0].sval),"",""); 
       }
#line 1687 "y.tab.c"
    break;

  case 57: /* function_decl: func_type FUNC IDENTIFIER $@4 LPAREN param_list_opt RPAREN block  */
#line 236 "parser.y"
        {
         emit("endfunc","","",""); 
        }
#line 1695 "y.tab.c"
    break;

  case 64: /* param: type IDENTIFIER  */
#line 258 "parser.y"
      {
        emit("param",(yyvsp[0].sval),"","");
      }
#line 1703 "y.tab.c"
    break;

  case 65: /* return_stmt: RETURN expression SEMICOLON  */
#line 265 "parser.y"
      { 
       emit("return",(yyvsp[-1].sval),"","");
      }
#line 1711 "y.tab.c"
    break;

  case 66: /* return_stmt: RETURN SEMICOLON  */
#line 269 "parser.y"
      {
        emit("return","","","");
      }
#line 1719 "y.tab.c"
    break;

  case 69: /* expression: assignment  */
#line 280 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1725 "y.tab.c"
    break;

  case 70: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET  */
#line 284 "parser.y"
                                              { (yyval.sval) = strdup((yyvsp[-3].sval)); }
#line 1731 "y.tab.c"
    break;

  case 71: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 289 "parser.y"
        { emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval)); (yyval.sval) = strdup((yyvsp[-2].sval)); }
#line 1737 "y.tab.c"
    break;

  case 72: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 291 "parser.y"
        { char* t = genVar(); emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t); emit("=", t, "", (yyvsp[-2].sval)); (yyval.sval) = t; }
#line 1743 "y.tab.c"
    break;

  case 73: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 293 "parser.y"
        { char* t = genVar(); emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t); emit("=", t, "", (yyvsp[-2].sval)); (yyval.sval) = t; }
#line 1749 "y.tab.c"
    break;

  case 74: /* assignment: indexed_id ASSIGN assignment  */
#line 294 "parser.y"
                                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1755 "y.tab.c"
    break;

  case 75: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 295 "parser.y"
                                            { emit("set_field", "this", (yyvsp[-2].sval), (yyvsp[0].sval)); (yyval.sval) = (yyvsp[0].sval); }
#line 1761 "y.tab.c"
    break;

  case 76: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 296 "parser.y"
                                                  { emit("set_field", (yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); (yyval.sval) = (yyvsp[0].sval); }
#line 1767 "y.tab.c"
    break;

  case 77: /* assignment: logic_expr  */
#line 297 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1773 "y.tab.c"
    break;

  case 78: /* logic_expr: logic_expr OR logic_expr  */
#line 301 "parser.y"
                               { char* t = genVar(); emit("||", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1779 "y.tab.c"
    break;

  case 79: /* logic_expr: logic_expr AND logic_expr  */
#line 302 "parser.y"
                                { char* t = genVar(); emit("&&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1785 "y.tab.c"
    break;

  case 80: /* logic_expr: NOT logic_expr  */
#line 303 "parser.y"
                     { char* t = genVar(); emit("!", (yyvsp[0].sval), "", t); (yyval.sval) = t; }
#line 1791 "y.tab.c"
    break;

  case 81: /* logic_expr: bitwise_expr  */
#line 304 "parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1797 "y.tab.c"
    break;

  case 82: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 308 "parser.y"
                                       { char* t = genVar(); emit("&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1803 "y.tab.c"
    break;

  case 83: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 309 "parser.y"
                                      { char* t = genVar(); emit("|", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1809 "y.tab.c"
    break;

  case 84: /* bitwise_expr: rel_expr  */
#line 310 "parser.y"
               { (yyval.sval) = (yyvsp[0].sval); }
#line 1815 "y.tab.c"
    break;

  case 85: /* rel_expr: arith_expr GT arith_expr  */
#line 314 "parser.y"
                               { char* t = genVar(); emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1821 "y.tab.c"
    break;

  case 86: /* rel_expr: arith_expr LT arith_expr  */
#line 315 "parser.y"
                               { char* t = genVar(); emit("<", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1827 "y.tab.c"
    break;

  case 87: /* rel_expr: arith_expr EQ arith_expr  */
#line 316 "parser.y"
                               { char* t = genVar(); emit("==", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1833 "y.tab.c"
    break;

  case 88: /* rel_expr: arith_expr  */
#line 317 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1839 "y.tab.c"
    break;

  case 89: /* arith_expr: arith_expr PLUS term  */
#line 321 "parser.y"
                           { char* t = genVar(); emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1845 "y.tab.c"
    break;

  case 90: /* arith_expr: arith_expr MINUS term  */
#line 322 "parser.y"
                            { char* t = genVar(); emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1851 "y.tab.c"
    break;

  case 91: /* arith_expr: term  */
#line 323 "parser.y"
           { (yyval.sval) = (yyvsp[0].sval); }
#line 1857 "y.tab.c"
    break;

  case 92: /* term: term MUL factor  */
#line 327 "parser.y"
                      { char* t = genVar(); emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1863 "y.tab.c"
    break;

  case 93: /* term: term DIV factor  */
#line 328 "parser.y"
                      { char* t = genVar(); emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1869 "y.tab.c"
    break;

  case 94: /* term: term MOD factor  */
#line 329 "parser.y"
                      { char* t = genVar(); emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1875 "y.tab.c"
    break;

  case 95: /* term: factor  */
#line 330 "parser.y"
             { (yyval.sval) = (yyvsp[0].sval); }
#line 1881 "y.tab.c"
    break;

  case 96: /* factor: IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 335 "parser.y"
      {
        char* t=genVar();
        emit("call",(yyvsp[-3].sval),"",t);
        (yyval.sval)=t;
      }
#line 1891 "y.tab.c"
    break;

  case 97: /* factor: IDENTIFIER  */
#line 340 "parser.y"
                 { (yyval.sval) = strdup((yyvsp[0].sval)); }
#line 1897 "y.tab.c"
    break;

  case 98: /* factor: indexed_id  */
#line 341 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1903 "y.tab.c"
    break;

  case 99: /* factor: INT_LITERAL  */
#line 342 "parser.y"
                  { char b[20]; sprintf(b, "%d", (yyvsp[0].ival)); (yyval.sval) = strdup(b); }
#line 1909 "y.tab.c"
    break;

  case 100: /* factor: FLOAT_LITERAL  */
#line 343 "parser.y"
                    { char b[20]; sprintf(b, "%f", (yyvsp[0].fval)); (yyval.sval) = strdup(b); }
#line 1915 "y.tab.c"
    break;

  case 101: /* factor: CHAR_LITERAL  */
#line 344 "parser.y"
                   { char b[20]; sprintf(b, "'%c'", (yyvsp[0].cval)); (yyval.sval) = strdup(b); }
#line 1921 "y.tab.c"
    break;

  case 102: /* factor: STRING_LITERAL  */
#line 345 "parser.y"
                     { (yyval.sval) = strdup((yyvsp[0].sval)); }
#line 1927 "y.tab.c"
    break;

  case 103: /* factor: TRUE  */
#line 346 "parser.y"
           { (yyval.sval) = strdup("1"); }
#line 1933 "y.tab.c"
    break;

  case 104: /* factor: FALSE  */
#line 347 "parser.y"
            { (yyval.sval) = strdup("0"); }
#line 1939 "y.tab.c"
    break;

  case 105: /* factor: LPAREN expression RPAREN  */
#line 348 "parser.y"
                               { (yyval.sval) = (yyvsp[-1].sval); }
#line 1945 "y.tab.c"
    break;

  case 106: /* $@5: %empty  */
#line 353 "parser.y"
        { char* f = getLabel(); char* e = getLabel(); pushIfLabels(f, e); }
#line 1951 "y.tab.c"
    break;

  case 107: /* $@6: %empty  */
#line 355 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 1957 "y.tab.c"
    break;

  case 108: /* $@7: %empty  */
#line 357 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 1963 "y.tab.c"
    break;

  case 109: /* if_stmt: IF LPAREN $@5 expression $@6 RPAREN block $@7 elif_list else_opt  */
#line 359 "parser.y"
        { emit("label", "", "", topEnd()); popIfLabels(); }
#line 1969 "y.tab.c"
    break;

  case 110: /* $@8: %empty  */
#line 364 "parser.y"
        { char* n = getLabel(); free(falseStack[topPtr]); falseStack[topPtr] = strdup(n); }
#line 1975 "y.tab.c"
    break;

  case 111: /* $@9: %empty  */
#line 366 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 1981 "y.tab.c"
    break;

  case 112: /* $@10: %empty  */
#line 368 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 1987 "y.tab.c"
    break;

  case 117: /* for_stmt: FOR LPAREN for_header block  */
#line 380 "parser.y"
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936
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
<<<<<<< HEAD
#line 2031 "y.tab.c"
    break;

  case 115: /* $@8: %empty  */
#line 563 "parser.y"
        {
                char* Lbegin = getLabel();
                char* Lend = getLabel();

                pushIfLabels(Lbegin,Lend);

                emit("label","","",Lbegin);
        }
#line 2044 "y.tab.c"
    break;

  case 116: /* $@9: %empty  */
#line 573 "parser.y"
        {
                emit("ifFalse",(yyvsp[0].sval),"",topEnd());
        }
#line 2052 "y.tab.c"
    break;

  case 117: /* $@10: %empty  */
#line 577 "parser.y"
        {
                forDepth++;
                forIncIdx[forDepth] = 0;
                inForIncrement[forDepth]=1;
        }
#line 2062 "y.tab.c"
    break;

  case 118: /* $@11: %empty  */
#line 583 "parser.y"
        {
                inForIncrement[forDepth]=0;
        }
#line 2070 "y.tab.c"
    break;

  case 119: /* for_stmt: FOR LPAREN expression $@8 SEMICOLON expression $@9 SEMICOLON $@10 expression $@11 RPAREN block  */
#line 587 "parser.y"
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
#line 2088 "y.tab.c"
    break;

  case 120: /* $@12: %empty  */
#line 602 "parser.y"
        {
                char* Lbegin = getLabel();
                char* Lend = getLabel();

                pushIfLabels(Lbegin,Lend);

                emit("label","","",Lbegin);
        }
#line 2101 "y.tab.c"
    break;

  case 121: /* $@13: %empty  */
#line 612 "parser.y"
        {
                emit("ifFalse",(yyvsp[0].sval),"",topEnd());
        }
#line 2109 "y.tab.c"
    break;

  case 122: /* $@14: %empty  */
#line 616 "parser.y"
        {
                forDepth++;
                forIncIdx[forDepth] = 0;
                inForIncrement[forDepth]=1;
        }
#line 2119 "y.tab.c"
    break;

  case 123: /* $@15: %empty  */
#line 622 "parser.y"
        {
                inForIncrement[forDepth]=0;
        }
#line 2127 "y.tab.c"
    break;

  case 124: /* for_stmt: FOR LPAREN type expression $@12 SEMICOLON expression $@13 SEMICOLON $@14 expression $@15 RPAREN block  */
#line 626 "parser.y"
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
#line 2145 "y.tab.c"
    break;


#line 2149 "y.tab.c"
=======
#line 2002 "y.tab.c"
    break;

  case 118: /* $@11: %empty  */
#line 394 "parser.y"
        { char* b = getLabel(); char* e = getLabel(); pushIfLabels(b, e); emit("label", "", "", b); }
#line 2008 "y.tab.c"
    break;

  case 119: /* $@12: %empty  */
#line 396 "parser.y"
        { emit("ifFalse", (yyvsp[-1].sval), "", topEnd()); forDepth++; forIncIdx[forDepth] = 0; inForIncrement[forDepth] = 1; }
#line 2014 "y.tab.c"
    break;

  case 120: /* $@13: %empty  */
#line 398 "parser.y"
        { inForIncrement[forDepth] = 0; }
#line 2020 "y.tab.c"
    break;

  case 126: /* var_decl_no_semi: type IDENTIFIER ASSIGN expression  */
#line 410 "parser.y"
                                        { emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval)); }
#line 2026 "y.tab.c"
    break;

  case 127: /* io_stmt: IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 415 "parser.y"
      {
        emit("in","","",(yyvsp[-6].sval));
      }
#line 2034 "y.tab.c"
    break;

  case 128: /* io_stmt: type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 419 "parser.y"
      {
        emit("in","","",(yyvsp[-6].sval));
      }
#line 2042 "y.tab.c"
    break;

  case 129: /* io_stmt: SHOW LPAREN expression RPAREN SEMICOLON  */
#line 423 "parser.y"
      {
       emit("out",(yyvsp[-2].sval),"","");
      }
#line 2050 "y.tab.c"
    break;

  case 130: /* io_stmt: SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON  */
#line 427 "parser.y"
     {
       char* t=genVar();
       emit("[]",(yyvsp[-5].sval),(yyvsp[-3].sval),t);
       emit("out",t,"","");
     }
#line 2060 "y.tab.c"
    break;


#line 2064 "y.tab.c"
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936

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

<<<<<<< HEAD
#line 648 "parser.y"
=======
#line 434 "parser.y"
>>>>>>> 1137fb808d213f8ad8f7057a42454c6cdddec936


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
