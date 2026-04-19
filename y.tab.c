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
#include "quad.h"
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
    char arg1[256];
    char arg2[256];
    char result[256];
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

// Add these globals near falseStack/endStack in parser.y
char* loopEndStack[100];
char* loopCondStack[100];
int loopTop = -1;

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

//function calls inside the same function
// Add these globals near your other pending_*_ir_idx variables
static int pending_calls[1000];       // IR indices of calls needing patch
static char pending_call_base[1000][64]; // original base name e.g. "add"
static int pending_call_count = 0;

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


#line 196 "y.tab.c"

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
    LPAREN = 284,                  /* LPAREN  */
    RPAREN = 285,                  /* RPAREN  */
    LBRACE = 286,                  /* LBRACE  */
    RBRACE = 287,                  /* RBRACE  */
    LBRACKET = 288,                /* LBRACKET  */
    RBRACKET = 289,                /* RBRACKET  */
    SEMICOLON = 290,               /* SEMICOLON  */
    COMMA = 291,                   /* COMMA  */
    IDENTIFIER = 292,              /* IDENTIFIER  */
    STRING_LITERAL = 293,          /* STRING_LITERAL  */
    INT_LITERAL = 294,             /* INT_LITERAL  */
    FLOAT_LITERAL = 295,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 296,            /* CHAR_LITERAL  */
    ASSIGN = 297,                  /* ASSIGN  */
    ADD_ASSIGN = 298,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 299,              /* SUB_ASSIGN  */
    OR = 300,                      /* OR  */
    AND = 301,                     /* AND  */
    BITOR = 302,                   /* BITOR  */
    BITAND = 303,                  /* BITAND  */
    EQ = 304,                      /* EQ  */
    GT = 305,                      /* GT  */
    LT = 306,                      /* LT  */
    PLUS = 307,                    /* PLUS  */
    MINUS = 308,                   /* MINUS  */
    MUL = 309,                     /* MUL  */
    DIV = 310,                     /* DIV  */
    MOD = 311,                     /* MOD  */
    NOT = 312                      /* NOT  */
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
#define LPAREN 284
#define RPAREN 285
#define LBRACE 286
#define RBRACE 287
#define LBRACKET 288
#define RBRACKET 289
#define SEMICOLON 290
#define COMMA 291
#define IDENTIFIER 292
#define STRING_LITERAL 293
#define INT_LITERAL 294
#define FLOAT_LITERAL 295
#define CHAR_LITERAL 296
#define ASSIGN 297
#define ADD_ASSIGN 298
#define SUB_ASSIGN 299
#define OR 300
#define AND 301
#define BITOR 302
#define BITAND 303
#define EQ 304
#define GT 305
#define LT 306
#define PLUS 307
#define MINUS 308
#define MUL 309
#define DIV 310
#define MOD 311
#define NOT 312

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 126 "parser.y"

    int       ival;
    float     fval;
    char      cval;
    char*     sval;
    DataType  dtype;
    AccessMod access;

