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

// Structure of IR code representation

typedef struct {
	char op[10];
	char arg1[20];
	char arg2[20];
	char result[20];
} Quad;

// Initialising the array for storing the Quadraples
Quad IR[10000];

// Index that points to the quadraple indicating representing the IR code
int IR_idx = 0;

// Variable that holds the count of number of temporary variables generated
int tempVarCnt = 0;

// Defining the function for generating new temporary variable
char* genVar();

// Function for emitting the IR code in the form of Quadruples
void emit(char* op, char* arg1, char* arg2, char* result);
void emit_forinc(char* op,char* arg1, char* arg2, char* result);
// Function for generating new labels
char* getLabel();

// variable that tracks the number of labels generated
int labelCnt = 0;

int forDepth = -1;
int inForIncrement[10];
Quad forIncBuff[10][100];
int forIncIdx[10];

char* falseStack[100];
char* endStack[100];
int topPtr = -1;

void pushIfLabels(char* falseLabel, char* endLabel) {
	topPtr++;
	falseStack[topPtr] = falseLabel;
	endStack[topPtr] = endLabel;
}

char* topFalse() {
	return falseStack[topPtr];
}

char* topEnd() {
	return endStack[topPtr];
}

void popIfLabels() {
	topPtr--;
}


#line 139 "y.tab.c"

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
    IDENTIFIER = 276,              /* IDENTIFIER  */
    INT_LITERAL = 277,             /* INT_LITERAL  */
    FLOAT_LITERAL = 278,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 279,            /* CHAR_LITERAL  */
    STRING_LITERAL = 280,          /* STRING_LITERAL  */
    PLUS = 281,                    /* PLUS  */
    MINUS = 282,                   /* MINUS  */
    MUL = 283,                     /* MUL  */
    DIV = 284,                     /* DIV  */
    MOD = 285,                     /* MOD  */
    GT = 286,                      /* GT  */
    LT = 287,                      /* LT  */
    EQ = 288,                      /* EQ  */
    AND = 289,                     /* AND  */
    OR = 290,                      /* OR  */
    NOT = 291,                     /* NOT  */
    BITAND = 292,                  /* BITAND  */
    BITOR = 293,                   /* BITOR  */
    ASSIGN = 294,                  /* ASSIGN  */
    ADD_ASSIGN = 295,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 296,              /* SUB_ASSIGN  */
    SEMICOLON = 297,               /* SEMICOLON  */
    COMMA = 298,                   /* COMMA  */
    LPAREN = 299,                  /* LPAREN  */
    RPAREN = 300,                  /* RPAREN  */
    LBRACE = 301,                  /* LBRACE  */
    RBRACE = 302,                  /* RBRACE  */
    LBRACKET = 303,                /* LBRACKET  */
    RBRACKET = 304                 /* RBRACKET  */
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
#define IDENTIFIER 276
#define INT_LITERAL 277
#define FLOAT_LITERAL 278
#define CHAR_LITERAL 279
#define STRING_LITERAL 280
#define PLUS 281
#define MINUS 282
#define MUL 283
#define DIV 284
#define MOD 285
#define GT 286
#define LT 287
#define EQ 288
#define AND 289
#define OR 290
#define NOT 291
#define BITAND 292
#define BITOR 293
#define ASSIGN 294
#define ADD_ASSIGN 295
#define SUB_ASSIGN 296
#define SEMICOLON 297
#define COMMA 298
#define LPAREN 299
#define RPAREN 300
#define LBRACE 301
#define RBRACE 302
#define LBRACKET 303
#define RBRACKET 304

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 69 "parser.y"

    	int ival;
    	float fval;
    	char cval;
    	char* sval;
    	char* place;

