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
    /*// Add near your other globals at the top of parser.y
    #ifndef QUAD_DEFINED
    typedef struct {
        char op[20];
        char arg1[256];
        char arg2[256];
        char result[256];
    } Quad;
    #define QUAD_DEFINED
    #endif*/

    Quad IR[10000];
    int IR_idx = 0;
    int tempVarCnt = 0;
    int labelCnt = 0;
    int parse_error_count = 0;

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

    

#line 198 "y.tab.c"

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
    EXTENDS = 284,                 /* EXTENDS  */
    LPAREN = 285,                  /* LPAREN  */
    RPAREN = 286,                  /* RPAREN  */
    LBRACE = 287,                  /* LBRACE  */
    RBRACE = 288,                  /* RBRACE  */
    LBRACKET = 289,                /* LBRACKET  */
    RBRACKET = 290,                /* RBRACKET  */
    SEMICOLON = 291,               /* SEMICOLON  */
    COMMA = 292,                   /* COMMA  */
    IDENTIFIER = 293,              /* IDENTIFIER  */
    STRING_LITERAL = 294,          /* STRING_LITERAL  */
    INT_LITERAL = 295,             /* INT_LITERAL  */
    FLOAT_LITERAL = 296,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 297,            /* CHAR_LITERAL  */
    ASSIGN = 298,                  /* ASSIGN  */
    ADD_ASSIGN = 299,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 300,              /* SUB_ASSIGN  */
    OR = 301,                      /* OR  */
    AND = 302,                     /* AND  */
    BITOR = 303,                   /* BITOR  */
    BITAND = 304,                  /* BITAND  */
    EQ = 305,                      /* EQ  */
    GT = 306,                      /* GT  */
    LT = 307,                      /* LT  */
    PLUS = 308,                    /* PLUS  */
    MINUS = 309,                   /* MINUS  */
    MUL = 310,                     /* MUL  */
    DIV = 311,                     /* DIV  */
    MOD = 312,                     /* MOD  */
    NOT = 313                      /* NOT  */
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
#define EXTENDS 284
#define LPAREN 285
#define RPAREN 286
#define LBRACE 287
#define RBRACE 288
#define LBRACKET 289
#define RBRACKET 290
#define SEMICOLON 291
#define COMMA 292
#define IDENTIFIER 293
#define STRING_LITERAL 294
#define INT_LITERAL 295
#define FLOAT_LITERAL 296
#define CHAR_LITERAL 297
#define ASSIGN 298
#define ADD_ASSIGN 299
#define SUB_ASSIGN 300
#define OR 301
#define AND 302
#define BITOR 303
#define BITAND 304
#define EQ 305
#define GT 306
#define LT 307
#define PLUS 308
#define MINUS 309
#define MUL 310
#define DIV 311
#define MOD 312
#define NOT 313

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 127 "parser.y"

        int       ival;
        float     fval;
        char      cval;
        char*     sval;
        DataType  dtype;
        AccessMod access;
    