#line 372 "y.tab.c"

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
  YYSYMBOL_BREAK = 18,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 19,                  /* CONTINUE  */
  YYSYMBOL_SEQ1 = 20,                      /* SEQ1  */
  YYSYMBOL_SEQ2 = 21,                      /* SEQ2  */
  YYSYMBOL_FUNC = 22,                      /* FUNC  */
  YYSYMBOL_ENTITY = 23,                    /* ENTITY  */
  YYSYMBOL_NEW = 24,                       /* NEW  */
  YYSYMBOL_PUBLIC = 25,                    /* PUBLIC  */
  YYSYMBOL_PRIVATE = 26,                   /* PRIVATE  */
  YYSYMBOL_THIS = 27,                      /* THIS  */
  YYSYMBOL_DOT = 28,                       /* DOT  */
  YYSYMBOL_LPAREN = 29,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 30,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 31,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 32,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 33,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 34,                  /* RBRACKET  */
  YYSYMBOL_SEMICOLON = 35,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 36,                     /* COMMA  */
  YYSYMBOL_IDENTIFIER = 37,                /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 38,            /* STRING_LITERAL  */
  YYSYMBOL_INT_LITERAL = 39,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 40,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 41,              /* CHAR_LITERAL  */
  YYSYMBOL_ASSIGN = 42,                    /* ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 43,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 44,                /* SUB_ASSIGN  */
  YYSYMBOL_OR = 45,                        /* OR  */
  YYSYMBOL_AND = 46,                       /* AND  */
  YYSYMBOL_BITOR = 47,                     /* BITOR  */
  YYSYMBOL_BITAND = 48,                    /* BITAND  */
  YYSYMBOL_EQ = 49,                        /* EQ  */
  YYSYMBOL_GT = 50,                        /* GT  */
  YYSYMBOL_LT = 51,                        /* LT  */
  YYSYMBOL_PLUS = 52,                      /* PLUS  */
  YYSYMBOL_MINUS = 53,                     /* MINUS  */
  YYSYMBOL_MUL = 54,                       /* MUL  */
  YYSYMBOL_DIV = 55,                       /* DIV  */
  YYSYMBOL_MOD = 56,                       /* MOD  */
  YYSYMBOL_NOT = 57,                       /* NOT  */
  YYSYMBOL_YYACCEPT = 58,                  /* $accept  */
  YYSYMBOL_program = 59,                   /* program  */
  YYSYMBOL_element = 60,                   /* element  */
  YYSYMBOL_statement = 61,                 /* statement  */
  YYSYMBOL_entity_decl = 62,               /* entity_decl  */
  YYSYMBOL_63_1 = 63,                      /* $@1  */
  YYSYMBOL_64_2 = 64,                      /* $@2  */
  YYSYMBOL_entity_body = 65,               /* entity_body  */
  YYSYMBOL_entity_member = 66,             /* entity_member  */
  YYSYMBOL_constructor_decl = 67,          /* constructor_decl  */
  YYSYMBOL_68_3 = 68,                      /* $@3  */
  YYSYMBOL_69_4 = 69,                      /* $@4  */
  YYSYMBOL_method_decl = 70,               /* method_decl  */
  YYSYMBOL_71_5 = 71,                      /* $@5  */
  YYSYMBOL_72_6 = 72,                      /* $@6  */
  YYSYMBOL_73_7 = 73,                      /* $@7  */
  YYSYMBOL_access_var_decl = 74,           /* access_var_decl  */
  YYSYMBOL_access_modifier = 75,           /* access_modifier  */
  YYSYMBOL_object_decl = 76,               /* object_decl  */
  YYSYMBOL_arg_list_opt = 77,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 78,                  /* arg_list  */
  YYSYMBOL_block = 79,                     /* block  */
  YYSYMBOL_80_8 = 80,                      /* $@8  */
  YYSYMBOL_stmt_list = 81,                 /* stmt_list  */
  YYSYMBOL_var_decl = 82,                  /* var_decl  */
  YYSYMBOL_id_list = 83,                   /* id_list  */
  YYSYMBOL_type = 84,                      /* type  */
  YYSYMBOL_array_decl = 85,                /* array_decl  */
  YYSYMBOL_86_9 = 86,                      /* $@9  */
  YYSYMBOL_87_10 = 87,                     /* $@10  */
  YYSYMBOL_array_init = 88,                /* array_init  */
  YYSYMBOL_array_init2d = 89,              /* array_init2d  */
  YYSYMBOL_row_list = 90,                  /* row_list  */
  YYSYMBOL_expr_list = 91,                 /* expr_list  */
  YYSYMBOL_function_decl = 92,             /* function_decl  */
  YYSYMBOL_93_11 = 93,                     /* $@11  */
  YYSYMBOL_94_12 = 94,                     /* $@12  */
  YYSYMBOL_95_13 = 95,                     /* $@13  */
  YYSYMBOL_func_type = 96,                 /* func_type  */
  YYSYMBOL_param_list_opt = 97,            /* param_list_opt  */
  YYSYMBOL_param_list = 98,                /* param_list  */
  YYSYMBOL_param = 99,                     /* param  */
  YYSYMBOL_return_stmt = 100,              /* return_stmt  */
  YYSYMBOL_expr_stmt = 101,                /* expr_stmt  */
  YYSYMBOL_expression = 102,               /* expression  */
  YYSYMBOL_indexed_id = 103,               /* indexed_id  */
  YYSYMBOL_assignment = 104,               /* assignment  */
  YYSYMBOL_logic_expr = 105,               /* logic_expr  */
  YYSYMBOL_bitwise_expr = 106,             /* bitwise_expr  */
  YYSYMBOL_rel_expr = 107,                 /* rel_expr  */
  YYSYMBOL_arith_expr = 108,               /* arith_expr  */
  YYSYMBOL_term = 109,                     /* term  */
  YYSYMBOL_factor = 110,                   /* factor  */
  YYSYMBOL_if_stmt = 111,                  /* if_stmt  */
  YYSYMBOL_112_14 = 112,                   /* $@14  */
  YYSYMBOL_113_15 = 113,                   /* $@15  */
  YYSYMBOL_114_16 = 114,                   /* $@16  */
  YYSYMBOL_115_17 = 115,                   /* $@17  */
  YYSYMBOL_elif_list = 116,                /* elif_list  */
  YYSYMBOL_117_18 = 117,                   /* $@18  */
  YYSYMBOL_118_19 = 118,                   /* $@19  */
  YYSYMBOL_119_20 = 119,                   /* $@20  */
  YYSYMBOL_120_21 = 120,                   /* $@21  */
  YYSYMBOL_else_opt = 121,                 /* else_opt  */
  YYSYMBOL_122_22 = 122,                   /* $@22  */
  YYSYMBOL_123_23 = 123,                   /* $@23  */
  YYSYMBOL_124_24 = 124,                   /* $@24  */
  YYSYMBOL_125_25 = 125,                   /* $@25  */
  YYSYMBOL_126_26 = 126,                   /* $@26  */
  YYSYMBOL_break_stmt = 127,               /* break_stmt  */
  YYSYMBOL_continue_stmt = 128,            /* continue_stmt  */
  YYSYMBOL_for_stmt = 129,                 /* for_stmt  */
  YYSYMBOL_130_27 = 130,                   /* $@27  */
  YYSYMBOL_for_header = 131,               /* for_header  */
  YYSYMBOL_132_28 = 132,                   /* $@28  */
  YYSYMBOL_133_29 = 133,                   /* $@29  */
  YYSYMBOL_134_30 = 134,                   /* $@30  */
  YYSYMBOL_for_init_opt = 135,             /* for_init_opt  */
  YYSYMBOL_for_cond_opt = 136,             /* for_cond_opt  */
  YYSYMBOL_for_inc_opt = 137,              /* for_inc_opt  */
  YYSYMBOL_var_decl_no_semi = 138,         /* var_decl_no_semi  */
  YYSYMBOL_io_stmt = 139                   /* io_stmt  */
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
#define YYLAST   572

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  82
/* YYNRULES -- Number of rules.  */
#define YYNRULES  180
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  376

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   312


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
      55,    56,    57
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   169,   169,   170,   174,   175,   176,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   200,
     199,   239,   238,   250,   251,   255,   256,   257,   261,   260,
     350,   349,   362,   361,   455,   454,   537,   536,   547,   561,
     577,   578,   582,   628,   685,   686,   690,   697,   707,   706,
     716,   717,   725,   726,   731,   738,   761,   768,   779,   786,
     793,   794,   795,   796,   797,   801,   819,   837,   836,   881,
     880,   921,   925,   929,   935,   944,   956,   973,   972,  1062,
    1061,  1148,  1147,  1158,  1159,  1163,  1164,  1168,  1169,  1174,
    1195,  1218,  1241,  1253,  1261,  1262,  1266,  1269,  1281,  1302,
    1315,  1327,  1340,  1353,  1370,  1372,  1398,  1424,  1428,  1430,
    1432,  1434,  1438,  1440,  1442,  1446,  1451,  1455,  1459,  1463,
    1470,  1476,  1480,  1486,  1492,  1498,  1502,  1509,  1587,  1616,
    1685,  1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,  1721,
    1738,  1740,  1745,  1720,  1749,  1758,  1775,  1777,  1782,  1757,
    1784,  1789,  1788,  1801,  1818,  1820,  1825,  1800,  1827,  1831,
    1842,  1862,  1861,  1888,  1897,  1906,  1913,  1896,  1918,  1919,
    1920,  1924,  1925,  1929,  1930,  1934,  1938,  1952,  1961,  1970,
    1972
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
  "FEED", "SHOW", "RETURN", "BREAK", "CONTINUE", "SEQ1", "SEQ2", "FUNC",
  "ENTITY", "NEW", "PUBLIC", "PRIVATE", "THIS", "DOT", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "SEMICOLON", "COMMA",
  "IDENTIFIER", "STRING_LITERAL", "INT_LITERAL", "FLOAT_LITERAL",
  "CHAR_LITERAL", "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "OR", "AND",
  "BITOR", "BITAND", "EQ", "GT", "LT", "PLUS", "MINUS", "MUL", "DIV",
  "MOD", "NOT", "$accept", "program", "element", "statement",
  "entity_decl", "$@1", "$@2", "entity_body", "entity_member",
  "constructor_decl", "$@3", "$@4", "method_decl", "$@5", "$@6", "$@7",
  "access_var_decl", "access_modifier", "object_decl", "arg_list_opt",
  "arg_list", "block", "$@8", "stmt_list", "var_decl", "id_list", "type",
  "array_decl", "$@9", "$@10", "array_init", "array_init2d", "row_list",
  "expr_list", "function_decl", "$@11", "$@12", "$@13", "func_type",
  "param_list_opt", "param_list", "param", "return_stmt", "expr_stmt",
  "expression", "indexed_id", "assignment", "logic_expr", "bitwise_expr",
  "rel_expr", "arith_expr", "term", "factor", "if_stmt", "$@14", "$@15",
  "$@16", "$@17", "elif_list", "$@18", "$@19", "$@20", "$@21", "else_opt",
  "$@22", "$@23", "$@24", "$@25", "$@26", "break_stmt", "continue_stmt",
  "for_stmt", "$@27", "for_header", "$@28", "$@29", "$@30", "for_init_opt",
  "for_cond_opt", "for_inc_opt", "var_decl_no_semi", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-259)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-162)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -259,   139,  -259,   -20,  -259,  -259,  -259,  -259,  -259,  -259,
     -11,    -9,  -259,  -259,    41,    16,    69,   106,    77,   126,
     490,   236,  -259,   172,  -259,  -259,  -259,  -259,   519,   512,
    -259,  -259,  -259,  -259,  -259,  -259,    21,  -259,  -259,   153,
    -259,  -259,   146,   148,  -259,   -18,    87,  -259,    80,   130,
    -259,  -259,  -259,  -259,  -259,  -259,  -259,   399,   392,   490,
     158,  -259,   333,   160,  -259,  -259,  -259,   169,   183,   167,
    -259,   351,   182,   186,   490,   490,   184,     9,   490,   490,
     136,   200,    95,  -259,  -259,  -259,   193,   195,   192,   162,
     198,  -259,   490,   512,   512,   519,   519,   519,   519,   519,
     519,   519,   519,   519,   519,   206,   490,   208,   454,    79,
    -259,   490,  -259,   213,   215,   205,  -259,  -259,   374,  -259,
     296,    21,  -259,   209,   226,   221,  -259,   224,   449,   230,
    -259,  -259,  -259,  -259,   223,   227,   232,   490,    53,    61,
     483,  -259,  -259,  -259,   233,  -259,   222,  -259,   151,   151,
     151,   130,   130,  -259,  -259,  -259,   241,  -259,   241,   243,
    -259,   241,   246,  -259,   247,   490,  -259,   260,   490,  -259,
    -259,   249,   490,  -259,   490,    63,   248,   251,   245,  -259,
    -259,  -259,   253,   252,  -259,   256,  -259,   259,   381,   255,
     263,   267,  -259,   268,  -259,   262,  -259,  -259,  -259,   273,
      74,   279,  -259,    28,  -259,  -259,   490,   490,   287,  -259,
     288,   284,   285,   289,   290,   291,   283,   292,  -259,    28,
     325,  -259,   490,   490,   302,  -259,  -259,  -259,  -259,  -259,
    -259,  -259,  -259,   112,  -259,   301,   303,   309,   305,  -259,
     308,  -259,   490,   310,   490,   311,   490,   312,   315,   319,
     316,   313,    -3,   329,   341,   241,  -259,  -259,   337,   338,
     345,   350,    15,    70,  -259,  -259,   241,    28,   339,   353,
    -259,   360,  -259,    66,  -259,  -259,   346,   490,   128,  -259,
     349,   490,   241,   241,  -259,  -259,  -259,    28,   386,   364,
     380,   383,   387,  -259,  -259,   490,   400,  -259,  -259,   490,
     407,   129,  -259,   403,  -259,   412,  -259,  -259,   433,   490,
     414,   416,  -259,  -259,  -259,  -259,  -259,  -259,  -259,   413,
    -259,   490,   415,   418,   440,  -259,  -259,   241,   241,   424,
     425,   426,  -259,   131,  -259,  -259,   455,  -259,   435,  -259,
    -259,    28,    28,   465,  -259,   490,   441,   241,  -259,   439,
     442,   444,  -259,  -259,  -259,   241,   241,   241,   445,   490,
    -259,  -259,  -259,  -259,  -259,   241,   447,  -259,  -259,   433,
     241,  -259,  -259,   433,   440,  -259
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,    60,    61,    62,    63,    64,    84,
       0,     0,   136,   137,     0,     0,     0,     0,     0,     0,
       0,     0,    95,   130,   135,   132,   133,   134,     0,     0,
       2,     4,     6,    16,    17,     7,    83,     8,     5,     0,
      13,     9,     0,   131,    96,   107,   111,   114,   118,   121,
     125,    10,    14,    15,    11,    12,    18,     0,     0,     0,
       0,    92,   130,     0,   159,   160,    19,     0,     0,     0,
      50,     0,     0,     0,    45,     0,    59,     0,     0,     0,
       0,     0,   130,   131,   126,   110,     0,     0,    59,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,     0,
      93,     0,    91,     0,     0,   128,   138,    51,   130,    53,
       0,     0,    79,   129,     0,    44,    47,     0,     0,     0,
      99,   100,   101,    56,     0,     0,     0,     0,     0,     0,
       0,    54,    77,   104,   108,   109,   113,   112,   117,   115,
     116,   119,   120,   122,   123,   124,     0,   140,     0,     0,
     169,     0,     0,   168,     0,     0,    24,     0,     0,    49,
      52,     0,     0,   127,     0,    97,     0,     0,     0,    58,
     128,   129,     0,     0,    67,     0,    69,     0,   130,     0,
       0,     0,   144,     0,   163,   175,   162,   164,   179,     0,
       0,     0,   105,    86,   106,    46,     0,     0,     0,    57,
       0,    97,     0,     0,     0,     0,     0,     0,    55,    86,
       0,   141,     0,   172,     0,    40,    41,    20,    28,    23,
      25,    26,    27,     0,    22,     0,     0,     0,    85,    88,
       0,   102,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   129,     0,     0,     0,   176,   171,     0,     0,
       0,     0,     0,     0,    90,    89,     0,     0,    98,     0,
     177,     0,    65,     0,    76,    68,     0,     0,     0,    70,
       0,    45,     0,     0,   142,   165,   180,    86,     0,     0,
       0,     0,     0,    80,    87,     0,     0,    98,    71,     0,
       0,     0,    72,     0,   178,     0,    78,    82,   150,   174,
       0,     0,    34,    39,    32,    38,   103,    42,    75,     0,
      74,     0,     0,     0,   158,   173,   166,     0,     0,     0,
       0,     0,    66,     0,    43,   145,   151,   143,     0,    29,
      31,    86,    86,     0,    73,     0,     0,     0,   167,     0,
       0,     0,   146,   153,   152,     0,     0,     0,     0,     0,
      35,    33,    37,   147,   154,     0,     0,   148,   155,   150,
       0,   149,   156,   150,   158,   157
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -259,  -259,  -259,   -59,  -259,  -259,  -259,  -259,  -259,  -259,
    -259,  -259,  -259,  -259,  -259,  -259,  -259,  -259,  -259,  -217,
    -259,  -145,  -259,  -259,  -259,   -34,     0,  -259,  -259,  -259,
    -259,  -259,  -259,  -258,  -259,  -259,  -259,  -259,  -259,  -215,
    -259,   204,  -259,  -259,   -15,   -19,   -71,   -26,   122,  -259,
      90,   121,   -14,  -259,  -259,  -259,  -259,  -259,  -200,  -259,
    -259,  -259,  -259,   105,  -259,  -259,  -259,  -259,  -259,  -259,
    -259,  -259,  -259,  -259,  -259,  -259,  -259,  -259,  -259,  -259,
    -259,  -259
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    30,    31,    32,   113,   114,   200,   229,   230,
     260,   261,   231,   330,   329,   331,   232,   233,    33,   124,
     125,    34,    71,   120,    35,    80,   236,    37,   213,   215,
     247,   250,   278,   273,    38,   190,   171,   191,    39,   237,
     238,   239,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,   106,   193,   255,   308,   324,   345,
     358,   365,   369,   337,   347,   359,   366,   370,   373,    52,
      53,    54,   108,   161,   223,   309,   338,   162,   258,   326,
     163,    55
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      63,    36,    89,    85,   253,    68,   130,   131,   132,    83,
      83,   192,   119,   194,    84,    56,   196,    60,    57,   301,
      58,   143,    12,    13,   129,   269,   281,    93,    94,    12,
      13,     4,     5,     6,     7,     8,    19,   289,    20,   172,
     130,    86,    87,    19,   109,    20,    62,    24,    25,    26,
      27,    61,   290,    62,    24,    25,    26,    27,    88,   126,
     127,   170,    28,   333,   305,   235,    29,   144,   145,    28,
      59,   121,   310,    29,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,   183,    89,   153,   154,
     155,   157,   291,   160,   185,   184,   206,   202,   298,   225,
     226,   204,   299,   186,    64,   207,   227,   292,   159,   164,
     284,   228,   165,   177,    66,     4,     5,     6,     7,     8,
     121,   293,   182,   136,    74,   189,   349,   350,   137,    97,
      98,    99,   100,   101,    95,    96,   241,   306,   307,     2,
       3,    65,     4,     5,     6,     7,     8,     9,    10,   262,
     199,    11,    12,    13,    67,    14,    15,    16,    17,   205,
     302,   320,    18,   344,   303,   299,    19,   299,    20,   371,
      21,   133,   134,   374,    22,    90,    23,    24,    25,    26,
      27,    91,   339,   340,   102,   103,   104,   148,   149,   150,
      92,   240,    28,   110,    72,   112,    29,   141,   134,   117,
      73,    74,   354,   100,   101,    75,   115,   256,   257,    76,
     360,   361,   362,   116,    77,    78,    79,   146,   147,   122,
     367,   151,   152,   123,   316,   372,   128,   126,   135,   271,
     138,   274,   139,   263,   140,   142,   156,    69,   158,   -48,
     -48,   -48,   -48,   -48,   166,   -48,   167,   168,   -48,   -48,
     -48,   172,   -48,   -48,   -48,   -48,   173,   174,   175,   178,
     179,   201,   274,   -48,   180,   -48,   126,   -48,    70,   181,
      96,   -48,    21,   -48,   -48,   -48,   -48,   -48,   203,    94,
     195,   197,   198,   210,   318,   208,   209,   211,   216,   -48,
     218,   212,   219,   -48,   325,   214,   220,     3,   221,     4,
       5,     6,     7,     8,   222,    10,   274,   224,    11,    12,
      13,   234,    14,    15,    16,    17,   242,   244,   243,   245,
     246,   251,   249,    19,   248,    20,   254,    21,   169,   252,
     352,    22,   259,   118,    24,    25,    26,    27,   264,   266,
     265,   267,   268,   280,   364,   270,   272,   275,   276,    28,
     277,   279,     3,    29,     4,     5,     6,     7,     8,   282,
      10,    73,    74,    11,    12,    13,    75,    14,    15,    16,
      17,   283,   285,   286,   287,   111,    78,    79,    19,   288,
      20,   295,    21,   296,   304,   300,    22,   311,   118,    24,
      25,    26,    27,   107,   297,  -161,  -161,  -161,  -161,  -161,
     105,   312,    73,    74,    28,  -161,  -161,    75,    29,   217,
      74,    76,  -139,  -139,    75,   313,    77,    78,    79,  -161,
     314,  -161,   315,   111,    78,    79,  -139,  -161,  -139,  -161,
    -161,  -161,  -161,  -161,   321,   317,  -139,  -139,  -139,  -139,
    -139,   319,   322,   323,   327,  -161,   328,   335,   332,  -161,
     334,   336,  -139,   341,   342,   343,  -139,     4,     5,     6,
       7,     8,    12,    13,   346,   348,   351,    12,    13,   355,
     353,   294,   356,   176,   357,   363,    19,   368,    20,   375,
       0,    19,     0,    20,     0,     0,    62,    24,    25,    26,
      27,    62,    24,    25,    26,    27,    12,    13,   187,     0,
       0,     0,    28,    12,    13,     0,    29,    28,     0,     0,
      19,    29,    20,     0,     0,     0,     0,    19,     0,    20,
     188,    24,    25,    26,    27,    12,    13,    62,    24,    25,
      26,    27,    12,    13,     0,     0,    28,     0,     0,    81,
      29,    20,     0,    28,     0,     0,    81,    29,    20,    82,
      24,    25,    26,    27,     0,     0,    82,    24,    25,    26,
      27,     0,     0,     0,     0,    28,     0,     0,     0,    29,
       0,     0,    28
};