#line 298 "y.tab.c"

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
  YYSYMBOL_IDENTIFIER = 21,                /* IDENTIFIER  */
  YYSYMBOL_INT_LITERAL = 22,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 23,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 24,              /* CHAR_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 25,            /* STRING_LITERAL  */
  YYSYMBOL_PLUS = 26,                      /* PLUS  */
  YYSYMBOL_MINUS = 27,                     /* MINUS  */
  YYSYMBOL_MUL = 28,                       /* MUL  */
  YYSYMBOL_DIV = 29,                       /* DIV  */
  YYSYMBOL_MOD = 30,                       /* MOD  */
  YYSYMBOL_GT = 31,                        /* GT  */
  YYSYMBOL_LT = 32,                        /* LT  */
  YYSYMBOL_EQ = 33,                        /* EQ  */
  YYSYMBOL_AND = 34,                       /* AND  */
  YYSYMBOL_OR = 35,                        /* OR  */
  YYSYMBOL_NOT = 36,                       /* NOT  */
  YYSYMBOL_BITAND = 37,                    /* BITAND  */
  YYSYMBOL_BITOR = 38,                     /* BITOR  */
  YYSYMBOL_ASSIGN = 39,                    /* ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 40,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 41,                /* SUB_ASSIGN  */
  YYSYMBOL_SEMICOLON = 42,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 43,                     /* COMMA  */
  YYSYMBOL_LPAREN = 44,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 45,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 46,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 47,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 48,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 49,                  /* RBRACKET  */
  YYSYMBOL_YYACCEPT = 50,                  /* $accept  */
  YYSYMBOL_program = 51,                   /* program  */
  YYSYMBOL_element = 52,                   /* element  */
  YYSYMBOL_statement = 53,                 /* statement  */
  YYSYMBOL_block = 54,                     /* block  */
  YYSYMBOL_stmt_list = 55,                 /* stmt_list  */
  YYSYMBOL_var_decl = 56,                  /* var_decl  */
  YYSYMBOL_id_list = 57,                   /* id_list  */
  YYSYMBOL_type = 58,                      /* type  */
  YYSYMBOL_array_decl = 59,                /* array_decl  */
  YYSYMBOL_array_init = 60,                /* array_init  */
  YYSYMBOL_expr_list = 61,                 /* expr_list  */
  YYSYMBOL_function_decl = 62,             /* function_decl  */
  YYSYMBOL_func_type = 63,                 /* func_type  */
  YYSYMBOL_param_list_opt = 64,            /* param_list_opt  */
  YYSYMBOL_param_list = 65,                /* param_list  */
  YYSYMBOL_param = 66,                     /* param  */
  YYSYMBOL_return_stmt = 67,               /* return_stmt  */
  YYSYMBOL_expr_stmt = 68,                 /* expr_stmt  */
  YYSYMBOL_expression = 69,                /* expression  */
  YYSYMBOL_indexed_id = 70,                /* indexed_id  */
  YYSYMBOL_assignment = 71,                /* assignment  */
  YYSYMBOL_logic_expr = 72,                /* logic_expr  */
  YYSYMBOL_bitwise_expr = 73,              /* bitwise_expr  */
  YYSYMBOL_rel_expr = 74,                  /* rel_expr  */
  YYSYMBOL_arith_expr = 75,                /* arith_expr  */
  YYSYMBOL_term = 76,                      /* term  */
  YYSYMBOL_factor = 77,                    /* factor  */
  YYSYMBOL_if_stmt = 78,                   /* if_stmt  */
  YYSYMBOL_79_1 = 79,                      /* $@1  */
  YYSYMBOL_80_2 = 80,                      /* $@2  */
  YYSYMBOL_elif_list = 81,                 /* elif_list  */
  YYSYMBOL_82_3 = 82,                      /* $@3  */
  YYSYMBOL_else_opt = 83,                  /* else_opt  */
  YYSYMBOL_84_4 = 84,                      /* $@4  */
  YYSYMBOL_for_stmt = 85,                  /* for_stmt  */
  YYSYMBOL_86_5 = 86,                      /* $@5  */
  YYSYMBOL_87_6 = 87,                      /* $@6  */
  YYSYMBOL_88_7 = 88,                      /* $@7  */
  YYSYMBOL_89_8 = 89,                      /* $@8  */
  YYSYMBOL_io_stmt = 90                    /* io_stmt  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYLAST   279

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  181

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   304


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
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   116,   116,   117,   121,   122,   126,   127,   128,   129,
     130,   131,   132,   133,   137,   138,   142,   143,   147,   151,
     159,   160,   164,   165,   166,   167,   168,   172,   173,   174,
     178,   182,   183,   187,   191,   192,   196,   197,   201,   202,
     206,   210,   211,   215,   216,   220,   224,   228,   233,   241,
     248,   249,   256,   262,   268,   274,   281,   287,   293,   299,
     305,   311,   317,   324,   330,   336,   343,   349,   355,   361,
     368,   372,   373,   379,   385,   391,   395,   401,   407,   415,
     423,   414,   437,   436,   449,   454,   453,   458,   465,   475,
     479,   485,   463,   505,   506
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
  "FEED", "SHOW", "RETURN", "SEQ1", "SEQ2", "FUNC", "IDENTIFIER",
  "INT_LITERAL", "FLOAT_LITERAL", "CHAR_LITERAL", "STRING_LITERAL", "PLUS",
  "MINUS", "MUL", "DIV", "MOD", "GT", "LT", "EQ", "AND", "OR", "NOT",
  "BITAND", "BITOR", "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "SEMICOLON",
  "COMMA", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET", "RBRACKET",
  "$accept", "program", "element", "statement", "block", "stmt_list",
  "var_decl", "id_list", "type", "array_decl", "array_init", "expr_list",
  "function_decl", "func_type", "param_list_opt", "param_list", "param",
  "return_stmt", "expr_stmt", "expression", "indexed_id", "assignment",
  "logic_expr", "bitwise_expr", "rel_expr", "arith_expr", "term", "factor",
  "if_stmt", "$@1", "$@2", "elif_list", "$@3", "else_opt", "$@4",
  "for_stmt", "$@5", "$@6", "$@7", "$@8", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-104)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -104,    82,  -104,  -104,  -104,  -104,  -104,  -104,  -104,     3,
      24,  -104,  -104,    28,     4,   -29,  -104,  -104,  -104,  -104,
     203,  -104,   235,   153,  -104,  -104,  -104,  -104,    31,  -104,
    -104,    -5,  -104,  -104,    41,   -15,  -104,     9,   -17,  -104,
      44,     7,  -104,  -104,  -104,  -104,  -104,   235,   235,    -9,
    -104,    55,    87,   235,   235,   235,    33,  -104,  -104,     6,
    -104,  -104,   198,    31,    96,   101,   108,    50,   106,  -104,
     235,   203,   203,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   235,  -104,   103,   235,  -104,   107,  -104,
    -104,  -104,   105,  -104,  -104,  -104,   -32,   102,   235,  -104,
     140,   120,  -104,  -104,   118,  -104,   134,     7,     7,    88,
      88,    88,  -104,  -104,  -104,   123,   130,   137,   148,  -104,
     135,   158,   160,   141,  -104,   131,   138,   235,  -104,   142,
     235,   143,   139,   144,  -104,   165,   145,   149,  -104,  -104,
    -104,   152,   -34,  -104,  -104,   154,   150,  -104,   138,   131,
     181,   164,  -104,   235,  -104,  -104,   186,  -104,  -104,   169,
     207,  -104,  -104,   180,  -104,  -104,  -104,   235,   167,   235,
     138,  -104,  -104,   185,  -104,   187,   138,   138,   181,  -104,
    -104
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     1,    22,    23,    24,    25,    26,    35,     0,
       0,    76,    77,     0,     0,    70,    72,    73,    74,    75,
       0,    44,     0,     0,     2,     4,    13,     6,    34,     7,
       5,     0,    12,     8,     0,    71,    45,    51,    55,    58,
      62,    65,    69,     9,    10,    11,    79,     0,     0,    70,
      42,     0,     0,     0,     0,     0,    70,    71,    54,     0,
      15,    17,     0,     0,     0,     0,    21,     0,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,    41,     0,    47,
      48,    49,     0,    78,    14,    16,     0,     0,     0,    18,
       0,     0,    50,    53,    52,    56,    57,    63,    64,    59,
      60,    61,    66,    67,    68,     0,     0,     0,     0,    46,
       0,     0,     0,     0,    20,    37,     0,     0,    94,     0,
       0,     0,     0,     0,    19,     0,     0,    36,    39,    80,
      89,     0,     0,    32,    27,     0,     0,    40,     0,     0,
      84,     0,    93,     0,    30,    28,     0,    33,    38,     0,
      87,    90,    31,     0,    82,    85,    81,     0,     0,     0,
       0,    91,    29,     0,    86,     0,     0,     0,    84,    92,
      83
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -104,  -104,  -104,   -20,  -103,  -104,  -104,  -104,     0,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,    84,  -104,  -104,   -14,
     -18,   -48,    -6,    56,  -104,     1,    70,    39,  -104,  -104,
    -104,    53,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    24,    25,    26,    62,    27,    67,    63,    29,
     131,   142,    30,    31,   136,   137,   138,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    83,
     150,   160,   169,   166,   170,    44,   116,   151,   167,   175,
      45
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      51,    28,    57,    61,    89,    90,    91,   120,    59,   153,
      52,    53,    54,   154,    58,    68,   121,    11,    12,    55,
      73,    74,   102,   139,    70,    49,    16,    17,    18,    19,
      86,    53,    54,    84,    85,    80,    81,    82,    89,    55,
      20,    92,    95,    71,    72,   157,    50,    46,    22,    64,
      65,    93,    66,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,   103,   104,   174,    47,   115,
      75,    76,    48,   178,   179,    77,    78,    79,   109,   110,
     111,    55,     2,    69,   123,     3,     4,     5,     6,     7,
       8,     9,    99,   100,    10,    11,    12,    87,    13,    14,
      11,    12,    88,    15,    16,    17,    18,    19,    49,    16,
      17,    18,    19,   140,    75,    76,   143,    96,    20,   112,
     113,   114,    97,    20,    21,   135,    22,   101,    23,   105,
     106,    22,    11,    12,     3,     4,     5,     6,     7,   162,
      56,    16,    17,    18,    19,   107,   108,    98,   117,   135,
     122,   118,    71,   171,   119,   173,     3,     4,     5,     6,
       7,   124,     9,    22,   125,    10,    11,    12,   126,    13,
      14,    73,   127,   129,    15,    16,    17,    18,    19,   128,
     132,   130,   133,   134,    23,   144,   147,   141,   145,    20,
     148,   159,   149,   146,   152,    21,   155,    22,   156,    23,
      60,     3,     4,     5,     6,     7,   161,     9,   163,   172,
      10,    11,    12,   164,    13,    14,    11,    12,   165,    15,
      16,    17,    18,    19,    56,    16,    17,    18,    19,   168,
     176,   180,   177,   158,    20,     0,     0,     0,     0,    20,
      21,     0,    22,     0,    23,    94,     0,    22,    11,    12,
       0,     0,     0,     0,     0,     0,    49,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,    22
};

static const yytype_int16 yycheck[] =
{
      14,     1,    20,    23,    52,    53,    54,    39,    22,    43,
      39,    40,    41,    47,    20,    20,    48,    13,    14,    48,
      37,    38,    70,   126,    39,    21,    22,    23,    24,    25,
      39,    40,    41,    47,    48,    28,    29,    30,    86,    48,
      36,    55,    62,    34,    35,   148,    42,    44,    44,    18,
      19,    45,    21,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    71,    72,   170,    44,    83,
      26,    27,    44,   176,   177,    31,    32,    33,    77,    78,
      79,    48,     0,    42,    98,     3,     4,     5,     6,     7,
       8,     9,    42,    43,    12,    13,    14,    42,    16,    17,
      13,    14,    15,    21,    22,    23,    24,    25,    21,    22,
      23,    24,    25,   127,    26,    27,   130,    21,    36,    80,
      81,    82,    21,    36,    42,   125,    44,    21,    46,    73,
      74,    44,    13,    14,     3,     4,     5,     6,     7,   153,
      21,    22,    23,    24,    25,    75,    76,    39,    45,   149,
      48,    44,    34,   167,    49,   169,     3,     4,     5,     6,
       7,    21,     9,    44,    44,    12,    13,    14,    45,    16,
      17,    37,    42,    25,    21,    22,    23,    24,    25,    42,
      22,    46,    22,    42,    46,    42,    21,    45,    49,    36,
      45,    10,    43,    49,    42,    42,    42,    44,    48,    46,
      47,     3,     4,     5,     6,     7,    42,     9,    22,    42,
      12,    13,    14,    44,    16,    17,    13,    14,    11,    21,
      22,    23,    24,    25,    21,    22,    23,    24,    25,    49,
      45,   178,    45,   149,    36,    -1,    -1,    -1,    -1,    36,
      42,    -1,    44,    -1,    46,    47,    -1,    44,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    51,     0,     3,     4,     5,     6,     7,     8,     9,
      12,    13,    14,    16,    17,    21,    22,    23,    24,    25,
      36,    42,    44,    46,    52,    53,    54,    56,    58,    59,
      62,    63,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    85,    90,    44,    44,    44,    21,
      42,    69,    39,    40,    41,    48,    21,    70,    72,    69,
      47,    53,    55,    58,    18,    19,    21,    57,    20,    42,
      39,    34,    35,    37,    38,    26,    27,    31,    32,    33,
      28,    29,    30,    79,    69,    69,    39,    42,    15,    71,
      71,    71,    69,    45,    47,    53,    21,    21,    39,    42,
      43,    21,    71,    72,    72,    73,    73,    76,    76,    75,
      75,    75,    77,    77,    77,    69,    86,    45,    44,    49,
      39,    48,    48,    69,    21,    44,    45,    42,    42,    25,
      46,    60,    22,    22,    42,    58,    64,    65,    66,    54,
      69,    45,    61,    69,    42,    49,    49,    21,    45,    43,
      80,    87,    42,    43,    47,    42,    48,    54,    66,    10,
      81,    42,    69,    22,    44,    11,    83,    88,    49,    82,
      84,    69,    42,    69,    54,    89,    45,    45,    54,    54,
      81
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    51,    52,    52,    53,    53,    53,    53,
      53,    53,    53,    53,    54,    54,    55,    55,    56,    56,
      57,    57,    58,    58,    58,    58,    58,    59,    59,    59,
      60,    61,    61,    62,    63,    63,    64,    64,    65,    65,
      66,    67,    67,    68,    68,    69,    70,    71,    71,    71,
      71,    71,    72,    72,    72,    72,    73,    73,    73,    74,
      74,    74,    74,    75,    75,    75,    76,    76,    76,    76,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    79,
      80,    78,    82,    81,    81,    84,    83,    83,    86,    87,
      88,    89,    85,    90,    90
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     2,     1,     3,     5,
       3,     1,     1,     1,     1,     1,     1,     6,     7,    10,
       3,     3,     1,     7,     1,     1,     1,     0,     3,     1,
       2,     3,     2,     2,     1,     1,     4,     3,     3,     3,
       3,     1,     3,     3,     2,     1,     3,     3,     1,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       0,     9,     0,     7,     0,     0,     3,     0,     0,     0,
       0,     0,    13,     7,     5
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
  case 18: /* var_decl: type id_list SEMICOLON  */