#line 377 "y.tab.c"

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
  YYSYMBOL_EXTENDS = 29,                   /* EXTENDS  */
  YYSYMBOL_LPAREN = 30,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 31,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 32,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 33,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 34,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 35,                  /* RBRACKET  */
  YYSYMBOL_SEMICOLON = 36,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 37,                     /* COMMA  */
  YYSYMBOL_IDENTIFIER = 38,                /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 39,            /* STRING_LITERAL  */
  YYSYMBOL_INT_LITERAL = 40,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 41,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 42,              /* CHAR_LITERAL  */
  YYSYMBOL_ASSIGN = 43,                    /* ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 44,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 45,                /* SUB_ASSIGN  */
  YYSYMBOL_OR = 46,                        /* OR  */
  YYSYMBOL_AND = 47,                       /* AND  */
  YYSYMBOL_BITOR = 48,                     /* BITOR  */
  YYSYMBOL_BITAND = 49,                    /* BITAND  */
  YYSYMBOL_EQ = 50,                        /* EQ  */
  YYSYMBOL_GT = 51,                        /* GT  */
  YYSYMBOL_LT = 52,                        /* LT  */
  YYSYMBOL_PLUS = 53,                      /* PLUS  */
  YYSYMBOL_MINUS = 54,                     /* MINUS  */
  YYSYMBOL_MUL = 55,                       /* MUL  */
  YYSYMBOL_DIV = 56,                       /* DIV  */
  YYSYMBOL_MOD = 57,                       /* MOD  */
  YYSYMBOL_NOT = 58,                       /* NOT  */
  YYSYMBOL_YYACCEPT = 59,                  /* $accept  */
  YYSYMBOL_program = 60,                   /* program  */
  YYSYMBOL_element = 61,                   /* element  */
  YYSYMBOL_statement = 62,                 /* statement  */
  YYSYMBOL_entity_decl = 63,               /* entity_decl  */
  YYSYMBOL_64_1 = 64,                      /* $@1  */
  YYSYMBOL_65_2 = 65,                      /* $@2  */
  YYSYMBOL_66_3 = 66,                      /* $@3  */
  YYSYMBOL_entity_body = 67,               /* entity_body  */
  YYSYMBOL_entity_member = 68,             /* entity_member  */
  YYSYMBOL_constructor_decl = 69,          /* constructor_decl  */
  YYSYMBOL_70_4 = 70,                      /* $@4  */
  YYSYMBOL_71_5 = 71,                      /* $@5  */
  YYSYMBOL_method_decl = 72,               /* method_decl  */
  YYSYMBOL_73_6 = 73,                      /* $@6  */
  YYSYMBOL_74_7 = 74,                      /* $@7  */
  YYSYMBOL_75_8 = 75,                      /* $@8  */
  YYSYMBOL_access_var_decl = 76,           /* access_var_decl  */
  YYSYMBOL_access_modifier = 77,           /* access_modifier  */
  YYSYMBOL_object_decl = 78,               /* object_decl  */
  YYSYMBOL_arg_list_opt = 79,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 80,                  /* arg_list  */
  YYSYMBOL_block = 81,                     /* block  */
  YYSYMBOL_82_9 = 82,                      /* $@9  */
  YYSYMBOL_stmt_list = 83,                 /* stmt_list  */
  YYSYMBOL_var_decl = 84,                  /* var_decl  */
  YYSYMBOL_id_list = 85,                   /* id_list  */
  YYSYMBOL_type = 86,                      /* type  */
  YYSYMBOL_array_decl = 87,                /* array_decl  */
  YYSYMBOL_88_10 = 88,                     /* $@10  */
  YYSYMBOL_89_11 = 89,                     /* $@11  */
  YYSYMBOL_array_init = 90,                /* array_init  */
  YYSYMBOL_array_init2d = 91,              /* array_init2d  */
  YYSYMBOL_row_list = 92,                  /* row_list  */
  YYSYMBOL_expr_list = 93,                 /* expr_list  */
  YYSYMBOL_function_decl = 94,             /* function_decl  */
  YYSYMBOL_95_12 = 95,                     /* $@12  */
  YYSYMBOL_96_13 = 96,                     /* $@13  */
  YYSYMBOL_97_14 = 97,                     /* $@14  */
  YYSYMBOL_func_type = 98,                 /* func_type  */
  YYSYMBOL_param_list_opt = 99,            /* param_list_opt  */
  YYSYMBOL_param_list = 100,               /* param_list  */
  YYSYMBOL_param = 101,                    /* param  */
  YYSYMBOL_return_stmt = 102,              /* return_stmt  */
  YYSYMBOL_expr_stmt = 103,                /* expr_stmt  */
  YYSYMBOL_expression = 104,               /* expression  */
  YYSYMBOL_indexed_id = 105,               /* indexed_id  */
  YYSYMBOL_assignment = 106,               /* assignment  */
  YYSYMBOL_logic_expr = 107,               /* logic_expr  */
  YYSYMBOL_bitwise_expr = 108,             /* bitwise_expr  */
  YYSYMBOL_rel_expr = 109,                 /* rel_expr  */
  YYSYMBOL_arith_expr = 110,               /* arith_expr  */
  YYSYMBOL_term = 111,                     /* term  */
  YYSYMBOL_factor = 112,                   /* factor  */
  YYSYMBOL_if_stmt = 113,                  /* if_stmt  */
  YYSYMBOL_114_15 = 114,                   /* $@15  */
  YYSYMBOL_115_16 = 115,                   /* $@16  */
  YYSYMBOL_116_17 = 116,                   /* $@17  */
  YYSYMBOL_117_18 = 117,                   /* $@18  */
  YYSYMBOL_elif_list = 118,                /* elif_list  */
  YYSYMBOL_119_19 = 119,                   /* $@19  */
  YYSYMBOL_120_20 = 120,                   /* $@20  */
  YYSYMBOL_121_21 = 121,                   /* $@21  */
  YYSYMBOL_122_22 = 122,                   /* $@22  */
  YYSYMBOL_else_opt = 123,                 /* else_opt  */
  YYSYMBOL_124_23 = 124,                   /* $@23  */
  YYSYMBOL_125_24 = 125,                   /* $@24  */
  YYSYMBOL_126_25 = 126,                   /* $@25  */
  YYSYMBOL_127_26 = 127,                   /* $@26  */
  YYSYMBOL_128_27 = 128,                   /* $@27  */
  YYSYMBOL_break_stmt = 129,               /* break_stmt  */
  YYSYMBOL_continue_stmt = 130,            /* continue_stmt  */
  YYSYMBOL_for_stmt = 131,                 /* for_stmt  */
  YYSYMBOL_132_28 = 132,                   /* $@28  */
  YYSYMBOL_for_header = 133,               /* for_header  */
  YYSYMBOL_134_29 = 134,                   /* $@29  */
  YYSYMBOL_135_30 = 135,                   /* $@30  */
  YYSYMBOL_136_31 = 136,                   /* $@31  */
  YYSYMBOL_for_init_opt = 137,             /* for_init_opt  */
  YYSYMBOL_for_cond_opt = 138,             /* for_cond_opt  */
  YYSYMBOL_for_inc_opt = 139,              /* for_inc_opt  */
  YYSYMBOL_var_decl_no_semi = 140,         /* var_decl_no_semi  */
  YYSYMBOL_io_stmt = 141                   /* io_stmt  */
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
#define YYLAST   595

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  83
/* YYNRULES -- Number of rules.  */
#define YYNRULES  183
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  383

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   313


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
      55,    56,    57,    58
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   170,   170,   171,   175,   176,   177,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   202,
     201,   276,   275,   315,   314,   325,   326,   330,   331,   332,
     336,   335,   425,   424,   437,   436,   530,   529,   612,   611,
     622,   636,   652,   653,   657,   704,   780,   781,   785,   792,
     802,   801,   811,   812,   820,   821,   826,   833,   857,   864,
     876,   883,   890,   891,   892,   893,   894,   895,   899,   917,
     935,   934,   979,   978,  1019,  1023,  1027,  1033,  1042,  1055,
    1073,  1072,  1162,  1161,  1248,  1247,  1258,  1259,  1263,  1264,
    1268,  1269,  1274,  1295,  1318,  1343,  1356,  1364,  1365,  1369,
    1372,  1384,  1405,  1419,  1432,  1446,  1459,  1476,  1478,  1505,
    1535,  1539,  1541,  1543,  1545,  1549,  1551,  1553,  1557,  1562,
    1566,  1570,  1574,  1581,  1587,  1591,  1597,  1603,  1609,  1613,
    1620,  1702,  1732,  1807,  1831,  1832,  1833,  1834,  1835,  1836,
    1837,  1838,  1843,  1860,  1862,  1867,  1842,  1871,  1880,  1897,
    1899,  1904,  1879,  1906,  1911,  1910,  1923,  1940,  1942,  1947,
    1922,  1949,  1953,  1964,  1984,  1983,  2010,  2019,  2028,  2035,
    2018,  2040,  2041,  2042,  2046,  2047,  2051,  2052,  2056,  2060,
    2075,  2085,  2094,  2096
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
  "ENTITY", "NEW", "PUBLIC", "PRIVATE", "THIS", "DOT", "EXTENDS", "LPAREN",
  "RPAREN", "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "SEMICOLON",
  "COMMA", "IDENTIFIER", "STRING_LITERAL", "INT_LITERAL", "FLOAT_LITERAL",
  "CHAR_LITERAL", "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "OR", "AND",
  "BITOR", "BITAND", "EQ", "GT", "LT", "PLUS", "MINUS", "MUL", "DIV",
  "MOD", "NOT", "$accept", "program", "element", "statement",
  "entity_decl", "$@1", "$@2", "$@3", "entity_body", "entity_member",
  "constructor_decl", "$@4", "$@5", "method_decl", "$@6", "$@7", "$@8",
  "access_var_decl", "access_modifier", "object_decl", "arg_list_opt",
  "arg_list", "block", "$@9", "stmt_list", "var_decl", "id_list", "type",
  "array_decl", "$@10", "$@11", "array_init", "array_init2d", "row_list",
  "expr_list", "function_decl", "$@12", "$@13", "$@14", "func_type",
  "param_list_opt", "param_list", "param", "return_stmt", "expr_stmt",
  "expression", "indexed_id", "assignment", "logic_expr", "bitwise_expr",
  "rel_expr", "arith_expr", "term", "factor", "if_stmt", "$@15", "$@16",
  "$@17", "$@18", "elif_list", "$@19", "$@20", "$@21", "$@22", "else_opt",
  "$@23", "$@24", "$@25", "$@26", "$@27", "break_stmt", "continue_stmt",
  "for_stmt", "$@28", "for_header", "$@29", "$@30", "$@31", "for_init_opt",
  "for_cond_opt", "for_inc_opt", "var_decl_no_semi", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-274)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-165)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -274,    15,  -274,    -1,  -274,  -274,  -274,  -274,  -274,  -274,
      13,    32,  -274,  -274,    34,   421,    36,    45,    54,    67,
     505,   244,  -274,   550,  -274,  -274,  -274,  -274,   535,   528,
    -274,  -274,  -274,  -274,  -274,  -274,    28,  -274,  -274,    75,
    -274,  -274,    74,    69,  -274,   144,   145,  -274,   156,    23,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,   103,   400,   505,
      90,  -274,   154,    99,  -274,  -274,   108,   115,   152,   163,
    -274,   358,   149,   187,   505,   505,   176,   162,   505,   505,
      92,   201,    91,  -274,  -274,  -274,   193,   195,   192,   181,
     199,  -274,   505,   528,   528,   535,   535,   535,   535,   535,
     535,   535,   535,   535,   535,   213,   505,   215,   463,     6,
    -274,   505,  -274,   216,   223,   227,   224,  -274,  -274,  -274,
     381,  -274,   302,    28,  -274,   226,   235,   233,  -274,   238,
     468,   245,  -274,  -274,  -274,  -274,   240,   241,   243,   505,
       7,    27,   498,  -274,  -274,  -274,   242,  -274,   239,  -274,
     168,   168,   168,    23,    23,  -274,  -274,  -274,   255,  -274,
     255,   252,  -274,   255,   256,  -274,   257,   505,  -274,  -274,
     293,   505,  -274,  -274,   265,   505,  -274,   505,    33,   258,
     261,   260,  -274,  -274,  -274,   269,   272,  -274,   277,  -274,
     270,   196,   286,   294,   295,  -274,   292,  -274,   283,  -274,
    -274,  -274,   296,   298,    76,   300,  -274,    82,  -274,  -274,
     505,   505,   297,  -274,   305,   311,   304,   314,   312,   316,
     289,   315,  -274,    82,   348,  -274,   505,   505,   319,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,   143,  -274,
     317,   320,   321,   331,  -274,   322,  -274,   505,   318,   505,
     333,   505,   337,   344,   347,   345,   349,    -4,   351,   352,
     255,  -274,  -274,   350,   353,    80,   354,   357,     8,    30,
    -274,  -274,   255,    82,   359,   360,  -274,   375,  -274,   101,
    -274,  -274,   355,   505,   121,  -274,   356,   505,   255,   255,
    -274,  -274,  -274,  -274,    82,   392,   379,   382,   383,   384,
    -274,  -274,   505,   387,  -274,  -274,   505,   393,   122,  -274,
     397,  -274,   401,  -274,  -274,   423,   505,   406,   412,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,   395,  -274,   505,   408,
     415,   435,  -274,  -274,   255,   255,   417,   419,   420,  -274,
     123,  -274,  -274,   443,  -274,   422,  -274,  -274,    82,    82,
     454,  -274,   505,   426,   255,  -274,   433,   434,   441,  -274,
    -274,  -274,   255,   255,   255,   442,   505,  -274,  -274,  -274,
    -274,  -274,   255,   447,  -274,  -274,   423,   255,  -274,  -274,
     423,   435,  -274
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,    62,    63,    64,    65,    66,    67,
       0,     0,   139,   140,     0,     0,     0,     0,     0,     0,
       0,     0,    98,   133,   138,   135,   136,   137,     0,     0,
       2,     4,     6,    16,    17,     7,    86,     8,     5,     0,
      13,     9,     0,   134,    99,   110,   114,   117,   121,   124,
     128,    10,    14,    15,    11,    12,    18,     0,     0,     0,
       0,    95,   133,     0,   162,   163,    21,     0,     0,     0,
      52,     0,     0,     0,    47,     0,    61,     0,     0,     0,
       0,     0,   133,   134,   129,   113,     0,     0,    61,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   173,     0,
      96,     0,    94,     0,     0,     0,   131,   141,    53,    67,
     133,    55,     0,     0,    82,   132,     0,    46,    49,     0,
       0,     0,   102,   103,   104,    58,     0,     0,     0,     0,
       0,     0,     0,    56,    80,   107,   111,   112,   116,   115,
     120,   118,   119,   122,   123,   125,   126,   127,     0,   143,
       0,     0,   172,     0,     0,   171,     0,     0,    19,    26,
       0,     0,    51,    54,     0,     0,   130,     0,   100,     0,
       0,     0,    60,   131,   132,     0,     0,    70,     0,    72,
       0,   133,     0,     0,     0,   147,     0,   166,   178,   165,
     167,   182,     0,     0,     0,     0,   108,    89,   109,    48,
       0,     0,     0,    59,     0,   100,     0,     0,     0,     0,
       0,     0,    57,    89,     0,   144,     0,   175,     0,    26,
      42,    43,    22,    30,    25,    27,    28,    29,     0,    24,
       0,     0,     0,    88,    91,     0,   105,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   132,     0,     0,
       0,   179,   174,     0,     0,     0,     0,     0,     0,     0,
      93,    92,     0,     0,   101,     0,   180,     0,    68,     0,
      79,    71,     0,     0,     0,    73,     0,    47,     0,     0,
     145,   168,   183,    20,    89,     0,     0,     0,     0,     0,
      83,    90,     0,     0,   101,    74,     0,     0,     0,    75,
       0,   181,     0,    81,    85,   153,   177,     0,     0,    36,
      41,    34,    40,   106,    44,    78,     0,    77,     0,     0,
       0,   161,   176,   169,     0,     0,     0,     0,     0,    69,
       0,    45,   148,   154,   146,     0,    31,    33,    89,    89,
       0,    76,     0,     0,     0,   170,     0,     0,     0,   149,
     156,   155,     0,     0,     0,     0,     0,    37,    35,    39,
     150,   157,     0,     0,   151,   158,   153,     0,   152,   159,
     153,   161,   160
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -274,  -274,  -274,   -59,  -274,  -274,  -274,  -274,   251,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -222,  -274,  -149,  -274,  -274,  -274,   -29,     0,  -274,  -274,
    -274,  -274,  -274,  -274,  -270,  -274,  -274,  -274,  -274,  -274,
    -217,  -274,   210,  -274,  -274,   -15,    70,   -75,   -19,   147,
    -274,    81,   164,   -20,  -274,  -274,  -274,  -274,  -274,  -273,
    -274,  -274,  -274,  -274,    93,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    30,    31,    32,   203,   114,   115,   204,   234,
     235,   266,   267,   236,   337,   336,   338,   237,   238,    33,
     126,   127,    34,    71,   122,    35,    80,   241,    37,   217,
     219,   252,   255,   284,   279,    38,   193,   174,   194,    39,
     242,   243,   244,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,   106,   196,   260,   315,   331,
     352,   365,   372,   376,   344,   354,   366,   373,   377,   380,
      52,    53,    54,   108,   163,   227,   316,   345,   164,   263,
     333,   165,    55
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      63,    36,   132,   133,   134,    68,   258,    89,    84,   195,
      85,   197,   121,   308,   199,     2,     3,   145,     4,     5,
       6,     7,     8,     9,    10,   275,   287,    11,    12,    13,
     296,    14,    15,    16,    17,    56,   132,   166,    18,   175,
     167,   186,    19,    57,   109,    20,   297,    21,    86,    87,
     187,    22,   298,    23,    24,    25,    26,    27,   340,   128,
     129,   188,    58,   173,    59,   312,    88,   210,   299,    28,
     189,   123,    64,    29,   146,   147,   211,   317,   102,   103,
     104,    65,   155,   156,   157,     4,     5,     6,     7,     8,
     119,   159,    66,   162,    89,    67,   206,    90,    83,    83,
     208,   230,   231,   378,   105,   230,   231,   381,   161,   232,
      91,   290,    92,   293,   233,   180,  -142,  -142,   233,   138,
     240,    74,   123,   300,   185,   139,   110,   192,   135,   136,
    -142,   356,   357,  -142,   305,   112,   246,   113,   306,   313,
     314,  -142,  -142,  -142,  -142,  -142,     4,     5,     6,     7,
       8,   119,   202,   116,   309,   327,   351,  -142,   310,   306,
     306,  -142,   209,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    12,    13,   131,   150,   151,
     152,   268,    73,   117,    74,   346,   347,   124,    75,    19,
      93,    94,    20,    95,    96,   245,   118,   111,    78,    79,
      62,    24,    25,    26,    27,   361,    97,    98,    99,   100,
     101,   261,   262,   367,   368,   369,    28,   143,   136,   130,
      29,   100,   101,   374,   221,   125,    74,   323,   379,   137,
      75,   140,   128,   141,   277,   142,   280,   144,   269,   111,
      78,    79,   148,   149,   158,    69,   160,   -50,   -50,   -50,
     -50,   -50,   -50,   -50,   168,   169,   -50,   -50,   -50,   170,
     -50,   -50,   -50,   -50,   153,   154,   176,   171,   280,   175,
     177,   -50,   128,   178,   -50,   181,   -50,    70,   182,   183,
     -50,   184,   -50,   -50,   -50,   -50,   -50,    21,    96,    94,
     198,   325,   200,   201,   205,   207,   212,   213,   -50,   214,
     220,   332,   -50,     3,   215,     4,     5,     6,     7,     8,
     119,    10,   216,   280,    11,    12,    13,   218,    14,    15,
      16,    17,   222,   225,   223,   224,   226,   247,   256,    19,
     229,   228,    20,   239,    21,   172,   248,   359,    22,   250,
     120,    24,    25,    26,    27,   249,   251,   253,   254,   259,
     264,   371,   272,   257,   276,   270,    28,   274,   271,     3,
      29,     4,     5,     6,     7,     8,   119,    10,   273,   278,
      11,    12,    13,   281,    14,    15,    16,    17,   282,   283,
     286,   285,   288,   289,   294,    19,   291,   295,    20,   292,
      21,   303,   311,   318,    22,   307,   120,    24,    25,    26,
      27,   107,   302,  -164,  -164,  -164,  -164,  -164,  -164,    73,
     304,    74,    28,  -164,  -164,    75,    29,   319,   320,    76,
     322,   321,    60,   324,    77,    78,    79,  -164,   326,   328,
    -164,   339,   329,   330,    12,    13,  -164,   334,  -164,  -164,
    -164,  -164,  -164,   335,   341,   342,   343,   348,    19,   349,
     350,    20,   353,   355,  -164,   358,   360,    61,  -164,    62,
      24,    25,    26,    27,   362,   363,     4,     5,     6,     7,
       8,   119,   364,   370,   382,    28,    12,    13,   375,    29,
     265,    12,    13,   301,     0,     0,     0,     0,     0,     0,
      19,     0,   179,    20,     0,    19,     0,     0,    20,     0,
       0,    62,    24,    25,    26,    27,    62,    24,    25,    26,
      27,    12,    13,   190,     0,     0,     0,    28,    12,    13,
       0,    29,    28,     0,     0,    19,    29,     0,    20,     0,
       0,     0,    19,     0,     0,    20,   191,    24,    25,    26,
      27,    12,    13,    62,    24,    25,    26,    27,    12,    13,
       0,     0,    28,     0,     0,    81,    29,     0,    20,    28,
       0,     0,    81,    29,     0,    20,    82,    24,    25,    26,
      27,     0,    72,    82,    24,    25,    26,    27,    73,     0,
      74,     0,    28,     0,    75,     0,    29,     0,    76,    28,
       0,     0,     0,    77,    78,    79
};