static const yytype_int16 yycheck[] =
{
      15,     1,    36,    29,   219,    20,    77,    78,    79,    28,
      29,   156,    71,   158,    28,    35,   161,     1,    29,   277,
      29,    92,    13,    14,    15,   242,    29,    45,    46,    13,
      14,     3,     4,     5,     6,     7,    27,    22,    29,    42,
     111,    20,    21,    27,    59,    29,    37,    38,    39,    40,
      41,    35,    37,    37,    38,    39,    40,    41,    37,    74,
      75,   120,    53,   321,   281,    37,    57,    93,    94,    53,
      29,    71,   287,    57,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,    33,   121,   102,   103,
     104,   106,    22,   108,    33,    42,    33,   168,    32,    25,
      26,   172,    36,    42,    35,    42,    32,    37,   108,    30,
     255,    37,    33,   128,    37,     3,     4,     5,     6,     7,
     120,   266,   137,    28,    29,   140,   341,   342,    33,    49,
      50,    51,    52,    53,    47,    48,   207,   282,   283,     0,
       1,    35,     3,     4,     5,     6,     7,     8,     9,    37,
     165,    12,    13,    14,    28,    16,    17,    18,    19,   174,
      32,    32,    23,    32,    36,    36,    27,    36,    29,   369,
      31,    35,    36,   373,    35,    22,    37,    38,    39,    40,
      41,    35,   327,   328,    54,    55,    56,    97,    98,    99,
      42,   206,    53,    35,    22,    35,    57,    35,    36,    32,
      28,    29,   347,    52,    53,    33,    37,   222,   223,    37,
     355,   356,   357,    30,    42,    43,    44,    95,    96,    37,
     365,   100,   101,    37,   295,   370,    42,   242,    28,   244,
      37,   246,    37,   233,    42,    37,    30,     1,    30,     3,
       4,     5,     6,     7,    31,     9,    31,    42,    12,    13,
      14,    42,    16,    17,    18,    19,    30,    36,    34,    29,
      37,     1,   277,    27,    37,    29,   281,    31,    32,    37,
      48,    35,    31,    37,    38,    39,    40,    41,    29,    46,
      37,    35,    35,    38,   299,    37,    35,    34,    29,    53,
      35,    39,    29,    57,   309,    39,    29,     1,    30,     3,
       4,     5,     6,     7,    42,     9,   321,    34,    12,    13,
      14,    32,    16,    17,    18,    19,    29,    33,    30,    34,
      31,    38,    31,    27,    34,    29,     1,    31,    32,    37,
     345,    35,    30,    37,    38,    39,    40,    41,    37,    30,
      37,    36,    34,    30,   359,    35,    35,    35,    33,    53,
      31,    35,     1,    57,     3,     4,     5,     6,     7,    30,
       9,    28,    29,    12,    13,    14,    33,    16,    17,    18,
      19,    30,    35,    35,    29,    42,    43,    44,    27,    29,
      29,    42,    31,    30,    35,    39,    35,     1,    37,    38,
      39,    40,    41,     1,    34,     3,     4,     5,     6,     7,
       1,    37,    28,    29,    53,    13,    14,    33,    57,    28,
      29,    37,    13,    14,    33,    35,    42,    43,    44,    27,
      37,    29,    35,    42,    43,    44,    27,    35,    29,    37,
      38,    39,    40,    41,    31,    35,    37,    38,    39,    40,
      41,    34,    30,    10,    30,    53,    30,    29,    35,    57,
      35,    11,    53,    29,    29,    29,    57,     3,     4,     5,
       6,     7,    13,    14,     9,    30,     1,    13,    14,    30,
      29,   267,    30,    24,    30,    30,    27,    30,    29,   374,
      -1,    27,    -1,    29,    -1,    -1,    37,    38,    39,    40,
      41,    37,    38,    39,    40,    41,    13,    14,    15,    -1,
      -1,    -1,    53,    13,    14,    -1,    57,    53,    -1,    -1,
      27,    57,    29,    -1,    -1,    -1,    -1,    27,    -1,    29,
      37,    38,    39,    40,    41,    13,    14,    37,    38,    39,
      40,    41,    13,    14,    -1,    -1,    53,    -1,    -1,    27,
      57,    29,    -1,    53,    -1,    -1,    27,    57,    29,    37,
      38,    39,    40,    41,    -1,    -1,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    57,
      -1,    -1,    53
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    59,     0,     1,     3,     4,     5,     6,     7,     8,
       9,    12,    13,    14,    16,    17,    18,    19,    23,    27,
      29,    31,    35,    37,    38,    39,    40,    41,    53,    57,
      60,    61,    62,    76,    79,    82,    84,    85,    92,    96,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   127,   128,   129,   139,    35,    29,    29,    29,
       1,    35,    37,   102,    35,    35,    37,    28,   102,     1,
      32,    80,    22,    28,    29,    33,    37,    42,    43,    44,
      83,    27,    37,   103,   110,   105,    20,    21,    37,    83,
      22,    35,    42,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,     1,   112,     1,   130,   102,
      35,    42,    35,    63,    64,    37,    30,    32,    37,    61,
      81,    84,    37,    37,    77,    78,   102,   102,    42,    15,
     104,   104,   104,    35,    36,    28,    28,    33,    37,    37,
      42,    35,    37,   104,   105,   105,   106,   106,   108,   108,
     108,   109,   109,   110,   110,   110,    30,   102,    30,    84,
     102,   131,   135,   138,    30,    33,    31,    31,    42,    32,
      61,    94,    42,    30,    36,    34,    24,   102,    29,    37,
      37,    37,   102,    33,    42,    33,    42,    15,    37,   102,
      93,    95,    79,   113,    79,    37,    79,    35,    35,   102,
      65,     1,   104,    29,   104,   102,    33,    42,    37,    35,
      38,    34,    39,    86,    39,    87,    29,    28,    35,    29,
      29,    30,    42,   132,    34,    25,    26,    32,    37,    66,
      67,    70,    74,    75,    32,    37,    84,    97,    98,    99,
     102,   104,    29,    30,    33,    34,    31,    88,    34,    31,
      89,    38,    37,    97,     1,   114,   102,   102,   136,    30,
      68,    69,    37,    84,    37,    37,    30,    36,    34,    77,
      35,   102,    35,    91,   102,    35,    33,    31,    90,    35,
      30,    29,    30,    30,    79,    35,    35,    29,    29,    22,
      37,    22,    37,    79,    99,    42,    30,    34,    32,    36,
      39,    91,    32,    36,    35,    77,    79,    79,   115,   133,
      97,     1,    37,    35,    37,    35,   104,    35,   102,    34,
      32,    31,    30,    10,   116,   102,   137,    30,    30,    72,
      71,    73,    35,    91,    35,    29,    11,   121,   134,    79,
      79,    29,    29,    29,    32,   117,     9,   122,    30,    97,
      97,     1,   102,    29,    79,    30,    30,    30,   118,   123,
      79,    79,    79,    30,   102,   119,   124,    79,    30,   120,
     125,   116,    79,   126,   116,   121
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    59,    60,    60,    60,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    63,
      62,    64,    62,    65,    65,    66,    66,    66,    68,    67,
      69,    67,    71,    70,    72,    70,    73,    70,    74,    74,
      75,    75,    76,    76,    77,    77,    78,    78,    80,    79,
      79,    79,    81,    81,    82,    82,    82,    82,    83,    83,
      84,    84,    84,    84,    84,    85,    85,    86,    85,    87,
      85,    88,    89,    90,    90,    91,    91,    93,    92,    94,
      92,    95,    92,    96,    96,    97,    97,    98,    98,    99,
      99,   100,   100,   100,   101,   101,   102,   103,   103,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   105,   105,
     105,   105,   106,   106,   106,   107,   107,   107,   107,   108,
     108,   108,   109,   109,   109,   109,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   112,
     113,   114,   115,   111,   111,   117,   118,   119,   120,   116,
     116,   122,   121,   123,   124,   125,   126,   121,   121,   127,
     128,   130,   129,   129,   132,   133,   134,   131,   135,   135,
     135,   136,   136,   137,   137,   138,   138,   139,   139,   139,
     139
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       6,     0,     6,     2,     0,     1,     1,     1,     0,     6,
       0,     6,     0,     9,     0,     9,     0,     9,     4,     4,
       1,     1,     9,    10,     1,     0,     3,     1,     0,     4,
       2,     3,     2,     1,     3,     5,     3,     5,     3,     1,
       1,     1,     1,     1,     1,     7,    10,     0,     7,     0,
       7,     3,     3,     5,     3,     3,     1,     0,     8,     0,
       8,     0,     8,     1,     1,     1,     0,     3,     1,     2,
       2,     3,     2,     3,     2,     1,     1,     4,     7,     3,
       3,     3,     6,     9,     3,     5,     5,     1,     3,     3,
       2,     1,     3,     3,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     2,     4,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       0,     0,     0,    11,     5,     0,     0,     0,     0,    10,
       0,     0,     3,     0,     0,     0,     0,    12,     0,     2,
       2,     0,     5,     5,     0,     0,     0,     9,     1,     1,
       0,     1,     0,     1,     0,     2,     4,     7,     8,     5,
       8
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
  case 18: /* statement: error SEMICOLON  */
#line 192 "parser.y"
        {
            printf("Invalid statement at line %d\n", yylineno);
            yyerrok;
        }
#line 1800 "y.tab.c"
    break;

  case 19: /* $@1: %empty  */
#line 200 "parser.y"
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
#line 1824 "y.tab.c"
    break;

  case 20: /* entity_decl: ENTITY IDENTIFIER $@1 LBRACE entity_body RBRACE  */
#line 220 "parser.y"
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
#line 1847 "y.tab.c"
    break;

  case 21: /* $@2: %empty  */
#line 239 "parser.y"
        { emit("entity", (yyvsp[0].sval), "", ""); }
#line 1853 "y.tab.c"
    break;

  case 22: /* entity_decl: ENTITY IDENTIFIER $@2 LBRACE error RBRACE  */
#line 241 "parser.y"
        {
            printf("Invalid entity body at line %d\n", yylineno);
            yyerrok;
            emit("end_entity", (yyvsp[-4].sval), "", "");
        }
#line 1863 "y.tab.c"
    break;

  case 28: /* $@3: %empty  */
#line 261 "parser.y"
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
#line 1889 "y.tab.c"
    break;

  case 29: /* constructor_decl: IDENTIFIER $@3 LPAREN param_list_opt RPAREN block  */
#line 283 "parser.y"
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
#line 1960 "y.tab.c"
    break;

  case 30: /* $@4: %empty  */
#line 350 "parser.y"
        { emit("constr", (yyvsp[0].sval), "", ""); }
#line 1966 "y.tab.c"
    break;

  case 31: /* constructor_decl: IDENTIFIER $@4 LPAREN error RPAREN block  */
#line 352 "parser.y"
        {
            printf("Invalid constructor parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_constr", (yyvsp[-5].sval), "", "");
        }
#line 1976 "y.tab.c"
    break;

  case 32: /* $@5: %empty  */
#line 362 "parser.y"
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
#line 2005 "y.tab.c"
    break;

  case 33: /* method_decl: access_modifier type FUNC IDENTIFIER $@5 LPAREN param_list_opt RPAREN block  */
#line 387 "parser.y"
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
#line 2075 "y.tab.c"
    break;

  case 34: /* $@6: %empty  */
#line 455 "parser.y"
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
#line 2100 "y.tab.c"
    break;

  case 35: /* method_decl: access_modifier IDENTIFIER FUNC IDENTIFIER $@6 LPAREN param_list_opt RPAREN block  */
#line 476 "parser.y"
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
#line 2165 "y.tab.c"
    break;

  case 36: /* $@7: %empty  */
#line 537 "parser.y"
        { emit("method", (yyvsp[0].sval), "", ""); }
#line 2171 "y.tab.c"
    break;

  case 37: /* method_decl: access_modifier type FUNC IDENTIFIER $@7 LPAREN error RPAREN block  */
#line 539 "parser.y"
        {
            printf("Invalid method parameters at line %d\n", yylineno);
            yyerrok;
            emit("end_method", (yyvsp[-5].sval), "", "");
        }
#line 2181 "y.tab.c"
    break;

  case 38: /* access_var_decl: access_modifier type IDENTIFIER SEMICOLON  */
#line 548 "parser.y"
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
#line 2198 "y.tab.c"
    break;

  case 39: /* access_var_decl: access_modifier IDENTIFIER IDENTIFIER SEMICOLON  */
#line 562 "parser.y"
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
#line 2215 "y.tab.c"
    break;

  case 40: /* access_modifier: PUBLIC  */
#line 577 "parser.y"
              { (yyval.access) = ACC_PUBLIC;  }
#line 2221 "y.tab.c"
    break;

  case 41: /* access_modifier: PRIVATE  */
#line 578 "parser.y"
              { (yyval.access) = ACC_PRIVATE; }
#line 2227 "y.tab.c"
    break;

  case 42: /* object_decl: IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 583 "parser.y"
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
#line 2277 "y.tab.c"
    break;

  case 43: /* object_decl: type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 629 "parser.y"
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
#line 2335 "y.tab.c"
    break;

  case 45: /* arg_list_opt: %empty  */
#line 686 "parser.y"
                        { call_arg_count = 0; }
#line 2341 "y.tab.c"
    break;

  case 46: /* arg_list: arg_list COMMA expression  */
#line 691 "parser.y"
        {
		if(call_arg_count < 64){
			call_arg_types[call_arg_count++] = last_expr_type;
		} 
		emit("arg", (yyvsp[0].sval), "", ""); 
	}
#line 2352 "y.tab.c"
    break;

  case 47: /* arg_list: expression  */
#line 698 "parser.y"
        { 
		call_arg_count = 0;
		call_arg_types[call_arg_count++] = last_expr_type;
		emit("arg", (yyvsp[0].sval), "", ""); 
	}
#line 2362 "y.tab.c"
    break;

  case 48: /* $@8: %empty  */
#line 707 "parser.y"
        {
            SymTable* bs = create_scope(SCOPE_BLOCK, "block", current_scope);
            current_scope = bs;
        }
#line 2371 "y.tab.c"
    break;

  case 49: /* block: LBRACE $@8 stmt_list RBRACE  */
#line 712 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 2380 "y.tab.c"
    break;

  case 51: /* block: LBRACE error RBRACE  */
#line 718 "parser.y"
        {
            printf("Error inside block at line %d\n", yylineno);
            yyerrok;
        }
#line 2389 "y.tab.c"
    break;

  case 54: /* var_decl: type id_list SEMICOLON  */
#line 732 "parser.y"
        {
            insert_var_list((yyvsp[-1].sval), (yyvsp[-2].dtype));
            free((yyvsp[-1].sval));
        }
#line 2398 "y.tab.c"
    break;

  case 55: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 739 "parser.y"
        {
	    if(last_expr_type != DT_UNKNOWN && last_expr_type != (yyvsp[-4].dtype)){
		//printf("==== $1 = %d and last_expr_type = %d\n", $1, last_expr_type);
		fprintf(stderr, "ERROR line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, (yyvsp[-3].sval), dt_names[(yyvsp[-4].dtype)], dt_names[last_expr_type]);
	    }
            //emit("=", $4, "", $2);
	    printf("Inserting the symbol: %s with the scope: %d\n", (yyvsp[-3].sval), current_scope->kind); 
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
                                        KIND_VAR, (yyvsp[-4].dtype), yylineno);
	    emit("=", ir_name_of((yyvsp[-1].sval)), "", ir_name_of((yyvsp[-3].sval)));
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
#line 2423 "y.tab.c"
    break;

  case 56: /* var_decl: IDENTIFIER id_list SEMICOLON  */
#line 762 "parser.y"
        {
            insert_var_list((yyvsp[-1].sval), DT_ENTITY);
            free((yyvsp[-1].sval));
        }
#line 2432 "y.tab.c"
    break;

  case 57: /* var_decl: IDENTIFIER IDENTIFIER ASSIGN expression SEMICOLON  */
#line 769 "parser.y"
        {
		fprintf(stderr, "ERROR line %d: cannot initialize entity variable '%s' with value of type %s.\n", yylineno, (yyvsp[-3].sval), dt_names[last_expr_type]);
		emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval));
		Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
						KIND_VAR, DT_ENTITY, yylineno);
		if (sym) sym->is_initialized = 1;
        }
