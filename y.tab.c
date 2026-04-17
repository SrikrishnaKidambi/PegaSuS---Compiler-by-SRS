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
#include "optimizer.h"
#include "asm_gen.h"
#include "transpiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;
extern int yylineno;
extern char* yytext;
// Add near your other globals at the top of parser.y
#ifndef QUAD_DEFINED
typedef struct {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
} Quad;
#define QUAD_DEFINED
#endif

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
static char arr_init_vals[256][32];
static int  arr_init_count = 0;

//store the idx for functions/methods and contructors (fix to issue pointed by Raghavendra)
static int pending_constr_ir_idx = -1;
static int pending_method_ir_idx = -1;
static int pending_func_ir_idx = -1;

DataType last_expr_type = DT_UNKNOWN;
DataType current_array_elem_type = DT_UNKNOWN;
int array_type_errors = 0;

// Utitilies for handling semantic errors in functions and function calls
static Symbol* current_function = NULL;         // A global variable that is used for storing the function(a symbol right!!) so that we can know what is the expected
                                                // return type
static DataType call_arg_types[64];	// Array for storing the type of argument in each function call
static int call_arg_count = 0;		// Store the number of arguments collected till now

// helper function for generating ir_name for a variable name in the loops 
static const char* ir_name_of(const char* name){
	Symbol* sym = lookup(current_scope, name);
	if(sym && sym->ir_name[0] != '\0'){
		return sym->ir_name;
	}
	return name;
}

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


#line 184 "y.tab.c"

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
#line 114 "parser.y"

    int       ival;
    float     fval;
    char      cval;
    char*     sval;
    DataType  dtype;
    AccessMod access;

#line 356 "y.tab.c"

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
  YYSYMBOL_121_23 = 121,                   /* $@23  */
  YYSYMBOL_122_24 = 122,                   /* $@24  */
  YYSYMBOL_123_25 = 123,                   /* $@25  */
  YYSYMBOL_124_26 = 124,                   /* $@26  */
  YYSYMBOL_for_stmt = 125,                 /* for_stmt  */
  YYSYMBOL_126_27 = 126,                   /* $@27  */
  YYSYMBOL_for_header = 127,               /* for_header  */
  YYSYMBOL_128_28 = 128,                   /* $@28  */
  YYSYMBOL_129_29 = 129,                   /* $@29  */
  YYSYMBOL_130_30 = 130,                   /* $@30  */
  YYSYMBOL_for_init_opt = 131,             /* for_init_opt  */
  YYSYMBOL_for_cond_opt = 132,             /* for_cond_opt  */
  YYSYMBOL_for_inc_opt = 133,              /* for_inc_opt  */
  YYSYMBOL_var_decl_no_semi = 134,         /* var_decl_no_semi  */
  YYSYMBOL_io_stmt = 135                   /* io_stmt  */
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
#define YYLAST   538

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  80
/* YYNRULES -- Number of rules.  */
#define YYNRULES  173
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  357

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
       0,   156,   156,   157,   161,   162,   163,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   185,   184,   224,
     223,   234,   235,   239,   240,   241,   245,   244,   334,   333,
     346,   345,   439,   438,   521,   520,   531,   545,   561,   562,
     566,   612,   669,   670,   674,   681,   691,   690,   700,   701,
     709,   710,   715,   722,   743,   750,   761,   768,   775,   776,
     777,   778,   779,   783,   801,   819,   818,   853,   852,   893,
     897,   901,   907,   916,   928,   945,   944,  1019,  1018,  1090,
    1089,  1100,  1101,  1105,  1106,  1110,  1111,  1116,  1137,  1160,
    1174,  1186,  1194,  1195,  1199,  1202,  1214,  1235,  1248,  1260,
    1273,  1275,  1301,  1327,  1331,  1333,  1335,  1337,  1341,  1343,
    1345,  1349,  1354,  1358,  1362,  1366,  1373,  1379,  1383,  1389,
    1395,  1401,  1405,  1412,  1480,  1549,  1556,  1557,  1558,  1559,
    1560,  1561,  1562,  1563,  1568,  1585,  1587,  1592,  1567,  1596,
    1605,  1622,  1624,  1629,  1604,  1631,  1636,  1635,  1648,  1665,
    1667,  1672,  1647,  1674,  1679,  1678,  1702,  1711,  1718,  1725,
    1710,  1730,  1731,  1732,  1736,  1737,  1741,  1742,  1746,  1750,
    1764,  1772,  1774,  1776
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
  "$@20", "$@21", "else_opt", "$@22", "$@23", "$@24", "$@25", "$@26",
  "for_stmt", "$@27", "for_header", "$@28", "$@29", "$@30", "for_init_opt",
  "for_cond_opt", "for_inc_opt", "var_decl_no_semi", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-248)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-155)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -248,   218,  -248,   -12,  -248,  -248,  -248,  -248,  -248,  -248,
       5,    17,  -248,  -248,    36,   363,    33,    61,   450,   148,
    -248,    97,  -248,  -248,  -248,  -248,   482,   477,  -248,  -248,
    -248,  -248,  -248,  -248,    -2,  -248,  -248,    85,  -248,  -248,
      60,    72,  -248,   123,   155,  -248,   142,    77,  -248,  -248,
    -248,  -248,  -248,   108,   356,   450,    82,  -248,   386,    87,
    -248,    90,   106,   110,  -248,   317,   101,   107,   450,   450,
     116,    11,   450,   450,   171,    71,  -248,  -248,  -248,   113,
     139,   118,   174,   141,  -248,   450,   477,   477,   482,   482,
     482,   482,   482,   482,   482,   482,   482,   482,   151,   450,
     154,   416,    14,  -248,   450,  -248,   173,   183,   177,  -248,
    -248,   496,  -248,   277,    -2,  -248,   193,   208,   203,  -248,
     206,   411,   213,  -248,  -248,  -248,  -248,   207,   209,    20,
      54,   443,  -248,  -248,  -248,   197,  -248,   202,  -248,   159,
     159,   159,    77,    77,  -248,  -248,  -248,   220,  -248,   220,
     217,  -248,   220,   225,  -248,   226,   450,  -248,   264,   450,
    -248,  -248,   240,   450,  -248,   450,   237,   235,   238,   236,
    -248,  -248,   242,  -248,   248,  -248,   247,   466,   244,   261,
     268,  -248,   269,  -248,   256,  -248,  -248,  -248,   243,    34,
     270,  -248,    24,  -248,  -248,   450,   271,  -248,   273,   267,
     274,   276,   280,   281,   284,  -248,    24,   304,  -248,   450,
     450,   299,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,
     257,  -248,   300,   301,   309,   305,  -248,   306,   450,   307,
     308,   450,   310,   314,   318,   315,   321,    -5,   323,   330,
     220,  -248,  -248,   332,   333,   340,   344,    -1,    21,  -248,
    -248,   220,    24,  -248,   345,  -248,  -248,     9,  -248,  -248,
     337,   450,    65,  -248,   342,   450,   220,   220,  -248,  -248,
    -248,    24,   377,   347,   346,   349,   352,  -248,  -248,   353,
    -248,   450,   348,    73,  -248,   358,  -248,   369,  -248,  -248,
     393,   450,   376,   378,  -248,  -248,  -248,  -248,  -248,  -248,
     372,  -248,   450,   375,   382,   399,  -248,  -248,   220,   220,
     388,   389,   404,  -248,    74,  -248,  -248,   423,  -248,   406,
    -248,  -248,    24,    24,   434,  -248,   450,   410,   220,  -248,
     412,   414,   417,  -248,  -248,  -248,   220,   220,   220,   431,
     450,  -248,  -248,  -248,  -248,  -248,   220,   432,  -248,  -248,
     393,   220,  -248,  -248,   393,   399,  -248
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,    58,    59,    60,    61,    62,    82,
       0,     0,   131,   132,     0,     0,     0,     0,     0,     0,
      93,   125,   130,   127,   128,   129,     0,     0,     2,     4,
       6,    14,    15,     7,    81,     8,     5,     0,    13,     9,
       0,   126,    94,   103,   107,   110,   114,   117,   121,    10,
      11,    12,    16,     0,     0,     0,     0,    90,   125,     0,
      17,     0,     0,     0,    48,     0,     0,     0,    43,     0,
      57,     0,     0,     0,     0,   125,   126,   122,   106,     0,
       0,    57,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   163,     0,    91,     0,    89,     0,     0,     0,   133,
      49,   125,    51,     0,     0,    77,   124,     0,    42,    45,
       0,     0,     0,    97,    98,    99,    54,     0,     0,     0,
       0,     0,    52,    75,   100,   104,   105,   109,   108,   113,
     111,   112,   115,   116,   118,   119,   120,     0,   135,     0,
       0,   162,     0,     0,   161,     0,     0,    22,     0,     0,
      47,    50,     0,     0,   123,     0,    95,     0,     0,     0,
      56,   124,     0,    65,     0,    67,     0,   125,     0,     0,
       0,   139,     0,   156,   168,   155,   157,   172,     0,     0,
       0,   101,    84,   102,    44,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,    53,    84,     0,   136,     0,
     165,     0,    38,    39,    18,    26,    21,    23,    24,    25,
       0,    20,     0,     0,     0,    83,    86,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,   124,     0,     0,
       0,   169,   164,     0,     0,     0,     0,     0,     0,    88,
      87,     0,     0,    96,     0,   170,    63,     0,    74,    66,
       0,     0,     0,    68,     0,    43,     0,     0,   137,   158,
     173,    84,     0,     0,     0,     0,     0,    78,    85,     0,
      69,     0,     0,     0,    70,     0,   171,     0,    76,    80,
     145,   167,     0,     0,    32,    37,    30,    36,    40,    73,
       0,    72,     0,     0,     0,   153,   166,   159,     0,     0,
       0,     0,     0,    64,     0,    41,   140,   146,   138,     0,
      27,    29,    84,    84,     0,    71,     0,     0,     0,   160,
       0,     0,     0,   141,   148,   147,     0,     0,     0,     0,
       0,    33,    31,    35,   142,   149,     0,     0,   143,   150,
     145,     0,   144,   151,   145,   153,   152
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -248,  -248,  -248,   -52,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -213,
    -248,  -140,  -248,  -248,  -248,   -26,     0,  -248,  -248,  -248,
    -248,  -248,  -248,  -247,  -248,  -248,  -248,  -248,  -248,  -204,
    -248,   187,  -248,  -248,   -15,   -16,   -67,    -4,   126,  -248,
      80,   135,    -6,  -248,  -248,  -248,  -248,  -248,  -240,  -248,
    -248,  -248,  -248,    89,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    28,    29,    30,   106,   107,   189,   216,   217,
     245,   246,   218,   311,   310,   312,   219,   220,    31,   117,
     118,    32,    65,   113,    33,    74,   223,    35,   200,   202,
     232,   235,   262,   257,    36,   179,   162,   180,    37,   224,
     225,   226,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    99,   182,   240,   290,   305,   326,
     339,   346,   350,   318,   328,   340,   347,   351,   354,    50,
     101,   152,   210,   291,   319,   153,   243,   307,   154,    51
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,    34,   238,    62,   123,   124,   125,   181,    82,   183,
      76,    76,   185,   112,   283,   254,    79,    80,   134,   273,
      77,    52,   265,    78,    12,    13,   122,     4,     5,     6,
       7,     8,    53,    81,   274,   163,    17,   123,    18,   280,
     102,   275,   155,   281,    54,   156,    58,    22,    23,    24,
      25,   172,   287,   119,   120,   314,   276,   212,   213,   222,
     173,   161,    26,    55,   214,   114,    27,   292,    60,   215,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,   135,   136,   148,   174,   151,    61,    82,   144,
     145,   146,   191,    84,   175,   284,   193,   128,    68,   285,
     268,   150,    69,   301,   325,    83,   168,   281,   281,    98,
     352,   277,    85,   114,   355,   103,   178,    66,   330,   331,
     105,  -134,  -134,    67,    68,   108,   288,   289,    69,    95,
      96,    97,    70,  -134,   109,  -134,   115,    71,    72,    73,
     110,   188,   116,  -134,  -134,  -134,  -134,  -134,   129,    63,
     194,   -46,   -46,   -46,   -46,   -46,   121,   -46,   131,  -134,
     -46,   -46,   -46,  -134,   -46,   -46,    86,    87,   320,   321,
     139,   140,   141,   -46,   130,   -46,   133,   -46,    64,   147,
     227,   -46,   149,   -46,   -46,   -46,   -46,   -46,   335,    90,
      91,    92,    93,    94,   241,   242,   341,   342,   343,   -46,
      88,    89,   157,   -46,   126,   127,   348,   132,   127,    93,
      94,   353,   158,   119,   137,   138,   258,   159,     2,     3,
     248,     4,     5,     6,     7,     8,     9,    10,   142,   143,
      11,    12,    13,   163,    14,    15,   164,   165,   166,    16,
     169,    87,   170,    17,   171,    18,   258,    19,    89,    19,
     119,    20,   184,    21,    22,    23,    24,    25,   186,   187,
       4,     5,     6,     7,     8,   190,   299,   192,   195,    26,
     196,   197,   198,    27,   203,   211,   306,   205,     3,   199,
       4,     5,     6,     7,     8,   201,    10,   258,   206,    11,
      12,    13,   247,    14,    15,   207,   209,   208,   228,   230,
     221,   229,    17,   231,    18,   239,    19,   160,   233,   234,
      20,   333,   111,    22,    23,    24,    25,   236,     3,   237,
       4,     5,     6,     7,     8,   345,    10,   244,    26,    11,
      12,    13,    27,    14,    15,   249,   250,   251,   253,   252,
     255,   256,    17,   259,    18,   260,    19,   261,   263,   264,
      20,   266,   111,    22,    23,    24,    25,   100,   267,  -154,
    -154,  -154,  -154,  -154,    56,   269,   270,   271,    26,  -154,
    -154,   272,    27,   279,   282,   286,    12,    13,   293,   295,
     300,  -154,   294,  -154,   296,   297,   298,   302,    17,  -154,
      18,  -154,  -154,  -154,  -154,  -154,    57,   303,    58,    22,
      23,    24,    25,   304,   308,   313,   309,  -154,   315,   316,
     317,  -154,    67,    68,    26,   322,   323,    69,    27,     4,
       5,     6,     7,     8,    12,    13,   104,    72,    73,    12,
      13,   324,   327,   167,   329,   332,    17,   334,    18,   278,
     336,    17,   337,    18,   356,   338,    58,    22,    23,    24,
      25,    58,    22,    23,    24,    25,    12,    13,   176,   344,
     349,     0,    26,    12,    13,     0,    27,    26,    17,     0,
      18,    27,     0,     0,     0,    17,     0,    18,   177,    22,
      23,    24,    25,     0,     0,    58,    22,    23,    24,    25,
      12,    13,   204,    68,    26,    12,    13,    69,    27,     0,
       0,    26,     0,     0,    18,    27,   104,    72,    73,    18,
       0,     0,    75,    22,    23,    24,    25,    75,    22,    23,
      24,    25,    67,    68,     0,     0,     0,    69,    26,     0,
       0,    70,    27,    26,     0,     0,    71,    72,    73
};

