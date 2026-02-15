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

// Function for generating new labels
char* getLabel();

// variable that tracks the number of labels generated
int labelCnt = 0;

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


#line 133 "y.tab.c"

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
    IDENTIFIER = 282,              /* IDENTIFIER  */
    INT_LITERAL = 283,             /* INT_LITERAL  */
    FLOAT_LITERAL = 284,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 285,            /* CHAR_LITERAL  */
    STRING_LITERAL = 286,          /* STRING_LITERAL  */
    PLUS = 287,                    /* PLUS  */
    MINUS = 288,                   /* MINUS  */
    MUL = 289,                     /* MUL  */
    DIV = 290,                     /* DIV  */
    MOD = 291,                     /* MOD  */
    GT = 292,                      /* GT  */
    LT = 293,                      /* LT  */
    EQ = 294,                      /* EQ  */
    AND = 295,                     /* AND  */
    OR = 296,                      /* OR  */
    NOT = 297,                     /* NOT  */
    ASSIGN = 298,                  /* ASSIGN  */
    ADD_ASSIGN = 299,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 300,              /* SUB_ASSIGN  */
    SEMICOLON = 301,               /* SEMICOLON  */
    COMMA = 302,                   /* COMMA  */
    LPAREN = 303,                  /* LPAREN  */
    RPAREN = 304,                  /* RPAREN  */
    LBRACE = 305,                  /* LBRACE  */
    RBRACE = 306,                  /* RBRACE  */
    LBRACKET = 307,                /* LBRACKET  */
    RBRACKET = 308                 /* RBRACKET  */
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
#define IDENTIFIER 282
#define INT_LITERAL 283
#define FLOAT_LITERAL 284
#define CHAR_LITERAL 285
#define STRING_LITERAL 286
#define PLUS 287
#define MINUS 288
#define MUL 289
#define DIV 290
#define MOD 291
#define GT 292
#define LT 293
#define EQ 294
#define AND 295
#define OR 296
#define NOT 297
#define ASSIGN 298
#define ADD_ASSIGN 299
#define SUB_ASSIGN 300
#define SEMICOLON 301
#define COMMA 302
#define LPAREN 303
#define RPAREN 304
#define LBRACE 305
#define RBRACE 306
#define LBRACKET 307
#define RBRACKET 308

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 63 "parser.y"

        int ival;
        float fval;
        char cval;
        char* sval;
        char* place;