#line 2444 "y.tab.c"
    break;

  case 58: /* id_list: id_list COMMA IDENTIFIER  */
#line 780 "parser.y"
        {
            char* buf = malloc(strlen((yyvsp[-2].sval)) + strlen((yyvsp[0].sval)) + 2);
            sprintf(buf, "%s,%s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval));
            (yyval.sval) = buf;
        }
#line 2455 "y.tab.c"
    break;

  case 59: /* id_list: IDENTIFIER  */
#line 787 "parser.y"
        {
            (yyval.sval) = strdup((yyvsp[0].sval));
        }
#line 2463 "y.tab.c"
    break;

  case 60: /* type: INT  */
#line 793 "parser.y"
             { (yyval.dtype) = DT_INT;    }
#line 2469 "y.tab.c"
    break;

  case 61: /* type: FP  */
#line 794 "parser.y"
             { (yyval.dtype) = DT_FLOAT;  }
#line 2475 "y.tab.c"
    break;

  case 62: /* type: CHR  */
#line 795 "parser.y"
             { (yyval.dtype) = DT_CHAR;   }
#line 2481 "y.tab.c"
    break;

  case 63: /* type: STRING  */
#line 796 "parser.y"
             { (yyval.dtype) = DT_STRING; }
#line 2487 "y.tab.c"
    break;

  case 64: /* type: BOOL  */
#line 797 "parser.y"
             { (yyval.dtype) = DT_BOOL;   }
#line 2493 "y.tab.c"
    break;

  case 65: /* array_decl: type SEQ1 IDENTIFIER LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 802 "parser.y"
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
#line 2513 "y.tab.c"
    break;

  case 66: /* array_decl: type SEQ2 IDENTIFIER LBRACKET INT_LITERAL RBRACKET LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 821 "parser.y"
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
#line 2530 "y.tab.c"
    break;

  case 67: /* $@9: %empty  */
#line 837 "parser.y"
        {
		current_array_elem_type = (yyvsp[-3].dtype);
		array_type_errors = 0;
	}
#line 2539 "y.tab.c"
    break;

  case 68: /* array_decl: type SEQ1 IDENTIFIER ASSIGN $@9 array_init SEMICOLON  */
#line 842 "parser.y"
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
            char init_str[512] = "";
            for (int i = 0; i < arr_init_count; i++) {
                if (i > 0) strcat(init_str, ",");
                strcat(init_str, arr_init_vals[i]);
            }
            emit("array_init", init_str, "", (yyvsp[-4].sval));
            /* ── END NEW ── */

            arr_init_count = 0;
            current_array_elem_type = DT_UNKNOWN;
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
#line 2582 "y.tab.c"
    break;

  case 69: /* $@10: %empty  */
#line 881 "parser.y"
        {
		current_array_elem_type = (yyvsp[-3].dtype);
		array_type_errors = 0;
		arr2d_rows = 0;
		arr2d_cols = 0;
	}
#line 2593 "y.tab.c"
    break;

  case 70: /* array_decl: type SEQ2 IDENTIFIER ASSIGN $@10 array_init2d SEMICOLON  */
#line 888 "parser.y"
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
#line 2628 "y.tab.c"
    break;

  case 71: /* array_init: LBRACE expr_list RBRACE  */
#line 921 "parser.y"
                              { (yyval.ival) = (yyvsp[-1].ival); }
#line 2634 "y.tab.c"
    break;

  case 72: /* array_init2d: LBRACE row_list RBRACE  */
#line 925 "parser.y"
                                   { (yyval.ival) = (yyvsp[-1].ival); }
#line 2640 "y.tab.c"
    break;

  case 73: /* row_list: row_list COMMA LBRACE expr_list RBRACE  */
#line 930 "parser.y"
        { 
		arr2d_rows++;
		arr2d_cols = (yyvsp[-1].ival);
		(yyval.ival) = (yyvsp[-4].ival) + (yyvsp[-1].ival); 
	}
#line 2650 "y.tab.c"
    break;

  case 74: /* row_list: LBRACE expr_list RBRACE  */
#line 936 "parser.y"
        { 
		arr2d_rows = 1;
		arr2d_cols = (yyvsp[-1].ival);
		(yyval.ival) = (yyvsp[-1].ival); 
	}
#line 2660 "y.tab.c"
    break;

  case 75: /* expr_list: expr_list COMMA expression  */
#line 945 "parser.y"
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
#line 2676 "y.tab.c"
    break;

  case 76: /* expr_list: expression  */
#line 957 "parser.y"
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
#line 2693 "y.tab.c"
    break;

  case 77: /* $@11: %empty  */
#line 973 "parser.y"
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
            emit("func", ir_name_of((yyvsp[0].sval)), "", "");
            pending_func_ir_idx = IR_idx - 1;

            // ADD THIS LINE right here:
            pending_call_count = 0;

            SymTable* fs = create_scope(SCOPE_FUNCTION, (yyvsp[0].sval), current_scope);
            if(sym) sym->attr.func.scope = fs;
            current_scope = fs;
        }