static const yytype_int16 yycheck[] =
{
      15,     1,   206,    18,    71,    72,    73,   147,    34,   149,
      26,    27,   152,    65,   261,   228,    18,    19,    85,    20,
      26,    33,    27,    27,    13,    14,    15,     3,     4,     5,
       6,     7,    27,    35,    35,    40,    25,   104,    27,    30,
      55,    20,    28,    34,    27,    31,    35,    36,    37,    38,
      39,    31,   265,    68,    69,   302,    35,    23,    24,    35,
      40,   113,    51,    27,    30,    65,    55,   271,    35,    35,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    86,    87,    99,    31,   101,    26,   114,    95,
      96,    97,   159,    33,    40,    30,   163,    26,    27,    34,
     240,   101,    31,    30,    30,    20,   121,    34,    34,     1,
     350,   251,    40,   113,   354,    33,   131,    20,   322,   323,
      33,    13,    14,    26,    27,    35,   266,   267,    31,    52,
      53,    54,    35,    25,    28,    27,    35,    40,    41,    42,
      30,   156,    35,    35,    36,    37,    38,    39,    35,     1,
     165,     3,     4,     5,     6,     7,    40,     9,    40,    51,
      12,    13,    14,    55,    16,    17,    43,    44,   308,   309,
      90,    91,    92,    25,    35,    27,    35,    29,    30,    28,
     195,    33,    28,    35,    36,    37,    38,    39,   328,    47,
      48,    49,    50,    51,   209,   210,   336,   337,   338,    51,
      45,    46,    29,    55,    33,    34,   346,    33,    34,    50,
      51,   351,    29,   228,    88,    89,   231,    40,     0,     1,
     220,     3,     4,     5,     6,     7,     8,     9,    93,    94,
      12,    13,    14,    40,    16,    17,    28,    34,    32,    21,
      27,    44,    35,    25,    35,    27,   261,    29,    46,    29,
     265,    33,    35,    35,    36,    37,    38,    39,    33,    33,
       3,     4,     5,     6,     7,     1,   281,    27,    31,    51,
      35,    33,    36,    55,    27,    32,   291,    33,     1,    37,
       3,     4,     5,     6,     7,    37,     9,   302,    27,    12,
      13,    14,    35,    16,    17,    27,    40,    28,    27,    32,
      30,    28,    25,    29,    27,     1,    29,    30,    32,    29,
      33,   326,    35,    36,    37,    38,    39,    36,     1,    35,
       3,     4,     5,     6,     7,   340,     9,    28,    51,    12,
      13,    14,    55,    16,    17,    35,    35,    28,    32,    34,
      33,    33,    25,    33,    27,    31,    29,    29,    33,    28,
      33,    28,    35,    36,    37,    38,    39,     1,    28,     3,
       4,     5,     6,     7,     1,    33,    33,    27,    51,    13,
      14,    27,    55,    28,    37,    33,    13,    14,     1,    33,
      32,    25,    35,    27,    35,    33,    33,    29,    25,    33,
      27,    35,    36,    37,    38,    39,    33,    28,    35,    36,
      37,    38,    39,    10,    28,    33,    28,    51,    33,    27,
      11,    55,    26,    27,    51,    27,    27,    31,    55,     3,
       4,     5,     6,     7,    13,    14,    40,    41,    42,    13,
      14,    27,     9,    22,    28,     1,    25,    27,    27,   252,
      28,    25,    28,    27,   355,    28,    35,    36,    37,    38,
      39,    35,    36,    37,    38,    39,    13,    14,    15,    28,
      28,    -1,    51,    13,    14,    -1,    55,    51,    25,    -1,
      27,    55,    -1,    -1,    -1,    25,    -1,    27,    35,    36,
      37,    38,    39,    -1,    -1,    35,    36,    37,    38,    39,
      13,    14,    26,    27,    51,    13,    14,    31,    55,    -1,
      -1,    51,    -1,    -1,    27,    55,    40,    41,    42,    27,
      -1,    -1,    35,    36,    37,    38,    39,    35,    36,    37,
      38,    39,    26,    27,    -1,    -1,    -1,    31,    51,    -1,
      -1,    35,    55,    51,    -1,    -1,    40,    41,    42
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    57,     0,     1,     3,     4,     5,     6,     7,     8,
       9,    12,    13,    14,    16,    17,    21,    25,    27,    29,
      33,    35,    36,    37,    38,    39,    51,    55,    58,    59,
      60,    74,    77,    80,    82,    83,    90,    94,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     125,   135,    33,    27,    27,    27,     1,    33,    35,   100,
      35,    26,   100,     1,    30,    78,    20,    26,    27,    31,
      35,    40,    41,    42,    81,    35,   101,   108,   103,    18,
      19,    35,    81,    20,    33,    40,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,     1,   110,
       1,   126,   100,    33,    40,    33,    61,    62,    35,    28,
      30,    35,    59,    79,    82,    35,    35,    75,    76,   100,
     100,    40,    15,   102,   102,   102,    33,    34,    26,    35,
      35,    40,    33,    35,   102,   103,   103,   104,   104,   106,
     106,   106,   107,   107,   108,   108,   108,    28,   100,    28,
      82,   100,   127,   131,   134,    28,    31,    29,    29,    40,
      30,    59,    92,    40,    28,    34,    32,    22,   100,    27,
      35,    35,    31,    40,    31,    40,    15,    35,   100,    91,
      93,    77,   111,    77,    35,    77,    33,    33,   100,    63,
       1,   102,    27,   102,   100,    31,    35,    33,    36,    37,
      84,    37,    85,    27,    26,    33,    27,    27,    28,    40,
     128,    32,    23,    24,    30,    35,    64,    65,    68,    72,
      73,    30,    35,    82,    95,    96,    97,   100,    27,    28,
      32,    29,    86,    32,    29,    87,    36,    35,    95,     1,
     112,   100,   100,   132,    28,    66,    67,    35,    82,    35,
      35,    28,    34,    32,    75,    33,    33,    89,   100,    33,
      31,    29,    88,    33,    28,    27,    28,    28,    77,    33,
      33,    27,    27,    20,    35,    20,    35,    77,    97,    28,
      30,    34,    37,    89,    30,    34,    33,    75,    77,    77,
     113,   129,    95,     1,    35,    33,    35,    33,    33,   100,
      32,    30,    29,    28,    10,   114,   100,   133,    28,    28,
      70,    69,    71,    33,    89,    33,    27,    11,   119,   130,
      77,    77,    27,    27,    27,    30,   115,     9,   120,    28,
      95,    95,     1,   100,    27,    77,    28,    28,    28,   116,
     121,    77,    77,    77,    28,   100,   117,   122,    77,    28,
     118,   123,   114,    77,   124,   114,   119
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
     108,   108,   108,   108,   110,   111,   112,   113,   109,   109,
     115,   116,   117,   118,   114,   114,   120,   119,   121,   122,
     123,   124,   119,   119,   126,   125,   125,   128,   129,   130,
     127,   131,   131,   131,   132,   132,   133,   133,   134,   134,
     135,   135,   135,   135
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
       3,     1,     2,     4,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     0,     0,     0,    11,     5,
       0,     0,     0,     0,    10,     0,     0,     3,     0,     0,
       0,     0,    12,     0,     0,     5,     5,     0,     0,     0,
       9,     1,     1,     0,     1,     0,     1,     0,     2,     4,
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
#line 177 "parser.y"
        {
            printf("Invalid statement at line %d\n", yylineno);
            yyerrok;
        }
#line 1760 "y.tab.c"
    break;

  case 17: /* $@1: %empty  */
#line 185 "parser.y"
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
#line 1784 "y.tab.c"
    break;

  case 18: /* entity_decl: ENTITY IDENTIFIER $@1 LBRACE entity_body RBRACE  */
#line 205 "parser.y"
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
#line 1807 "y.tab.c"
    break;

  case 19: /* $@2: %empty  */
#line 224 "parser.y"
        { emit("entity", (yyvsp[0].sval), "", ""); }
#line 1813 "y.tab.c"
    break;

  case 20: /* entity_decl: ENTITY IDENTIFIER $@2 LBRACE error RBRACE  */
#line 226 "parser.y"
        {
            printf("Invalid entity body at line %d\n", yylineno);
            yyerrok;
            emit("end_entity", (yyvsp[-4].sval), "", "");
        }
#line 1823 "y.tab.c"
    break;

  case 26: /* $@3: %empty  */
#line 245 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_CONSTRUCTOR, DT_VOID, yylineno);
            if(sym){
                strncpy(sym->attr.ctor.belongs_to, current_scope->name, 63);
                sym->attr.ctor.param_count = 0;
                sym->attr.ctor.param_list  = NULL;
                snprintf(sym->attr.ctor.entry_label, 32, "ctor_%s", (yyvsp[0].sval));
            }
            Symbol* entity_sym = lookup(current_scope->parent,
                                        current_scope->name);
            if(entity_sym && entity_sym->kind == KIND_ENTITY)
                add_name(&entity_sym->attr.entity.constructors_list, (yyvsp[0].sval));


            SymTable* cs = create_scope(SCOPE_CONSTRUCTOR, (yyvsp[0].sval), current_scope);
            if(sym) sym->attr.ctor.scope = cs;
            current_scope = cs;
            emit("constr", (yyvsp[0].sval), "", "");           // emit with original name as placeholder
            pending_constr_ir_idx = IR_idx - 1;  // record which quad to patch later
        }
