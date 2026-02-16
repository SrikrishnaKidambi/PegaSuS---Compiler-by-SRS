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
    BITAND = 298,                  /* BITAND  */
    BITOR = 299,                   /* BITOR  */
    ASSIGN = 300,                  /* ASSIGN  */
    ADD_ASSIGN = 301,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 302,              /* SUB_ASSIGN  */
    SEMICOLON = 303,               /* SEMICOLON  */
    COMMA = 304,                   /* COMMA  */
    LPAREN = 305,                  /* LPAREN  */
    RPAREN = 306,                  /* RPAREN  */
    LBRACE = 307,                  /* LBRACE  */
    RBRACE = 308,                  /* RBRACE  */
    LBRACKET = 309,                /* LBRACKET  */
    RBRACKET = 310                 /* RBRACKET  */
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
#define BITAND 298
#define BITOR 299
#define ASSIGN 300
#define ADD_ASSIGN 301
#define SUB_ASSIGN 302
#define SEMICOLON 303
#define COMMA 304
#define LPAREN 305
#define RPAREN 306
#define LBRACE 307
#define RBRACE 308
#define LBRACKET 309
#define RBRACKET 310

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

#line 310 "y.tab.c"

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
  YYSYMBOL_BITAND = 43,                    /* BITAND  */
  YYSYMBOL_BITOR = 44,                     /* BITOR  */
  YYSYMBOL_ASSIGN = 45,                    /* ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 46,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 47,                /* SUB_ASSIGN  */
  YYSYMBOL_SEMICOLON = 48,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 49,                     /* COMMA  */
  YYSYMBOL_LPAREN = 50,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 51,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 52,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 53,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 54,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 55,                  /* RBRACKET  */
  YYSYMBOL_YYACCEPT = 56,                  /* $accept  */
  YYSYMBOL_program = 57,                   /* program  */
  YYSYMBOL_element = 58,                   /* element  */
  YYSYMBOL_entity_decl = 59,               /* entity_decl  */
  YYSYMBOL_entity_body = 60,               /* entity_body  */
  YYSYMBOL_entity_member = 61,             /* entity_member  */
  YYSYMBOL_constructor_decl = 62,          /* constructor_decl  */
  YYSYMBOL_method_decl = 63,               /* method_decl  */
  YYSYMBOL_access_var_decl = 64,           /* access_var_decl  */
  YYSYMBOL_access_modifier = 65,           /* access_modifier  */
  YYSYMBOL_object_decl = 66,               /* object_decl  */
  YYSYMBOL_arg_list_opt = 67,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 68,                  /* arg_list  */
  YYSYMBOL_statement = 69,                 /* statement  */
  YYSYMBOL_block = 70,                     /* block  */
  YYSYMBOL_stmt_list = 71,                 /* stmt_list  */
  YYSYMBOL_var_decl = 72,                  /* var_decl  */
  YYSYMBOL_id_list = 73,                   /* id_list  */
  YYSYMBOL_type = 74,                      /* type  */
  YYSYMBOL_array_decl = 75,                /* array_decl  */
  YYSYMBOL_array_init = 76,                /* array_init  */
  YYSYMBOL_expr_list = 77,                 /* expr_list  */
  YYSYMBOL_function_decl = 78,             /* function_decl  */
  YYSYMBOL_func_type = 79,                 /* func_type  */
  YYSYMBOL_param_list_opt = 80,            /* param_list_opt  */
  YYSYMBOL_param_list = 81,                /* param_list  */
  YYSYMBOL_param = 82,                     /* param  */
  YYSYMBOL_return_stmt = 83,               /* return_stmt  */
  YYSYMBOL_expr_stmt = 84,                 /* expr_stmt  */
  YYSYMBOL_expression = 85,                /* expression  */
  YYSYMBOL_indexed_id = 86,                /* indexed_id  */
  YYSYMBOL_assignment = 87,                /* assignment  */
  YYSYMBOL_logic_expr = 88,                /* logic_expr  */
  YYSYMBOL_bitwise_expr = 89,              /* bitwise_expr  */
  YYSYMBOL_rel_expr = 90,                  /* rel_expr  */
  YYSYMBOL_arith_expr = 91,                /* arith_expr  */
  YYSYMBOL_term = 92,                      /* term  */
  YYSYMBOL_factor = 93,                    /* factor  */
  YYSYMBOL_if_stmt = 94,                   /* if_stmt  */
  YYSYMBOL_95_1 = 95,                      /* $@1  */
  YYSYMBOL_96_2 = 96,                      /* $@2  */
  YYSYMBOL_97_3 = 97,                      /* $@3  */
  YYSYMBOL_elif_list = 98,                 /* elif_list  */
  YYSYMBOL_99_4 = 99,                      /* $@4  */
  YYSYMBOL_100_5 = 100,                    /* $@5  */
  YYSYMBOL_101_6 = 101,                    /* $@6  */
  YYSYMBOL_else_opt = 102,                 /* else_opt  */
  YYSYMBOL_103_7 = 103,                    /* $@7  */
  YYSYMBOL_for_stmt = 104,                 /* for_stmt  */
  YYSYMBOL_105_8 = 105,                    /* $@8  */
  YYSYMBOL_106_9 = 106,                    /* $@9  */
  YYSYMBOL_107_10 = 107,                   /* $@10  */
  YYSYMBOL_108_11 = 108,                   /* $@11  */
  YYSYMBOL_io_stmt = 109                   /* io_stmt  */
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
#define YYLAST   314

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  118
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  235

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
       0,   120,   120,   121,   125,   126,   127,   128,   132,   136,
     137,   141,   142,   143,   147,   151,   155,   159,   160,   164,
     168,   169,   173,   174,   178,   179,   180,   181,   182,   183,
     184,   185,   189,   190,   194,   195,   199,   203,   211,   212,
     216,   217,   218,   219,   220,   221,   225,   226,   227,   231,
     235,   236,   240,   244,   245,   249,   250,   254,   255,   259,
     263,   264,   268,   269,   273,   277,   281,   286,   294,   301,
     302,   306,   310,   317,   323,   329,   335,   342,   348,   354,
     360,   366,   372,   378,   385,   391,   397,   404,   410,   416,
     422,   429,   433,   434,   440,   446,   452,   456,   462,   468,
     476,   482,   487,   475,   501,   510,   515,   500,   519,   524,
     523,   528,   535,   545,   549,   555,   533,   575,   576
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
  "MUL", "DIV", "MOD", "GT", "LT", "EQ", "AND", "OR", "NOT", "BITAND",
  "BITOR", "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "SEMICOLON", "COMMA",
  "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET", "RBRACKET",
  "$accept", "program", "element", "entity_decl", "entity_body",
  "entity_member", "constructor_decl", "method_decl", "access_var_decl",
  "access_modifier", "object_decl", "arg_list_opt", "arg_list",
  "statement", "block", "stmt_list", "var_decl", "id_list", "type",
  "array_decl", "array_init", "expr_list", "function_decl", "func_type",
  "param_list_opt", "param_list", "param", "return_stmt", "expr_stmt",
  "expression", "indexed_id", "assignment", "logic_expr", "bitwise_expr",
  "rel_expr", "arith_expr", "term", "factor", "if_stmt", "$@1", "$@2",
  "$@3", "elif_list", "$@4", "$@5", "$@6", "else_opt", "$@7", "for_stmt",
  "$@8", "$@9", "$@10", "$@11", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-168)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-46)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -168,    96,  -168,  -168,  -168,  -168,  -168,  -168,  -168,   -33,
     -26,  -168,  -168,   -10,   216,   -16,    28,   254,  -168,  -168,
    -168,  -168,   105,  -168,   240,   158,  -168,  -168,  -168,  -168,
    -168,  -168,     1,  -168,  -168,    36,  -168,  -168,    11,    18,
    -168,    -6,    26,  -168,    35,   106,  -168,  -168,  -168,  -168,
    -168,   240,   240,   147,  -168,    17,    23,    71,    84,    69,
      16,   240,   240,   240,    62,  -168,  -168,    80,   257,  -168,
    -168,   209,     1,    93,    95,    92,   -22,   112,  -168,   240,
     105,   105,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   240,  -168,   107,   240,  -168,  -168,    98,   111,
     135,   110,  -168,  -168,  -168,   113,  -168,  -168,  -168,   -36,
     115,   240,  -168,   139,   126,  -168,  -168,   137,  -168,   136,
     106,   106,    74,    74,    74,  -168,  -168,  -168,  -168,   132,
     133,   -11,   240,   240,   155,   153,  -168,   144,   163,   169,
     150,  -168,    88,   148,   240,  -168,  -168,  -168,   152,  -168,
    -168,  -168,  -168,  -168,    88,  -168,  -168,   154,   166,   240,
     157,   164,   165,  -168,  -168,   176,   173,   160,  -168,   175,
    -168,    88,    -5,   240,   180,   -17,  -168,  -168,   183,   178,
    -168,   175,    88,  -168,   185,   184,   215,   200,   198,   201,
    -168,  -168,   240,  -168,  -168,   224,  -168,  -168,   245,  -168,
     175,   206,  -168,   212,   240,  -168,   208,   229,   274,   240,
    -168,    88,  -168,  -168,   238,  -168,  -168,  -168,  -168,   236,
    -168,   240,   175,   237,   175,  -168,  -168,   175,  -168,   246,
    -168,   175,  -168,   245,  -168
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     1,    40,    41,    42,    43,    44,    54,     0,
       0,    97,    98,     0,     0,     0,     0,    91,    93,    94,
      95,    96,     0,    63,     0,     0,     2,     6,     7,     4,
      31,    24,    53,    25,     5,     0,    30,    26,     0,    92,
      64,    72,    76,    79,    83,    86,    90,    27,    28,    29,
     100,     0,     0,    91,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    75,     0,    91,    33,
      35,     0,     0,     0,     0,    39,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,     0,     0,    60,    10,     0,     0,
       0,     0,    66,    67,    68,     0,    99,    32,    34,     0,
       0,     0,    36,     0,     0,    69,    74,    73,    77,    78,
      84,    85,    80,    81,    82,    87,    88,    89,   101,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
       0,    38,    56,     0,     0,   118,    17,    18,     0,     8,
       9,    11,    12,    13,     0,    70,    71,     0,     0,     0,
       0,     0,     0,    37,    45,     0,     0,    55,    58,     0,
     113,    56,     0,    21,     0,     0,    51,    46,     0,     0,
      59,     0,     0,   102,     0,     0,     0,     0,     0,    20,
      23,   117,     0,    49,    47,     0,    52,    57,   108,   114,
       0,     0,    16,     0,     0,    50,     0,     0,   111,     0,
      14,    56,    19,    22,     0,   104,   109,   103,   115,     0,
      48,     0,     0,     0,     0,   105,   110,     0,    15,     0,
     116,     0,   106,   108,   107
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,   -18,  -167,  -168,  -168,  -168,     0,  -168,
    -168,  -168,  -168,  -168,  -163,  -168,   114,  -168,  -168,   -14,
      -1,   -56,   -19,    46,  -168,    63,    68,   -39,  -168,  -168,
    -168,  -168,    56,  -168,  -168,  -168,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,  -168
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    26,    27,   131,   150,   151,   152,   153,   154,
      28,   188,   189,    29,    30,    71,    31,    76,   165,    33,
     160,   175,    34,    35,   166,   167,   168,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    92,
     143,   198,   208,   221,   229,   233,   217,   222,    48,   129,
     184,   209,   223,    49
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,    32,   183,    66,   102,   103,   104,    70,   185,   137,
      67,    56,   146,   147,   196,   186,   148,    50,   138,    73,
      74,    65,   187,   115,    51,    72,   112,   113,    75,    11,
      12,   101,   192,   210,    80,    81,   193,    93,    94,   102,
      52,    16,   149,    53,    18,    19,    20,    21,   219,   105,
     125,   126,   127,   108,    57,   226,    77,   228,    22,    78,
     230,   116,   117,    79,   232,    96,    24,    84,    85,    82,
      83,    72,    86,    87,    88,    97,   155,   156,   128,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,     3,     4,     5,     6,     7,     2,   140,    98,     3,
       4,     5,     6,     7,     8,     9,    84,    85,    10,    11,
      12,    99,    13,    14,   100,   164,    63,    15,    11,    12,
     109,    16,   110,    17,    18,    19,    20,    21,   118,   119,
     170,   106,    64,    18,    19,    20,    21,   111,    22,   114,
      89,    90,    91,   132,    23,   176,    24,    22,    25,   122,
     123,   124,   120,   121,   172,    24,   133,   134,   130,   190,
     135,     3,     4,     5,     6,     7,   141,     9,   136,   139,
      10,    11,    12,    58,    13,    14,   142,    80,   205,    82,
     144,   145,   157,    16,   158,    68,    18,    19,    20,    21,
     213,   161,    95,    61,    62,   218,   159,   162,   163,   169,
      22,    63,   171,   180,   173,   177,    23,   225,    24,   182,
      25,    69,     3,     4,     5,     6,     7,   174,     9,   178,
     179,    10,    11,    12,   181,    13,    14,    25,   191,    11,
      12,   194,   195,   199,    16,   200,    68,    18,    19,    20,
      21,    16,   201,    53,    18,    19,    20,    21,   202,   203,
     204,    22,   206,    11,    12,   207,   211,    23,    22,    24,
     212,    25,   107,   214,    54,    16,    24,    53,    18,    19,
      20,    21,   -45,   -45,   -45,   -45,   -45,    11,    12,   215,
      58,    59,    22,    58,   -45,   216,   220,   224,   227,   234,
      24,    64,    18,    19,    20,    21,   197,   231,     0,    60,
      61,    62,    60,    61,    62,     0,     0,     0,    63,     0,
       0,    63,     0,     0,    24
};