#line 2720 "y.tab.c"
    break;

  case 78: /* function_decl: func_type FUNC IDENTIFIER $@11 LPAREN param_list_opt RPAREN block  */
#line 996 "parser.y"
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

            for(int i = 0; i < pending_call_count; i++){
                if(strcmp(pending_call_base[i], (yyvsp[-5].sval)) == 0){
                    strncpy(IR[pending_calls[i]].arg1, mangled_ir, 19);
                    for(int j = i; j < pending_call_count - 1; j++){
                        pending_calls[j] = pending_calls[j+1];
                        strncpy(pending_call_base[j], pending_call_base[j+1], 63);
                    }
                    pending_call_count--;
                    i--;
                }
            }

            print_table(current_scope);
            current_scope = current_scope->parent;
            current_function = NULL;
            emit("endfunc", "", "", "");
        }
#line 2789 "y.tab.c"
    break;

  case 79: /* $@12: %empty  */
#line 1062 "parser.y"
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

            emit("func", ir_name_of((yyvsp[0].sval)), "", "");
            pending_func_ir_idx = IR_idx - 1;

            // ADD THIS LINE right here:
            pending_call_count = 0;

            SymTable* fs = create_scope(SCOPE_FUNCTION, (yyvsp[0].sval), current_scope);
            if(sym) sym->attr.func.scope = fs;
            current_scope = fs;
        }