#line 300 "y.tab.c"

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
  YYSYMBOL_IDENTIFIER = 27,                /* IDENTIFIER  */
  YYSYMBOL_INT_LITERAL = 28,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 29,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 30,              /* CHAR_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 31,            /* STRING_LITERAL  */
  YYSYMBOL_PLUS = 32,                      /* PLUS  */
  YYSYMBOL_MINUS = 33,                     /* MINUS  */
  YYSYMBOL_MUL = 34,                       /* MUL  */
  YYSYMBOL_DIV = 35,                       /* DIV  */
  YYSYMBOL_MOD = 36,                       /* MOD  */
  YYSYMBOL_GT = 37,                        /* GT  */
  YYSYMBOL_LT = 38,                        /* LT  */
  YYSYMBOL_EQ = 39,                        /* EQ  */
  YYSYMBOL_AND = 40,                       /* AND  */
  YYSYMBOL_OR = 41,                        /* OR  */
  YYSYMBOL_NOT = 42,                       /* NOT  */
  YYSYMBOL_ASSIGN = 43,                    /* ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 44,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 45,                /* SUB_ASSIGN  */
  YYSYMBOL_SEMICOLON = 46,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 47,                     /* COMMA  */
  YYSYMBOL_LPAREN = 48,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 49,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 50,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 51,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 52,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 53,                  /* RBRACKET  */
  YYSYMBOL_YYACCEPT = 54,                  /* $accept  */
  YYSYMBOL_program = 55,                   /* program  */
  YYSYMBOL_element = 56,                   /* element  */
  YYSYMBOL_entity_decl = 57,               /* entity_decl  */
  YYSYMBOL_entity_body = 58,               /* entity_body  */
  YYSYMBOL_entity_member = 59,             /* entity_member  */
  YYSYMBOL_constructor_decl = 60,          /* constructor_decl  */
  YYSYMBOL_method_decl = 61,               /* method_decl  */
  YYSYMBOL_access_var_decl = 62,           /* access_var_decl  */
  YYSYMBOL_access_modifier = 63,           /* access_modifier  */
  YYSYMBOL_object_decl = 64,               /* object_decl  */
  YYSYMBOL_arg_list_opt = 65,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 66,                  /* arg_list  */
  YYSYMBOL_statement = 67,                 /* statement  */
  YYSYMBOL_block = 68,                     /* block  */
  YYSYMBOL_stmt_list = 69,                 /* stmt_list  */
  YYSYMBOL_var_decl = 70,                  /* var_decl  */
  YYSYMBOL_id_list = 71,                   /* id_list  */
  YYSYMBOL_type = 72,                      /* type  */
  YYSYMBOL_array_decl = 73,                /* array_decl  */
  YYSYMBOL_array_init = 74,                /* array_init  */
  YYSYMBOL_expr_list = 75,                 /* expr_list  */
  YYSYMBOL_function_decl = 76,             /* function_decl  */
  YYSYMBOL_func_type = 77,                 /* func_type  */
  YYSYMBOL_param_list_opt = 78,            /* param_list_opt  */
  YYSYMBOL_param_list = 79,                /* param_list  */
  YYSYMBOL_param = 80,                     /* param  */
  YYSYMBOL_return_stmt = 81,               /* return_stmt  */
  YYSYMBOL_expr_stmt = 82,                 /* expr_stmt  */
  YYSYMBOL_expression = 83,                /* expression  */
  YYSYMBOL_indexed_id = 84,                /* indexed_id  */
  YYSYMBOL_assignment = 85,                /* assignment  */
  YYSYMBOL_logic_expr = 86,                /* logic_expr  */
  YYSYMBOL_rel_expr = 87,                  /* rel_expr  */
  YYSYMBOL_arith_expr = 88,                /* arith_expr  */
  YYSYMBOL_term = 89,                      /* term  */
  YYSYMBOL_factor = 90,                    /* factor  */
  YYSYMBOL_if_stmt = 91,                   /* if_stmt  */
  YYSYMBOL_92_1 = 92,                      /* $@1  */
  YYSYMBOL_93_2 = 93,                      /* $@2  */
  YYSYMBOL_elif_list = 94,                 /* elif_list  */
  YYSYMBOL_95_3 = 95,                      /* $@3  */
  YYSYMBOL_else_opt = 96,                  /* else_opt  */
  YYSYMBOL_97_4 = 97,                      /* $@4  */
  YYSYMBOL_for_stmt = 98,                  /* for_stmt  */
  YYSYMBOL_io_stmt = 99                    /* io_stmt  */
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
#define YYLAST   387

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  232

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   308


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
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   107,   107,   108,   112,   113,   114,   115,   119,   123,
     124,   128,   129,   130,   134,   138,   142,   146,   147,   151,
     155,   156,   160,   161,   165,   166,   167,   168,   169,   170,
     171,   172,   176,   177,   181,   182,   186,   187,   191,   192,
     196,   197,   198,   199,   200,   201,   205,   206,   207,   211,
     215,   216,   220,   224,   225,   229,   230,   234,   235,   239,
     243,   244,   248,   249,   253,   257,   261,   266,   273,   279,
     280,   284,   288,   292,   293,   294,   295,   299,   300,   301,
     302,   306,   312,   318,   325,   331,   337,   343,   350,   354,
     355,   361,   367,   373,   377,   383,   389,   397,   405,   396,
     419,   418,   431,   436,   435,   440,   444,   444,   448,   449
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
  "PUBLIC", "PRIVATE", "THIS", "DOT", "IDENTIFIER", "INT_LITERAL",
  "FLOAT_LITERAL", "CHAR_LITERAL", "STRING_LITERAL", "PLUS", "MINUS",
  "MUL", "DIV", "MOD", "GT", "LT", "EQ", "AND", "OR", "NOT", "ASSIGN",
  "ADD_ASSIGN", "SUB_ASSIGN", "SEMICOLON", "COMMA", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "$accept", "program",
  "element", "entity_decl", "entity_body", "entity_member",
  "constructor_decl", "method_decl", "access_var_decl", "access_modifier",
  "object_decl", "arg_list_opt", "arg_list", "statement", "block",
  "stmt_list", "var_decl", "id_list", "type", "array_decl", "array_init",
  "expr_list", "function_decl", "func_type", "param_list_opt",
  "param_list", "param", "return_stmt", "expr_stmt", "expression",
  "indexed_id", "assignment", "logic_expr", "rel_expr", "arith_expr",
  "term", "factor", "if_stmt", "$@1", "$@2", "elif_list", "$@3",
  "else_opt", "$@4", "for_stmt", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-166)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-46)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -166,   118,  -166,  -166,  -166,  -166,  -166,  -166,  -166,   -43,
     -37,  -166,  -166,   -18,   290,    -3,    36,   -11,  -166,  -166,
    -166,  -166,    25,  -166,   321,   169,  -166,  -166,  -166,  -166,
    -166,  -166,     1,  -166,  -166,    26,  -166,  -166,    30,    38,
    -166,     3,    20,    33,    44,  -166,  -166,  -166,  -166,  -166,
     248,   321,   158,  -166,    61,    40,    87,    88,    73,   297,
     321,   321,   321,    65,  -166,  -166,    70,   335,  -166,  -166,
     218,     1,    93,   106,    94,    22,   109,  -166,   321,    25,
      25,    81,    81,    81,    81,    81,    81,    81,    81,   321,
     257,   321,    92,    91,   321,  -166,  -166,    98,    99,   128,
     103,  -166,  -166,  -166,   101,  -166,  -166,  -166,   -17,   100,
     321,  -166,   130,   107,  -166,  -166,   121,    33,    33,    33,
      44,    44,  -166,  -166,  -166,   113,   112,   321,   117,    -6,
     321,   321,   138,   136,  -166,   119,   143,   149,   134,  -166,
     185,   137,   321,   159,  -166,  -166,  -166,   147,  -166,  -166,
    -166,  -166,  -166,   185,  -166,  -166,   156,   160,   321,   161,
     155,   163,  -166,  -166,   186,   165,   171,  -166,  -166,   180,
     321,   185,     2,   321,   182,   -24,  -166,  -166,   183,   181,
    -166,   137,   185,   226,   321,   188,   189,   212,   194,   192,
     195,  -166,  -166,   321,  -166,  -166,   216,  -166,  -166,   202,
     245,   208,   137,   137,   210,  -166,   213,   321,  -166,   214,
    -166,  -166,  -166,   137,  -166,  -166,   185,  -166,  -166,   217,
     321,   137,  -166,   223,  -166,   225,  -166,   137,   137,  -166,
     226,  -166
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     1,    40,    41,    42,    43,    44,    54,     0,
       0,    94,    95,     0,     0,     0,     0,    88,    90,    91,
      92,    93,     0,    63,     0,     0,     2,     6,     7,     4,
      31,    24,    53,    25,     5,     0,    30,    26,     0,    89,
      64,    72,    76,    80,    83,    87,    27,    28,    29,    97,
       0,     0,    88,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    75,     0,    88,    33,    35,
       0,     0,     0,     0,    39,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,    60,    10,     0,     0,     0,
       0,    66,    67,    68,     0,    96,    32,    34,     0,     0,
       0,    36,     0,     0,    69,    74,    73,    77,    78,    79,
      81,    82,    84,    85,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,    38,
      56,     0,     0,     0,   109,    17,    18,     0,     8,     9,
      11,    12,    13,     0,    70,    71,     0,     0,     0,     0,
       0,     0,    37,    45,     0,     0,    55,    58,    98,     0,
       0,    56,     0,    21,     0,     0,    51,    46,     0,     0,
      59,     0,     0,   102,     0,     0,     0,     0,     0,     0,
      20,    23,   108,     0,    49,    47,     0,    52,    57,     0,
     105,     0,     0,     0,     0,    16,     0,     0,    50,     0,
     100,   103,    99,     0,   106,    14,    56,    19,    22,     0,
       0,     0,   107,     0,    48,     0,   104,     0,     0,    15,
     102,   101
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -166,  -166,  -166,  -166,  -166,  -166,  -166,  -166,  -166,  -166,
    -166,  -166,  -166,   -21,  -139,  -166,  -166,  -166,     0,  -166,
    -166,  -166,  -166,  -166,  -165,  -166,    83,  -166,  -166,   -14,
      18,   -47,   -19,  -166,     4,   -13,     5,  -166,  -166,  -166,
      50,  -166,  -166,  -166,  -166,  -166
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    26,    27,   129,   149,   150,   151,   152,   153,
      28,   189,   190,    29,    30,    70,    31,    75,   164,    33,
     159,   175,    34,    35,   165,   166,   167,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    89,   183,
     200,   220,   212,   221,    47,    48
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      54,    32,   168,    65,    69,    49,   186,   -45,   -45,   -45,
      66,    50,   101,   102,   103,    57,    58,   145,   146,    72,
      73,   147,   187,   193,    55,    71,   135,   194,    74,   188,
      51,   114,    59,    60,    61,   136,    92,    93,    11,    12,
      64,    62,   197,    79,    80,   148,    76,   101,   104,   107,
      91,   223,    63,    18,    19,    20,    21,    81,    82,    83,
     115,   116,    56,   214,   215,    84,    85,    22,   111,   112,
      71,   120,   121,    24,   222,   125,    77,   126,    86,    87,
      88,    78,   226,   154,   155,   117,   118,   119,   229,   230,
      96,   122,   123,   124,    11,    12,   138,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    95,    63,    18,
      19,    20,    21,   143,    97,    98,    99,    62,     2,   105,
     108,     3,     4,     5,     6,     7,     8,     9,   169,    24,
      10,    11,    12,   109,    13,    14,   113,   110,   127,    15,
     128,   130,   131,    16,   176,    17,    18,    19,    20,    21,
     132,   133,   137,   172,   134,   140,   185,   139,   142,   191,
      22,    79,   141,   144,    23,   156,    24,   157,    25,   158,
     201,   160,     3,     4,     5,     6,     7,   161,     9,   208,
     162,    10,    11,    12,    57,    13,    14,    25,     3,     4,
       5,     6,     7,   218,    16,   171,    67,    18,    19,    20,
      21,    94,    60,    61,   173,   170,   225,   177,   178,   174,
      62,    22,   163,   180,   181,    23,   179,    24,   182,    25,
      68,     3,     4,     5,     6,     7,   184,     9,   192,   195,
      10,    11,    12,   196,    13,    14,   199,   202,   203,   204,
     205,   206,   207,    16,   209,    67,    18,    19,    20,    21,
     210,     3,     4,     5,     6,     7,   211,   213,   216,   217,
      22,    11,    12,   224,    23,   198,    24,   219,    25,   106,
     -45,   -45,   227,    16,   228,    90,    18,    19,    20,    21,
     231,     0,   -45,    57,   -45,   -45,   -45,   -45,   -45,     0,
      22,     0,     0,     0,     0,     0,    24,     0,     0,   -45,
      94,    60,    61,    11,    12,   -45,     0,     0,     0,    62,
      11,    12,   100,     0,     0,    16,     0,    52,    18,    19,
      20,    21,    16,     0,    52,    18,    19,    20,    21,     0,
       0,     0,    22,     0,    11,    12,    53,     0,    24,    22,
       0,     0,     0,     0,     0,    24,    16,     0,    52,    18,
      19,    20,    21,   -45,   -45,     0,     0,     0,     0,     0,
       0,    57,   -45,    22,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,     0,     0,     0,     0,     0,     0,    62
};