static const yytype_int16 yycheck[] =
{
      14,     1,   169,    22,    60,    61,    62,    25,   171,    45,
      24,    27,    23,    24,   181,    20,    27,    50,    54,    18,
      19,    22,    27,    79,    50,    25,    48,    49,    27,    13,
      14,    15,    49,   200,    40,    41,    53,    51,    52,    95,
      50,    25,    53,    27,    28,    29,    30,    31,   211,    63,
      89,    90,    91,    71,    26,   222,    20,   224,    42,    48,
     227,    80,    81,    45,   231,    48,    50,    32,    33,    43,
      44,    71,    37,    38,    39,    52,   132,   133,    92,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,     3,     4,     5,     6,     7,     0,   111,    27,     3,
       4,     5,     6,     7,     8,     9,    32,    33,    12,    13,
      14,    27,    16,    17,    45,    27,    54,    21,    13,    14,
      27,    25,    27,    27,    28,    29,    30,    31,    82,    83,
     144,    51,    27,    28,    29,    30,    31,    45,    42,    27,
      34,    35,    36,    45,    48,   159,    50,    42,    52,    86,
      87,    88,    84,    85,   154,    50,    45,    22,    51,   173,
      50,     3,     4,     5,     6,     7,    27,     9,    55,    54,
      12,    13,    14,    26,    16,    17,    50,    40,   192,    43,
      48,    48,    27,    25,    31,    27,    28,    29,    30,    31,
     204,    28,    45,    46,    47,   209,    52,    28,    48,    51,
      42,    54,    50,    27,    50,    48,    48,   221,    50,    49,
      52,    53,     3,     4,     5,     6,     7,    51,     9,    55,
      55,    12,    13,    14,    51,    16,    17,    52,    48,    13,
      14,    48,    54,    48,    25,    51,    27,    28,    29,    30,
      31,    25,    27,    27,    28,    29,    30,    31,    48,    51,
      49,    42,    28,    13,    14,    10,    50,    48,    42,    50,
      48,    52,    53,    55,    48,    25,    50,    27,    28,    29,
      30,    31,    18,    19,    20,    18,    19,    13,    14,    50,
      26,    27,    42,    26,    27,    11,    48,    51,    51,   233,
      50,    27,    28,    29,    30,    31,   182,    51,    -1,    45,
      46,    47,    45,    46,    47,    -1,    -1,    -1,    54,    -1,
      -1,    54,    -1,    -1,    50
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    57,     0,     3,     4,     5,     6,     7,     8,     9,
      12,    13,    14,    16,    17,    21,    25,    27,    28,    29,
      30,    31,    42,    48,    50,    52,    58,    59,    66,    69,
      70,    72,    74,    75,    78,    79,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,   104,   109,
      50,    50,    50,    27,    48,    85,    27,    26,    26,    27,
      45,    46,    47,    54,    27,    86,    88,    85,    27,    53,
      69,    71,    74,    18,    19,    27,    73,    20,    48,    45,
      40,    41,    43,    44,    32,    33,    37,    38,    39,    34,
      35,    36,    95,    85,    85,    45,    48,    52,    27,    27,
      45,    15,    87,    87,    87,    85,    51,    53,    69,    27,
      27,    45,    48,    49,    27,    87,    88,    88,    89,    89,
      92,    92,    91,    91,    91,    93,    93,    93,    85,   105,
      51,    60,    45,    45,    22,    50,    55,    45,    54,    54,
      85,    27,    50,    96,    48,    48,    23,    24,    27,    53,
      61,    62,    63,    64,    65,    87,    87,    27,    31,    52,
      76,    28,    28,    48,    27,    74,    80,    81,    82,    51,
      85,    50,    74,    50,    51,    77,    85,    48,    55,    55,
      27,    51,    49,    70,   106,    80,    20,    27,    67,    68,
      85,    48,    49,    53,    48,    54,    70,    82,    97,    48,
      51,    27,    48,    51,    49,    85,    28,    10,    98,   107,
      70,    50,    48,    85,    55,    50,    11,   102,    85,    80,
      48,    99,   103,   108,    51,    85,    70,    51,    70,   100,
      70,    51,    70,   101,    98
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    57,    58,    58,    58,    58,    59,    60,
      60,    61,    61,    61,    62,    63,    64,    65,    65,    66,
      67,    67,    68,    68,    69,    69,    69,    69,    69,    69,
      69,    69,    70,    70,    71,    71,    72,    72,    73,    73,
      74,    74,    74,    74,    74,    74,    75,    75,    75,    76,
      77,    77,    78,    79,    79,    80,    80,    81,    81,    82,
      83,    83,    84,    84,    85,    86,    87,    87,    87,    87,
      87,    87,    87,    88,    88,    88,    88,    89,    89,    89,
      90,    90,    90,    90,    91,    91,    91,    92,    92,    92,
      92,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      95,    96,    97,    94,    99,   100,   101,    98,    98,   103,
     102,   102,   105,   106,   107,   108,   104,   109,   109
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
       5,     5,     1,     3,     3,     2,     1,     3,     3,     1,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     0,     0,    10,     0,     0,     0,     9,     0,     0,
       3,     0,     0,     0,     0,     0,    13,     7,     5
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
  case 36: /* var_decl: type id_list SEMICOLON  */
#line 200 "parser.y"
        {
		//no need to emit anything here and later stage add code for symbol table here
	}
#line 1582 "y.tab.c"
    break;

  case 37: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 204 "parser.y"
        {
		//code for symbol table comes here
		emit("=",(yyvsp[-1].sval),"",(yyvsp[-3].sval));
	}
#line 1591 "y.tab.c"
    break;

  case 66: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 282 "parser.y"
        {
		emit("=", (yyvsp[0].sval), "", (yyvsp[-2].sval));
		(yyval.sval) = strdup((yyvsp[-2].sval));
	}
#line 1600 "y.tab.c"
    break;

  case 67: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 287 "parser.y"
        {
		char* temp = genVar();
		emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
		emit("=", temp, "", (yyvsp[-2].sval));
		(yyval.sval) = temp;
	}
#line 1611 "y.tab.c"
    break;

  case 68: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 295 "parser.y"
        {
                char* temp = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                emit("=", temp, "", (yyvsp[-2].sval));
		(yyval.sval) = temp;
        }
#line 1622 "y.tab.c"
    break;

  case 70: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 303 "parser.y"
        {
                emit("", (yyvsp[0].sval), "", (yyvsp[-2].sval));  
        }
#line 1630 "y.tab.c"
    break;

  case 71: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 307 "parser.y"
        {
                emit("", (yyvsp[0].sval), "", (yyvsp[-2].sval));
        }
#line 1638 "y.tab.c"
    break;

  case 72: /* assignment: logic_expr  */
#line 311 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1646 "y.tab.c"
    break;

  case 73: /* logic_expr: logic_expr OR logic_expr  */
#line 318 "parser.y"
        {
		char* temp = genVar();
		emit("||",(yyvsp[-2].sval),(yyvsp[0].sval),temp); //temp = $1 || $2
		(yyval.sval) = temp;
	}
#line 1656 "y.tab.c"
    break;

  case 74: /* logic_expr: logic_expr AND logic_expr  */
#line 324 "parser.y"
        {
                char* temp = genVar();
                emit("&&",(yyvsp[-2].sval),(yyvsp[0].sval),temp); //temp = $1 && $2
                (yyval.sval) = temp;
        }
#line 1666 "y.tab.c"
    break;

  case 75: /* logic_expr: NOT logic_expr  */
#line 330 "parser.y"
        {
                char* temp = genVar();
                emit("!",(yyvsp[0].sval),"",temp); //temp = !$2
                (yyval.sval) = temp;
        }
#line 1676 "y.tab.c"
    break;

  case 76: /* logic_expr: bitwise_expr  */
#line 336 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1684 "y.tab.c"
    break;

  case 77: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 343 "parser.y"
        {
		char* temp = genVar();
		emit("&", (yyvsp[-2].sval), (yyvsp[0].sval) , temp); //temp = $1 & $3
		(yyval.sval) = temp;
	}
#line 1694 "y.tab.c"
    break;

  case 78: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 349 "parser.y"
        {
		char* temp = genVar();
		emit("|",(yyvsp[-2].sval),(yyvsp[0].sval), temp);
		(yyval.sval) = temp;
	}
#line 1704 "y.tab.c"
    break;

  case 79: /* bitwise_expr: rel_expr  */
#line 355 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1712 "y.tab.c"
    break;

  case 80: /* rel_expr: arith_expr GT arith_expr  */
#line 361 "parser.y"
        {
		char* temp = genVar();
		emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), temp); //temp = $1 > $3
		(yyval.sval) = temp;
	}