#line 2815 "y.tab.c"
    break;

  case 80: /* function_decl: IDENTIFIER FUNC IDENTIFIER $@12 LPAREN param_list_opt RPAREN block  */
#line 1084 "parser.y"
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

            for(int i = 0; i < pending_call_count; i++){
                if(strcmp(pending_call_base[i], (yyvsp[-5].sval)) == 0){
                    strncpy(IR[pending_calls[i]].arg1, mangled_ir, 19);
                    for(int j = i; j < pending_call_count - 1; j++){
                        pending_calls[j] = pending_calls[j+1];
                        strncpy(pending_call_base[j], pending_call_base[j+1], 63);
                    }
                    pending_call_count--;
                    i--;
                }
            }

            print_table(current_scope);
            current_scope = current_scope->parent;
            current_function = NULL;
            emit("endfunc", "", "", "");
        }
#line 2882 "y.tab.c"
    break;

  case 81: /* $@13: %empty  */
#line 1148 "parser.y"
        { emit("func", (yyvsp[0].sval), "", ""); }
#line 2888 "y.tab.c"
    break;

  case 82: /* function_decl: func_type FUNC IDENTIFIER $@13 LPAREN error RPAREN block  */
#line 1150 "parser.y"
        {
            printf("Invalid parameter list at line %d\n", yylineno);
            yyerrok;
            emit("endfunc", "", "", "");
        }
#line 2898 "y.tab.c"
    break;

  case 83: /* func_type: type  */
#line 1158 "parser.y"
           { (yyval.dtype) = (yyvsp[0].dtype);      }
#line 2904 "y.tab.c"
    break;

  case 84: /* func_type: VOID  */
#line 1159 "parser.y"
           { (yyval.dtype) = DT_VOID; }
#line 2910 "y.tab.c"
    break;

  case 89: /* param: type IDENTIFIER  */
#line 1175 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_PARAM, (yyvsp[-1].dtype), yylineno);
            emit("param", ir_name_of((yyvsp[0].sval)), "", "");
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
#line 2934 "y.tab.c"
    break;

  case 90: /* param: IDENTIFIER IDENTIFIER  */