#line 148 "parser.y"
        {
		//no need to emit anything here and later stage add code for symbol table here
	}
#line 1515 "y.tab.c"
    break;

  case 19: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 152 "parser.y"
        {
		//code for symbol table comes here
		emit("=",(yyvsp[-1].sval),"",(yyvsp[-3].sval));
	}
#line 1524 "y.tab.c"
    break;

  case 47: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 229 "parser.y"
        {
		emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval));
		(yyval.sval) = strdup((yyvsp[-2].sval));
	}
#line 1533 "y.tab.c"
    break;

  case 48: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 234 "parser.y"
        {
		char* temp = genVar();
		emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
		emit("=", temp, "", (yyvsp[-2].sval));
		(yyval.sval) = temp;
	}
#line 1544 "y.tab.c"
    break;

  case 49: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 242 "parser.y"
        {
                char* temp = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                emit("=", temp, "", (yyvsp[-2].sval));
		(yyval.sval) = temp;
        }
#line 1555 "y.tab.c"
    break;

  case 51: /* assignment: logic_expr  */
#line 250 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1563 "y.tab.c"
    break;

  case 52: /* logic_expr: logic_expr OR logic_expr  */
#line 257 "parser.y"
        {
		char* temp = genVar();
		emit("||",(yyvsp[-2].sval),(yyvsp[0].sval),temp); //temp = $1 || $2
		(yyval.sval) = temp;
	}
