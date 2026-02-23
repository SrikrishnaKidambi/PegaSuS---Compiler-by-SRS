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
#define YYLAST   375

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  267

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
       0,    83,    83,    84,    88,    89,    90,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   107,   106,   117,   118,
     122,   123,   124,   129,   128,   140,   139,   150,   154,   155,
     159,   164,   174,   175,   179,   184,   191,   192,   196,   197,
     201,   202,   207,   208,   212,   212,   212,   212,   212,   212,
     216,   217,   218,   222,   226,   227,   232,   231,   242,   243,
     247,   248,   252,   253,   257,   264,   268,   275,   276,   280,
     284,   294,   314,   316,   318,   320,   321,   322,   323,   327,
     328,   329,   330,   334,   335,   336,   340,   341,   342,   343,
     347,   348,   349,   353,   354,   355,   356,   360,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   379,   381,   383,
     378,   390,   392,   394,   389,   396,   400,   401,   405,   420,
     422,   424,   419,   429,   430,   431,   435,   436,   440,   444,
     448,   452
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

#define YYPACT_NINF (-209)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-50)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -209,   104,  -209,  -209,  -209,  -209,  -209,  -209,  -209,     3,
       9,  -209,  -209,    15,   135,    37,    56,   284,   208,  -209,
     333,  -209,  -209,  -209,  -209,   311,  -209,  -209,  -209,  -209,
    -209,  -209,    21,  -209,  -209,    99,  -209,  -209,    89,    88,
    -209,   -37,    31,  -209,    51,   129,  -209,  -209,  -209,  -209,
    -209,     8,   284,  -209,    27,   101,  -209,   103,   108,  -209,
    -209,   245,    21,   109,   284,   284,   107,   252,   284,   284,
      30,  -209,  -209,   115,   117,   118,    81,   126,  -209,   284,
     311,   311,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   284,    39,   130,  -209,   140,   159,  -209,    75,
     284,  -209,   177,   167,  -209,  -209,  -209,   168,   181,   182,
    -209,   186,   197,   196,  -209,  -209,  -209,    33,   195,   279,
    -209,   192,  -209,  -209,   184,  -209,   183,  -209,   105,   105,
     105,   129,   129,  -209,  -209,  -209,  -209,   190,  -209,  -209,
     198,   284,  -209,   284,   284,  -209,   284,   201,   199,   200,
     203,   213,   216,   228,   153,   223,  -209,   233,   236,   284,
     284,  -209,   237,   100,  -209,  -209,  -209,   284,   241,   243,
     244,   284,   240,   253,   250,   260,  -209,    20,   140,  -209,
     263,   271,  -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,
      20,   269,   284,   270,   272,    41,  -209,  -209,   277,   274,
     -11,  -209,   275,   285,   278,  -209,  -209,  -209,   293,   300,
      -3,  -209,   301,  -209,  -209,  -209,   284,   291,   297,   284,
    -209,   140,    20,   321,   284,  -209,    20,   298,   299,   302,
    -209,   304,  -209,     0,  -209,  -209,   310,   329,  -209,   313,
    -209,  -209,  -209,   309,   312,  -209,   140,  -209,   315,   140,
     317,  -209,  -209,   284,  -209,  -209,  -209,    20,  -209,   326,
     327,   140,   140,  -209,  -209,   321,  -209
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,    44,    45,    46,    47,    48,    59,     0,
       0,   104,   105,     0,     0,     0,     0,     0,     0,    68,
      98,   103,   100,   101,   102,     0,     2,     4,     6,    14,
      15,     7,    58,     8,     5,     0,    13,     9,     0,    99,
      69,    78,    82,    85,    89,    92,    96,    10,    11,    12,
     107,   125,     0,    66,    98,     0,    16,     0,     0,    37,
      39,     0,     0,     0,    33,     0,     0,     0,     0,     0,
      98,    99,    81,     0,     0,    43,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,   124,     0,     0,   123,     0,
       0,    65,     0,     0,   106,    36,    38,     0,     0,    32,
      35,     0,     0,     0,    72,    73,    74,     0,     0,     0,
      40,     0,    56,    75,    79,    80,    84,    83,    88,    86,
      87,    90,    91,    93,    94,    95,   108,   126,   118,   119,
       0,     0,    19,     0,     0,    97,     0,    70,     0,     0,
       0,     0,     0,     0,    98,     0,    42,     0,     0,     0,
       0,   130,     0,     0,    76,    77,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    41,    61,     0,   127,
       0,     0,    28,    29,    17,    23,    18,    20,    21,    22,
       0,     0,    33,     0,     0,     0,    55,    50,     0,     0,
       0,    49,     0,     0,    60,    63,   109,   120,     0,     0,
       0,    71,     0,   128,    51,    53,     0,     0,     0,     0,
      64,     0,     0,   115,     0,   131,    61,     0,     0,     0,
      54,     0,   129,     0,    57,    62,     0,   117,   121,     0,
      25,    27,    30,     0,     0,   111,     0,   110,     0,     0,
       0,    52,    31,     0,   116,   122,    24,    61,   112,     0,
       0,     0,     0,    26,   113,   115,   114
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -209,  -209,  -209,   -13,  -209,  -209,  -209,  -209,  -209,  -209,
    -209,  -209,  -209,  -209,  -209,   164,   138,   -95,  -209,  -209,
    -209,     1,  -209,  -209,  -209,  -209,  -209,  -209,  -208,  -209,
     136,  -209,  -209,   -14,     6,   -59,   -21,    93,  -209,   112,
     116,   110,  -209,  -209,  -209,  -209,    96,  -209,  -209,  -209,
    -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    26,    27,    28,   102,   163,   186,   187,   209,
     188,   250,   189,   190,    29,   108,   109,    30,    61,    31,
      76,   202,    33,   172,   195,    34,   157,    35,   203,   204,
     205,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    92,   158,   223,   237,   253,   260,   265,
     247,    48,    96,   160,   224,   248,    97,    98,    49
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   138,    32,    58,    72,    60,    80,    81,   114,   115,
     116,     3,     4,     5,     6,     7,   219,   227,   239,    62,
     123,    11,    12,     3,     4,     5,     6,     7,   244,   144,
      50,    71,   228,    16,   146,    17,    51,    95,    99,    73,
      74,   114,    52,    93,    21,    22,    23,    24,   106,   259,
     110,   111,    94,    63,    64,   201,    75,    64,    65,   124,
     125,    65,    62,    25,   150,    63,    64,   100,    68,    69,
      65,   215,    56,   151,   -49,   216,    82,    83,   136,   100,
      68,    69,    57,   206,   164,   165,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    84,    85,
      86,    87,    88,   140,     2,   155,   141,     3,     4,     5,
       6,     7,     8,     9,   120,   121,    10,    11,    12,    77,
      13,    14,    78,   182,   183,    15,   234,   162,    79,    16,
     184,    17,   166,    18,   101,   185,   104,    19,   103,    20,
      21,    22,    23,    24,   107,   179,   180,   112,    11,    12,
     117,   254,   118,   191,   256,    87,    88,   196,   119,    25,
      16,   122,    17,    11,    12,   137,   263,   264,    53,    18,
      54,    21,    22,    23,    24,   126,   127,    17,   110,   175,
      64,    89,    90,    91,    65,    70,    21,    22,    23,    24,
      25,   210,   139,   100,    68,    69,   128,   129,   130,   133,
     134,   135,   230,   131,   132,   110,   142,   143,   144,   145,
     238,     3,     4,     5,     6,     7,   146,     9,   147,   148,
      10,    11,    12,   149,    13,    14,   152,   156,    81,    83,
     159,   161,   167,    16,   168,    17,   169,    18,    59,   258,
     170,    19,   171,    20,    21,    22,    23,    24,     3,     4,
       5,     6,     7,   173,     9,   174,   176,    10,    11,    12,
     177,    13,    14,    25,   178,    11,    12,   113,   192,   181,
      16,   193,    17,   197,    18,   105,   194,    16,    19,    17,
      20,    21,    22,    23,    24,   198,   199,    54,    21,    22,
      23,    24,    11,    12,   153,   200,   207,    11,    12,   208,
      25,   211,   218,   213,    16,   214,    17,    25,   217,    16,
     220,    17,   222,   221,   154,    21,    22,    23,    24,    54,
      21,    22,    23,    24,    11,    12,   225,   226,   231,   229,
     232,   236,   241,   240,    25,   242,   243,   245,    17,    25,
     246,   249,   251,   255,   257,   252,    70,    21,    22,    23,
      24,   -49,   -49,   -49,   261,   262,   212,   233,   235,    63,
      64,   266,     0,     0,    65,     0,    25,     0,    66,     0,
       0,     0,     0,    67,    68,    69
};