static const yytype_int16 yycheck[] =
{
      15,     1,    77,    78,    79,    20,   223,    36,    28,   158,
      29,   160,    71,   283,   163,     0,     1,    92,     3,     4,
       5,     6,     7,     8,     9,   247,    30,    12,    13,    14,
      22,    16,    17,    18,    19,    36,   111,    31,    23,    43,
      34,    34,    27,    30,    59,    30,    38,    32,    20,    21,
      43,    36,    22,    38,    39,    40,    41,    42,   328,    74,
      75,    34,    30,   122,    30,   287,    38,    34,    38,    54,
      43,    71,    36,    58,    93,    94,    43,   294,    55,    56,
      57,    36,   102,   103,   104,     3,     4,     5,     6,     7,
       8,   106,    38,   108,   123,    28,   171,    22,    28,    29,
     175,    25,    26,   376,     1,    25,    26,   380,   108,    33,
      36,   260,    43,    33,    38,   130,    13,    14,    38,    28,
      38,    30,   122,   272,   139,    34,    36,   142,    36,    37,
      27,   348,   349,    30,    33,    36,   211,    29,    37,   288,
     289,    38,    39,    40,    41,    42,     3,     4,     5,     6,
       7,     8,   167,    38,    33,    33,    33,    54,    37,    37,
      37,    58,   177,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    13,    14,    15,    97,    98,
      99,    38,    28,    31,    30,   334,   335,    38,    34,    27,
      46,    47,    30,    48,    49,   210,    33,    43,    44,    45,
      38,    39,    40,    41,    42,   354,    50,    51,    52,    53,
      54,   226,   227,   362,   363,   364,    54,    36,    37,    43,
      58,    53,    54,   372,    28,    38,    30,   302,   377,    28,
      34,    38,   247,    38,   249,    43,   251,    38,   238,    43,
      44,    45,    95,    96,    31,     1,    31,     3,     4,     5,
       6,     7,     8,     9,    38,    32,    12,    13,    14,    32,
      16,    17,    18,    19,   100,   101,    31,    43,   283,    43,
      37,    27,   287,    35,    30,    30,    32,    33,    38,    38,
      36,    38,    38,    39,    40,    41,    42,    32,    49,    47,
      38,   306,    36,    36,     1,    30,    38,    36,    54,    39,
      30,   316,    58,     1,    35,     3,     4,     5,     6,     7,
       8,     9,    40,   328,    12,    13,    14,    40,    16,    17,
      18,    19,    36,    31,    30,    30,    43,    30,    39,    27,
      32,    35,    30,    33,    32,    33,    31,   352,    36,    35,
      38,    39,    40,    41,    42,    34,    32,    35,    32,     1,
      31,   366,    31,    38,    36,    38,    54,    35,    38,     1,
      58,     3,     4,     5,     6,     7,     8,     9,    37,    36,
      12,    13,    14,    36,    16,    17,    18,    19,    34,    32,
      31,    36,    31,    31,    30,    27,    36,    30,    30,    36,
      32,    31,    36,     1,    36,    40,    38,    39,    40,    41,
      42,     1,    43,     3,     4,     5,     6,     7,     8,    28,
      35,    30,    54,    13,    14,    34,    58,    38,    36,    38,
      36,    38,     1,    36,    43,    44,    45,    27,    35,    32,
      30,    36,    31,    10,    13,    14,    36,    31,    38,    39,
      40,    41,    42,    31,    36,    30,    11,    30,    27,    30,
      30,    30,     9,    31,    54,     1,    30,    36,    58,    38,
      39,    40,    41,    42,    31,    31,     3,     4,     5,     6,
       7,     8,    31,    31,   381,    54,    13,    14,    31,    58,
     229,    13,    14,   273,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    24,    30,    -1,    27,    -1,    -1,    30,    -1,
      -1,    38,    39,    40,    41,    42,    38,    39,    40,    41,
      42,    13,    14,    15,    -1,    -1,    -1,    54,    13,    14,
      -1,    58,    54,    -1,    -1,    27,    58,    -1,    30,    -1,
      -1,    -1,    27,    -1,    -1,    30,    38,    39,    40,    41,
      42,    13,    14,    38,    39,    40,    41,    42,    13,    14,
      -1,    -1,    54,    -1,    -1,    27,    58,    -1,    30,    54,
      -1,    -1,    27,    58,    -1,    30,    38,    39,    40,    41,
      42,    -1,    22,    38,    39,    40,    41,    42,    28,    -1,
      30,    -1,    54,    -1,    34,    -1,    58,    -1,    38,    54,
      -1,    -1,    -1,    43,    44,    45
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    60,     0,     1,     3,     4,     5,     6,     7,     8,
       9,    12,    13,    14,    16,    17,    18,    19,    23,    27,
      30,    32,    36,    38,    39,    40,    41,    42,    54,    58,
      61,    62,    63,    78,    81,    84,    86,    87,    94,    98,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   129,   130,   131,   141,    36,    30,    30,    30,
       1,    36,    38,   104,    36,    36,    38,    28,   104,     1,
      33,    82,    22,    28,    30,    34,    38,    43,    44,    45,
      85,    27,    38,   105,   112,   107,    20,    21,    38,    85,
      22,    36,    43,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,     1,   114,     1,   132,   104,
      36,    43,    36,    29,    65,    66,    38,    31,    33,     8,
      38,    62,    83,    86,    38,    38,    79,    80,   104,   104,
      43,    15,   106,   106,   106,    36,    37,    28,    28,    34,
      38,    38,    43,    36,    38,   106,   107,   107,   108,   108,
     110,   110,   110,   111,   111,   112,   112,   112,    31,   104,
      31,    86,   104,   133,   137,   140,    31,    34,    38,    32,
      32,    43,    33,    62,    96,    43,    31,    37,    35,    24,
     104,    30,    38,    38,    38,   104,    34,    43,    34,    43,
      15,    38,   104,    95,    97,    81,   115,    81,    38,    81,
      36,    36,   104,    64,    67,     1,   106,    30,   106,   104,
      34,    43,    38,    36,    39,    35,    40,    88,    40,    89,
      30,    28,    36,    30,    30,    31,    43,   134,    35,    32,
      25,    26,    33,    38,    68,    69,    72,    76,    77,    33,
      38,    86,    99,   100,   101,   104,   106,    30,    31,    34,
      35,    32,    90,    35,    32,    91,    39,    38,    99,     1,
     116,   104,   104,   138,    31,    67,    70,    71,    38,    86,
      38,    38,    31,    37,    35,    79,    36,   104,    36,    93,
     104,    36,    34,    32,    92,    36,    31,    30,    31,    31,
      81,    36,    36,    33,    30,    30,    22,    38,    22,    38,
      81,   101,    43,    31,    35,    33,    37,    40,    93,    33,
      37,    36,    79,    81,    81,   117,   135,    99,     1,    38,
      36,    38,    36,   106,    36,   104,    35,    33,    32,    31,
      10,   118,   104,   139,    31,    31,    74,    73,    75,    36,
      93,    36,    30,    11,   123,   136,    81,    81,    30,    30,
      30,    33,   119,     9,   124,    31,    99,    99,     1,   104,
      30,    81,    31,    31,    31,   120,   125,    81,    81,    81,
      31,   104,   121,   126,    81,    31,   122,   127,   118,    81,
     128,   118,   123
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    59,    60,    60,    61,    61,    61,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    64,
      63,    65,    63,    66,    63,    67,    67,    68,    68,    68,
      70,    69,    71,    69,    73,    72,    74,    72,    75,    72,
      76,    76,    77,    77,    78,    78,    79,    79,    80,    80,
      82,    81,    81,    81,    83,    83,    84,    84,    84,    84,
      85,    85,    86,    86,    86,    86,    86,    86,    87,    87,
      88,    87,    89,    87,    90,    91,    92,    92,    93,    93,
      95,    94,    96,    94,    97,    94,    98,    98,    99,    99,
     100,   100,   101,   101,   102,   102,   102,   103,   103,   104,
     105,   105,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   107,   107,   107,   107,   108,   108,   108,   109,   109,
     109,   109,   110,   110,   110,   111,   111,   111,   111,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   114,   115,   116,   117,   113,   113,   119,   120,
     121,   122,   118,   118,   124,   123,   125,   126,   127,   128,
     123,   123,   129,   130,   132,   131,   131,   134,   135,   136,
     133,   137,   137,   137,   138,   138,   139,   139,   140,   140,
     141,   141,   141,   141
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       8,     0,     6,     0,     6,     2,     0,     1,     1,     1,
       0,     6,     0,     6,     0,     9,     0,     9,     0,     9,
       4,     4,     1,     1,     9,    10,     1,     0,     3,     1,
       0,     4,     2,     3,     2,     1,     3,     5,     3,     5,
       3,     1,     1,     1,     1,     1,     1,     1,     7,    10,
       0,     7,     0,     7,     3,     3,     5,     3,     3,     1,
       0,     8,     0,     8,     0,     8,     1,     1,     1,     0,
       3,     1,     2,     2,     3,     2,     3,     2,     1,     1,
       4,     7,     3,     3,     3,     6,     9,     3,     5,     5,
       1,     3,     3,     2,     1,     3,     3,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     2,
       4,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     0,     0,     0,    11,     5,     0,     0,
       0,     0,    10,     0,     0,     3,     0,     0,     0,     0,
      12,     0,     2,     2,     0,     5,     5,     0,     0,     0,
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
  case 18: /* statement: error SEMICOLON  */
#line 193 "parser.y"
            {
                printf("Invalid statement at line %d\n", yylineno);
                yyerrok;
            }
#line 1814 "y.tab.c"
    break;

  case 19: /* $@1: %empty  */
#line 202 "parser.y"
        {
            /* Verify parent exists */
            Symbol* parent_sym = lookup(global_scope, (yyvsp[0].sval));
            if (!parent_sym || parent_sym->kind != KIND_ENTITY) {
                char buf[256];
                snprintf(buf, sizeof(buf),
                    "line %d: Parent entity '%s' not found", yylineno, (yyvsp[0].sval));
                semantic_error(buf);
            }

            /* Insert child entity symbol */
            Symbol* sym = insert_symbol(global_scope, (yyvsp[-2].sval),
                                        KIND_ENTITY, DT_ENTITY, yylineno);
            if (sym) {
                strncpy(sym->attr.entity.class_name, (yyvsp[-2].sval), 63);
                sym->attr.entity.fields_list       = NULL;
                sym->attr.entity.methods_list      = NULL;
                sym->attr.entity.constructors_list = NULL;
                sym->attr.entity.class_size        = 0;
                strncpy(sym->attr.entity.parent_class, (yyvsp[0].sval), 63);  /* record parent */

                /* Pre-populate inherited field/method name lists from parent */
                if (parent_sym && parent_sym->kind == KIND_ENTITY) {
                    for (NameNode* n = parent_sym->attr.entity.fields_list; n; n = n->next)
                        add_name(&sym->attr.entity.fields_list, n->name);
                    for (NameNode* n = parent_sym->attr.entity.methods_list; n; n = n->next)
                        add_name(&sym->attr.entity.methods_list, n->name);
                }
            }

            emit("entity_extends", (yyvsp[-2].sval), (yyvsp[0].sval), "");

            SymTable* es = create_scope(SCOPE_ENTITY, (yyvsp[-2].sval), current_scope);
            register_entity_scope(es);
            if (sym) sym->attr.entity.scope = es;
            current_scope = es;

            /* Copy inherited symbols into child scope so lookups work */
            if (parent_sym && parent_sym->kind == KIND_ENTITY) {
                SymTable* psc = find_entity_scope((yyvsp[0].sval));
                if (psc) {
                    for (int b = 0; b < HASH_SIZE; b++) {
                        for (Symbol* ps = psc->buckets[b]; ps; ps = ps->next) {
                            if (!lookup_local(es, ps->name)) {
                                Symbol* cp = calloc(1, sizeof(Symbol));
                                *cp = *ps;
                                cp->next = NULL;
                                unsigned int h = hash_fn_pub(ps->name);
                                cp->next = es->buckets[h];
                                es->buckets[h] = cp;
                                es->symbol_count++;
                                if (ps->kind == KIND_FIELD)
                                    es->next_offset += ps->size;
                            }
                        }
                    }
                }
            }
        }
#line 1878 "y.tab.c"
    break;

  case 20: /* entity_decl: ENTITY IDENTIFIER EXTENDS IDENTIFIER $@1 LBRACE entity_body RBRACE  */
#line 262 "parser.y"
        {
            int class_sz = current_scope->next_offset;
            Symbol* sym = lookup(current_scope->parent, current_scope->name);
            if (sym && sym->kind == KIND_ENTITY) {
                sym->attr.entity.class_size = class_sz;
                sym->size = class_sz;
                current_scope->parent->next_offset = sym->offset + class_sz;
            }
            print_table(current_scope);
            current_scope = current_scope->parent;
            emit("end_entity", (yyvsp[-6].sval), "", "");
        }
#line 1895 "y.tab.c"
    break;

  case 21: /* $@2: %empty  */
#line 276 "parser.y"
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
#line 1919 "y.tab.c"
    break;

  case 22: /* entity_decl: ENTITY IDENTIFIER $@2 LBRACE entity_body RBRACE  */
#line 296 "parser.y"
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
#line 1942 "y.tab.c"
    break;

  case 23: /* $@3: %empty  */
#line 315 "parser.y"
            { emit("entity", (yyvsp[0].sval), "", ""); }
#line 1948 "y.tab.c"
    break;

  case 24: /* entity_decl: ENTITY IDENTIFIER $@3 LBRACE error RBRACE  */
#line 317 "parser.y"
            {
                printf("Invalid entity body at line %d\n", yylineno);
                yyerrok;
                emit("end_entity", (yyvsp[-4].sval), "", "");
            }
#line 1958 "y.tab.c"
    break;

  case 30: /* $@4: %empty  */
#line 336 "parser.y"
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
#line 1984 "y.tab.c"
    break;

  case 31: /* constructor_decl: IDENTIFIER $@4 LPAREN param_list_opt RPAREN block  */
#line 358 "parser.y"
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
#line 2055 "y.tab.c"
    break;

  case 32: /* $@5: %empty  */
#line 425 "parser.y"
            { emit("constr", (yyvsp[0].sval), "", ""); }
#line 2061 "y.tab.c"
    break;

  case 33: /* constructor_decl: IDENTIFIER $@5 LPAREN error RPAREN block  */
#line 427 "parser.y"
            {
                printf("Invalid constructor parameters at line %d\n", yylineno);
                yyerrok;
                emit("end_constr", (yyvsp[-5].sval), "", "");
            }
#line 2071 "y.tab.c"
    break;

  case 34: /* $@6: %empty  */
#line 437 "parser.y"
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
#line 2100 "y.tab.c"
    break;

  case 35: /* method_decl: access_modifier type FUNC IDENTIFIER $@6 LPAREN param_list_opt RPAREN block  */
#line 462 "parser.y"
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
#line 2170 "y.tab.c"
    break;

  case 36: /* $@7: %empty  */
#line 530 "parser.y"
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
#line 2195 "y.tab.c"
    break;

  case 37: /* method_decl: access_modifier IDENTIFIER FUNC IDENTIFIER $@7 LPAREN param_list_opt RPAREN block  */
#line 551 "parser.y"
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
#line 2260 "y.tab.c"
    break;

  case 38: /* $@8: %empty  */
#line 612 "parser.y"
            { emit("method", (yyvsp[0].sval), "", ""); }
#line 2266 "y.tab.c"
    break;

  case 39: /* method_decl: access_modifier type FUNC IDENTIFIER $@8 LPAREN error RPAREN block  */
#line 614 "parser.y"
            {
                printf("Invalid method parameters at line %d\n", yylineno);
                yyerrok;
                emit("end_method", (yyvsp[-5].sval), "", "");
            }
#line 2276 "y.tab.c"
    break;

  case 40: /* access_var_decl: access_modifier type IDENTIFIER SEMICOLON  */
#line 623 "parser.y"
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
#line 2293 "y.tab.c"
    break;

  case 41: /* access_var_decl: access_modifier IDENTIFIER IDENTIFIER SEMICOLON  */
#line 637 "parser.y"
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
#line 2310 "y.tab.c"
    break;

  case 42: /* access_modifier: PUBLIC  */
#line 652 "parser.y"
                  { (yyval.access) = ACC_PUBLIC;  }
#line 2316 "y.tab.c"
    break;

  case 43: /* access_modifier: PRIVATE  */
#line 653 "parser.y"
                  { (yyval.access) = ACC_PRIVATE; }
#line 2322 "y.tab.c"
    break;

  case 44: /* object_decl: IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 658 "parser.y"
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
				parse_error_count++;
                        }
                    }
                }

                emit("new", (yyvsp[-4].sval), "", (yyvsp[-7].sval));
                emit("push_ptr", (yyvsp[-7].sval), "", "");
                emit("call_constr", mangled_ctor, "", "");
                call_arg_count = 0;
            }
#line 2373 "y.tab.c"
    break;

  case 45: /* object_decl: type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 705 "parser.y"
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
			parse_error_count++;
                } else {
                       SymTable* ent_scope = find_entity_scope(obj_sym->attr.object.entity_name);
                    if(!ent_scope){
                        fprintf(stderr, "ERROR line %d: entity '%s' scope not found.\n",
                                yylineno, obj_sym->attr.object.entity_name);
                        parse_error_count++;
                    } else {
                        /* Walk inheritance chain to resolve method */
                        Symbol* method_sym = NULL;
                        const char* search_name = obj_sym->attr.object.entity_name;
                        while (search_name && search_name[0] && !method_sym) {
                            SymTable* sc = find_entity_scope(search_name);
                            if (sc) method_sym = lookup_local(sc, mangled_call);
                            if (!method_sym) {
                                Symbol* cls_s = lookup(global_scope, search_name);
                                if (cls_s && cls_s->kind == KIND_ENTITY
                                        && cls_s->attr.entity.parent_class[0])
                                    search_name = cls_s->attr.entity.parent_class;
                                else break;
                            }
                        }
                        if(!method_sym || method_sym->kind != KIND_METHOD){
                            fprintf(stderr, "ERROR line %d: Method '%s' not found in '%s' or its parents.\n",
                                    yylineno, mangled_call, obj_sym->attr.object.entity_name);
                            parse_error_count++;
                        }
                         else {
                            /* check private access */
                            if(method_sym->attr.method.access == ACC_PRIVATE){
                                fprintf(stderr, "ERROR line %d: Method '%s' is private.\n",
                                        yylineno, mangled_call);
				parse_error_count++;
                            }
                            /* return type check uses mangled symbol directly */
                            if(method_sym->attr.method.return_type != (yyvsp[-9].dtype)){
                                fprintf(stderr,
                                    "ERROR line %d: cannot assign result of '%s' (returns %s)"
                                    " to '%s' (declared as %s).\n",
                                    yylineno, mangled_call,
                                    dt_names[method_sym->attr.method.return_type],
                                    (yyvsp[-8].sval), dt_names[(yyvsp[-9].dtype)]);
				parse_error_count++;
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
#line 2450 "y.tab.c"
    break;

  case 47: /* arg_list_opt: %empty  */
#line 781 "parser.y"
                        { call_arg_count = 0; }
#line 2456 "y.tab.c"
    break;

  case 48: /* arg_list: arg_list COMMA expression  */
#line 786 "parser.y"
        {
            if(call_arg_count < 64){
                call_arg_types[call_arg_count++] = last_expr_type;
            } 
            emit("arg", (yyvsp[0].sval), "", ""); 
        }
#line 2467 "y.tab.c"
    break;

  case 49: /* arg_list: expression  */
#line 793 "parser.y"
        { 
            call_arg_count = 0;
            call_arg_types[call_arg_count++] = last_expr_type;
            emit("arg", (yyvsp[0].sval), "", ""); 
        }
#line 2477 "y.tab.c"
    break;

  case 50: /* $@9: %empty  */
#line 802 "parser.y"
            {
                SymTable* bs = create_scope(SCOPE_BLOCK, "block", current_scope);
                current_scope = bs;
            }
#line 2486 "y.tab.c"
    break;

  case 51: /* block: LBRACE $@9 stmt_list RBRACE  */
#line 807 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 2495 "y.tab.c"
    break;

  case 53: /* block: LBRACE error RBRACE  */
#line 813 "parser.y"
            {
                printf("Error inside block at line %d\n", yylineno);
                yyerrok;
            }
#line 2504 "y.tab.c"
    break;

  case 56: /* var_decl: type id_list SEMICOLON  */
#line 827 "parser.y"
            {
                insert_var_list((yyvsp[-1].sval), (yyvsp[-2].dtype));
                free((yyvsp[-1].sval));
            }
#line 2513 "y.tab.c"
    break;

  case 57: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 834 "parser.y"
            {
            if(last_expr_type != DT_UNKNOWN && last_expr_type != (yyvsp[-4].dtype)){
            //printf("==== $1 = %d and last_expr_type = %d\n", $1, last_expr_type);
            fprintf(stderr, "ERROR line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, (yyvsp[-3].sval), dt_names[(yyvsp[-4].dtype)], dt_names[last_expr_type]);
            	parse_error_count++;
		}
                //emit("=", $4, "", $2);
            //printf("Inserting the symbol: %s with the scope: %d\n", $2, current_scope->kind); 
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
#line 2539 "y.tab.c"
    break;

  case 58: /* var_decl: IDENTIFIER id_list SEMICOLON  */
#line 858 "parser.y"
            {
                insert_var_list((yyvsp[-1].sval), DT_ENTITY);
                free((yyvsp[-1].sval));
            }
#line 2548 "y.tab.c"
    break;

  case 59: /* var_decl: IDENTIFIER IDENTIFIER ASSIGN expression SEMICOLON  */
#line 865 "parser.y"
            {
            fprintf(stderr, "ERROR line %d: cannot initialize entity variable '%s' with value of type %s.\n", yylineno, (yyvsp[-3].sval), dt_names[last_expr_type]);
            parse_error_count++;
		emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval));
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
                            KIND_VAR, DT_ENTITY, yylineno);
            if (sym) sym->is_initialized = 1;
            }
#line 2561 "y.tab.c"
    break;

  case 60: /* id_list: id_list COMMA IDENTIFIER  */
#line 877 "parser.y"
            {
                char* buf = malloc(strlen((yyvsp[-2].sval)) + strlen((yyvsp[0].sval)) + 2);
                sprintf(buf, "%s,%s", (yyvsp[-2].sval), (yyvsp[0].sval));
                free((yyvsp[-2].sval));
                (yyval.sval) = buf;
            }
#line 2572 "y.tab.c"
    break;

  case 61: /* id_list: IDENTIFIER  */
#line 884 "parser.y"
            {
                (yyval.sval) = strdup((yyvsp[0].sval));
            }
#line 2580 "y.tab.c"
    break;

  case 62: /* type: INT  */
#line 890 "parser.y"
                 { (yyval.dtype) = DT_INT;    }
#line 2586 "y.tab.c"
    break;

  case 63: /* type: FP  */
#line 891 "parser.y"
                 { (yyval.dtype) = DT_FLOAT;  }
#line 2592 "y.tab.c"
    break;

  case 64: /* type: CHR  */
#line 892 "parser.y"
                 { (yyval.dtype) = DT_CHAR;   }
#line 2598 "y.tab.c"
    break;

  case 65: /* type: STRING  */
#line 893 "parser.y"
                 { (yyval.dtype) = DT_STRING; }
#line 2604 "y.tab.c"
    break;

  case 66: /* type: BOOL  */
#line 894 "parser.y"
                 { (yyval.dtype) = DT_BOOL;   }
#line 2610 "y.tab.c"
    break;

  case 67: /* type: VOID  */
#line 895 "parser.y"
                 { (yyval.dtype) = DT_VOID;   }
#line 2616 "y.tab.c"
    break;

  case 68: /* array_decl: type SEQ1 IDENTIFIER LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 900 "parser.y"
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
#line 2636 "y.tab.c"
    break;

  case 69: /* array_decl: type SEQ2 IDENTIFIER LBRACKET INT_LITERAL RBRACKET LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 919 "parser.y"
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
#line 2653 "y.tab.c"
    break;

  case 70: /* $@10: %empty  */
#line 935 "parser.y"
        {
            current_array_elem_type = (yyvsp[-3].dtype);
            array_type_errors = 0;
        }
#line 2662 "y.tab.c"
    break;

  case 71: /* array_decl: type SEQ1 IDENTIFIER ASSIGN $@10 array_init SEMICOLON  */
#line 940 "parser.y"
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
#line 2705 "y.tab.c"
    break;

  case 72: /* $@11: %empty  */
#line 979 "parser.y"
        {
            current_array_elem_type = (yyvsp[-3].dtype);
            array_type_errors = 0;
            arr2d_rows = 0;
            arr2d_cols = 0;
        }
#line 2716 "y.tab.c"
    break;

  case 73: /* array_decl: type SEQ2 IDENTIFIER ASSIGN $@11 array_init2d SEMICOLON  */
#line 986 "parser.y"
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
#line 2751 "y.tab.c"
    break;

  case 74: /* array_init: LBRACE expr_list RBRACE  */
#line 1019 "parser.y"
                                  { (yyval.ival) = (yyvsp[-1].ival); }
#line 2757 "y.tab.c"
    break;

  case 75: /* array_init2d: LBRACE row_list RBRACE  */
#line 1023 "parser.y"
                                   { (yyval.ival) = (yyvsp[-1].ival); }
#line 2763 "y.tab.c"
    break;

  case 76: /* row_list: row_list COMMA LBRACE expr_list RBRACE  */
#line 1028 "parser.y"
        { 
            arr2d_rows++;
            arr2d_cols = (yyvsp[-1].ival);
            (yyval.ival) = (yyvsp[-4].ival) + (yyvsp[-1].ival); 
        }
#line 2773 "y.tab.c"
    break;

  case 77: /* row_list: LBRACE expr_list RBRACE  */
#line 1034 "parser.y"
        { 
            arr2d_rows = 1;
            arr2d_cols = (yyvsp[-1].ival);
            (yyval.ival) = (yyvsp[-1].ival); 
        }
#line 2783 "y.tab.c"
    break;

  case 78: /* expr_list: expr_list COMMA expression  */
#line 1043 "parser.y"
        {
            if(current_array_elem_type != DT_UNKNOWN && last_expr_type != DT_UNKNOWN && last_expr_type != current_array_elem_type){
                {
                    fprintf(stderr, "ERROR line %d: array initializer type mismatch - expected %s, got %s\n", yylineno, dt_names[current_array_elem_type], dt_names[last_expr_type]);
                    array_type_errors++;
			parse_error_count++;
                } 
            }
            if (arr_init_count < 256)
                strncpy(arr_init_vals[arr_init_count++], (yyvsp[0].sval), 31);
            (yyval.ival) = (yyvsp[-2].ival) + 1;
        }
#line 2800 "y.tab.c"
    break;

  case 79: /* expr_list: expression  */
#line 1056 "parser.y"
        {
                    if(current_array_elem_type != DT_UNKNOWN && last_expr_type != DT_UNKNOWN && last_expr_type != current_array_elem_type){
                            {
                                    fprintf(stderr, "ERROR line %d: array initializer type mismatch - expected %s, got %s\n", yylineno, dt_names[current_array_elem_type], dt_names[last_expr_type]);
                                    array_type_errors++;
				    parse_error_count++;
                            }
                }
    //            arr_init_count = 0;
        if (arr_init_count < 256)
                strncpy(arr_init_vals[arr_init_count++], (yyvsp[0].sval), 31);
            (yyval.ival) = 1;
        }
#line 2818 "y.tab.c"
    break;

  case 80: /* $@12: %empty  */
#line 1073 "parser.y"
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
#line 2845 "y.tab.c"
    break;

  case 81: /* function_decl: func_type FUNC IDENTIFIER $@12 LPAREN param_list_opt RPAREN block  */
#line 1096 "parser.y"
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
#line 2914 "y.tab.c"
    break;

  case 82: /* $@13: %empty  */
#line 1162 "parser.y"
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
#line 2940 "y.tab.c"
    break;

  case 83: /* function_decl: IDENTIFIER FUNC IDENTIFIER $@13 LPAREN param_list_opt RPAREN block  */
#line 1184 "parser.y"
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
#line 3007 "y.tab.c"
    break;

  case 84: /* $@14: %empty  */
#line 1248 "parser.y"
            { emit("func", (yyvsp[0].sval), "", ""); }
#line 3013 "y.tab.c"
    break;

  case 85: /* function_decl: func_type FUNC IDENTIFIER $@14 LPAREN error RPAREN block  */
#line 1250 "parser.y"
            {
                printf("Invalid parameter list at line %d\n", yylineno);
                yyerrok;
                emit("endfunc", "", "", "");
            }
#line 3023 "y.tab.c"
    break;

  case 86: /* func_type: type  */
#line 1258 "parser.y"
               { (yyval.dtype) = (yyvsp[0].dtype);      }
#line 3029 "y.tab.c"
    break;

  case 87: /* func_type: VOID  */
#line 1259 "parser.y"
               { (yyval.dtype) = DT_VOID; }
#line 3035 "y.tab.c"
    break;

  case 92: /* param: type IDENTIFIER  */
#line 1275 "parser.y"
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
#line 3059 "y.tab.c"
    break;

  case 93: /* param: IDENTIFIER IDENTIFIER  */
#line 1296 "parser.y"
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
#line 3083 "y.tab.c"
    break;

  case 94: /* return_stmt: RETURN expression SEMICOLON  */
#line 1319 "parser.y"
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
                	parse_error_count++;
		}
                else if(last_expr_type != DT_UNKNOWN && last_expr_type != expected){
                    fprintf(stderr, "ERROR line %d: function '%s' expected to return %s"
                        " but returns %s", yylineno, current_function->name, dt_names[expected], dt_names[last_expr_type]); 
                	parse_error_count++;
		}
            }
            emit("return", (yyvsp[-1].sval), "", ""); 
        }