#line 1573 "y.tab.c"
    break;

  case 53: /* logic_expr: logic_expr AND logic_expr  */
#line 263 "parser.y"
        {
                char* temp = genVar();
                emit("&&",(yyvsp[-2].sval),(yyvsp[0].sval),temp); //temp = $1 && $2
                (yyval.sval) = temp;
        }
#line 1583 "y.tab.c"
    break;

  case 54: /* logic_expr: NOT logic_expr  */
#line 269 "parser.y"
        {
                char* temp = genVar();
                emit("!",(yyvsp[0].sval),"",temp); //temp = !$2
                (yyval.sval) = temp;
        }
#line 1593 "y.tab.c"
    break;

  case 55: /* logic_expr: bitwise_expr  */
#line 275 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1601 "y.tab.c"
    break;

  case 56: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 282 "parser.y"
        {
		char* temp = genVar();
		emit("&", (yyvsp[-2].sval), (yyvsp[0].sval) , temp); //temp = $1 & $3
		(yyval.sval) = temp;
	}
#line 1611 "y.tab.c"
    break;

  case 57: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 288 "parser.y"
        {
		char* temp = genVar();
		emit("|",(yyvsp[-2].sval),(yyvsp[0].sval), temp);
		(yyval.sval) = temp;
	}
#line 1621 "y.tab.c"
    break;

  case 58: /* bitwise_expr: rel_expr  */