#line 1722 "y.tab.c"
    break;

  case 81: /* rel_expr: arith_expr LT arith_expr  */
#line 367 "parser.y"
        {
		char* temp = genVar();
		emit("<",(yyvsp[-2].sval) , (yyvsp[0].sval), temp); //temp = $1 < $3
		(yyval.sval) = temp;
	}
#line 1732 "y.tab.c"
    break;

  case 82: /* rel_expr: arith_expr EQ arith_expr  */
#line 373 "parser.y"
        {
		char* temp = genVar();
		emit("==",(yyvsp[-2].sval), (yyvsp[0].sval), temp); //temp = $1 == $3
		(yyval.sval) = temp;
	}
#line 1742 "y.tab.c"
    break;

  case 83: /* rel_expr: arith_expr  */
#line 379 "parser.y"
        {
		(yyval.sval) = (yyvsp[0].sval);
	}
#line 1750 "y.tab.c"
    break;

  case 84: /* arith_expr: arith_expr PLUS term  */
#line 386 "parser.y"
        {
                char* temp = genVar();
                emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1760 "y.tab.c"
    break;

  case 85: /* arith_expr: arith_expr MINUS term  */
#line 392 "parser.y"
        {
                char* temp = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1770 "y.tab.c"
    break;

  case 86: /* arith_expr: term  */
#line 398 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1778 "y.tab.c"
    break;

  case 87: /* term: term MUL factor  */
#line 405 "parser.y"
        {
                char* temp = genVar();
                emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1788 "y.tab.c"
    break;

  case 88: /* term: term DIV factor  */
#line 411 "parser.y"
        {
                char* temp = genVar();
                emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1798 "y.tab.c"
    break;

  case 89: /* term: term MOD factor  */
#line 417 "parser.y"
        {
                char* temp = genVar();
                emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), temp);
                (yyval.sval) = temp;
        }
#line 1808 "y.tab.c"
    break;

  case 90: /* term: factor  */
#line 423 "parser.y"
        {
                (yyval.sval) = (yyvsp[0].sval);
        }
#line 1816 "y.tab.c"
    break;

  case 91: /* factor: IDENTIFIER  */
#line 430 "parser.y"
        {
                (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 1824 "y.tab.c"
    break;

  case 93: /* factor: INT_LITERAL  */
#line 435 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", (yyvsp[0].ival));
                (yyval.sval) = strdup(buffer);
        }
#line 1834 "y.tab.c"
    break;

  case 94: /* factor: FLOAT_LITERAL  */
#line 441 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%f", (yyvsp[0].fval));
                (yyval.sval) = strdup(buffer);
        }
#line 1844 "y.tab.c"
    break;

  case 95: /* factor: CHAR_LITERAL  */
#line 447 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "'%c'", (yyvsp[0].cval));
                (yyval.sval) = strdup(buffer);
        }
#line 1854 "y.tab.c"
    break;

  case 96: /* factor: STRING_LITERAL  */
#line 453 "parser.y"
        {
                (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 1862 "y.tab.c"
    break;

  case 97: /* factor: TRUE  */
#line 457 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", 1);
                (yyval.sval) = strdup(buffer);
        }
#line 1872 "y.tab.c"
    break;

  case 98: /* factor: FALSE  */
#line 463 "parser.y"
        {
                char buffer[20];
                sprintf(buffer, "%d", 0);
                (yyval.sval) = strdup(buffer);
        }
#line 1882 "y.tab.c"
    break;

  case 99: /* factor: LPAREN expression RPAREN  */
#line 469 "parser.y"
        {
                (yyval.sval) = (yyvsp[-1].sval);
        }
#line 1890 "y.tab.c"
    break;

  case 100: /* $@1: %empty  */
#line 476 "parser.y"
        {
                char* falseLabel = getLabel();
                char* Lend = getLabel();
                pushIfLabels(falseLabel, Lend);
        }
#line 1900 "y.tab.c"
    break;

  case 101: /* $@2: %empty  */
#line 482 "parser.y"
        {
		emit("ifFalse",(yyvsp[0].sval), "",topFalse());
	}
#line 1908 "y.tab.c"
    break;

  case 102: /* $@3: %empty  */
#line 487 "parser.y"
        {
		emit("goto", "", "", topEnd());
		emit("label", "", "", topFalse());
	}
#line 1917 "y.tab.c"
    break;

  case 103: /* if_stmt: IF LPAREN $@1 expression $@2 RPAREN block $@3 elif_list else_opt  */
#line 493 "parser.y"
        {
                emit("label", "", "", topEnd());
                popIfLabels();
        }
#line 1926 "y.tab.c"
    break;

  case 104: /* $@4: %empty  */
#line 501 "parser.y"
        {
		char* prevFalseLabel = topFalse();
		char* nextFalseLabel = getLabel();
		
		//emit("goto", "", "", topEnd());
		//emit("label", "", "", prevFalseLabel);
		falseStack[topPtr] = nextFalseLabel;
	}
#line 1939 "y.tab.c"
    break;

  case 105: /* $@5: %empty  */
#line 510 "parser.y"
        {
		emit("ifFalse",(yyvsp[0].sval),"",topFalse());
	}
#line 1947 "y.tab.c"
    break;

  case 106: /* $@6: %empty  */
#line 515 "parser.y"
        {
		emit("goto", "", "", topEnd());
	}
#line 1955 "y.tab.c"
    break;

  case 109: /* $@7: %empty  */
#line 524 "parser.y"
        {
                emit("label", "", "", topFalse());
        }
#line 1963 "y.tab.c"
    break;

  case 112: /* $@8: %empty  */
#line 535 "parser.y"
        {
		char* Lbegin = getLabel();
		char* Lend = getLabel();
		
		pushIfLabels(Lbegin,Lend);
		
		emit("label","","",Lbegin);
	}
#line 1976 "y.tab.c"
    break;

  case 113: /* $@9: %empty  */
#line 545 "parser.y"
        {
		emit("ifFalse",(yyvsp[0].sval),"",topEnd());
	}
#line 1984 "y.tab.c"
    break;

  case 114: /* $@10: %empty  */
#line 549 "parser.y"
        {
		forDepth++;
		forIncIdx[forDepth] = 0;
		inForIncrement[forDepth]=1;
	}
#line 1994 "y.tab.c"
    break;

  case 115: /* $@11: %empty  */
#line 555 "parser.y"
        {
		inForIncrement[forDepth]=0;
	}
#line 2002 "y.tab.c"
    break;

  case 116: /* for_stmt: FOR LPAREN expression $@8 SEMICOLON expression $@9 SEMICOLON $@10 expression $@11 RPAREN block  */
#line 559 "parser.y"
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
#line 2020 "y.tab.c"
    break;


#line 2024 "y.tab.c"

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

#line 579 "parser.y"


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