#line 3112 "y.tab.c"
    break;

  case 95: /* return_stmt: RETURN SEMICOLON  */
#line 1344 "parser.y"
        {
            if(current_function){
                DataType expected = current_function->attr.func.return_type;
                if(expected != DT_VOID){
                    fprintf(stderr, "ERROR line %d: Function '%s' is non-void (returns %s)"
                        " but has empty return.\n", yylineno, current_function->name, dt_names[expected]);
                parse_error_count++;
		}
            } 
            emit("return", "", "", ""); 
        }
#line 3128 "y.tab.c"
    break;

  case 96: /* return_stmt: RETURN error SEMICOLON  */
#line 1357 "parser.y"
            {
                printf("Invalid return statement at line %d\n", yylineno);
                yyerrok;
            }
#line 3137 "y.tab.c"
    break;

  case 99: /* expression: assignment  */
#line 1369 "parser.y"
                     { (yyval.sval) = (yyvsp[0].sval); }
#line 3143 "y.tab.c"
    break;

  case 100: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET  */
#line 1373 "parser.y"
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
#line 3159 "y.tab.c"
    break;

  case 101: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET  */
#line 1385 "parser.y"
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
#line 3181 "y.tab.c"
    break;

  case 102: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 1406 "parser.y"
            {
            // Checking the type before assignment
            Symbol* lhs = require_declared(current_scope, (yyvsp[-2].sval), yylineno);
            if(lhs && last_expr_type != DT_UNKNOWN){
                if(lhs->datatype != last_expr_type){
                    fprintf(stderr, "ERROR line %d: cannot assign %s to '%s' "
                    "(declared as %s).\n", yylineno, dt_names[last_expr_type], (yyvsp[-2].sval), dt_names[lhs->datatype]);
                	parse_error_count++;
		}
            } 
            emit("=", (yyvsp[0].sval), "", ir_name_of((yyvsp[-2].sval))); 
            (yyval.sval) = strdup((yyvsp[-2].sval)); 
        }