#line 294 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1629 "y.tab.c"
    break;

  case 59: /* rel_expr: arith_expr GT arith_expr  */
#line 300 "parser.y"
        {
		char* temp = genVar();
		emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), temp); //temp = $1 > $3
		(yyval.sval) = temp;
	}
#line 1639 "y.tab.c"
    break;

  case 60: /* rel_expr: arith_expr LT arith_expr  */
#line 306 "parser.y"
        {
		char* temp = genVar();
		emit("<",(yyvsp[-2].sval) , (yyvsp[0].sval), temp); //temp = $1 < $3
		(yyval.sval) = temp;
	}
#line 1649 "y.tab.c"
    break;

  case 61: /* rel_expr: arith_expr EQ arith_expr  */
#line 312 "parser.y"
        {
		char* temp = genVar();
		emit("==",(yyvsp[-2].sval), (yyvsp[0].sval), temp); //temp = $1 == $3
		(yyval.sval) = temp;
	}
#line 1659 "y.tab.c"
    break;

  case 62: /* rel_expr: arith_expr  */
#line 318 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1667 "y.tab.c"
    break;

  case 63: /* arith_expr: arith_expr PLUS term  */
#line 325 "parser.y"
        {
		char* temp = genVar();
		emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
		(yyval.sval) = temp;
	}