static const yytype_int16 yycheck[] =
{
      14,    96,     1,    17,    25,    18,    43,    44,    67,    68,
      69,     3,     4,     5,     6,     7,    27,    20,   226,    18,
      79,    13,    14,     3,     4,     5,     6,     7,    28,    40,
      27,    25,    35,    25,    34,    27,    27,    51,    52,    18,
      19,   100,    27,    35,    36,    37,    38,    39,    61,   257,
      64,    65,    51,    26,    27,    35,    35,    27,    31,    80,
      81,    31,    61,    55,    31,    26,    27,    40,    41,    42,
      31,    30,    35,    40,    35,    34,    45,    46,    92,    40,
      41,    42,    26,   178,   143,   144,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    47,    48,
      49,    50,    51,    28,     0,   119,    31,     3,     4,     5,
       6,     7,     8,     9,    33,    34,    12,    13,    14,    20,
      16,    17,    33,    23,    24,    21,   221,   141,    40,    25,
      30,    27,   146,    29,    33,    35,    28,    33,    35,    35,
      36,    37,    38,    39,    35,   159,   160,    40,    13,    14,
      35,   246,    35,   167,   249,    50,    51,   171,    40,    55,
      25,    35,    27,    13,    14,    35,   261,   262,    33,    29,
      35,    36,    37,    38,    39,    82,    83,    27,   192,    26,
      27,    52,    53,    54,    31,    35,    36,    37,    38,    39,
      55,   190,    33,    40,    41,    42,    84,    85,    86,    89,
      90,    91,   216,    87,    88,   219,    29,    40,    40,    28,
     224,     3,     4,     5,     6,     7,    34,     9,    32,    22,
      12,    13,    14,    27,    16,    17,    31,    35,    44,    46,
      40,    33,    31,    25,    35,    27,    36,    29,    30,   253,
      37,    33,    29,    35,    36,    37,    38,    39,     3,     4,
       5,     6,     7,    37,     9,    27,    33,    12,    13,    14,
      27,    16,    17,    55,    28,    13,    14,    15,    27,    32,
      25,    28,    27,    33,    29,    30,    32,    25,    33,    27,
      35,    36,    37,    38,    39,    32,    36,    35,    36,    37,
      38,    39,    13,    14,    15,    35,    33,    13,    14,    28,
      55,    32,    28,    33,    25,    33,    27,    55,    31,    25,
      35,    27,    34,    28,    35,    36,    37,    38,    39,    35,
      36,    37,    38,    39,    13,    14,    33,    27,    37,    28,
      33,    10,    33,    35,    55,    33,    32,    27,    27,    55,
      11,    28,    33,    28,    27,    33,    35,    36,    37,    38,
      39,    18,    19,    20,    28,    28,   192,   219,   222,    26,
      27,   265,    -1,    -1,    31,    -1,    55,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    42
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    57,     0,     3,     4,     5,     6,     7,     8,     9,
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
     109,    33,    89,    62,    91,    91,    89,    31,    35,    36,
      37,    29,    79,    37,    27,    26,    33,    27,    28,    89,
      89,    32,    23,    24,    30,    35,    63,    64,    66,    68,
      69,    89,    27,    28,    32,    80,    89,    33,    32,    36,
      35,    35,    77,    84,    85,    86,    73,    33,    28,    65,
      77,    32,    71,    33,    33,    30,    34,    31,    28,    27,
      35,    28,    34,   101,   110,    33,    27,    20,    35,    28,
      89,    37,    33,    72,    73,    86,    10,   102,    89,    84,
      35,    33,    33,    32,    28,    27,    11,   106,   111,    28,
      67,    33,    33,   103,    73,    28,    73,    27,    89,    84,
     104,    28,    28,    73,    73,   105,   102
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    57,    58,    58,    58,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    61,    60,    62,    62,
      63,    63,    63,    65,    64,    67,    66,    68,    69,    69,
      70,    70,    71,    71,    72,    72,    73,    73,    74,    74,
      75,    75,    76,    76,    77,    77,    77,    77,    77,    77,
      78,    78,    78,    79,    80,    80,    82,    81,    83,    83,
      84,    84,    85,    85,    86,    87,    87,    88,    88,    89,
      90,    90,    91,    91,    91,    91,    91,    91,    91,    92,
      92,    92,    92,    93,    93,    93,    94,    94,    94,    94,
      95,    95,    95,    96,    96,    96,    96,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    99,   100,   101,
      98,   103,   104,   105,   102,   102,   106,   106,   107,   109,
     110,   111,   108,   112,   112,   112,   113,   113,   114,   114,
     114,   114
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     6,     2,     0,
       1,     1,     1,     0,     6,     0,     9,     4,     1,     1,
       9,    10,     1,     0,     3,     1,     3,     2,     2,     1,
       3,     5,     3,     1,     1,     1,     1,     1,     1,     1,
       6,     7,    10,     3,     3,     1,     0,     8,     1,     1,
       1,     0,     3,     1,     2,     3,     2,     2,     1,     1,
       4,     7,     3,     3,     3,     3,     5,     5,     1,     3,
       3,     2,     1,     3,     3,     1,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     0,     0,
      10,     0,     0,     0,     9,     0,     2,     0,     4,     0,
       0,     0,     9,     1,     1,     0,     2,     4,     7,     8,
       5,     8
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
#line 285 "parser.y"
        {
		char* t1 = genVar();
		emit("*",(yyvsp[-1].sval),"type.width",t1);
		
		char* t2 = genVar();
		emit("[]",(yyvsp[-3].sval),t1,t2);
		
		(yyval.sval) = t2;
	}
#line 1739 "y.tab.c"
    break;

  case 71: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET  */
#line 295 "parser.y"
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
#line 1759 "y.tab.c"
    break;

  case 72: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 315 "parser.y"
        { emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval)); (yyval.sval) = strdup((yyvsp[-2].sval)); }