#line 3199 "y.tab.c"
    break;

  case 103: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 1420 "parser.y"
            { 
            Symbol* lhs = require_declared(current_scope, (yyvsp[-2].sval), yylineno);
            if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
                fprintf(stderr, "ERROR line %d: type mismatch in '+=' : '%s' is %s but RHS is %s.\n", yylineno, (yyvsp[-2].sval), dt_names[lhs->datatype], dt_names[last_expr_type]);
            	parse_error_count++;	
	    }
            char* t = genVar(); 
            const char* irn = ir_name_of((yyvsp[-2].sval));
            emit("+", irn, (yyvsp[0].sval), t); 
            emit("=", t, "", irn); 
            (yyval.sval) = t; 
        }
#line 3216 "y.tab.c"
    break;

  case 104: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 1433 "parser.y"
            { 	
            Symbol* lhs = require_declared(current_scope, (yyvsp[-2].sval), yylineno);
            if(lhs && last_expr_type != DT_UNKNOWN && lhs->datatype != last_expr_type){
                fprintf(stderr, "ERROR line %d: type mismatch in '-=' : '%s' is %s but RHS is %s.\n", yylineno, (yyvsp[-2].sval), dt_names[lhs->datatype], dt_names[last_expr_type]);
            	parse_error_count++;
	    } 
                
            char* t = genVar(); 
            const char* irn = ir_name_of((yyvsp[-2].sval));
            emit("-", irn, (yyvsp[0].sval), t); 
            emit("=", t, "", irn); 	
            (yyval.sval) = t; 
        }