#line 1677 "y.tab.c"
    break;

  case 64: /* arith_expr: arith_expr MINUS term  */
#line 331 "parser.y"
        {
		char* temp = genVar();
		emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
		(yyval.sval) = temp;
	}
#line 1687 "y.tab.c"
    break;

  case 65: /* arith_expr: term  */
#line 337 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1695 "y.tab.c"
    break;

  case 66: /* term: term MUL factor  */
#line 344 "parser.y"
        {
		char* temp = genVar();
		emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
		(yyval.sval) = temp;
	}
#line 1705 "y.tab.c"
    break;

  case 67: /* term: term DIV factor  */
#line 350 "parser.y"
        {
		char* temp = genVar();
		emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
		(yyval.sval) = temp;
	}
#line 1715 "y.tab.c"
    break;

  case 68: /* term: term MOD factor  */
#line 356 "parser.y"
        {
		char* temp = genVar();
		emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
		(yyval.sval) = temp;
	}
#line 1725 "y.tab.c"
    break;

  case 69: /* term: factor  */
#line 362 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1733 "y.tab.c"
    break;

  case 70: /* factor: IDENTIFIER  */
#line 369 "parser.y"
        {
		(yyval.sval) = strdup((yyvsp[0].sval));
	}
#line 1741 "y.tab.c"
    break;

  case 72: /* factor: INT_LITERAL  */
#line 374 "parser.y"
        {
		char buffer[20];
		sprintf(buffer, "%d", (yyvsp[0].ival));
		(yyval.sval) = strdup(buffer);
	}
#line 1751 "y.tab.c"
    break;

  case 73: /* factor: FLOAT_LITERAL  */
#line 380 "parser.y"
        {
		char buffer[20];
		sprintf(buffer, "%f", (yyvsp[0].fval));
		(yyval.sval) = strdup(buffer);
	}
#line 1761 "y.tab.c"
    break;

  case 74: /* factor: CHAR_LITERAL  */
#line 386 "parser.y"
        {
		char buffer[20];
		sprintf(buffer, "'%c'", (yyvsp[0].cval));
		(yyval.sval) = strdup(buffer);
	}
#line 1771 "y.tab.c"
    break;

  case 75: /* factor: STRING_LITERAL  */
#line 392 "parser.y"
        {
		(yyval.sval) = strdup((yyvsp[0].sval));
	}
#line 1779 "y.tab.c"
    break;

  case 76: /* factor: TRUE  */
#line 396 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", 1);
                (yyval.sval) = strdup(buffer);
        }
#line 1789 "y.tab.c"
    break;

  case 77: /* factor: FALSE  */
#line 402 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", 0);
                (yyval.sval) = strdup(buffer);
        }