#line 1849 "y.tab.c"
    break;

  case 27: /* constructor_decl: IDENTIFIER $@3 LPAREN param_list_opt RPAREN block  */
#line 267 "parser.y"
        {
            

            //Step 1: mangle the constructor symbol name in entity scope
            for(int i = 0; i < HASH_SIZE; i++){
                Symbol* s = current_scope->parent->buckets[i];
                while(s){
                    Symbol* next = s->next;
                    if(strcmp(s->name, (yyvsp[-5].sval)) == 0 && s->kind == KIND_CONSTRUCTOR
                            && !is_already_mangled(s->name)){
                        char newName[80];
                        overloaded_ctor_name(newName, (yyvsp[-5].sval), s->attr.ctor.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, (yyvsp[-5].sval));
                    }
                    s = next;
                }
            }

            //step2: add mangled name to entity constructors list
            Symbol* entity_sym2 = lookup(current_scope->parent->parent,
                                         current_scope->parent->name);
            if(entity_sym2 && entity_sym2->kind == KIND_ENTITY){
                for(int i = 0; i < HASH_SIZE; i++){
                    for(Symbol* s = current_scope->parent->buckets[i]; s; s = s->next){
                        if(s->kind == KIND_CONSTRUCTOR &&
                           strncmp(s->name, (yyvsp[-5].sval), strlen((yyvsp[-5].sval))) == 0 &&
                           s->name[strlen((yyvsp[-5].sval))] == '_'){
                            if(!name_in_list(entity_sym2->attr.entity.constructors_list, s->name)){
                                add_name(&entity_sym2->attr.entity.constructors_list, s->name);
                            }
                        }
                    }
                }
            }

            //step 3: find the mangled name for this specific constructor by matching param count against current constructor scope
            char mangled_ir[80];
            strcpy(mangled_ir, (yyvsp[-5].sval));   /* fallback */
            for(int i = 0; i < HASH_SIZE; i++){
                for(Symbol* s = current_scope->parent->buckets[i]; s; s = s->next){
                    if(s->kind == KIND_CONSTRUCTOR &&
                       strncmp(s->name, (yyvsp[-5].sval), strlen((yyvsp[-5].sval))) == 0 &&
                       s->name[strlen((yyvsp[-5].sval))] == '_'){
                        int pc = 0;
                        for(ParamNode* p = s->attr.ctor.param_list; p; p = p->next) pc++;
                        int cur_pc = 0;
                        for(int b = 0; b < HASH_SIZE; b++)
                            for(Symbol* ps = current_scope->buckets[b]; ps; ps = ps->next)
                                if(ps->kind == KIND_PARAM) cur_pc++;
                        if(pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            //emit the mangled name
            if(pending_constr_ir_idx >= 0){
                strncpy(IR[pending_constr_ir_idx].arg1, mangled_ir, 19);
                pending_constr_ir_idx = -1;
            }

            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_constr", mangled_ir, "", "");
        }
#line 1920 "y.tab.c"
    break;

  case 28: /* $@4: %empty  */
#line 334 "parser.y"
        { emit("constr", (yyvsp[0].sval), "", ""); }
#line 1926 "y.tab.c"
    break;

  case 29: /* constructor_decl: IDENTIFIER $@4 LPAREN error RPAREN block  */
#line 336 "parser.y"
        {
            printf("Invalid constructor parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_constr", (yyvsp[-5].sval), "", "");
        }
#line 1936 "y.tab.c"
    break;

  case 30: /* $@5: %empty  */
#line 346 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_METHOD, (yyvsp[-2].dtype), yylineno);
            current_function = sym;
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
            //emit("method",...) moved to closing action

            SymTable* ms = create_scope(SCOPE_METHOD, (yyvsp[0].sval), current_scope);
	    if(sym) sym->attr.method.scope = ms;
            current_scope = ms;

            emit("method", (yyvsp[0].sval), "",""); //placeholder
            pending_method_ir_idx = IR_idx - 1;  // record index
        }
#line 1965 "y.tab.c"
    break;

  case 31: /* method_decl: access_modifier type FUNC IDENTIFIER $@5 LPAREN param_list_opt RPAREN block  */
#line 371 "parser.y"
        {
            
            for (int i = 0; i < HASH_SIZE; i++) {
                Symbol* s = current_scope->parent->buckets[i];
                while (s) {
                    Symbol* next = s->next;  /* save before rehash modifies list */
                    if (strcmp(s->name, (yyvsp[-5].sval)) == 0 && s->kind == KIND_METHOD
                            && strchr(s->name, '$') == NULL) {
                        char newName[80];
                        overloaded_method_name(newName, (yyvsp[-5].sval), s->attr.method.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, (yyvsp[-5].sval));  /* fix: move to correct bucket */
                    }
                    s = next;
                }
            }

            /* add mangled name to entity methods list — no duplicates */
            Symbol* entity_sym2 = lookup(current_scope->parent->parent,
                                         current_scope->parent->name);
            if (entity_sym2 && entity_sym2->kind == KIND_ENTITY) {
                for (int i = 0; i < HASH_SIZE; i++) {
                    for (Symbol* s = current_scope->parent->buckets[i]; s; s = s->next) {
                        if (s->kind == KIND_METHOD &&
                            strncmp(s->name, (yyvsp[-5].sval), strlen((yyvsp[-5].sval))) == 0 &&
                            s->name[strlen((yyvsp[-5].sval))] == '_') {
                            if (!name_in_list(entity_sym2->attr.entity.methods_list, s->name)) {
                                add_name(&entity_sym2->attr.entity.methods_list, s->name);
                            }
                        }
                    }
                }
            }

            
            char mangled_ir[80];
            strcpy(mangled_ir, (yyvsp[-5].sval));  /* fallback to original name */
            for (int i = 0; i < HASH_SIZE; i++) {
                for (Symbol* s = current_scope->parent->buckets[i]; s; s = s->next) {
                    if (s->kind == KIND_METHOD &&
                        strncmp(s->name, (yyvsp[-5].sval), strlen((yyvsp[-5].sval))) == 0 &&
                        s->name[strlen((yyvsp[-5].sval))] == '_') {
                        /* match by param count against current method scope */
                        int pc = 0;
                        for (ParamNode* p = s->attr.method.param_list; p; p = p->next) pc++;
                        int cur_pc = 0;
                        for (int b = 0; b < HASH_SIZE; b++)
                            for (Symbol* ps = current_scope->buckets[b]; ps; ps = ps->next)
                                if (ps->kind == KIND_PARAM) cur_pc++;
                        if (pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            if(pending_method_ir_idx >= 0){
                strncpy(IR[pending_method_ir_idx].arg1, mangled_ir, 19);
                pending_method_ir_idx = -1;
            }
            
            print_table(current_scope);
            current_scope = current_scope->parent;
	    current_function = NULL;
            emit("end_method", mangled_ir, "", "");
        }
#line 2035 "y.tab.c"
    break;

  case 32: /* $@6: %empty  */
#line 439 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_METHOD, DT_ENTITY, yylineno);
            current_function = sym;
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
            
            SymTable* ms = create_scope(SCOPE_METHOD, (yyvsp[0].sval), current_scope);
	    if(sym) sym->attr.method.scope = ms;
            current_scope = ms;
        }
#line 2060 "y.tab.c"
    break;

  case 33: /* method_decl: access_modifier IDENTIFIER FUNC IDENTIFIER $@6 LPAREN param_list_opt RPAREN block  */
#line 460 "parser.y"
        {
            
            for (int i = 0; i < HASH_SIZE; i++) {
                Symbol* s = current_scope->parent->buckets[i];
                while (s) {
                    Symbol* next = s->next;  /* save before rehash modifies list */
                    if (strcmp(s->name, (yyvsp[-5].sval)) == 0 && s->kind == KIND_METHOD
                            && strchr(s->name, '$') == NULL) {
                        char newName[80];
                        overloaded_method_name(newName, (yyvsp[-5].sval), s->attr.method.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, (yyvsp[-5].sval));  /* fix: move to correct bucket */
                    }
                    s = next;
                }
            }

            /* add mangled name to entity methods list — no duplicates */
            Symbol* entity_sym2 = lookup(current_scope->parent->parent,
                                         current_scope->parent->name);
            if (entity_sym2 && entity_sym2->kind == KIND_ENTITY) {
                for (int i = 0; i < HASH_SIZE; i++) {
                    for (Symbol* s = current_scope->parent->buckets[i]; s; s = s->next) {
                        if (s->kind == KIND_METHOD &&
                            strncmp(s->name, (yyvsp[-5].sval), strlen((yyvsp[-5].sval))) == 0 &&
                            s->name[strlen((yyvsp[-5].sval))] == '_') {
                            if (!name_in_list(entity_sym2->attr.entity.methods_list, s->name)) {
                                add_name(&entity_sym2->attr.entity.methods_list, s->name);
                            }
                        }
                    }
                }
            }

            
            char mangled_ir[80];
            strcpy(mangled_ir, (yyvsp[-5].sval));
            for (int i = 0; i < HASH_SIZE; i++) {
                for (Symbol* s = current_scope->parent->buckets[i]; s; s = s->next) {
                    if (s->kind == KIND_METHOD &&
                        strncmp(s->name, (yyvsp[-5].sval), strlen((yyvsp[-5].sval))) == 0 &&
                        s->name[strlen((yyvsp[-5].sval))] == '_') {
                        int pc = 0;
                        for (ParamNode* p = s->attr.method.param_list; p; p = p->next) pc++;
                        int cur_pc = 0;
                        for (int b = 0; b < HASH_SIZE; b++)
                            for (Symbol* ps = current_scope->buckets[b]; ps; ps = ps->next)
                                if (ps->kind == KIND_PARAM) cur_pc++;
                        if (pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            emit("method", mangled_ir, "", "");
            print_table(current_scope);
            current_scope = current_scope->parent;
	    current_function = NULL;
            emit("end_method", mangled_ir, "", "");
        }
#line 2125 "y.tab.c"
    break;

  case 34: /* $@7: %empty  */
#line 521 "parser.y"
        { emit("method", (yyvsp[0].sval), "", ""); }
#line 2131 "y.tab.c"
    break;

  case 35: /* method_decl: access_modifier type FUNC IDENTIFIER $@7 LPAREN error RPAREN block  */
#line 523 "parser.y"
        {
            printf("Invalid method parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_method", (yyvsp[-5].sval), "", "");
        }
#line 2141 "y.tab.c"
    break;

  case 36: /* access_var_decl: access_modifier type IDENTIFIER SEMICOLON  */
#line 532 "parser.y"
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
#line 2158 "y.tab.c"
    break;

  case 37: /* access_var_decl: access_modifier IDENTIFIER IDENTIFIER SEMICOLON  */
#line 546 "parser.y"
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
#line 2175 "y.tab.c"
    break;

  case 38: /* access_modifier: PUBLIC  */
#line 561 "parser.y"
              { (yyval.access) = ACC_PUBLIC;  }
#line 2181 "y.tab.c"
    break;

  case 39: /* access_modifier: PRIVATE  */
#line 562 "parser.y"
              { (yyval.access) = ACC_PRIVATE; }
#line 2187 "y.tab.c"
    break;

  case 40: /* object_decl: IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 567 "parser.y"
        {
            Symbol* class_sym = lookup(current_scope, (yyvsp[-4].sval));
            if(!class_sym || class_sym->kind != KIND_ENTITY){
                char buf[256];
                snprintf(buf, sizeof(buf),
                    "line %d: Entity '%s' not found to instantiate", yylineno, (yyvsp[-4].sval));
                semantic_error(buf);
            }
            Symbol* obj = insert_symbol(current_scope, (yyvsp[-7].sval),
                                        KIND_OBJECT, DT_OBJECT, yylineno);
            if(obj){
                strncpy(obj->attr.object.entity_name,
                        class_sym->attr.entity.class_name, 63);
                obj->size = class_sym->attr.entity.class_size;
                current_scope->next_offset += obj->size;
            }

            //build mangled constructor name from arg types
            char mangled_ctor[80];
            strcpy(mangled_ctor, (yyvsp[-4].sval));
            strcat(mangled_ctor, "_");
            for(int i = 0; i < call_arg_count; i++){
                char code[2] = {dt_code(call_arg_types[i]), '\0'};
                strcat(mangled_ctor, code);
            }
            //verify if the constructor exists

            if(class_sym && class_sym->kind == KIND_ENTITY){
                SymTable* esc = find_entity_scope(class_sym->attr.entity.class_name);
                if(esc){
                    Symbol* ctor_sym = lookup_local(esc, mangled_ctor);
                    if(!ctor_sym || ctor_sym->kind != KIND_CONSTRUCTOR){
                        fprintf(stderr,
                            "ERROR line %d: No constructor '%s' matches"
                            " the given argument types.\n",
                            yylineno, mangled_ctor);
                    }
                }
            }

            emit("new", (yyvsp[-4].sval), "", (yyvsp[-7].sval));
            emit("push_ptr", (yyvsp[-7].sval), "", "");
            emit("call_constr", mangled_ctor, "", "");
            call_arg_count = 0;
        }
#line 2237 "y.tab.c"
    break;

  case 41: /* object_decl: type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 613 "parser.y"
        {
            /* build mangled call name from arg types */
            char mangled_call[80];
            strcpy(mangled_call, (yyvsp[-4].sval));
            strcat(mangled_call, "_");
            for (int i = 0; i < call_arg_count; i++) {
                char code[2] = { dt_code(call_arg_types[i]), '\0' };
                strcat(mangled_call, code);
            }

            /*use lookup_local on entity scope directly
               instead of relying on check_method_access chain */
            Symbol* obj_sym = lookup(current_scope, (yyvsp[-6].sval));
            if(!obj_sym || obj_sym->kind != KIND_OBJECT){
                fprintf(stderr, "ERROR line %d: '%s' is not an object.\n", yylineno, (yyvsp[-6].sval));
            } else {
                SymTable* ent_scope = find_entity_scope(obj_sym->attr.object.entity_name);
                if(!ent_scope){
                    fprintf(stderr, "ERROR line %d: entity '%s' scope not found.\n",
                            yylineno, obj_sym->attr.object.entity_name);
                } else {
                    Symbol* method_sym = lookup_local(ent_scope, mangled_call);
                    if(!method_sym || method_sym->kind != KIND_METHOD){
                        fprintf(stderr, "ERROR line %d: Method '%s' not found in '%s'.\n",
                                yylineno, mangled_call, obj_sym->attr.object.entity_name);
                    } else {
                        /* check private access */
                        if(method_sym->attr.method.access == ACC_PRIVATE){
                            fprintf(stderr, "ERROR line %d: Method '%s' is private.\n",
                                    yylineno, mangled_call);
                        }
                        /* return type check uses mangled symbol directly */
                        if(method_sym->attr.method.return_type != (yyvsp[-9].dtype)){
                            fprintf(stderr,
                                "ERROR line %d: cannot assign result of '%s' (returns %s)"
                                " to '%s' (declared as %s).\n",
                                yylineno, mangled_call,
                                dt_names[method_sym->attr.method.return_type],
                                (yyvsp[-8].sval), dt_names[(yyvsp[-9].dtype)]);
                        }
                    }
                }
            }

            Symbol* sym = insert_symbol(current_scope, (yyvsp[-8].sval), KIND_VAR, (yyvsp[-9].dtype), yylineno);
            if(sym){
                sym->is_initialized = 1;
            }
            char* t = genVar();
            emit("push_ptr", (yyvsp[-6].sval), "", "");
            emit("call_method", mangled_call, "", t);
            emit("=", t, "", (yyvsp[-8].sval));
        }
#line 2295 "y.tab.c"
    break;

  case 43: /* arg_list_opt: %empty  */
#line 670 "parser.y"
                        { call_arg_count = 0; }
#line 2301 "y.tab.c"
    break;

  case 44: /* arg_list: arg_list COMMA expression  */
#line 675 "parser.y"
        {
		if(call_arg_count < 64){
			call_arg_types[call_arg_count++] = last_expr_type;
		} 
		emit("arg", (yyvsp[0].sval), "", ""); 
	}
#line 2312 "y.tab.c"
    break;

  case 45: /* arg_list: expression  */
#line 682 "parser.y"
        { 
		call_arg_count = 0;
		call_arg_types[call_arg_count++] = last_expr_type;
		emit("arg", (yyvsp[0].sval), "", ""); 
	}
#line 2322 "y.tab.c"
    break;

  case 46: /* $@8: %empty  */
#line 691 "parser.y"
        {
            SymTable* bs = create_scope(SCOPE_BLOCK, "block", current_scope);
            current_scope = bs;
        }
#line 2331 "y.tab.c"
    break;

  case 47: /* block: LBRACE $@8 stmt_list RBRACE  */
#line 696 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 2340 "y.tab.c"
    break;

  case 49: /* block: LBRACE error RBRACE  */
#line 702 "parser.y"
        {
            printf("Error inside block at line %d\n", yylineno);
            yyerrok;
        }
#line 2349 "y.tab.c"
    break;

  case 52: /* var_decl: type id_list SEMICOLON  */
#line 716 "parser.y"
        {
            insert_var_list((yyvsp[-1].sval), (yyvsp[-2].dtype));
            free((yyvsp[-1].sval));
        }
#line 2358 "y.tab.c"
    break;

  case 53: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 723 "parser.y"
        {
	    if(last_expr_type != DT_UNKNOWN && last_expr_type != (yyvsp[-4].dtype)){
		//printf("==== $1 = %d and last_expr_type = %d\n", $1, last_expr_type);
		fprintf(stderr, "ERROR line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, (yyvsp[-3].sval), dt_names[(yyvsp[-4].dtype)], dt_names[last_expr_type]);
	    }
            emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval));
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
                                        KIND_VAR, (yyvsp[-4].dtype), yylineno);
            if (sym){
		sym->is_initialized = 1;
		if(isConstant((yyvsp[-1].sval))){
			strncpy(sym->init_value, (yyvsp[-1].sval), 63); 
		}
		else{
			sym->init_value[0] = '\0';	// Populate the init value only when it is a constant else compute during the runtime
		}
	    }
	}
#line 2381 "y.tab.c"
    break;

  case 54: /* var_decl: IDENTIFIER id_list SEMICOLON  */
#line 744 "parser.y"
        {
            insert_var_list((yyvsp[-1].sval), DT_ENTITY);
            free((yyvsp[-1].sval));
        }
#line 2390 "y.tab.c"
    break;

  case 55: /* var_decl: IDENTIFIER IDENTIFIER ASSIGN expression SEMICOLON  */
#line 751 "parser.y"
        {
		fprintf(stderr, "ERROR line %d: cannot initialize entity variable '%s' with value of type %s.\n", yylineno, (yyvsp[-3].sval), dt_names[last_expr_type]);
		emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval));
		Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
						KIND_VAR, DT_ENTITY, yylineno);
		if (sym) sym->is_initialized = 1;
        }
#line 2402 "y.tab.c"
    break;

  case 56: /* id_list: id_list COMMA IDENTIFIER  */
#line 762 "parser.y"
        {
            char* buf = malloc(strlen((yyvsp[-2].sval)) + strlen((yyvsp[0].sval)) + 2);
            sprintf(buf, "%s,%s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval));
            (yyval.sval) = buf;
        }
#line 2413 "y.tab.c"
    break;

  case 57: /* id_list: IDENTIFIER  */
#line 769 "parser.y"
        {
            (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 2421 "y.tab.c"
    break;

  case 58: /* type: INT  */
#line 775 "parser.y"
             { (yyval.dtype) = DT_INT;    }
#line 2427 "y.tab.c"
    break;

  case 59: /* type: FP  */
#line 776 "parser.y"
             { (yyval.dtype) = DT_FLOAT;  }
#line 2433 "y.tab.c"
    break;

  case 60: /* type: CHR  */
#line 777 "parser.y"
             { (yyval.dtype) = DT_CHAR;   }
#line 2439 "y.tab.c"
    break;

  case 61: /* type: STRING  */
#line 778 "parser.y"
             { (yyval.dtype) = DT_STRING; }
#line 2445 "y.tab.c"
    break;

  case 62: /* type: BOOL  */
#line 779 "parser.y"
             { (yyval.dtype) = DT_BOOL;   }
#line 2451 "y.tab.c"
    break;

  case 63: /* array_decl: type SEQ1 IDENTIFIER LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 784 "parser.y"
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
#line 2471 "y.tab.c"
    break;

  case 64: /* array_decl: type SEQ2 IDENTIFIER LBRACKET INT_LITERAL RBRACKET LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 803 "parser.y"
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
#line 2488 "y.tab.c"
    break;

  case 65: /* $@9: %empty  */
#line 819 "parser.y"
        {
		current_array_elem_type = (yyvsp[-3].dtype);
		array_type_errors = 0;
	}
#line 2497 "y.tab.c"
    break;

  case 66: /* array_decl: type SEQ1 IDENTIFIER ASSIGN $@9 array_init SEMICOLON  */
#line 824 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-4].sval),
                                        KIND_ARRAY, (yyvsp[-6].dtype), yylineno);
            if (sym) {
                sym->attr.array.dimensions = 1;
                sym->attr.array.is_initialized = 1;
                sym->attr.array.dim1 = (yyvsp[-1].ival);
                sym->attr.array.dim2 = 0;
                sym->size = datatype_size((yyvsp[-6].dtype)) * (yyvsp[-1].ival);
                current_scope->next_offset = sym->offset + sym->size;
                sym->attr.array.init_count = arr_init_count;
                for (int i = 0; i < arr_init_count; i++)
                    strncpy(sym->attr.array.init_values[i], arr_init_vals[i], 31);
            }
             /* ADD: emit IR for each element */
//            for (int i = 0; i < arr_init_count; i++) {
  //              char idx_str[16];
    //            sprintf(idx_str, "%d", i);
   //             char* t1 = genVar();
   //             char* t2 = genVar();
   //             emit("*",  idx_str,"type.width",t1);
   //             emit("[]", $3,t1,t2);
  //              emit("=",arr_init_vals[i],"",t2);
  //          }

            arr_init_count = 0; 
		current_array_elem_type = DT_UNKNOWN;
        }
#line 2530 "y.tab.c"
    break;

  case 67: /* $@10: %empty  */
#line 853 "parser.y"
        {
		current_array_elem_type = (yyvsp[-3].dtype);
		array_type_errors = 0;
		arr2d_rows = 0;
		arr2d_cols = 0;
	}
#line 2541 "y.tab.c"
    break;

  case 68: /* array_decl: type SEQ2 IDENTIFIER ASSIGN $@10 array_init2d SEMICOLON  */
#line 860 "parser.y"
        {
		Symbol* sym = insert_symbol(current_scope, (yyvsp[-4].sval), KIND_ARRAY, (yyvsp[-6].dtype), yylineno);
		if(sym) {
			sym->attr.array.dimensions = 2;
			sym->attr.array.dim1 = arr2d_rows;
			sym->attr.array.dim2 = arr2d_cols;
			sym->attr.array.is_initialized = 1;
			sym->size = datatype_size((yyvsp[-6].dtype)) * arr2d_rows * arr2d_cols;
			current_scope->next_offset = sym->offset + sym->size;

            sym->attr.array.init_count = arr_init_count;
			for (int i = 0; i < arr_init_count; i++)
			    strncpy(sym->attr.array.init_values[i], arr_init_vals[i], 31);
		}
 //       for (int i = 0; i < arr2d_rows; i++) {
//		    for (int j = 0; j < arr2d_cols; j++) {
//		        int linear = i * arr2d_cols + j;
//		        char idx_str[16];
//		        sprintf(idx_str, "%d", linear);
//		        char* t1 = genVar();
//		        char* t2 = genVar();
//		        emit("*",  idx_str,"type.width", t1);
//		        emit("[]", $3, t1, t2);
//		        emit("=",  arr_init_vals[linear],"",t2);
//		    }
//		}

		arr_init_count = 0;
		current_array_elem_type = DT_UNKNOWN;
	}
#line 2576 "y.tab.c"
    break;

  case 69: /* array_init: LBRACE expr_list RBRACE  */
#line 893 "parser.y"
                              { (yyval.ival) = (yyvsp[-1].ival); }
#line 2582 "y.tab.c"
    break;

  case 70: /* array_init2d: LBRACE row_list RBRACE  */
#line 897 "parser.y"
                                   { (yyval.ival) = (yyvsp[-1].ival); }
#line 2588 "y.tab.c"
    break;

  case 71: /* row_list: row_list COMMA LBRACE expr_list RBRACE  */
#line 902 "parser.y"
        { 
		arr2d_rows++;
		arr2d_cols = (yyvsp[-1].ival);
		(yyval.ival) = (yyvsp[-4].ival) + (yyvsp[-1].ival); 
	}
#line 2598 "y.tab.c"
    break;

  case 72: /* row_list: LBRACE expr_list RBRACE  */
#line 908 "parser.y"
        { 
		arr2d_rows = 1;
		arr2d_cols = (yyvsp[-1].ival);
		(yyval.ival) = (yyvsp[-1].ival); 
	}
#line 2608 "y.tab.c"
    break;

  case 73: /* expr_list: expr_list COMMA expression  */
#line 917 "parser.y"
        {
		if(current_array_elem_type != DT_UNKNOWN && last_expr_type != DT_UNKNOWN && last_expr_type != current_array_elem_type){
			{
				fprintf(stderr, "ERROR line %d: array initializer type mismatch - expected %s, got %s\n", yylineno, dt_names[current_array_elem_type], dt_names[last_expr_type]);
				array_type_errors++;
			} 
		}
        if (arr_init_count < 256)
            strncpy(arr_init_vals[arr_init_count++], (yyvsp[0].sval), 31);
		(yyval.ival) = (yyvsp[-2].ival) + 1;
	}
#line 2624 "y.tab.c"
    break;

  case 74: /* expr_list: expression  */
#line 929 "parser.y"
        {
                if(current_array_elem_type != DT_UNKNOWN && last_expr_type != DT_UNKNOWN && last_expr_type != current_array_elem_type){
                        {
                                fprintf(stderr, "ERROR line %d: array initializer type mismatch - expected %s, got %s\n", yylineno, dt_names[current_array_elem_type], dt_names[last_expr_type]);
                                array_type_errors++;
                        }
        	}
//            arr_init_count = 0;
	if (arr_init_count < 256)
            strncpy(arr_init_vals[arr_init_count++], (yyvsp[0].sval), 31);
		(yyval.ival) = 1;
	}
#line 2641 "y.tab.c"
    break;

  case 75: /* $@11: %empty  */
#line 945 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_FUNCTION, (yyvsp[-2].dtype), yylineno);
            if(sym){
                sym->attr.func.return_type = (yyvsp[-2].dtype);
                sym->attr.func.param_count = 0;
                sym->attr.func.param_list  = NULL;
                snprintf(sym->attr.func.entry_label, 32, "func_%s", (yyvsp[0].sval));
            }
            current_function = sym;

            // emit func with original name as placeholder, record index
            emit("func", (yyvsp[0].sval), "", "");
            pending_func_ir_idx = IR_idx - 1;

            SymTable* fs = create_scope(SCOPE_FUNCTION, (yyvsp[0].sval), current_scope);
            if(sym) sym->attr.func.scope = fs;
            current_scope = fs;
        }
#line 2665 "y.tab.c"
    break;

  case 76: /* function_decl: func_type FUNC IDENTIFIER $@11 LPAREN param_list_opt RPAREN block  */
#line 965 "parser.y"
        {
            // Step 1: mangle the symbol in global scope
            for(int i = 0; i < HASH_SIZE; i++){
                Symbol* s = current_scope->parent->buckets[i];
                while(s){
                    Symbol* next = s->next;
                    if(strcmp(s->name, (yyvsp[-5].sval)) == 0 && s->kind == KIND_FUNCTION
                            && !is_already_mangled(s->name)){
                        char newName[80];
                        overloaded_method_name(newName, (yyvsp[-5].sval),
                                               s->attr.func.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, (yyvsp[-5].sval));
                    }
                    s = next;
                }
            }

            // Step 2: find mangled name matching current param count
            char mangled_ir[80];
            strcpy(mangled_ir, (yyvsp[-5].sval));
            for(int i = 0; i < HASH_SIZE; i++){
                for(Symbol* s = current_scope->parent->buckets[i];
                    s; s = s->next){
                    if(s->kind == KIND_FUNCTION &&
                       strncmp(s->name, (yyvsp[-5].sval), strlen((yyvsp[-5].sval))) == 0 &&
                       s->name[strlen((yyvsp[-5].sval))] == '_'){
                        int pc = 0;
                        for(ParamNode* p = s->attr.func.param_list;
                            p; p = p->next) pc++;
                        int cur_pc = 0;
                        for(int b = 0; b < HASH_SIZE; b++)
                            for(Symbol* ps = current_scope->buckets[b];
                                ps; ps = ps->next)
                                if(ps->kind == KIND_PARAM) cur_pc++;
                        if(pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            // Step 3: patch the placeholder func quad
            if(pending_func_ir_idx >= 0){
                strncpy(IR[pending_func_ir_idx].arg1, mangled_ir, 19);
                pending_func_ir_idx = -1;
            }

            print_table(current_scope);
            current_scope = current_scope->parent;
            current_function = NULL;
            emit("endfunc", "", "", "");
        }
#line 2722 "y.tab.c"
    break;

  case 77: /* $@12: %empty  */
#line 1019 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_FUNCTION, DT_ENTITY, yylineno);
            if(sym){
                sym->attr.func.return_type = DT_ENTITY;
                sym->attr.func.param_count = 0;
                sym->attr.func.param_list  = NULL;
                snprintf(sym->attr.func.entry_label, 32, "func_%s", (yyvsp[0].sval));
            }
            current_function = sym;

            emit("func", (yyvsp[0].sval), "", "");
            pending_func_ir_idx = IR_idx - 1;

            SymTable* fs = create_scope(SCOPE_FUNCTION, (yyvsp[0].sval), current_scope);
            if(sym) sym->attr.func.scope = fs;
            current_scope = fs;
        }
#line 2745 "y.tab.c"
    break;

  case 78: /* function_decl: IDENTIFIER FUNC IDENTIFIER $@12 LPAREN param_list_opt RPAREN block  */
#line 1038 "parser.y"
        {
            // same mangling steps
            for(int i = 0; i < HASH_SIZE; i++){
                Symbol* s = current_scope->parent->buckets[i];
                while(s){
                    Symbol* next = s->next;
                    if(strcmp(s->name, (yyvsp[-5].sval)) == 0 && s->kind == KIND_FUNCTION
                            && !is_already_mangled(s->name)){
                        char newName[80];
                        overloaded_method_name(newName, (yyvsp[-5].sval),
                                               s->attr.func.param_list);
                        strncpy(s->name, newName, 63);
                        rehash_symbol(current_scope->parent, s, (yyvsp[-5].sval));
                    }
                    s = next;
                }
            }

            char mangled_ir[80];
            strcpy(mangled_ir, (yyvsp[-5].sval));
            for(int i = 0; i < HASH_SIZE; i++){
                for(Symbol* s = current_scope->parent->buckets[i];
                    s; s = s->next){
                    if(s->kind == KIND_FUNCTION &&
                       strncmp(s->name, (yyvsp[-5].sval), strlen((yyvsp[-5].sval))) == 0 &&
                       s->name[strlen((yyvsp[-5].sval))] == '_'){
                        int pc = 0;
                        for(ParamNode* p = s->attr.func.param_list;
                            p; p = p->next) pc++;
                        int cur_pc = 0;
                        for(int b = 0; b < HASH_SIZE; b++)
                            for(Symbol* ps = current_scope->buckets[b];
                                ps; ps = ps->next)
                                if(ps->kind == KIND_PARAM) cur_pc++;
                        if(pc == cur_pc)
                            strncpy(mangled_ir, s->name, 79);
                    }
                }
            }

            if(pending_func_ir_idx >= 0){
                strncpy(IR[pending_func_ir_idx].arg1, mangled_ir, 19);
                pending_func_ir_idx = -1;
            }

            print_table(current_scope);
            current_scope = current_scope->parent;
            current_function = NULL;
            emit("endfunc", "", "", "");
        }
#line 2800 "y.tab.c"
    break;

  case 79: /* $@13: %empty  */
#line 1090 "parser.y"
        { emit("func", (yyvsp[0].sval), "", ""); }
#line 2806 "y.tab.c"
    break;

  case 80: /* function_decl: func_type FUNC IDENTIFIER $@13 LPAREN error RPAREN block  */
#line 1092 "parser.y"
        {
            printf("Invalid parameter list at line %d\n", yylineno);
            yyerrok;
            emit("endfunc", "", "", "");
        }
#line 2816 "y.tab.c"
    break;

  case 81: /* func_type: type  */
#line 1100 "parser.y"
           { (yyval.dtype) = (yyvsp[0].dtype);      }
#line 2822 "y.tab.c"
    break;

  case 82: /* func_type: VOID  */
#line 1101 "parser.y"
           { (yyval.dtype) = DT_VOID; }
#line 2828 "y.tab.c"
    break;

  case 87: /* param: type IDENTIFIER  */
#line 1117 "parser.y"
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
#line 2852 "y.tab.c"
    break;

  case 88: /* param: IDENTIFIER IDENTIFIER  */
#line 1138 "parser.y"
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
#line 2876 "y.tab.c"
    break;

  case 89: /* return_stmt: RETURN expression SEMICOLON  */
#line 1161 "parser.y"
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
#line 2894 "y.tab.c"
    break;

  case 90: /* return_stmt: RETURN SEMICOLON  */
#line 1175 "parser.y"
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
#line 2909 "y.tab.c"
    break;

  case 91: /* return_stmt: RETURN error SEMICOLON  */
#line 1187 "parser.y"
        {
            printf("Invalid return statement at line %d\n", yylineno);
            yyerrok;
        }
#line 2918 "y.tab.c"
    break;

  case 94: /* expression: assignment  */
#line 1199 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2924 "y.tab.c"
    break;

  case 95: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET  */
#line 1203 "parser.y"
        {
	    Symbol* asym = require_declared(current_scope, (yyvsp[-3].sval), yylineno);
	    char width_str[16];
	    if (asym && asym->kind == KIND_ARRAY)
		sprintf(width_str, "%d", datatype_size(asym->datatype));
	    else
		strcpy(width_str, "type.width");
            char* t1 = genVar(); emit("*",  (yyvsp[-1].sval), width_str, t1);
            char* t2 = genVar(); emit("[]", (yyvsp[-3].sval), t1,t2);
            (yyval.sval) = t2;
        }
#line 2940 "y.tab.c"
    break;

  case 96: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET  */
#line 1215 "parser.y"
        {
            Symbol* asym = require_declared(current_scope, (yyvsp[-6].sval), yylineno);
	    char cols_str[16];
	    char width_str[16];
	    if(asym && asym->kind == KIND_ARRAY) {
		sprintf(cols_str, "%d", asym->attr.array.dim2);
		sprintf(width_str, "%d", datatype_size(asym->datatype));}
	    else{
		strcpy(cols_str, "array.cols");
		strcpy(width_str, "type.width");
   	    }
            char* t1 = genVar(); emit("*",  (yyvsp[-4].sval), cols_str, t1);
            char* t2 = genVar(); emit("+",  t1, (yyvsp[-1].sval),t2);
            char* t3 = genVar(); emit("*",  t2, width_str, t3);
            char* t4 = genVar(); emit("[]", (yyvsp[-6].sval), t3,t4);
            (yyval.sval) = t4;
        }
#line 2962 "y.tab.c"
    break;

  case 97: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 1236 "parser.y"
        {
		// Checking the type before assignment
		Symbol* lhs = require_declared(current_scope, (yyvsp[-2].sval), yylineno);
		if(lhs && last_expr_type != DT_UNKNOWN){
			if(lhs->datatype != last_expr_type){
				fprintf(stderr, "ERROR line %d: cannot assign %s to '%s' "
				"(declared as %s).\n", yylineno, dt_names[last_expr_type], (yyvsp[-2].sval), dt_names[lhs->datatype]);
			}
		} 
		emit("=", (yyvsp[0].sval), "", ir_name_of((yyvsp[-2].sval))); 
		(yyval.sval) = strdup((yyvsp[-2].sval)); 
	}
#line 2979 "y.tab.c"
    break;

  case 98: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 1249 "parser.y"
        { 
		Symbol* lhs = require_declared(current_scope, (yyvsp[-2].sval), yylineno);
		if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
			fprintf(stderr, "ERROR line %d: type mismatch in '+=' : '%s' is %s but RHS is %s.\n", yylineno, (yyvsp[-2].sval), dt_names[lhs->datatype], dt_names[last_expr_type]);
		}
		char* t = genVar(); 
		const char* irn = ir_name_of((yyvsp[-2].sval));
		emit("+", irn, (yyvsp[0].sval), t); 
		emit("=", t, "", irn); 
		(yyval.sval) = t; 
	}
#line 2995 "y.tab.c"
    break;

  case 99: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 1261 "parser.y"
        { 	
		Symbol* lhs = require_declared(current_scope, (yyvsp[-2].sval), yylineno);
		if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
			fprintf(stderr, "ERROR line %d: type mismatch in '-=' : '%s' is %s but RHS is %s.\n", yylineno, (yyvsp[-2].sval), dt_names[lhs->datatype], dt_names[last_expr_type]);
		}
			
		char* t = genVar(); 
		const char* irn = ir_name_of((yyvsp[-2].sval));
		emit("-", irn, (yyvsp[0].sval), t); 
		emit("=", t, "", irn); 	
		(yyval.sval) = t; 
	}
#line 3012 "y.tab.c"
    break;

  case 100: /* assignment: indexed_id ASSIGN assignment  */
#line 1274 "parser.y"
        { (yyval.sval) = (yyvsp[0].sval); }
#line 3018 "y.tab.c"
    break;

  case 101: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 1276 "parser.y"
        { //check_field_access("this", $3); 
		//Symbol* method_sym = lookup(current_scope, current_scope->name);
		const char* entity_name = NULL;
		if(current_function && current_function->kind == KIND_METHOD){
			entity_name = current_function->attr.method.belongs_to;
		}
		if(entity_name){
			Symbol* entity_sym = lookup(global_scope, entity_name);
			if(entity_sym && entity_sym->kind == KIND_ENTITY){
				SymTable* esc = find_entity_scope(entity_name);
				Symbol* field = esc ? lookup_local(esc, (yyvsp[-2].sval)):NULL;
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
#line 3048 "y.tab.c"
    break;

  case 102: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 1302 "parser.y"
        { 
		check_field_access((yyvsp[-4].sval),(yyvsp[-2].sval),yylineno); 
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
#line 3078 "y.tab.c"
    break;

  case 103: /* assignment: logic_expr  */
#line 1327 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 3084 "y.tab.c"
    break;

  case 104: /* logic_expr: logic_expr OR logic_expr  */
#line 1332 "parser.y"
        { char* t = genVar(); emit("||", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3090 "y.tab.c"
    break;

  case 105: /* logic_expr: logic_expr AND logic_expr  */
#line 1334 "parser.y"
        { char* t = genVar(); emit("&&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3096 "y.tab.c"
    break;

  case 106: /* logic_expr: NOT logic_expr  */
#line 1336 "parser.y"
        { char* t = genVar(); emit("!", (yyvsp[0].sval), "", t); (yyval.sval) = t; }
#line 3102 "y.tab.c"
    break;

  case 107: /* logic_expr: bitwise_expr  */
#line 1337 "parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 3108 "y.tab.c"
    break;

  case 108: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 1342 "parser.y"
        { char* t = genVar(); emit("&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3114 "y.tab.c"
    break;

  case 109: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 1344 "parser.y"
        { char* t = genVar(); emit("|", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3120 "y.tab.c"
    break;

  case 110: /* bitwise_expr: rel_expr  */
#line 1345 "parser.y"
               { (yyval.sval) = (yyvsp[0].sval); }
#line 3126 "y.tab.c"
    break;

  case 111: /* rel_expr: arith_expr GT arith_expr  */
#line 1350 "parser.y"
        {
          char* t = genVar(); emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
            
        }
#line 3135 "y.tab.c"
    break;

  case 112: /* rel_expr: arith_expr LT arith_expr  */
#line 1355 "parser.y"
        {
                char* t = genVar(); emit("<", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
        }
#line 3143 "y.tab.c"
    break;

  case 113: /* rel_expr: arith_expr EQ arith_expr  */
#line 1359 "parser.y"
        {
                char* t = genVar(); emit("==", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
        }
#line 3151 "y.tab.c"
    break;

  case 114: /* rel_expr: arith_expr  */
#line 1362 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 3157 "y.tab.c"
    break;

  case 115: /* arith_expr: arith_expr PLUS term  */
#line 1367 "parser.y"
       {
                char* t = genVar();
                emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
             
        }
#line 3168 "y.tab.c"
    break;

  case 116: /* arith_expr: arith_expr MINUS term  */
#line 1374 "parser.y"
        {  
                char* t = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
        }
#line 3178 "y.tab.c"
    break;

  case 117: /* arith_expr: term  */
#line 1379 "parser.y"
           { (yyval.sval) = (yyvsp[0].sval); }
#line 3184 "y.tab.c"
    break;

  case 118: /* term: term MUL factor  */
#line 1384 "parser.y"
        {
                char* t = genVar();
                emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
        }
#line 3194 "y.tab.c"
    break;

  case 119: /* term: term DIV factor  */
#line 1390 "parser.y"
        {
                char* t = genVar();
                emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
        }
#line 3204 "y.tab.c"
    break;

  case 120: /* term: term MOD factor  */
#line 1396 "parser.y"
        {
                char* t = genVar();
                emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
        }
#line 3214 "y.tab.c"
    break;

  case 121: /* term: factor  */
#line 1401 "parser.y"
             { (yyval.sval) = (yyvsp[0].sval); }
#line 3220 "y.tab.c"
    break;

  case 122: /* factor: MINUS factor  */
#line 1406 "parser.y"
        {
            char* t = genVar();
            emit("-", "0", (yyvsp[0].sval), t);
            (yyval.sval) = t;
            last_expr_type = DT_INT;
        }
#line 3231 "y.tab.c"
    break;

  case 123: /* factor: IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 1413 "parser.y"
    {
        char* t = genVar();

        // Build mangled name from collected arg types
        char mangled_call[80];
        strcpy(mangled_call, (yyvsp[-3].sval));
        strcat(mangled_call, "_");
        for(int i = 0; i < call_arg_count; i++){
            char code[2] = {dt_code(call_arg_types[i]), '\0'};
            strcat(mangled_call, code);
        }

        // Try mangled name first, fall back to original name
        Symbol* fsym = lookup(current_scope, mangled_call);
        if(!fsym){
            fsym = lookup(current_scope, (yyvsp[-3].sval));
        }

        if(!fsym){
            fprintf(stderr,
                "ERROR line %d: call to undeclared function '%s'.\n",
                yylineno, (yyvsp[-3].sval));
            last_expr_type = DT_UNKNOWN;
        }
        else if(fsym->kind != KIND_FUNCTION && fsym->kind != KIND_METHOD){
            fprintf(stderr,
                "ERROR line %d: '%s' is not a function.\n", yylineno, (yyvsp[-3].sval));
            last_expr_type = DT_UNKNOWN;
        }
        else{
            ParamNode* ep = (fsym->kind == KIND_FUNCTION)
                            ? fsym->attr.func.param_list
                            : fsym->attr.method.param_list;
            int expected_count = (fsym->kind == KIND_FUNCTION)
                                  ? fsym->attr.func.param_count
                                  : fsym->attr.method.param_count;
            if(call_arg_count != expected_count){
                fprintf(stderr,
                    "ERROR line %d: function '%s' expects %d arg(s),"
                    " got %d.\n",
                    yylineno, (yyvsp[-3].sval), expected_count, call_arg_count);
            }
            else{
                for(int i = 0; i < call_arg_count && ep;
                    i++, ep = ep->next){
                    if(call_arg_types[i] != DT_UNKNOWN &&
                       call_arg_types[i] != ep->datatype){
                        fprintf(stderr,
                            "ERROR line %d: function '%s' argument %d"
                            " - expected %s, but got %s.\n",
                            yylineno, (yyvsp[-3].sval), i+1,
                            dt_names[ep->datatype],
                            dt_names[call_arg_types[i]]);
                    }
                }
            }
            last_expr_type = (fsym->kind == KIND_FUNCTION)
                              ? fsym->attr.func.return_type
                              : fsym->attr.method.return_type;
        }

        // emit with mangled name if overloaded, original otherwise
        const char* emit_name = (fsym && is_already_mangled(fsym->name))
                         ? fsym->name : (yyvsp[-3].sval);
        emit("call", emit_name, "", t);
        (yyval.sval) = t;
    }
#line 3303 "y.tab.c"
    break;

  case 124: /* factor: IDENTIFIER DOT IDENTIFIER  */
#line 1481 "parser.y"
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
            /* get entity name safely */
            const char* entity_name = obj->attr.object.entity_name;
            if (!entity_name || entity_name[0] == '\0') {
                fprintf(stderr,
                    "ERROR line %d: object '%s' has no entity type.\n",
                    yylineno, (yyvsp[-2].sval));
            }
            else {
                /* look up the entity symbol in global scope */
                Symbol* cls = lookup(global_scope, entity_name);
                if (!cls || cls->kind != KIND_ENTITY) {
                    fprintf(stderr,
                        "ERROR line %d: entity '%s' not defined.\n",
                        yylineno, entity_name);
                }
                else {
                    /* get the entity's scope (stored in EntityAttr) */
                    SymTable* escope = cls->attr.entity.scope;
                    if (!escope) {
                        fprintf(stderr,
                            "ERROR line %d: entity '%s' has no scope.\n",
                            yylineno, entity_name);
                    }
                    else {
                        /* look up the field inside entity scope */
                        Symbol* field = lookup_local(escope, (yyvsp[0].sval));
                        if (!field || field->kind != KIND_FIELD) {
                            fprintf(stderr,
                                "ERROR line %d: '%s' is not a field of '%s'.\n",
                                yylineno, (yyvsp[0].sval), entity_name);
                        }
                        else {
                            //check private access
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
#line 3376 "y.tab.c"
    break;

  case 125: /* factor: IDENTIFIER  */
#line 1551 "parser.y"
        { 
		Symbol* s = require_declared(current_scope, (yyvsp[0].sval), yylineno);
		last_expr_type = s ? s->datatype : DT_UNKNOWN;
		(yyval.sval) = strdup(ir_name_of((yyvsp[0].sval))); 
	}
#line 3386 "y.tab.c"
    break;

  case 126: /* factor: indexed_id  */
#line 1556 "parser.y"
                      { (yyval.sval) = (yyvsp[0].sval); }
#line 3392 "y.tab.c"
    break;

  case 127: /* factor: INT_LITERAL  */
#line 1557 "parser.y"
                      { char b[20]; sprintf(b, "%d",   (yyvsp[0].ival)); (yyval.sval) = strdup(b); last_expr_type = DT_INT; }
#line 3398 "y.tab.c"
    break;

  case 128: /* factor: FLOAT_LITERAL  */
#line 1558 "parser.y"
                      { char b[20]; sprintf(b, "%f",   (yyvsp[0].fval)); (yyval.sval) = strdup(b);  last_expr_type = DT_FLOAT; }
#line 3404 "y.tab.c"
    break;

  case 129: /* factor: CHAR_LITERAL  */
#line 1559 "parser.y"
                      { char b[20]; sprintf(b, "'%c'", (yyvsp[0].cval)); (yyval.sval) = strdup(b); last_expr_type = DT_CHAR; }
#line 3410 "y.tab.c"
    break;

  case 130: /* factor: STRING_LITERAL  */
#line 1560 "parser.y"
                      { (yyval.sval) = strdup((yyvsp[0].sval));  last_expr_type = DT_STRING; }
#line 3416 "y.tab.c"
    break;

  case 131: /* factor: TRUE  */
#line 1561 "parser.y"
                      { (yyval.sval) = strdup("1"); last_expr_type = DT_BOOL; }
#line 3422 "y.tab.c"
    break;

  case 132: /* factor: FALSE  */
#line 1562 "parser.y"
                      { (yyval.sval) = strdup("0"); last_expr_type = DT_BOOL; }
#line 3428 "y.tab.c"
    break;

  case 133: /* factor: LPAREN expression RPAREN  */
#line 1563 "parser.y"
                               { (yyval.sval) = (yyvsp[-1].sval); }
#line 3434 "y.tab.c"
    break;

  case 134: /* $@14: %empty  */
#line 1568 "parser.y"
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
#line 3455 "y.tab.c"
    break;

  case 135: /* $@15: %empty  */
#line 1585 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3461 "y.tab.c"
    break;

  case 136: /* $@16: %empty  */
#line 1587 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3470 "y.tab.c"
    break;

  case 137: /* $@17: %empty  */
#line 1592 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3476 "y.tab.c"
    break;

  case 138: /* if_stmt: IF LPAREN $@14 expression $@15 RPAREN $@16 block $@17 elif_list else_opt  */
#line 1594 "parser.y"
        { emit("label", "", "", topEnd()); popIfLabels(); }
#line 3482 "y.tab.c"
    break;

  case 139: /* if_stmt: IF LPAREN error RPAREN block  */
#line 1597 "parser.y"
        {
            printf("Invalid IF condition at line %d\n", yylineno);
            yyerrok;
        }
#line 3491 "y.tab.c"
    break;

  case 140: /* $@18: %empty  */
#line 1605 "parser.y"
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
#line 3512 "y.tab.c"
    break;

  case 141: /* $@19: %empty  */
#line 1622 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3518 "y.tab.c"
    break;

  case 142: /* $@20: %empty  */
#line 1624 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3527 "y.tab.c"
    break;

  case 143: /* $@21: %empty  */
#line 1629 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3533 "y.tab.c"
    break;

  case 146: /* $@22: %empty  */
#line 1636 "parser.y"
        {
            char else_name[32];
            snprintf(else_name, 32, "else_%d", if_cnt++);
            SymTable* es = create_scope(SCOPE_ELSE, else_name, current_scope);
            current_scope = es;
        }
#line 3544 "y.tab.c"
    break;

  case 147: /* else_opt: ELSE $@22 block  */
#line 1643 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3553 "y.tab.c"
    break;

  case 148: /* $@23: %empty  */
#line 1648 "parser.y"
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
#line 3574 "y.tab.c"
    break;

  case 149: /* $@24: %empty  */
#line 1665 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3580 "y.tab.c"
    break;

  case 150: /* $@25: %empty  */
#line 1667 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3589 "y.tab.c"
    break;

  case 151: /* $@26: %empty  */
#line 1672 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3595 "y.tab.c"
    break;

  case 154: /* $@27: %empty  */
#line 1679 "parser.y"
        {
            char for_name[32];
            snprintf(for_name, 32, "for_%d", for_cnt++);
            SymTable* fs = create_scope(SCOPE_FOR, for_name, current_scope);
            current_scope = fs;
        }
#line 3606 "y.tab.c"
    break;

  case 155: /* for_stmt: FOR LPAREN $@27 for_header block  */
#line 1686 "parser.y"
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
#line 3626 "y.tab.c"
    break;

  case 156: /* for_stmt: FOR LPAREN error RPAREN block  */
#line 1703 "parser.y"
        {
            printf("Invalid FOR header at line %d\n", yylineno);
            yyerrok;
        }
#line 3635 "y.tab.c"
    break;

  case 157: /* $@28: %empty  */
#line 1711 "parser.y"
        {
            char* b = getLabel();
            char* e = getLabel();
            pushIfLabels(b, e);
            emit("label", "", "", b);
        }
#line 3646 "y.tab.c"
    break;

  case 158: /* $@29: %empty  */
#line 1718 "parser.y"
        {
            emit("ifFalse", (yyvsp[-1].sval) ? (yyvsp[-1].sval) : "1", "", topEnd());
            forDepth++;
            forIncIdx[forDepth]      = 0;
            inForIncrement[forDepth] = 1;
        }
#line 3657 "y.tab.c"
    break;

  case 159: /* $@30: %empty  */
#line 1725 "parser.y"
        { inForIncrement[forDepth] = 0; }
#line 3663 "y.tab.c"
    break;

  case 161: /* for_init_opt: var_decl_no_semi  */
#line 1730 "parser.y"
                        { (yyval.sval) = NULL; }
#line 3669 "y.tab.c"
    break;

  case 162: /* for_init_opt: expression  */
#line 1731 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 3675 "y.tab.c"
    break;

  case 163: /* for_init_opt: %empty  */
#line 1732 "parser.y"
                    { (yyval.sval) = NULL; }
#line 3681 "y.tab.c"
    break;

  case 164: /* for_cond_opt: expression  */
#line 1736 "parser.y"
                    { (yyval.sval) = (yyvsp[0].sval); }
#line 3687 "y.tab.c"
    break;

  case 165: /* for_cond_opt: %empty  */
#line 1737 "parser.y"
                    { (yyval.sval) = strdup("1"); }
#line 3693 "y.tab.c"
    break;

  case 166: /* for_inc_opt: expression  */
#line 1741 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 3699 "y.tab.c"
    break;

  case 167: /* for_inc_opt: %empty  */
#line 1742 "parser.y"
                        { (yyval.sval) = NULL; }
#line 3705 "y.tab.c"
    break;

  case 168: /* var_decl_no_semi: type IDENTIFIER  */
#line 1747 "parser.y"
        {
            insert_symbol(current_scope, (yyvsp[0].sval), KIND_VAR, (yyvsp[-1].dtype), yylineno);
        }
#line 3713 "y.tab.c"
    break;

  case 169: /* var_decl_no_semi: type IDENTIFIER ASSIGN expression  */
#line 1751 "parser.y"
        {
	    	if(last_expr_type != DT_UNKNOWN && last_expr_type != (yyvsp[-3].dtype)){
			fprintf(stderr, "ERROR line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, (yyvsp[-2].sval), dt_names[(yyvsp[-3].dtype)], dt_names[last_expr_type]);
		}
            	//emit("=", $4, "", $2);
           	Symbol* sym = insert_symbol(current_scope, (yyvsp[-2].sval),
                                        KIND_VAR, (yyvsp[-3].dtype), yylineno);
            	if (sym) sym->is_initialized = 1;
		emit("=", (yyvsp[0].sval), "", ir_name_of((yyvsp[-2].sval)));
        }
#line 3728 "y.tab.c"
    break;

  case 170: /* io_stmt: IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 1765 "parser.y"
        {
		Symbol* var = require_declared(current_scope, (yyvsp[-6].sval), yylineno);
		if(!var){
			fprintf(stderr, "ERROR line %d: assignment to undeclared variable '%s'.\n", yylineno, (yyvsp[-6].sval));
		}
		emit("in", "", "", (yyvsp[-6].sval)); 
	}
#line 3740 "y.tab.c"
    break;

  case 171: /* io_stmt: type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 1773 "parser.y"
        { emit("in", "", "", (yyvsp[-6].sval)); }
#line 3746 "y.tab.c"
    break;

  case 172: /* io_stmt: SHOW LPAREN expression RPAREN SEMICOLON  */
#line 1775 "parser.y"
        { emit("out", (yyvsp[-2].sval), "", ""); }
#line 3752 "y.tab.c"
    break;

  case 173: /* io_stmt: SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON  */
#line 1777 "parser.y"
        {
            char* t = genVar();
            emit("[]",  (yyvsp[-5].sval), (yyvsp[-3].sval), t);
            emit("out", t,  "", "");
        }
#line 3762 "y.tab.c"
    break;


#line 3766 "y.tab.c"

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

#line 1784 "parser.y"


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

int main(int argc, char* argv[]) {
    	global_scope  = create_scope(SCOPE_GLOBAL, "global", NULL);
    	current_scope = global_scope;


        //check --oalloc flag
	    // Parse the -S flag (stats mode)
	    int compare_mode = 0;
	    for(int i = 1; i < argc; i++){
	    	if(strcmp(argv[i], "-S") == 0){
	    		compare_mode = 1;
	    		printf("Mode: Comparision (standard + optimal both generated)\n");
	    	}
            else if(strcmp(argv[i],"--oalloc")==0){
                use_optimized_regalloc=1;
                printf("Register allocation: OPTIMIZED\n");
            }
	    }	

    	//global_scope  = create_scope(SCOPE_GLOBAL, "global", NULL);
    	//current_scope = global_scope;
    	yyin = stdin;
    	yyparse();

    	printf("\n========== GLOBAL SCOPE ==========\n");
    	print_table(global_scope);
         
int opt_level = 3;   //default=O3
int do_python=0;
for (int i = 1; i < argc; i++) {
    if      (strcmp(argv[i], "-O0") == 0) opt_level = 0;
    else if (strcmp(argv[i], "-O1") == 0) opt_level = 1;
    else if (strcmp(argv[i], "-O2") == 0) opt_level = 2;
    else if (strcmp(argv[i], "-O3") == 0) opt_level = 3;
    else if (strcmp(argv[i], "-py") == 0) do_python = 1;
}
printf("Running Optimizations (-%c%d)\n", 'O', opt_level);

//O1
if (opt_level >= 1) {
    algebraic_simplification();
    constant_folding();
    constant_propagation();
    copy_propagation(opt_level);
    //second pass to clean up copies exposed by propagation 
    constant_folding();
    constant_propagation();
}

//O2
if (opt_level >= 2) {
    common_subexpression_elimination();
    dead_code_elimination();
}

//O3
if (opt_level >= 3) {
    strength_reduction();
    loop_invariant_code_motion();
    induction_variable_elimination();
    dead_code_elimination();   // final cleanup after loop optimizations
}
       printf("\n========== IR Code Visualization Section ==========\n");
	print_original_IR();
	print_opt_IR();

    /*printf("\nParsing Successful\nGenerated quadruple table:\n");
    printf("%-15s %-15s %-15s %-15s\n", "OP", "ARG1", "ARG2", "RESULT");
    for (int i = 0; i < IR_idx; i++)
        printf("%-15s %-15s %-15s %-15s\n",
               IR[i].op, IR[i].arg1, IR[i].arg2, IR[i].result);*/
    	FILE *asm_file = fopen("output.s", "w");
    	if (!asm_file) {
        	perror("Failed to open assembly file");
       		 return 1;
    	}
    	asmSetOutput(asm_file);
	use_template_matching = 1;	// Turning on the optimal instruction selection
    	printf("\nGenerating RISC-V Assembly...\n");
    	generateASM(); 

    fclose(asm_file);
    printf("Assembly code saved to 'output.s'\n");
   
 if (do_python) {
    FILE* py_file = fopen("output.py", "w");
    if (!py_file) {
        perror("Failed to open output.py");
    } else {
        printf("\nTranspiling to Python...\n");
        transpile_to_python(py_file);
        fclose(py_file);
        printf("Python code saved to 'output.py'\n");
    }
}

	if(compare_mode){
		FILE* std_file = fopen("output_standard.s", "w");
		if(!std_file){
			perror("Failed to open output_standard.s");
			return 1;
		}
		asmSetOutput(std_file);
		use_template_matching = 0;	// turn off the optimal ins selection
		printf("Generating Assembly code using standard if/else chain");
		generateASM();
		fclose(std_file);
		printf("Standard Assembly code saved to 'output_standard.s' file\n");
	
		// Now compare the code from two files using a function
			
	}
   	return 0;
}