#line 1765 "y.tab.c"
    break;

  case 73: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 317 "parser.y"
        { char* t = genVar(); emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t); emit("=", t, "", (yyvsp[-2].sval)); (yyval.sval) = t; }
#line 1771 "y.tab.c"
    break;

  case 74: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 319 "parser.y"
        { char* t = genVar(); emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t); emit("=", t, "", (yyvsp[-2].sval)); (yyval.sval) = t; }
#line 1777 "y.tab.c"
    break;

  case 75: /* assignment: indexed_id ASSIGN assignment  */
#line 320 "parser.y"
                                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1783 "y.tab.c"
    break;

  case 76: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 321 "parser.y"
                                            { emit("set_field", "this", (yyvsp[-2].sval), (yyvsp[0].sval)); (yyval.sval) = (yyvsp[0].sval); }
#line 1789 "y.tab.c"
    break;

  case 77: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 322 "parser.y"
                                                  { emit("set_field", (yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); (yyval.sval) = (yyvsp[0].sval); }
#line 1795 "y.tab.c"
    break;

  case 78: /* assignment: logic_expr  */
#line 323 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1801 "y.tab.c"
    break;

  case 79: /* logic_expr: logic_expr OR logic_expr  */
#line 327 "parser.y"
                               { char* t = genVar(); emit("||", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1807 "y.tab.c"
    break;

  case 80: /* logic_expr: logic_expr AND logic_expr  */
#line 328 "parser.y"
                                { char* t = genVar(); emit("&&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1813 "y.tab.c"
    break;

  case 81: /* logic_expr: NOT logic_expr  */
#line 329 "parser.y"
                     { char* t = genVar(); emit("!", (yyvsp[0].sval), "", t); (yyval.sval) = t; }
#line 1819 "y.tab.c"
    break;

  case 82: /* logic_expr: bitwise_expr  */
#line 330 "parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1825 "y.tab.c"
    break;

  case 83: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 334 "parser.y"
                                       { char* t = genVar(); emit("&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1831 "y.tab.c"
    break;

  case 84: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 335 "parser.y"
                                      { char* t = genVar(); emit("|", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1837 "y.tab.c"
    break;

  case 85: /* bitwise_expr: rel_expr  */
#line 336 "parser.y"
               { (yyval.sval) = (yyvsp[0].sval); }
#line 1843 "y.tab.c"
    break;

  case 86: /* rel_expr: arith_expr GT arith_expr  */
#line 340 "parser.y"
                               { char* t = genVar(); emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1849 "y.tab.c"
    break;

  case 87: /* rel_expr: arith_expr LT arith_expr  */
#line 341 "parser.y"
                               { char* t = genVar(); emit("<", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1855 "y.tab.c"
    break;

  case 88: /* rel_expr: arith_expr EQ arith_expr  */
#line 342 "parser.y"
                               { char* t = genVar(); emit("==", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1861 "y.tab.c"
    break;

  case 89: /* rel_expr: arith_expr  */
#line 343 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1867 "y.tab.c"
    break;

  case 90: /* arith_expr: arith_expr PLUS term  */
#line 347 "parser.y"
                           { char* t = genVar(); emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1873 "y.tab.c"
    break;

  case 91: /* arith_expr: arith_expr MINUS term  */
#line 348 "parser.y"
                            { char* t = genVar(); emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1879 "y.tab.c"
    break;

  case 92: /* arith_expr: term  */
#line 349 "parser.y"
           { (yyval.sval) = (yyvsp[0].sval); }
#line 1885 "y.tab.c"
    break;

  case 93: /* term: term MUL factor  */
#line 353 "parser.y"
                      { char* t = genVar(); emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1891 "y.tab.c"
    break;

  case 94: /* term: term DIV factor  */
#line 354 "parser.y"
                      { char* t = genVar(); emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1897 "y.tab.c"
    break;

  case 95: /* term: term MOD factor  */
#line 355 "parser.y"
                      { char* t = genVar(); emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 1903 "y.tab.c"
    break;

  case 96: /* term: factor  */
#line 356 "parser.y"
             { (yyval.sval) = (yyvsp[0].sval); }
#line 1909 "y.tab.c"
    break;

  case 97: /* factor: IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 361 "parser.y"
      {
        char* t=genVar();
        emit("call",(yyvsp[-3].sval),"",t);
        (yyval.sval)=t;
      }
#line 1919 "y.tab.c"
    break;

  case 98: /* factor: IDENTIFIER  */
#line 366 "parser.y"
                 { (yyval.sval) = strdup((yyvsp[0].sval)); }
#line 1925 "y.tab.c"
    break;

  case 99: /* factor: indexed_id  */
#line 367 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 1931 "y.tab.c"
    break;

  case 100: /* factor: INT_LITERAL  */
#line 368 "parser.y"
                  { char b[20]; sprintf(b, "%d", (yyvsp[0].ival)); (yyval.sval) = strdup(b); }
#line 1937 "y.tab.c"
    break;

  case 101: /* factor: FLOAT_LITERAL  */
#line 369 "parser.y"
                    { char b[20]; sprintf(b, "%f", (yyvsp[0].fval)); (yyval.sval) = strdup(b); }
#line 1943 "y.tab.c"
    break;

  case 102: /* factor: CHAR_LITERAL  */
#line 370 "parser.y"
                   { char b[20]; sprintf(b, "'%c'", (yyvsp[0].cval)); (yyval.sval) = strdup(b); }
#line 1949 "y.tab.c"
    break;

  case 103: /* factor: STRING_LITERAL  */
#line 371 "parser.y"
                     { (yyval.sval) = strdup((yyvsp[0].sval)); }
#line 1955 "y.tab.c"
    break;

  case 104: /* factor: TRUE  */
#line 372 "parser.y"
           { (yyval.sval) = strdup("1"); }
#line 1961 "y.tab.c"
    break;

  case 105: /* factor: FALSE  */
#line 373 "parser.y"
            { (yyval.sval) = strdup("0"); }
#line 1967 "y.tab.c"
    break;

  case 106: /* factor: LPAREN expression RPAREN  */
#line 374 "parser.y"
                               { (yyval.sval) = (yyvsp[-1].sval); }
#line 1973 "y.tab.c"
    break;

  case 107: /* $@5: %empty  */
#line 379 "parser.y"
        { char* f = getLabel(); char* e = getLabel(); pushIfLabels(f, e); }
#line 1979 "y.tab.c"
    break;

  case 108: /* $@6: %empty  */
#line 381 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 1985 "y.tab.c"
    break;

  case 109: /* $@7: %empty  */
#line 383 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 1991 "y.tab.c"
    break;

  case 110: /* if_stmt: IF LPAREN $@5 expression $@6 RPAREN block $@7 elif_list else_opt  */
#line 385 "parser.y"
        { emit("label", "", "", topEnd()); popIfLabels(); }
#line 1997 "y.tab.c"
    break;

  case 111: /* $@8: %empty  */
#line 390 "parser.y"
        { char* n = getLabel(); free(falseStack[topPtr]); falseStack[topPtr] = strdup(n); }
#line 2003 "y.tab.c"
    break;

  case 112: /* $@9: %empty  */
#line 392 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 2009 "y.tab.c"
    break;

  case 113: /* $@10: %empty  */
#line 394 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 2015 "y.tab.c"
    break;

  case 118: /* for_stmt: FOR LPAREN for_header block  */
#line 406 "parser.y"
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
#line 2030 "y.tab.c"
    break;

  case 119: /* $@11: %empty  */
#line 420 "parser.y"
        { char* b = getLabel(); char* e = getLabel(); pushIfLabels(b, e); emit("label", "", "", b); }
#line 2036 "y.tab.c"
    break;

  case 120: /* $@12: %empty  */
#line 422 "parser.y"
        { emit("ifFalse", (yyvsp[-1].sval), "", topEnd()); forDepth++; forIncIdx[forDepth] = 0; inForIncrement[forDepth] = 1; }
#line 2042 "y.tab.c"
    break;

  case 121: /* $@13: %empty  */
#line 424 "parser.y"
        { inForIncrement[forDepth] = 0; }
#line 2048 "y.tab.c"
    break;

  case 127: /* var_decl_no_semi: type IDENTIFIER ASSIGN expression  */
#line 436 "parser.y"
                                        { emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval)); }
#line 2054 "y.tab.c"
    break;

  case 128: /* io_stmt: IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 441 "parser.y"
      {
        emit("in","","",(yyvsp[-6].sval));
      }
#line 2062 "y.tab.c"
    break;

  case 129: /* io_stmt: type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 445 "parser.y"
      {
        emit("in","","",(yyvsp[-6].sval));
      }
#line 2070 "y.tab.c"
    break;

  case 130: /* io_stmt: SHOW LPAREN expression RPAREN SEMICOLON  */
#line 449 "parser.y"
      {
       emit("out",(yyvsp[-2].sval),"","");
      }
#line 2078 "y.tab.c"
    break;

  case 131: /* io_stmt: SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON  */
#line 453 "parser.y"
     {
       char* t=genVar();
       emit("[]",(yyvsp[-5].sval),(yyvsp[-3].sval),t);
       emit("out",t,"","");
     }
#line 2088 "y.tab.c"
    break;


#line 2092 "y.tab.c"

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

#line 460 "parser.y"


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