#line 1799 "y.tab.c"
    break;

  case 78: /* factor: LPAREN expression RPAREN  */
#line 408 "parser.y"
        {
		(yyval.sval) = (yyvsp[-1].sval);
	}
#line 1807 "y.tab.c"
    break;

  case 79: /* $@1: %empty  */
#line 415 "parser.y"
        {
		char* falseLabel = getLabel();
		char* Lend = getLabel();
		pushIfLabels(falseLabel, Lend);
	}
#line 1817 "y.tab.c"
    break;

  case 80: /* $@2: %empty  */
#line 423 "parser.y"
        {
		emit("goto", "", "", topEnd());
		emit("label", "", "", topFalse());
	}
#line 1826 "y.tab.c"
    break;

  case 81: /* if_stmt: IF LPAREN $@1 expression RPAREN block $@2 elif_list else_opt  */
#line 429 "parser.y"
        {
		emit("label", "", "", topEnd());
		popIfLabels();
	}
#line 1835 "y.tab.c"
    break;

  case 82: /* $@3: %empty  */
#line 437 "parser.y"
        {
		char* prevFalseLabel = topFalse();
		char* nextFalseLabel = getLabel();
		
		emit("goto", "", "", topEnd());
		emit("label", "", "", prevFalseLabel);
		
		falseStack[topPtr] = nextFalseLabel;
	}
#line 1849 "y.tab.c"
    break;

  case 85: /* $@4: %empty  */
#line 454 "parser.y"
        {
		emit("label", "", "", topFalse());
	}
#line 1857 "y.tab.c"
    break;

  case 88: /* $@5: %empty  */
#line 465 "parser.y"
        {
		char* Lbegin = getLabel();
		char* Lend = getLabel();
		
		pushIfLabels(Lbegin,Lend);
		
		emit("label","","",Lbegin);
	}
#line 1870 "y.tab.c"
    break;

  case 89: /* $@6: %empty  */
#line 475 "parser.y"
        {
		emit("ifFalse",(yyvsp[0].sval),"",topEnd());
	}
#line 1878 "y.tab.c"
    break;

  case 90: /* $@7: %empty  */
#line 479 "parser.y"
        {
		forDepth++;
		forIncIdx[forDepth] = 0;
		inForIncrement[forDepth]=1;
	}
#line 1888 "y.tab.c"
    break;

  case 91: /* $@8: %empty  */
#line 485 "parser.y"
        {
		inForIncrement[forDepth]=0;
	}
#line 1896 "y.tab.c"
    break;

  case 92: /* for_stmt: FOR LPAREN expression $@5 SEMICOLON expression $@6 SEMICOLON $@7 expression $@8 RPAREN block  */
#line 489 "parser.y"
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
#line 1914 "y.tab.c"
    break;


#line 1918 "y.tab.c"

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

#line 509 "parser.y"


/* Implementing the helper functions "genVar" and "emit" for
   generating IR code in the form of Quadruples
*/

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
void emit_forinc(char* op, char* arg1, char* arg2, char* result){
	strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].op,op);
	strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].arg1,arg1);
	strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].arg2, arg2);
    	strcpy(forIncBuff[forDepth][forIncIdx[forDepth]].result, result);
	forIncIdx[forDepth]++;
}

void emit(char* op, char* arg1, char* arg2, char* result) {
//	printf("EMIT called: %s %s %s %s  (flag=%d)\n",op,arg1,arg2,result,inForIncrement);	
//	fflush(stdout);
	if(forDepth >= 0 && inForIncrement[forDepth]){
		emit_forinc(op,arg1,arg2,result);
		return;
	}

	strcpy(IR[IR_idx].op, op);
	strcpy(IR[IR_idx].arg1, arg1);
	strcpy(IR[IR_idx].arg2, arg2);
	strcpy(IR[IR_idx].result, result);
	IR_idx++;
}

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}

int main() {
    yyin = stdin;
    yyparse();
    //inForIncrement=0;
    printf("Parsing Successful\n");
    printf("Generated quadruple table:\n");

    printf("%-10s %-10s %-10s %-10s\n", "OP", "ARG1", "ARG2", "RESULT");
 //   printf("%d",IR_idx);
    for(int i = 0; i < IR_idx; i++){
        printf("%-10s %-10s %-10s %-10s\n",
               IR[i].op,
               IR[i].arg1,
               IR[i].arg2,
               IR[i].result);
    }

    return 0;
}