#line 3234 "y.tab.c"
    break;

  case 105: /* assignment: IDENTIFIER LBRACKET expression RBRACKET ASSIGN assignment  */
#line 1447 "parser.y"
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
#line 3251 "y.tab.c"
    break;

  case 106: /* assignment: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET ASSIGN assignment  */
#line 1460 "parser.y"
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
#line 3272 "y.tab.c"
    break;

  case 107: /* assignment: indexed_id ASSIGN assignment  */
#line 1477 "parser.y"
            { (yyval.sval) = (yyvsp[0].sval); }
#line 3278 "y.tab.c"
    break;

  case 108: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 1479 "parser.y"
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
                    	parse_error_count++;
	            }
                }
            }	
            emit("set_field", "this", (yyvsp[-2].sval), (yyvsp[0].sval)); 
            (yyval.sval) = (yyvsp[0].sval); 
        }
#line 3309 "y.tab.c"
    break;

  case 109: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 1506 "parser.y"
            { 
            check_field_access((yyvsp[-4].sval),(yyvsp[-2].sval),yylineno); 
            Symbol* obj = lookup(current_scope, (yyvsp[-4].sval));
            if(!obj || obj->kind != KIND_OBJECT){
                fprintf(stderr, "ERROR line %d: '%s' is not an object.\n", yylineno, (yyvsp[-4].sval));
		parse_error_count++;
            }
            else{
                const char* entity_name = obj->attr.object.entity_name;
                SymTable* ent_scope = find_entity_scope(entity_name);
                if(!ent_scope){
		    parse_error_count++;
                    fprintf(stderr, "ERROR line %d: Entity %s never defined.\n", yylineno, entity_name);
                }
                else{
                    Symbol* field = lookup_local(ent_scope, (yyvsp[-2].sval));
                    if(!field || field->kind != KIND_FIELD){
			parse_error_count++;
                        fprintf(stderr, "ERROR line %d: '%s' is not a field of entity '%s'.\n", yylineno, (yyvsp[-2].sval), entity_name);
                    }
                    else if(last_expr_type != DT_UNKNOWN && field->datatype != last_expr_type){
			parse_error_count++;
                        fprintf(stderr, "ERROR line %d: cannot assign %s to '%s.%s' (declared as %s).\n", yylineno, dt_names[last_expr_type], entity_name, (yyvsp[-2].sval), dt_names[field->datatype]);
                    }
                }
            }
            emit("set_field", (yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); 
            (yyval.sval) = (yyvsp[0].sval); 
        }
#line 3343 "y.tab.c"
    break;

  case 110: /* assignment: logic_expr  */
#line 1535 "parser.y"
                     { (yyval.sval) = (yyvsp[0].sval); }
#line 3349 "y.tab.c"
    break;

  case 111: /* logic_expr: logic_expr OR logic_expr  */
#line 1540 "parser.y"
            { char* t = genVar(); emit("||", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3355 "y.tab.c"
    break;

  case 112: /* logic_expr: logic_expr AND logic_expr  */
#line 1542 "parser.y"
            { char* t = genVar(); emit("&&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3361 "y.tab.c"
    break;

  case 113: /* logic_expr: NOT logic_expr  */
#line 1544 "parser.y"
            { char* t = genVar(); emit("!", (yyvsp[0].sval), "", t); (yyval.sval) = t; }
#line 3367 "y.tab.c"
    break;

  case 114: /* logic_expr: bitwise_expr  */
#line 1545 "parser.y"
                       { (yyval.sval) = (yyvsp[0].sval); }
#line 3373 "y.tab.c"
    break;

  case 115: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 1550 "parser.y"
            { char* t = genVar(); emit("&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3379 "y.tab.c"
    break;

  case 116: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 1552 "parser.y"
            { char* t = genVar(); emit("|", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3385 "y.tab.c"
    break;

  case 117: /* bitwise_expr: rel_expr  */
#line 1553 "parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 3391 "y.tab.c"
    break;

  case 118: /* rel_expr: arith_expr GT arith_expr  */
#line 1558 "parser.y"
            {
            char* t = genVar(); emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
                
            }
#line 3400 "y.tab.c"
    break;

  case 119: /* rel_expr: arith_expr LT arith_expr  */
#line 1563 "parser.y"
            {
                    char* t = genVar(); emit("<", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
            }
#line 3408 "y.tab.c"
    break;

  case 120: /* rel_expr: arith_expr EQ arith_expr  */
#line 1567 "parser.y"
            {
                    char* t = genVar(); emit("==", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
            }
#line 3416 "y.tab.c"
    break;

  case 121: /* rel_expr: arith_expr  */
#line 1570 "parser.y"
                     { (yyval.sval) = (yyvsp[0].sval); }
#line 3422 "y.tab.c"
    break;

  case 122: /* arith_expr: arith_expr PLUS term  */
#line 1575 "parser.y"
        {
                    char* t = genVar();
                    emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
                
            }
#line 3433 "y.tab.c"
    break;

  case 123: /* arith_expr: arith_expr MINUS term  */
#line 1582 "parser.y"
            {  
                    char* t = genVar();
                    emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
            }
#line 3443 "y.tab.c"
    break;

  case 124: /* arith_expr: term  */
#line 1587 "parser.y"
               { (yyval.sval) = (yyvsp[0].sval); }
#line 3449 "y.tab.c"
    break;

  case 125: /* term: term MUL factor  */
#line 1592 "parser.y"
            {
                    char* t = genVar();
                    emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
            }
#line 3459 "y.tab.c"
    break;

  case 126: /* term: term DIV factor  */
#line 1598 "parser.y"
            {
                    char* t = genVar();
                    emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
            }
#line 3469 "y.tab.c"
    break;

  case 127: /* term: term MOD factor  */
#line 1604 "parser.y"
            {
                    char* t = genVar();
                    emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
            }
#line 3479 "y.tab.c"
    break;

  case 128: /* term: factor  */
#line 1609 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 3485 "y.tab.c"
    break;

  case 129: /* factor: MINUS factor  */
#line 1614 "parser.y"
            {
                char* t = genVar();
                emit("-", "0", (yyvsp[0].sval), t);
                (yyval.sval) = t;
                last_expr_type = DT_INT;
            }
#line 3496 "y.tab.c"
    break;

  case 130: /* factor: IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 1621 "parser.y"
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
		parse_error_count++;
                last_expr_type = DT_UNKNOWN;
            }
            else if(fsym->kind != KIND_FUNCTION && fsym->kind != KIND_METHOD){
                fprintf(stderr,
                    "ERROR line %d: '%s' is not a function.\n", yylineno, (yyvsp[-3].sval));
		parse_error_count++;
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
			parse_error_count++;
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
				parse_error_count++;
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
#line 3582 "y.tab.c"
    break;

  case 131: /* factor: THIS DOT IDENTIFIER  */
#line 1703 "parser.y"
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
			parse_error_count++;
                        last_expr_type = DT_UNKNOWN;
                    }
                }
                else{
                    last_expr_type = DT_UNKNOWN;
                }
                emit("get_field", "this", (yyvsp[0].sval), t);
                (yyval.sval) = t;
            }
#line 3615 "y.tab.c"
    break;

  case 132: /* factor: IDENTIFIER DOT IDENTIFIER  */
#line 1733 "parser.y"
        {
            char* t = genVar();
            last_expr_type = DT_UNKNOWN;  /* safe default */

            /* Step 1: look up the object variable */
            Symbol* obj = lookup(current_scope, (yyvsp[-2].sval));
            if (!obj) {
                fprintf(stderr,
                    "ERROR line %d: undeclared identifier '%s'.\n",
                    yylineno, (yyvsp[-2].sval));
		parse_error_count++;
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
			parse_error_count++;
                }
                else {
                    /* look up the entity symbol in global scope */
                    Symbol* cls = lookup(global_scope, entity_name);
                    if (!cls || cls->kind != KIND_ENTITY) {
                        fprintf(stderr,
                            "ERROR line %d: entity '%s' not defined.\n",
                            yylineno, entity_name);
			parse_error_count++;
                    }
                    else {
                        /* get the entity's scope (stored in EntityAttr) */
                        SymTable* escope = cls->attr.entity.scope;
                        if (!escope) {
                            fprintf(stderr,
                                "ERROR line %d: entity '%s' has no scope.\n",
                                yylineno, entity_name);
				parse_error_count++;
                        }
                        else {
                            /* look up the field inside entity scope */
                            Symbol* field = lookup_local(escope, (yyvsp[0].sval));
                            if (!field || field->kind != KIND_FIELD) {
                                fprintf(stderr,
                                    "ERROR line %d: '%s' is not a field of '%s'.\n",
                                    yylineno, (yyvsp[0].sval), entity_name);
				parse_error_count++;
                            }
                            else {
                                //check private access
                                if (field->attr.field.access == ACC_PRIVATE &&
                                    strcmp(current_scope->name, entity_name) != 0) {
                                    fprintf(stderr,
                                        "ERROR line %d: field '%s' of '%s' is private.\n",
                                        yylineno, (yyvsp[0].sval), entity_name);
					parse_error_count++;
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
#line 3694 "y.tab.c"
    break;

  case 133: /* factor: IDENTIFIER  */
#line 1808 "parser.y"
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
#line 3722 "y.tab.c"
    break;

  case 134: /* factor: indexed_id  */
#line 1831 "parser.y"
                          { (yyval.sval) = (yyvsp[0].sval); }
#line 3728 "y.tab.c"
    break;

  case 135: /* factor: INT_LITERAL  */
#line 1832 "parser.y"
                          { char b[20]; sprintf(b, "%d",   (yyvsp[0].ival)); (yyval.sval) = strdup(b); last_expr_type = DT_INT; }
#line 3734 "y.tab.c"
    break;

  case 136: /* factor: FLOAT_LITERAL  */
#line 1833 "parser.y"
                          { char b[20]; sprintf(b, "%f",   (yyvsp[0].fval)); (yyval.sval) = strdup(b);  last_expr_type = DT_FLOAT; }
#line 3740 "y.tab.c"
    break;

  case 137: /* factor: CHAR_LITERAL  */
#line 1834 "parser.y"
                          { char b[20]; sprintf(b, "'%c'", (yyvsp[0].cval)); (yyval.sval) = strdup(b); last_expr_type = DT_CHAR; }
#line 3746 "y.tab.c"
    break;

  case 138: /* factor: STRING_LITERAL  */
#line 1835 "parser.y"
                          { (yyval.sval) = strdup((yyvsp[0].sval));  last_expr_type = DT_STRING; }
#line 3752 "y.tab.c"
    break;

  case 139: /* factor: TRUE  */
#line 1836 "parser.y"
                          { (yyval.sval) = strdup("1"); last_expr_type = DT_BOOL; }
#line 3758 "y.tab.c"
    break;

  case 140: /* factor: FALSE  */
#line 1837 "parser.y"
                          { (yyval.sval) = strdup("0"); last_expr_type = DT_BOOL; }
#line 3764 "y.tab.c"
    break;

  case 141: /* factor: LPAREN expression RPAREN  */
#line 1838 "parser.y"
                                   { (yyval.sval) = (yyvsp[-1].sval); }
#line 3770 "y.tab.c"
    break;

  case 142: /* $@15: %empty  */
#line 1843 "parser.y"
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
#line 3791 "y.tab.c"
    break;

  case 143: /* $@16: %empty  */
#line 1860 "parser.y"
            { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3797 "y.tab.c"
    break;

  case 144: /* $@17: %empty  */
#line 1862 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 3806 "y.tab.c"
    break;

  case 145: /* $@18: %empty  */
#line 1867 "parser.y"
            { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3812 "y.tab.c"
    break;

  case 146: /* if_stmt: IF LPAREN $@15 expression $@16 RPAREN $@17 block $@18 elif_list else_opt  */
#line 1869 "parser.y"
            { emit("label", "", "", topEnd()); popIfLabels(); }
#line 3818 "y.tab.c"
    break;

  case 147: /* if_stmt: IF LPAREN error RPAREN block  */
#line 1872 "parser.y"
            {
                printf("Invalid IF condition at line %d\n", yylineno);
                yyerrok;
            }
#line 3827 "y.tab.c"
    break;

  case 148: /* $@19: %empty  */
#line 1880 "parser.y"
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
#line 3848 "y.tab.c"
    break;

  case 149: /* $@20: %empty  */
#line 1897 "parser.y"
            { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3854 "y.tab.c"
    break;

  case 150: /* $@21: %empty  */
#line 1899 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 3863 "y.tab.c"
    break;

  case 151: /* $@22: %empty  */
#line 1904 "parser.y"
            { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3869 "y.tab.c"
    break;

  case 154: /* $@23: %empty  */
#line 1911 "parser.y"
            {
                char else_name[32];
                snprintf(else_name, 32, "else_%d", if_cnt++);
                SymTable* es = create_scope(SCOPE_ELSE, else_name, current_scope);
                current_scope = es;
            }
#line 3880 "y.tab.c"
    break;

  case 155: /* else_opt: ELSE $@23 block  */
#line 1918 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 3889 "y.tab.c"
    break;

  case 156: /* $@24: %empty  */
#line 1923 "parser.y"
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
#line 3910 "y.tab.c"
    break;

  case 157: /* $@25: %empty  */
#line 1940 "parser.y"
            { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3916 "y.tab.c"
    break;

  case 158: /* $@26: %empty  */
#line 1942 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 3925 "y.tab.c"
    break;

  case 159: /* $@27: %empty  */
#line 1947 "parser.y"
            { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3931 "y.tab.c"
    break;

  case 162: /* break_stmt: BREAK SEMICOLON  */
#line 1954 "parser.y"
        {
            if(loopTop < 0){
                fprintf(stderr, "ERROR line %d: 'break' outside loop.\n", yylineno);
            } else {
                emit("goto", "", "", loopEndStack[loopTop]);
            }
        }
#line 3943 "y.tab.c"
    break;

  case 163: /* continue_stmt: CONTINUE SEMICOLON  */
#line 1965 "parser.y"
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
#line 3963 "y.tab.c"
    break;

  case 164: /* $@28: %empty  */
#line 1984 "parser.y"
            {
                char for_name[32];
                snprintf(for_name, 32, "for_%d", for_cnt++);
                SymTable* fs = create_scope(SCOPE_FOR, for_name, current_scope);
                current_scope = fs;
            }
#line 3974 "y.tab.c"
    break;

  case 165: /* for_stmt: FOR LPAREN $@28 for_header block  */
#line 1991 "parser.y"
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
#line 3997 "y.tab.c"
    break;

  case 166: /* for_stmt: FOR LPAREN error RPAREN block  */
#line 2011 "parser.y"
            {
                printf("Invalid FOR header at line %d\n", yylineno);
                yyerrok;
            }
#line 4006 "y.tab.c"
    break;

  case 167: /* $@29: %empty  */
#line 2019 "parser.y"
            {
                char* b = getLabel();
                char* e = getLabel();
                pushIfLabels(b, e);
                loopEndStack[++loopTop] = strdup(e); 
                loopCondStack[loopTop]  = strdup(b);
                emit("label", "", "", b);
            }
#line 4019 "y.tab.c"
    break;

  case 168: /* $@30: %empty  */
#line 2028 "parser.y"
            {
                emit("ifFalse", (yyvsp[-1].sval) ? (yyvsp[-1].sval) : "1", "", topEnd());
                forDepth++;
                forIncIdx[forDepth]      = 0;
                inForIncrement[forDepth] = 1;
            }
#line 4030 "y.tab.c"
    break;

  case 169: /* $@31: %empty  */
#line 2035 "parser.y"
            { inForIncrement[forDepth] = 0; }
#line 4036 "y.tab.c"
    break;

  case 171: /* for_init_opt: var_decl_no_semi  */
#line 2040 "parser.y"
                                { printf("Hit var_decl_no_semi");(yyval.sval) = NULL; }
#line 4042 "y.tab.c"
    break;

  case 172: /* for_init_opt: expression  */
#line 2041 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 4048 "y.tab.c"
    break;

  case 173: /* for_init_opt: %empty  */
#line 2042 "parser.y"
                        { printf("Matching with empty string wala in for_init\n");(yyval.sval) = NULL; }
#line 4054 "y.tab.c"
    break;

  case 174: /* for_cond_opt: expression  */
#line 2046 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 4060 "y.tab.c"
    break;

  case 175: /* for_cond_opt: %empty  */
#line 2047 "parser.y"
                        { (yyval.sval) = strdup("1"); }
#line 4066 "y.tab.c"
    break;

  case 176: /* for_inc_opt: expression  */
#line 2051 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 4072 "y.tab.c"
    break;

  case 177: /* for_inc_opt: %empty  */
#line 2052 "parser.y"
                        { (yyval.sval) = NULL; }
#line 4078 "y.tab.c"
    break;

  case 178: /* var_decl_no_semi: type IDENTIFIER  */
#line 2057 "parser.y"
            {
                insert_symbol(current_scope, (yyvsp[0].sval), KIND_VAR, (yyvsp[-1].dtype), yylineno);
            }
#line 4086 "y.tab.c"
    break;

  case 179: /* var_decl_no_semi: type IDENTIFIER ASSIGN expression  */
#line 2061 "parser.y"
            {
                if(last_expr_type != DT_UNKNOWN && last_expr_type != (yyvsp[-3].dtype)){
                fprintf(stderr, "ERROR line %d: Cannot initialize '%s' (declared as %s) with value of type %s.\n", yylineno, (yyvsp[-2].sval), dt_names[(yyvsp[-3].dtype)], dt_names[last_expr_type]);
            		parse_error_count++;
		}
                    //emit("=", $4, "", $2);
                Symbol* sym = insert_symbol(current_scope, (yyvsp[-2].sval),
                                            KIND_VAR, (yyvsp[-3].dtype), yylineno);
                    if (sym) sym->is_initialized = 1;
            emit("=", (yyvsp[0].sval), "", ir_name_of((yyvsp[-2].sval)));
            }
#line 4102 "y.tab.c"
    break;

  case 180: /* io_stmt: IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 2076 "parser.y"
            {
            Symbol* var = require_declared(current_scope, (yyvsp[-6].sval), yylineno);
            if(!var){
		parse_error_count++;
                fprintf(stderr, "ERROR line %d: assignment to undeclared variable '%s'.\n", yylineno, (yyvsp[-6].sval));
            }
            emit("out", (yyvsp[-2].sval), "", "");
            emit("in", "", "", (yyvsp[-6].sval)); 
        }
#line 4116 "y.tab.c"
    break;

  case 181: /* io_stmt: type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 2086 "parser.y"
            { 
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-6].sval), KIND_VAR, (yyvsp[-7].dtype), yylineno);
            if(sym){
                sym->is_initialized = 1;
            }
            emit("out", (yyvsp[-2].sval), "", "");
            emit("in", "", "", (yyvsp[-6].sval)); 
        }
#line 4129 "y.tab.c"
    break;

  case 182: /* io_stmt: SHOW LPAREN expression RPAREN SEMICOLON  */
#line 2095 "parser.y"
            { emit("out", (yyvsp[-2].sval), "", ""); }
#line 4135 "y.tab.c"
    break;

  case 183: /* io_stmt: SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON  */
#line 2097 "parser.y"
            {
                char* t = genVar();
                emit("[]",  (yyvsp[-5].sval), (yyvsp[-3].sval), t);
                emit("out", t,  "", "");
            }
#line 4145 "y.tab.c"
    break;


#line 4149 "y.tab.c"

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

#line 2104 "parser.y"


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
                parse_error_count++;
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
			parse_error_count++;
                }
            }
       	}

        yyin = stdin;
        yyparse();
	
	// check if there are any errors and if yes then return
	if (parse_error_count > 0) {
		fprintf(stderr, "\n%d error(s) found. Aborting compilation.\n", parse_error_count);
    		return 1;
	}
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