#line 1196 "parser.y"
        {
            Symbol* sym = insert_symbol(current_scope, (yyvsp[0].sval),
                                        KIND_PARAM, DT_ENTITY, yylineno);
            emit("param", ir_name_of((yyvsp[0].sval)), "", "");
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
#line 2958 "y.tab.c"
    break;

  case 91: /* return_stmt: RETURN expression SEMICOLON  */
#line 1219 "parser.y"
        { 
		if(current_function) {
			DataType expected;
            if(current_function->kind == KIND_FUNCTION){
                 expected = current_function->attr.func.return_type;
            }
            else if(current_function->kind == KIND_METHOD){
                expected = current_function->attr.method.return_type;
            }
            else{
                expected = DT_VOID;
            }
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
#line 2985 "y.tab.c"
    break;

  case 92: /* return_stmt: RETURN SEMICOLON  */
#line 1242 "parser.y"
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
#line 3000 "y.tab.c"
    break;

  case 93: /* return_stmt: RETURN error SEMICOLON  */
#line 1254 "parser.y"
        {
            printf("Invalid return statement at line %d\n", yylineno);
            yyerrok;
        }
#line 3009 "y.tab.c"
    break;

  case 96: /* expression: assignment  */
#line 1266 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 3015 "y.tab.c"
    break;

  case 97: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET  */
#line 1270 "parser.y"
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
#line 3031 "y.tab.c"
    break;

  case 98: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET  */
#line 1282 "parser.y"
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
#line 3053 "y.tab.c"
    break;

  case 99: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 1303 "parser.y"
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
#line 3070 "y.tab.c"
    break;

  case 100: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 1316 "parser.y"
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
#line 3086 "y.tab.c"
    break;

  case 101: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 1328 "parser.y"
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
#line 3103 "y.tab.c"
    break;

  case 102: /* assignment: IDENTIFIER LBRACKET expression RBRACKET ASSIGN assignment  */
#line 1341 "parser.y"
    {
        Symbol* asym = require_declared(current_scope, (yyvsp[-5].sval), yylineno);
        char width_str[16];
        if(asym && asym->kind == KIND_ARRAY)
            sprintf(width_str, "%d", datatype_size(asym->datatype));
        else
            strcpy(width_str, "type.width");
        char* t1 = genVar();
        emit("*", (yyvsp[-3].sval), width_str, t1);
        emit("[]=", (yyvsp[-5].sval), t1, (yyvsp[0].sval));
        (yyval.sval) = (yyvsp[0].sval);
    }
#line 3120 "y.tab.c"
    break;

  case 103: /* assignment: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET ASSIGN assignment  */
#line 1354 "parser.y"
    {
        Symbol* asym = require_declared(current_scope, (yyvsp[-8].sval), yylineno);
        char cols_str[16]; char width_str[16];
        if(asym && asym->kind == KIND_ARRAY){
            sprintf(cols_str, "%d", asym->attr.array.dim2);
            sprintf(width_str, "%d", datatype_size(asym->datatype));
        } else {
            strcpy(cols_str, "array.cols");
            strcpy(width_str, "type.width");
        }
        char* t1 = genVar(); emit("*", (yyvsp[-6].sval), cols_str, t1);
        char* t2 = genVar(); emit("+", t1, (yyvsp[-3].sval), t2);
        char* t3 = genVar(); emit("*", t2, width_str, t3);
        emit("[]=", (yyvsp[-8].sval), t3, (yyvsp[0].sval));
        (yyval.sval) = (yyvsp[0].sval);
    }
#line 3141 "y.tab.c"
    break;

  case 104: /* assignment: indexed_id ASSIGN assignment  */
#line 1371 "parser.y"
        { (yyval.sval) = (yyvsp[0].sval); }
#line 3147 "y.tab.c"
    break;

  case 105: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 1373 "parser.y"
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
#line 3177 "y.tab.c"
    break;

  case 106: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 1399 "parser.y"
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
#line 3207 "y.tab.c"
    break;

  case 107: /* assignment: logic_expr  */
#line 1424 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 3213 "y.tab.c"
    break;

  case 108: /* logic_expr: logic_expr OR logic_expr  */
#line 1429 "parser.y"
        { char* t = genVar(); emit("||", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3219 "y.tab.c"
    break;

  case 109: /* logic_expr: logic_expr AND logic_expr  */
#line 1431 "parser.y"
        { char* t = genVar(); emit("&&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3225 "y.tab.c"
    break;

  case 110: /* logic_expr: NOT logic_expr  */
#line 1433 "parser.y"
        { char* t = genVar(); emit("!", (yyvsp[0].sval), "", t); (yyval.sval) = t; }
#line 3231 "y.tab.c"
    break;

  case 111: /* logic_expr: bitwise_expr  */
#line 1434 "parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 3237 "y.tab.c"
    break;

  case 112: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 1439 "parser.y"
        { char* t = genVar(); emit("&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3243 "y.tab.c"
    break;

  case 113: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 1441 "parser.y"
        { char* t = genVar(); emit("|", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3249 "y.tab.c"
    break;

  case 114: /* bitwise_expr: rel_expr  */
#line 1442 "parser.y"
               { (yyval.sval) = (yyvsp[0].sval); }
#line 3255 "y.tab.c"
    break;

  case 115: /* rel_expr: arith_expr GT arith_expr  */
#line 1447 "parser.y"
        {
          char* t = genVar(); emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
            
        }
#line 3264 "y.tab.c"
    break;

  case 116: /* rel_expr: arith_expr LT arith_expr  */
#line 1452 "parser.y"
        {
                char* t = genVar(); emit("<", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
        }
#line 3272 "y.tab.c"
    break;

  case 117: /* rel_expr: arith_expr EQ arith_expr  */
#line 1456 "parser.y"
        {
                char* t = genVar(); emit("==", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
        }
#line 3280 "y.tab.c"
    break;

  case 118: /* rel_expr: arith_expr  */
#line 1459 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 3286 "y.tab.c"
    break;

  case 119: /* arith_expr: arith_expr PLUS term  */
#line 1464 "parser.y"
       {
                char* t = genVar();
                emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
             
        }
#line 3297 "y.tab.c"
    break;

  case 120: /* arith_expr: arith_expr MINUS term  */
#line 1471 "parser.y"
        {  
                char* t = genVar();
                emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
        }
#line 3307 "y.tab.c"
    break;

  case 121: /* arith_expr: term  */
#line 1476 "parser.y"
           { (yyval.sval) = (yyvsp[0].sval); }
#line 3313 "y.tab.c"
    break;

  case 122: /* term: term MUL factor  */
#line 1481 "parser.y"
        {
                char* t = genVar();
                emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
        }
#line 3323 "y.tab.c"
    break;

  case 123: /* term: term DIV factor  */
#line 1487 "parser.y"
        {
                char* t = genVar();
                emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
        }
#line 3333 "y.tab.c"
    break;

  case 124: /* term: term MOD factor  */
#line 1493 "parser.y"
        {
                char* t = genVar();
                emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                (yyval.sval) = t;
        }
#line 3343 "y.tab.c"
    break;

  case 125: /* term: factor  */
#line 1498 "parser.y"
             { (yyval.sval) = (yyvsp[0].sval); }
#line 3349 "y.tab.c"
    break;

  case 126: /* factor: MINUS factor  */
#line 1503 "parser.y"
        {
            char* t = genVar();
            emit("-", "0", (yyvsp[0].sval), t);
            (yyval.sval) = t;
            last_expr_type = DT_INT;
        }
#line 3360 "y.tab.c"
    break;

  case 127: /* factor: IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 1510 "parser.y"
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

        // ===== FIX: if we emitted unmangled, record for later patching =====
        if(fsym && !is_already_mangled(fsym->name)){
            if(pending_call_count < 1000){
                pending_calls[pending_call_count] = IR_idx - 1;
                strncpy(pending_call_base[pending_call_count], (yyvsp[-3].sval), 63);
                pending_call_count++;
            }
        }

        (yyval.sval) = t;
    }
#line 3442 "y.tab.c"
    break;

  case 128: /* factor: THIS DOT IDENTIFIER  */
#line 1588 "parser.y"
        {
            char* t = genVar();
            
            const char* entity_name = NULL;
            if(current_function && current_function->kind == KIND_METHOD){
                entity_name = current_function->attr.method.belongs_to;
            }

            if(entity_name){
                SymTable* esc = find_entity_scope(entity_name);
                Symbol* field = esc ? lookup_local(esc, (yyvsp[0].sval)) : NULL;
                if(field && field->kind == KIND_FIELD){
                    last_expr_type = field->datatype;
                }
                else{
                    fprintf(stderr,
                        "ERROR line %d: '%s' is not a field of '%s'.\n",
                        yylineno, (yyvsp[0].sval), entity_name);
                    last_expr_type = DT_UNKNOWN;
                }
            }
            else{
                last_expr_type = DT_UNKNOWN;
            }
            emit("get_field", "this", (yyvsp[0].sval), t);
            (yyval.sval) = t;
        }
#line 3474 "y.tab.c"
    break;

  case 129: /* factor: IDENTIFIER DOT IDENTIFIER  */
#line 1617 "parser.y"
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
#line 3547 "y.tab.c"
    break;

  case 130: /* factor: IDENTIFIER  */
#line 1686 "parser.y"
    { 
        int handled = 0;
        
        /* Check if bare field name inside a method */
        if(current_function && current_function->kind == KIND_METHOD){
            const char* ename = current_function->attr.method.belongs_to;
            SymTable* esc = find_entity_scope(ename);
            Symbol* field = esc ? lookup_local(esc, (yyvsp[0].sval)) : NULL;
            if(field && field->kind == KIND_FIELD){
                char* t = genVar();
                last_expr_type = field->datatype;
                emit("get_field", "this", (yyvsp[0].sval), t);
                (yyval.sval) = t;
                handled = 1;
            }
        }
        
        if(!handled){
            Symbol* s = require_declared(current_scope, (yyvsp[0].sval), yylineno);
            last_expr_type = s ? s->datatype : DT_UNKNOWN;
            (yyval.sval) = strdup(ir_name_of((yyvsp[0].sval)));
        }
    }
#line 3575 "y.tab.c"
    break;

  case 131: /* factor: indexed_id  */
#line 1709 "parser.y"
                      { (yyval.sval) = (yyvsp[0].sval); }
#line 3581 "y.tab.c"
    break;

  case 132: /* factor: INT_LITERAL  */
#line 1710 "parser.y"
                      { char b[20]; sprintf(b, "%d",   (yyvsp[0].ival)); (yyval.sval) = strdup(b); last_expr_type = DT_INT; }
#line 3587 "y.tab.c"
    break;

  case 133: /* factor: FLOAT_LITERAL  */
#line 1711 "parser.y"
                      { char b[20]; sprintf(b, "%f",   (yyvsp[0].fval)); (yyval.sval) = strdup(b);  last_expr_type = DT_FLOAT; }
#line 3593 "y.tab.c"
    break;

  case 134: /* factor: CHAR_LITERAL  */
#line 1712 "parser.y"
                      { char b[20]; sprintf(b, "'%c'", (yyvsp[0].cval)); (yyval.sval) = strdup(b); last_expr_type = DT_CHAR; }
#line 3599 "y.tab.c"
    break;

  case 135: /* factor: STRING_LITERAL  */
#line 1713 "parser.y"
                      { (yyval.sval) = strdup((yyvsp[0].sval));  last_expr_type = DT_STRING; }
#line 3605 "y.tab.c"
    break;

  case 136: /* factor: TRUE  */
#line 1714 "parser.y"
                      { (yyval.sval) = strdup("1"); last_expr_type = DT_BOOL; }
#line 3611 "y.tab.c"
    break;

  case 137: /* factor: FALSE  */
#line 1715 "parser.y"
                      { (yyval.sval) = strdup("0"); last_expr_type = DT_BOOL; }
#line 3617 "y.tab.c"
    break;

  case 138: /* factor: LPAREN expression RPAREN  */
#line 1716 "parser.y"
                               { (yyval.sval) = (yyvsp[-1].sval); }
#line 3623 "y.tab.c"
    break;

  case 139: /* $@14: %empty  */
#line 1721 "parser.y"
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
#line 3644 "y.tab.c"
    break;

  case 140: /* $@15: %empty  */
#line 1738 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3650 "y.tab.c"
    break;

  case 141: /* $@16: %empty  */
#line 1740 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3659 "y.tab.c"
    break;

  case 142: /* $@17: %empty  */
#line 1745 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3665 "y.tab.c"
    break;

  case 143: /* if_stmt: IF LPAREN $@14 expression $@15 RPAREN $@16 block $@17 elif_list else_opt  */
#line 1747 "parser.y"
        { emit("label", "", "", topEnd()); popIfLabels(); }
#line 3671 "y.tab.c"
    break;

  case 144: /* if_stmt: IF LPAREN error RPAREN block  */
#line 1750 "parser.y"
        {
            printf("Invalid IF condition at line %d\n", yylineno);
            yyerrok;
        }
#line 3680 "y.tab.c"
    break;

  case 145: /* $@18: %empty  */
#line 1758 "parser.y"
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
#line 3701 "y.tab.c"
    break;

  case 146: /* $@19: %empty  */
#line 1775 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3707 "y.tab.c"
    break;

  case 147: /* $@20: %empty  */
#line 1777 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3716 "y.tab.c"
    break;

  case 148: /* $@21: %empty  */
#line 1782 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3722 "y.tab.c"
    break;

  case 151: /* $@22: %empty  */
#line 1789 "parser.y"
        {
            char else_name[32];
            snprintf(else_name, 32, "else_%d", if_cnt++);
            SymTable* es = create_scope(SCOPE_ELSE, else_name, current_scope);
            current_scope = es;
        }
#line 3733 "y.tab.c"
    break;

  case 152: /* else_opt: ELSE $@22 block  */
#line 1796 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3742 "y.tab.c"
    break;

  case 153: /* $@23: %empty  */
#line 1801 "parser.y"
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
#line 3763 "y.tab.c"
    break;

  case 154: /* $@24: %empty  */
#line 1818 "parser.y"
        { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3769 "y.tab.c"
    break;

  case 155: /* $@25: %empty  */
#line 1820 "parser.y"
        {
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3778 "y.tab.c"
    break;

  case 156: /* $@26: %empty  */
#line 1825 "parser.y"
        { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3784 "y.tab.c"
    break;

  case 159: /* break_stmt: BREAK SEMICOLON  */
#line 1832 "parser.y"
        {
            if(loopTop < 0){
                fprintf(stderr, "ERROR line %d: 'break' outside loop.\n", yylineno);
            } else {
                emit("goto", "", "", loopEndStack[loopTop]);
            }
        }
#line 3796 "y.tab.c"
    break;

  case 160: /* continue_stmt: CONTINUE SEMICOLON  */
#line 1843 "parser.y"
        {
            if(loopTop < 0){
                fprintf(stderr, "ERROR line %d: 'continue' outside loop.\n", yylineno);
            } else {
                if(forDepth >= 0){
                    for(int i = 0; i < forIncIdx[forDepth]; i++){
                        emit(forIncBuff[forDepth][i].op,
                             forIncBuff[forDepth][i].arg1,
                             forIncBuff[forDepth][i].arg2,
                             forIncBuff[forDepth][i].result);
                    }
                }
                emit("goto", "", "", loopCondStack[loopTop]);
            }
        }
#line 3816 "y.tab.c"
    break;

  case 161: /* $@27: %empty  */
#line 1862 "parser.y"
        {
            char for_name[32];
            snprintf(for_name, 32, "for_%d", for_cnt++);
            SymTable* fs = create_scope(SCOPE_FOR, for_name, current_scope);
            current_scope = fs;
        }
#line 3827 "y.tab.c"
    break;

  case 162: /* for_stmt: FOR LPAREN $@27 for_header block  */
#line 1869 "parser.y"
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
            free(loopEndStack[loopTop]);
            free(loopCondStack[loopTop]);
            loopTop--;
            popIfLabels();

            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 3850 "y.tab.c"
    break;

  case 163: /* for_stmt: FOR LPAREN error RPAREN block  */
#line 1889 "parser.y"
        {
            printf("Invalid FOR header at line %d\n", yylineno);
            yyerrok;
        }
#line 3859 "y.tab.c"
    break;

  case 164: /* $@28: %empty  */
#line 1897 "parser.y"
        {
            char* b = getLabel();
            char* e = getLabel();
            pushIfLabels(b, e);
            loopEndStack[++loopTop] = strdup(e);
            loopCondStack[loopTop]  = strdup(b);
            emit("label", "", "", b);
        }
#line 3872 "y.tab.c"
    break;

  case 165: /* $@29: %empty  */
#line 1906 "parser.y"
        {
            emit("ifFalse", (yyvsp[-1].sval) ? (yyvsp[-1].sval) : "1", "", topEnd());
            forDepth++;
            forIncIdx[forDepth]      = 0;
            inForIncrement[forDepth] = 1;
        }
#line 3883 "y.tab.c"
    break;

  case 166: /* $@30: %empty  */
#line 1913 "parser.y"
        { inForIncrement[forDepth] = 0; }
#line 3889 "y.tab.c"
    break;

  case 168: /* for_init_opt: var_decl_no_semi  */
#line 1918 "parser.y"
                        { printf("Hit var_decl_no_semi");(yyval.sval) = NULL; }
#line 3895 "y.tab.c"
    break;

  case 169: /* for_init_opt: expression  */
#line 1919 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 3901 "y.tab.c"
    break;

  case 170: /* for_init_opt: %empty  */
#line 1920 "parser.y"
                    { printf("Matching with empty string wala in for_init\n");(yyval.sval) = NULL; }
#line 3907 "y.tab.c"
    break;

  case 171: /* for_cond_opt: expression  */
#line 1924 "parser.y"
                    { (yyval.sval) = (yyvsp[0].sval); }
#line 3913 "y.tab.c"
    break;

  case 172: /* for_cond_opt: %empty  */
#line 1925 "parser.y"
                    { (yyval.sval) = strdup("1"); }
#line 3919 "y.tab.c"
    break;

  case 173: /* for_inc_opt: expression  */
#line 1929 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 3925 "y.tab.c"
    break;

  case 174: /* for_inc_opt: %empty  */
#line 1930 "parser.y"
                        { (yyval.sval) = NULL; }
#line 3931 "y.tab.c"
    break;

  case 175: /* var_decl_no_semi: type IDENTIFIER  */
#line 1935 "parser.y"
        {
            insert_symbol(current_scope, (yyvsp[0].sval), KIND_VAR, (yyvsp[-1].dtype), yylineno);
        }
#line 3939 "y.tab.c"
    break;

  case 176: /* var_decl_no_semi: type IDENTIFIER ASSIGN expression  */
#line 1939 "parser.y"
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
#line 3954 "y.tab.c"
    break;

  case 177: /* io_stmt: IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 1953 "parser.y"
        {
		Symbol* var = require_declared(current_scope, (yyvsp[-6].sval), yylineno);
		if(!var){
			fprintf(stderr, "ERROR line %d: assignment to undeclared variable '%s'.\n", yylineno, (yyvsp[-6].sval));
		}
		emit("out", (yyvsp[-2].sval), "", "");
		emit("in", "", "", (yyvsp[-6].sval)); 
	}
#line 3967 "y.tab.c"
    break;

  case 178: /* io_stmt: type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 1962 "parser.y"
        { 
		Symbol* sym = insert_symbol(current_scope, (yyvsp[-6].sval), KIND_VAR, (yyvsp[-7].dtype), yylineno);
		if(sym){
			sym->is_initialized = 1;
		}
		emit("out", (yyvsp[-2].sval), "", "");
		emit("in", "", "", (yyvsp[-6].sval)); 
	}
#line 3980 "y.tab.c"
    break;

  case 179: /* io_stmt: SHOW LPAREN expression RPAREN SEMICOLON  */
#line 1971 "parser.y"
        { emit("out", (yyvsp[-2].sval), "", ""); }
#line 3986 "y.tab.c"
    break;

  case 180: /* io_stmt: SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON  */
#line 1973 "parser.y"
        {
            char* t = genVar();
            emit("[]",  (yyvsp[-5].sval), (yyvsp[-3].sval), t);
            emit("out", t,  "", "");
        }
#line 3996 "y.tab.c"
    break;


#line 4000 "y.tab.c"

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

#line 1980 "parser.y"


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

    int compare_mode = 0;
    StatsMode stats_mode = STATS_NONE;   // default: no stats printed

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--noalloc") == 0) {
            use_optimized_regalloc = 0;
            printf("Register allocation: BASIC\n");
        }
        else if (strcmp(argv[i], "-S") == 0) {
            // bare -S means print ALL stats
            stats_mode = STATS_ALL;
            printf("Stats mode: ALL(Register Allocation + Instruction Selection)\n");
        }
        else if (strncmp(argv[i], "-S=", 3) == 0) {
            const char* sub = argv[i] + 3;   // part after "="
            if (strcmp(sub, "regAlloc") == 0) {
                stats_mode = STATS_REGALLOC;
                printf("Stats mode: regAlloc\n");
            }
	    else if(strcmp(sub, "instrSel") == 0){
		stats_mode = STATS_INSTRSEL;
		printf("Stats mode: Instruction Selection comparision\n");
	    }
            else {
                fprintf(stderr, "Warning: unknown stats category '%s', "
                                "use -S or -S=regAlloc\n", sub);
            }
        }
    }

    yyin = stdin;
    yyparse();

    printf("\n========== GLOBAL SCOPE ==========\n");
    print_table(global_scope);

    int opt_level = 3;
    int do_python  = 0;
    for (int i = 1; i < argc; i++) {
        if      (strcmp(argv[i], "-O0") == 0) opt_level = 0;
        else if (strcmp(argv[i], "-O1") == 0) opt_level = 1;
        else if (strcmp(argv[i], "-O2") == 0) opt_level = 2;
        else if (strcmp(argv[i], "-O3") == 0) opt_level = 3;
        else if (strcmp(argv[i], "-py") == 0) do_python  = 1;
    }
    printf("Running Optimizations (-O%d)\n", opt_level);

    if (opt_level >= 1) {
        algebraic_simplification();
        constant_folding();
        constant_propagation();
        copy_propagation(opt_level);
        constant_folding();
        constant_propagation();
    }
    if (opt_level >= 2) {
        common_subexpression_elimination();
        dead_code_elimination();
    }
    if (opt_level >= 3) {
        strength_reduction();
        loop_invariant_code_motion();
        induction_variable_elimination();
        dead_code_elimination();
    }

    printf("\n========== IR Code Visualization Section ==========\n");
    print_original_IR();
    print_opt_IR();

    FILE* asm_file = fopen("output.s", "w");
    if (!asm_file) { perror("Failed to open assembly file"); return 1; }
    asmSetOutput(asm_file);
    use_template_matching = 1;
    printf("\nGenerating RISC-V Assembly...\n");
    generateASM();
    fclose(asm_file);
    printf("Assembly code saved to 'output.s'\n");

    // ── Print stats to terminal (not into the .s file) ──────────────────
    printAsmStats(stats_mode);

    if (do_python) {
        FILE* py_file = fopen("output.py", "w");
        if (!py_file) { perror("Failed to open output.py"); }
        else {
            printf("\nTranspiling to Python...\n");
            transpile_to_python(py_file);
            fclose(py_file);
            printf("Python code saved to 'output.py'\n");
        }
    }

    if (compare_mode) {
        FILE* std_file = fopen("output_standard.s", "w");
        if (!std_file) { perror("Failed to open output_standard.s"); return 1; }
        asmSetOutput(std_file);
        use_template_matching = 0;
        printf("\nGenerating standard assembly...\n");
        generateASM();
        fclose(std_file);
        printf("Standard assembly saved to 'output_standard.s'\n");

        // Print stats for the standard run too so you can compare
        printf("\n--- Standard run stats ---\n");
        printAsmStats(stats_mode);
    }

    return 0;
}