static const yytype_int16 yycheck[] =
{
      14,     1,   141,    22,    25,    48,   171,    18,    19,    20,
      24,    48,    59,    60,    61,    26,    27,    23,    24,    18,
      19,    27,    20,    47,    27,    25,    43,    51,    27,    27,
      48,    78,    43,    44,    45,    52,    50,    51,    13,    14,
      22,    52,   181,    40,    41,    51,    20,    94,    62,    70,
      50,   216,    27,    28,    29,    30,    31,    37,    38,    39,
      79,    80,    26,   202,   203,    32,    33,    42,    46,    47,
      70,    84,    85,    48,   213,    89,    46,    91,    34,    35,
      36,    43,   221,   130,   131,    81,    82,    83,   227,   228,
      50,    86,    87,    88,    13,    14,   110,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    46,    27,    28,
      29,    30,    31,   127,    27,    27,    43,    52,     0,    49,
      27,     3,     4,     5,     6,     7,     8,     9,   142,    48,
      12,    13,    14,    27,    16,    17,    27,    43,    46,    21,
      49,    43,    43,    25,   158,    27,    28,    29,    30,    31,
      22,    48,    52,   153,    53,    48,   170,    27,    46,   173,
      42,    40,    49,    46,    46,    27,    48,    31,    50,    50,
     184,    28,     3,     4,     5,     6,     7,    28,     9,   193,
      46,    12,    13,    14,    26,    16,    17,    50,     3,     4,
       5,     6,     7,   207,    25,    48,    27,    28,    29,    30,
      31,    43,    44,    45,    48,    46,   220,    46,    53,    49,
      52,    42,    27,    27,    49,    46,    53,    48,    47,    50,
      51,     3,     4,     5,     6,     7,    46,     9,    46,    46,
      12,    13,    14,    52,    16,    17,    10,    49,    49,    27,
      46,    49,    47,    25,    28,    27,    28,    29,    30,    31,
      48,     3,     4,     5,     6,     7,    11,    49,    48,    46,
      42,    13,    14,    46,    46,   182,    48,    53,    50,    51,
      13,    14,    49,    25,    49,    27,    28,    29,    30,    31,
     230,    -1,    25,    26,    27,    28,    29,    30,    31,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    42,
      43,    44,    45,    13,    14,    48,    -1,    -1,    -1,    52,
      13,    14,    15,    -1,    -1,    25,    -1,    27,    28,    29,
      30,    31,    25,    -1,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    42,    -1,    13,    14,    46,    -1,    48,    42,
      -1,    -1,    -1,    -1,    -1,    48,    25,    -1,    27,    28,
      29,    30,    31,    18,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    42,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    44,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    52
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    55,     0,     3,     4,     5,     6,     7,     8,     9,
      12,    13,    14,    16,    17,    21,    25,    27,    28,    29,
      30,    31,    42,    46,    48,    50,    56,    57,    64,    67,
      68,    70,    72,    73,    76,    77,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    98,    99,    48,
      48,    48,    27,    46,    83,    27,    26,    26,    27,    43,
      44,    45,    52,    27,    84,    86,    83,    27,    51,    67,
      69,    72,    18,    19,    27,    71,    20,    46,    43,    40,
      41,    37,    38,    39,    32,    33,    34,    35,    36,    92,
      27,    72,    83,    83,    43,    46,    50,    27,    27,    43,
      15,    85,    85,    85,    83,    49,    51,    67,    27,    27,
      43,    46,    47,    27,    85,    86,    86,    88,    88,    88,
      89,    89,    90,    90,    90,    83,    83,    46,    49,    58,
      43,    43,    22,    48,    53,    43,    52,    52,    83,    27,
      48,    49,    46,    83,    46,    23,    24,    27,    51,    59,
      60,    61,    62,    63,    85,    85,    27,    31,    50,    74,
      28,    28,    46,    27,    72,    78,    79,    80,    68,    83,
      46,    48,    72,    48,    49,    75,    83,    46,    53,    53,
      27,    49,    47,    93,    46,    83,    78,    20,    27,    65,
      66,    83,    46,    47,    51,    46,    52,    68,    80,    10,
      94,    83,    49,    49,    27,    46,    49,    47,    83,    28,
      48,    11,    96,    49,    68,    68,    48,    46,    83,    53,
      95,    97,    68,    78,    46,    83,    68,    49,    49,    68,
      68,    94
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    54,    55,    55,    56,    56,    56,    56,    57,    58,
      58,    59,    59,    59,    60,    61,    62,    63,    63,    64,
      65,    65,    66,    66,    67,    67,    67,    67,    67,    67,
      67,    67,    68,    68,    69,    69,    70,    70,    71,    71,
      72,    72,    72,    72,    72,    72,    73,    73,    73,    74,
      75,    75,    76,    77,    77,    78,    78,    79,    79,    80,
      81,    81,    82,    82,    83,    84,    85,    85,    85,    85,
      85,    85,    85,    86,    86,    86,    86,    87,    87,    87,
      87,    88,    88,    88,    89,    89,    89,    89,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    92,    93,    91,
      95,    94,    94,    97,    96,    96,    98,    98,    99,    99
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     5,     2,
       0,     1,     1,     1,     5,     8,     4,     1,     1,     9,
       1,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     2,     1,     3,     5,     3,     1,
       1,     1,     1,     1,     1,     1,     6,     7,    10,     3,
       3,     1,     7,     1,     1,     1,     0,     3,     1,     2,
       3,     2,     2,     1,     1,     4,     3,     3,     3,     3,
       5,     5,     1,     3,     3,     2,     1,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     0,     9,
       0,     7,     0,     0,     3,     0,     9,    10,     7,     5
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
  case 66: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 262 "parser.y"
        {
                emit("", (yyvsp[0].sval), "", (yyvsp[-2].sval));
                (yyval.sval) = strdup((yyvsp[-2].sval));
        }
#line 1570 "y.tab.c"
    break;

  case 67: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 267 "parser.y"
        {
                char* temp = genVar();
                emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                emit("", temp, "", (yyvsp[-2].sval));
        }
#line 1580 "y.tab.c"
    break;

  case 68: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 274 "parser.y"
        {
                char* temp = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                emit("", temp, "", (yyvsp[-2].sval));
        }
#line 1590 "y.tab.c"
    break;

  case 70: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 281 "parser.y"
        {
                emit("", (yyvsp[0].sval), "", (yyvsp[-2].sval));  
        }
#line 1598 "y.tab.c"
    break;

  case 71: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 285 "parser.y"
        {
                emit("", (yyvsp[0].sval), "", (yyvsp[-2].sval));
        }
#line 1606 "y.tab.c"
    break;

  case 81: /* arith_expr: arith_expr PLUS term  */
#line 307 "parser.y"
        {
                char* temp = genVar();
                emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1616 "y.tab.c"
    break;

  case 82: /* arith_expr: arith_expr MINUS term  */
#line 313 "parser.y"
        {
                char* temp = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1626 "y.tab.c"
    break;

  case 83: /* arith_expr: term  */
#line 319 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1634 "y.tab.c"
    break;

  case 84: /* term: term MUL factor  */
#line 326 "parser.y"
        {
                char* temp = genVar();
                emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1644 "y.tab.c"
    break;

  case 85: /* term: term DIV factor  */
#line 332 "parser.y"
        {
                char* temp = genVar();
                emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1654 "y.tab.c"
    break;

  case 86: /* term: term MOD factor  */
#line 338 "parser.y"
        {
                char* temp = genVar();
                emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1664 "y.tab.c"
    break;

  case 87: /* term: factor  */
#line 344 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1672 "y.tab.c"
    break;

  case 88: /* factor: IDENTIFIER  */
#line 351 "parser.y"
        {
                (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 1680 "y.tab.c"
    break;

  case 90: /* factor: INT_LITERAL  */
#line 356 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", (yyvsp[0].ival));
                (yyval.sval) = strdup(buffer);
        }
#line 1690 "y.tab.c"
    break;

  case 91: /* factor: FLOAT_LITERAL  */
#line 362 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%f", (yyvsp[0].fval));
                (yyval.sval) = strdup(buffer);
        }
#line 1700 "y.tab.c"
    break;

  case 92: /* factor: CHAR_LITERAL  */
#line 368 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "'%c'", (yyvsp[0].cval));
                (yyval.sval) = strdup(buffer);
        }
#line 1710 "y.tab.c"
    break;

  case 93: /* factor: STRING_LITERAL  */
#line 374 "parser.y"
        {
                (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 1718 "y.tab.c"
    break;

  case 94: /* factor: TRUE  */
#line 378 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", 1);
                (yyval.sval) = strdup(buffer);
        }
#line 1728 "y.tab.c"
    break;

  case 95: /* factor: FALSE  */
#line 384 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", 0);
                (yyval.sval) = strdup(buffer);
        }
#line 1738 "y.tab.c"
    break;

  case 96: /* factor: LPAREN expression RPAREN  */
#line 390 "parser.y"
        {
                (yyval.sval) = (yyvsp[-1].sval);
        }
#line 1746 "y.tab.c"
    break;

  case 97: /* $@1: %empty  */
#line 397 "parser.y"
        {
                char* falseLabel = getLabel();
                char* Lend = getLabel();
                pushIfLabels(falseLabel, Lend);
        }
#line 1756 "y.tab.c"
    break;

  case 98: /* $@2: %empty  */
#line 405 "parser.y"
        {
                emit("goto", "", "", topEnd());
                emit("label", "", "", topFalse());
        }
#line 1765 "y.tab.c"
    break;

  case 99: /* if_stmt: IF LPAREN $@1 expression RPAREN block $@2 elif_list else_opt  */
#line 411 "parser.y"
        {
                emit("label", "", "", topEnd());
                popIfLabels();
        }
#line 1774 "y.tab.c"
    break;

  case 100: /* $@3: %empty  */
#line 419 "parser.y"
        {
                char* prevFalseLabel = topFalse();
                char* nextFalseLabel = getLabel();

                emit("goto", "", "", topEnd());
                emit("label", "", "", prevFalseLabel);

                falseStack[topPtr] = nextFalseLabel;
        }
#line 1788 "y.tab.c"
    break;

  case 103: /* $@4: %empty  */
#line 436 "parser.y"
        {
                emit("label", "", "", topFalse());
        }
#line 1796 "y.tab.c"
    break;


#line 1800 "y.tab.c"

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

#line 452 "parser.y"


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

void emit(char* op, char* arg1, char* arg2, char* result) {
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
        char* s = "hi";
        // Starting the process of parsing the code.
        yyparse();
        printf("Parsing Successful\n");
        printf("Generated three address code:\n");
        for(int i = 0; i < IR_idx; i++){
                printf("%s = %s %s %s\n", IR[i].result, IR[i].arg1, IR[i].op, IR[i].arg2);
        }
        return 0;
}
