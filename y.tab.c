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
    SNAPSHOT = 258,                /* SNAPSHOT  */
    REWIND = 259,                  /* REWIND  */
    INT = 260,                     /* INT  */
    FP = 261,                      /* FP  */
    CHR = 262,                     /* CHR  */
    STRING = 263,                  /* STRING  */
    BOOL = 264,                    /* BOOL  */
    VOID = 265,                    /* VOID  */
    IF = 266,                      /* IF  */
    ELIF = 267,                    /* ELIF  */
    ELSE = 268,                    /* ELSE  */
    FOR = 269,                     /* FOR  */
    TRUE = 270,                    /* TRUE  */
    FALSE = 271,                   /* FALSE  */
    FEED = 272,                    /* FEED  */
    SHOW = 273,                    /* SHOW  */
    RETURN = 274,                  /* RETURN  */
    BREAK = 275,                   /* BREAK  */
    CONTINUE = 276,                /* CONTINUE  */
    SEQ1 = 277,                    /* SEQ1  */
    SEQ2 = 278,                    /* SEQ2  */
    FUNC = 279,                    /* FUNC  */
    ENTITY = 280,                  /* ENTITY  */
    NEW = 281,                     /* NEW  */
    PUBLIC = 282,                  /* PUBLIC  */
    PRIVATE = 283,                 /* PRIVATE  */
    THIS = 284,                    /* THIS  */
    DOT = 285,                     /* DOT  */
    EXTENDS = 286,                 /* EXTENDS  */
    LPAREN = 287,                  /* LPAREN  */
    RPAREN = 288,                  /* RPAREN  */
    LBRACE = 289,                  /* LBRACE  */
    RBRACE = 290,                  /* RBRACE  */
    LBRACKET = 291,                /* LBRACKET  */
    RBRACKET = 292,                /* RBRACKET  */
    SEMICOLON = 293,               /* SEMICOLON  */
    COMMA = 294,                   /* COMMA  */
    IDENTIFIER = 295,              /* IDENTIFIER  */
    STRING_LITERAL = 296,          /* STRING_LITERAL  */
    INT_LITERAL = 297,             /* INT_LITERAL  */
    FLOAT_LITERAL = 298,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 299,            /* CHAR_LITERAL  */
    ASSIGN = 300,                  /* ASSIGN  */
    ADD_ASSIGN = 301,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 302,              /* SUB_ASSIGN  */
    OR = 303,                      /* OR  */
    AND = 304,                     /* AND  */
    BITOR = 305,                   /* BITOR  */
    BITAND = 306,                  /* BITAND  */
    EQ = 307,                      /* EQ  */
    GT = 308,                      /* GT  */
    LT = 309,                      /* LT  */
    PLUS = 310,                    /* PLUS  */
    MINUS = 311,                   /* MINUS  */
    MUL = 312,                     /* MUL  */
    DIV = 313,                     /* DIV  */
    MOD = 314,                     /* MOD  */
    NOT = 315                      /* NOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define SNAPSHOT 258
#define REWIND 259
#define INT 260
#define FP 261
#define CHR 262
#define STRING 263
#define BOOL 264
#define VOID 265
#define IF 266
#define ELIF 267
#define ELSE 268
#define FOR 269
#define TRUE 270
#define FALSE 271
#define FEED 272
#define SHOW 273
#define RETURN 274
#define BREAK 275
#define CONTINUE 276
#define SEQ1 277
#define SEQ2 278
#define FUNC 279
#define ENTITY 280
#define NEW 281
#define PUBLIC 282
#define PRIVATE 283
#define THIS 284
#define DOT 285
#define EXTENDS 286
#define LPAREN 287
#define RPAREN 288
#define LBRACE 289
#define RBRACE 290
#define LBRACKET 291
#define RBRACKET 292
#define SEMICOLON 293
#define COMMA 294
#define IDENTIFIER 295
#define STRING_LITERAL 296
#define INT_LITERAL 297
#define FLOAT_LITERAL 298
#define CHAR_LITERAL 299
#define ASSIGN 300
#define ADD_ASSIGN 301
#define SUB_ASSIGN 302
#define OR 303
#define AND 304
#define BITOR 305
#define BITAND 306
#define EQ 307
#define GT 308
#define LT 309
#define PLUS 310
#define MINUS 311
#define MUL 312
#define DIV 313
#define MOD 314
#define NOT 315

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
    

#line 381 "y.tab.c"

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
  YYSYMBOL_SNAPSHOT = 3,                   /* SNAPSHOT  */
  YYSYMBOL_REWIND = 4,                     /* REWIND  */
  YYSYMBOL_INT = 5,                        /* INT  */
  YYSYMBOL_FP = 6,                         /* FP  */
  YYSYMBOL_CHR = 7,                        /* CHR  */
  YYSYMBOL_STRING = 8,                     /* STRING  */
  YYSYMBOL_BOOL = 9,                       /* BOOL  */
  YYSYMBOL_VOID = 10,                      /* VOID  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_ELIF = 12,                      /* ELIF  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_FOR = 14,                       /* FOR  */
  YYSYMBOL_TRUE = 15,                      /* TRUE  */
  YYSYMBOL_FALSE = 16,                     /* FALSE  */
  YYSYMBOL_FEED = 17,                      /* FEED  */
  YYSYMBOL_SHOW = 18,                      /* SHOW  */
  YYSYMBOL_RETURN = 19,                    /* RETURN  */
  YYSYMBOL_BREAK = 20,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 21,                  /* CONTINUE  */
  YYSYMBOL_SEQ1 = 22,                      /* SEQ1  */
  YYSYMBOL_SEQ2 = 23,                      /* SEQ2  */
  YYSYMBOL_FUNC = 24,                      /* FUNC  */
  YYSYMBOL_ENTITY = 25,                    /* ENTITY  */
  YYSYMBOL_NEW = 26,                       /* NEW  */
  YYSYMBOL_PUBLIC = 27,                    /* PUBLIC  */
  YYSYMBOL_PRIVATE = 28,                   /* PRIVATE  */
  YYSYMBOL_THIS = 29,                      /* THIS  */
  YYSYMBOL_DOT = 30,                       /* DOT  */
  YYSYMBOL_EXTENDS = 31,                   /* EXTENDS  */
  YYSYMBOL_LPAREN = 32,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 33,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 34,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 35,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 36,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 37,                  /* RBRACKET  */
  YYSYMBOL_SEMICOLON = 38,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 39,                     /* COMMA  */
  YYSYMBOL_IDENTIFIER = 40,                /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 41,            /* STRING_LITERAL  */
  YYSYMBOL_INT_LITERAL = 42,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 43,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 44,              /* CHAR_LITERAL  */
  YYSYMBOL_ASSIGN = 45,                    /* ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 46,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 47,                /* SUB_ASSIGN  */
  YYSYMBOL_OR = 48,                        /* OR  */
  YYSYMBOL_AND = 49,                       /* AND  */
  YYSYMBOL_BITOR = 50,                     /* BITOR  */
  YYSYMBOL_BITAND = 51,                    /* BITAND  */
  YYSYMBOL_EQ = 52,                        /* EQ  */
  YYSYMBOL_GT = 53,                        /* GT  */
  YYSYMBOL_LT = 54,                        /* LT  */
  YYSYMBOL_PLUS = 55,                      /* PLUS  */
  YYSYMBOL_MINUS = 56,                     /* MINUS  */
  YYSYMBOL_MUL = 57,                       /* MUL  */
  YYSYMBOL_DIV = 58,                       /* DIV  */
  YYSYMBOL_MOD = 59,                       /* MOD  */
  YYSYMBOL_NOT = 60,                       /* NOT  */
  YYSYMBOL_YYACCEPT = 61,                  /* $accept  */
  YYSYMBOL_program = 62,                   /* program  */
  YYSYMBOL_element = 63,                   /* element  */
  YYSYMBOL_statement = 64,                 /* statement  */
  YYSYMBOL_entity_decl = 65,               /* entity_decl  */
  YYSYMBOL_66_1 = 66,                      /* $@1  */
  YYSYMBOL_67_2 = 67,                      /* $@2  */
  YYSYMBOL_68_3 = 68,                      /* $@3  */
  YYSYMBOL_entity_body = 69,               /* entity_body  */
  YYSYMBOL_entity_member = 70,             /* entity_member  */
  YYSYMBOL_constructor_decl = 71,          /* constructor_decl  */
  YYSYMBOL_72_4 = 72,                      /* $@4  */
  YYSYMBOL_73_5 = 73,                      /* $@5  */
  YYSYMBOL_method_decl = 74,               /* method_decl  */
  YYSYMBOL_75_6 = 75,                      /* $@6  */
  YYSYMBOL_76_7 = 76,                      /* $@7  */
  YYSYMBOL_77_8 = 77,                      /* $@8  */
  YYSYMBOL_access_var_decl = 78,           /* access_var_decl  */
  YYSYMBOL_access_modifier = 79,           /* access_modifier  */
  YYSYMBOL_object_decl = 80,               /* object_decl  */
  YYSYMBOL_arg_list_opt = 81,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 82,                  /* arg_list  */
  YYSYMBOL_block = 83,                     /* block  */
  YYSYMBOL_84_9 = 84,                      /* $@9  */
  YYSYMBOL_stmt_list = 85,                 /* stmt_list  */
  YYSYMBOL_var_decl = 86,                  /* var_decl  */
  YYSYMBOL_id_list = 87,                   /* id_list  */
  YYSYMBOL_type = 88,                      /* type  */
  YYSYMBOL_array_decl = 89,                /* array_decl  */
  YYSYMBOL_90_10 = 90,                     /* $@10  */
  YYSYMBOL_91_11 = 91,                     /* $@11  */
  YYSYMBOL_array_init = 92,                /* array_init  */
  YYSYMBOL_array_init2d = 93,              /* array_init2d  */
  YYSYMBOL_row_list = 94,                  /* row_list  */
  YYSYMBOL_expr_list = 95,                 /* expr_list  */
  YYSYMBOL_function_decl = 96,             /* function_decl  */
  YYSYMBOL_97_12 = 97,                     /* $@12  */
  YYSYMBOL_98_13 = 98,                     /* $@13  */
  YYSYMBOL_99_14 = 99,                     /* $@14  */
  YYSYMBOL_func_type = 100,                /* func_type  */
  YYSYMBOL_param_list_opt = 101,           /* param_list_opt  */
  YYSYMBOL_param_list = 102,               /* param_list  */
  YYSYMBOL_param = 103,                    /* param  */
  YYSYMBOL_return_stmt = 104,              /* return_stmt  */
  YYSYMBOL_expr_stmt = 105,                /* expr_stmt  */
  YYSYMBOL_expression = 106,               /* expression  */
  YYSYMBOL_indexed_id = 107,               /* indexed_id  */
  YYSYMBOL_assignment = 108,               /* assignment  */
  YYSYMBOL_logic_expr = 109,               /* logic_expr  */
  YYSYMBOL_bitwise_expr = 110,             /* bitwise_expr  */
  YYSYMBOL_rel_expr = 111,                 /* rel_expr  */
  YYSYMBOL_arith_expr = 112,               /* arith_expr  */
  YYSYMBOL_term = 113,                     /* term  */
  YYSYMBOL_factor = 114,                   /* factor  */
  YYSYMBOL_if_stmt = 115,                  /* if_stmt  */
  YYSYMBOL_116_15 = 116,                   /* $@15  */
  YYSYMBOL_117_16 = 117,                   /* $@16  */
  YYSYMBOL_118_17 = 118,                   /* $@17  */
  YYSYMBOL_119_18 = 119,                   /* $@18  */
  YYSYMBOL_elif_list = 120,                /* elif_list  */
  YYSYMBOL_121_19 = 121,                   /* $@19  */
  YYSYMBOL_122_20 = 122,                   /* $@20  */
  YYSYMBOL_123_21 = 123,                   /* $@21  */
  YYSYMBOL_124_22 = 124,                   /* $@22  */
  YYSYMBOL_else_opt = 125,                 /* else_opt  */
  YYSYMBOL_126_23 = 126,                   /* $@23  */
  YYSYMBOL_127_24 = 127,                   /* $@24  */
  YYSYMBOL_128_25 = 128,                   /* $@25  */
  YYSYMBOL_129_26 = 129,                   /* $@26  */
  YYSYMBOL_130_27 = 130,                   /* $@27  */
  YYSYMBOL_break_stmt = 131,               /* break_stmt  */
  YYSYMBOL_continue_stmt = 132,            /* continue_stmt  */
  YYSYMBOL_snapshot_stmt = 133,            /* snapshot_stmt  */
  YYSYMBOL_134_28 = 134,                   /* $@28  */
  YYSYMBOL_135_29 = 135,                   /* $@29  */
  YYSYMBOL_snapshot_stmt_list = 136,       /* snapshot_stmt_list  */
  YYSYMBOL_rewind_stmt = 137,              /* rewind_stmt  */
  YYSYMBOL_snapshot_var_list = 138,        /* snapshot_var_list  */
  YYSYMBOL_for_stmt = 139,                 /* for_stmt  */
  YYSYMBOL_140_30 = 140,                   /* $@30  */
  YYSYMBOL_for_header = 141,               /* for_header  */
  YYSYMBOL_142_31 = 142,                   /* $@31  */
  YYSYMBOL_143_32 = 143,                   /* $@32  */
  YYSYMBOL_144_33 = 144,                   /* $@33  */
  YYSYMBOL_for_init_opt = 145,             /* for_init_opt  */
  YYSYMBOL_for_cond_opt = 146,             /* for_cond_opt  */
  YYSYMBOL_for_inc_opt = 147,              /* for_inc_opt  */
  YYSYMBOL_var_decl_no_semi = 148,         /* var_decl_no_semi  */
  YYSYMBOL_io_stmt = 149                   /* io_stmt  */
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
#define YYLAST   675

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  89
/* YYNRULES -- Number of rules.  */
#define YYNRULES  193
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  404

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   315


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
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   172,   172,   173,   177,   178,   179,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   206,   205,   280,   279,   319,   318,   329,   330,   334,
     335,   336,   340,   339,   429,   428,   441,   440,   534,   533,
     616,   615,   626,   640,   656,   657,   661,   708,   784,   785,
     789,   796,   806,   805,   815,   816,   824,   825,   830,   837,
     861,   868,   880,   887,   894,   895,   896,   897,   898,   899,
     903,   921,   939,   938,   983,   982,  1023,  1027,  1031,  1037,
    1046,  1059,  1077,  1076,  1166,  1165,  1252,  1251,  1262,  1263,
    1267,  1268,  1272,  1273,  1278,  1299,  1322,  1347,  1360,  1368,
    1369,  1373,  1376,  1388,  1409,  1423,  1436,  1450,  1463,  1480,
    1482,  1509,  1539,  1543,  1545,  1547,  1549,  1553,  1555,  1557,
    1561,  1566,  1570,  1574,  1578,  1585,  1591,  1595,  1601,  1607,
    1613,  1617,  1624,  1706,  1736,  1811,  1835,  1836,  1837,  1838,
    1839,  1840,  1841,  1842,  1847,  1864,  1866,  1871,  1846,  1875,
    1884,  1901,  1903,  1908,  1883,  1910,  1915,  1914,  1927,  1944,
    1946,  1951,  1926,  1953,  1957,  1968,  1988,  2003,  1987,  2016,
    2018,  2023,  2030,  2037,  2045,  2044,  2071,  2080,  2089,  2096,
    2079,  2101,  2102,  2103,  2107,  2108,  2112,  2113,  2117,  2121,
    2136,  2146,  2155,  2157
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
  "\"end of file\"", "error", "\"invalid token\"", "SNAPSHOT", "REWIND",
  "INT", "FP", "CHR", "STRING", "BOOL", "VOID", "IF", "ELIF", "ELSE",
  "FOR", "TRUE", "FALSE", "FEED", "SHOW", "RETURN", "BREAK", "CONTINUE",
  "SEQ1", "SEQ2", "FUNC", "ENTITY", "NEW", "PUBLIC", "PRIVATE", "THIS",
  "DOT", "EXTENDS", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET",
  "RBRACKET", "SEMICOLON", "COMMA", "IDENTIFIER", "STRING_LITERAL",
  "INT_LITERAL", "FLOAT_LITERAL", "CHAR_LITERAL", "ASSIGN", "ADD_ASSIGN",
  "SUB_ASSIGN", "OR", "AND", "BITOR", "BITAND", "EQ", "GT", "LT", "PLUS",
  "MINUS", "MUL", "DIV", "MOD", "NOT", "$accept", "program", "element",
  "statement", "entity_decl", "$@1", "$@2", "$@3", "entity_body",
  "entity_member", "constructor_decl", "$@4", "$@5", "method_decl", "$@6",
  "$@7", "$@8", "access_var_decl", "access_modifier", "object_decl",
  "arg_list_opt", "arg_list", "block", "$@9", "stmt_list", "var_decl",
  "id_list", "type", "array_decl", "$@10", "$@11", "array_init",
  "array_init2d", "row_list", "expr_list", "function_decl", "$@12", "$@13",
  "$@14", "func_type", "param_list_opt", "param_list", "param",
  "return_stmt", "expr_stmt", "expression", "indexed_id", "assignment",
  "logic_expr", "bitwise_expr", "rel_expr", "arith_expr", "term", "factor",
  "if_stmt", "$@15", "$@16", "$@17", "$@18", "elif_list", "$@19", "$@20",
  "$@21", "$@22", "else_opt", "$@23", "$@24", "$@25", "$@26", "$@27",
  "break_stmt", "continue_stmt", "snapshot_stmt", "$@28", "$@29",
  "snapshot_stmt_list", "rewind_stmt", "snapshot_var_list", "for_stmt",
  "$@30", "for_header", "$@31", "$@32", "$@33", "for_init_opt",
  "for_cond_opt", "for_inc_opt", "var_decl_no_semi", "io_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-371)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-175)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -371,   178,  -371,   -26,   -15,   -13,  -371,  -371,  -371,  -371,
    -371,  -371,    14,    67,  -371,  -371,    82,     9,    19,    89,
      96,   108,   608,   320,  -371,    83,  -371,  -371,  -371,  -371,
     263,   615,  -371,  -371,  -371,  -371,  -371,  -371,    -8,  -371,
    -371,   119,  -371,  -371,   127,   116,  -371,    34,    58,  -371,
     121,   147,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,   131,   608,   110,   256,   608,   142,  -371,   -10,   153,
    -371,  -371,   164,   169,   182,   176,  -371,   494,   177,   191,
     608,   608,   190,   541,   608,   608,   107,   206,    45,  -371,
    -371,  -371,   197,   199,   195,   163,   205,  -371,   608,   615,
     615,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,  -371,     5,   214,   216,   608,   220,   536,    88,  -371,
     608,  -371,   219,   226,   236,   228,  -371,  -371,  -371,   244,
    -371,   378,    -8,  -371,   230,   248,   243,  -371,   249,   573,
     261,  -371,  -371,  -371,  -371,   262,   270,   271,   608,    23,
      25,   578,  -371,  -371,  -371,   252,  -371,   264,  -371,   168,
     168,   168,   147,   147,  -371,  -371,  -371,   273,  -371,   276,
     283,  -371,   283,   278,  -371,   283,   284,  -371,   295,   608,
    -371,  -371,   336,   608,  -371,  -371,   316,   608,  -371,   608,
      26,   310,   313,   312,  -371,  -371,  -371,   322,   315,  -371,
     323,  -371,   334,   222,   329,   339,   340,  -371,   335,  -371,
    -371,   341,  -371,   328,  -371,  -371,  -371,   331,   344,    39,
     355,  -371,   150,  -371,  -371,   608,   608,   343,  -371,   358,
     359,   363,   367,   365,   369,   364,   366,  -371,   150,   403,
    -371,  -371,   608,   608,   375,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,   337,  -371,   371,   374,   376,   384,
    -371,   380,  -371,   608,   377,   608,   386,   608,   387,   390,
     393,   391,   395,    -3,   397,   398,   494,   283,  -371,  -371,
     394,   410,    77,   401,   404,    -6,    16,  -371,  -371,   283,
     150,   408,   402,  -371,   412,  -371,    41,  -371,  -371,   416,
     608,    81,  -371,   421,   608,   283,   283,  -371,   436,  -371,
    -371,  -371,  -371,   150,   459,   422,   423,   424,   425,  -371,
    -371,   608,   428,  -371,  -371,   608,   430,    98,  -371,   435,
    -371,   439,  -371,  -371,  -371,  -371,   461,   608,   442,   448,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,   444,  -371,   608,
     445,   452,   472,  -371,  -371,   283,   283,   454,   455,   456,
    -371,   109,  -371,  -371,   478,  -371,   457,  -371,  -371,   150,
     150,   490,  -371,   608,   462,   283,  -371,   460,   473,   474,
    -371,  -371,  -371,   283,   283,   283,   483,   608,  -371,  -371,
    -371,  -371,  -371,   283,   484,  -371,  -371,   461,   283,  -371,
    -371,   461,   472,  -371
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,     0,     0,    64,    65,    66,    67,
      68,    69,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,     0,     0,   100,   135,   140,   137,   138,   139,
       0,     0,     2,     4,     6,    18,    19,     7,    88,     8,
       5,     0,    13,     9,     0,   136,   101,   112,   116,   119,
     123,   126,   130,    10,    14,    15,    16,    17,    11,    12,
      20,     0,     0,     0,     0,     0,     0,    97,   135,     0,
     164,   165,    23,     0,     0,     0,    54,     0,     0,     0,
      49,     0,    63,     0,     0,     0,     0,     0,   135,   136,
     131,   115,     0,     0,    63,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,     0,     0,     0,     0,     0,   183,     0,    98,
       0,    96,     0,     0,     0,   133,   143,    55,    69,   135,
      57,     0,     0,    84,   134,     0,    48,    51,     0,     0,
       0,   104,   105,   106,    60,     0,     0,     0,     0,     0,
       0,     0,    58,    82,   109,   113,   114,   118,   117,   122,
     120,   121,   124,   125,   127,   128,   129,     0,   166,     0,
       0,   145,     0,     0,   182,     0,     0,   181,     0,     0,
      21,    28,     0,     0,    53,    56,     0,     0,   132,     0,
     102,     0,     0,     0,    62,   133,   134,     0,     0,    72,
       0,    74,     0,   135,     0,     0,     0,   172,     0,   171,
     149,     0,   176,   188,   175,   177,   192,     0,     0,     0,
       0,   110,    91,   111,    50,     0,     0,     0,    61,     0,
     102,     0,     0,     0,     0,     0,     0,    59,    91,     0,
     167,   146,     0,   185,     0,    28,    44,    45,    24,    32,
      27,    29,    30,    31,     0,    26,     0,     0,     0,    90,
      93,     0,   107,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   134,     0,     0,     0,     0,   189,   184,
       0,     0,     0,     0,     0,     0,     0,    95,    94,     0,
       0,   103,     0,   190,     0,    70,     0,    81,    73,     0,
       0,     0,    75,     0,    49,     0,     0,   170,     0,   147,
     178,   193,    22,    91,     0,     0,     0,     0,     0,    85,
      92,     0,     0,   103,    76,     0,     0,     0,    77,     0,
     191,     0,    83,    87,   168,   169,   155,   187,     0,     0,
      38,    43,    36,    42,   108,    46,    80,     0,    79,     0,
       0,     0,   163,   186,   179,     0,     0,     0,     0,     0,
      71,     0,    47,   150,   156,   148,     0,    33,    35,    91,
      91,     0,    78,     0,     0,     0,   180,     0,     0,     0,
     151,   158,   157,     0,     0,     0,     0,     0,    39,    37,
      41,   152,   159,     0,     0,   153,   160,   155,     0,   154,
     161,   155,   163,   162
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -371,  -371,  -371,   -76,  -371,  -371,  -371,  -371,   266,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -250,  -371,  -142,  -371,  -371,  -371,   -35,     1,  -371,  -371,
    -371,  -371,  -371,  -371,  -289,  -371,  -371,  -371,  -371,  -371,
    -229,  -371,   229,  -371,  -371,   -17,   -14,   -77,   -27,   126,
    -371,    64,   123,    -7,  -371,  -371,  -371,  -371,  -371,  -370,
    -371,  -371,  -371,  -371,   118,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    32,    33,    34,   218,   123,   124,   219,   250,
     251,   283,   284,   252,   358,   357,   359,   253,   254,    35,
     135,   136,    36,    77,   131,    37,    86,   257,    39,   232,
     234,   268,   271,   301,   296,    40,   205,   186,   206,    41,
     258,   259,   260,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,   115,   211,   277,   336,   352,
     373,   386,   393,   397,   365,   375,   387,   394,   398,   401,
      54,    55,    56,   208,   276,   308,    57,   112,    58,   117,
     175,   243,   337,   366,   176,   280,   354,   177,    59
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      69,   130,    38,    95,    91,    74,   141,   142,   143,   274,
      66,   327,    60,   292,    92,    93,    89,    89,   315,    62,
      79,   154,    80,    90,    14,    15,    81,   399,   210,   304,
     212,   402,    94,   214,   316,   120,    84,    85,    21,    61,
     317,    22,   187,   141,   167,   113,    63,    67,   118,    68,
      26,    27,    28,    29,   331,   185,   318,    70,   168,   198,
     361,   200,   225,   137,   138,    30,   246,   247,   199,    31,
     201,   226,   155,   156,   248,   147,   324,    80,   132,   249,
     325,   148,    99,   100,   338,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    95,   171,    64,
     174,   164,   165,   166,   246,   247,   221,    78,   101,   102,
     223,   114,   312,    79,    65,    80,   328,   249,   173,    81,
     329,   178,   192,    82,   179,  -144,  -144,    71,    83,    84,
      85,   197,   132,   348,   204,   309,    72,   325,    73,  -144,
     377,   378,  -144,    96,   372,   144,   145,   319,   325,   262,
    -144,  -144,  -144,  -144,  -144,     6,     7,     8,     9,    10,
     128,    98,   217,   332,   333,    97,  -144,   159,   160,   161,
    -144,   111,   224,   103,   104,   105,   106,   107,     2,     3,
     119,     4,     5,     6,     7,     8,     9,    10,    11,    12,
     256,   121,    13,    14,    15,   122,    16,    17,    18,    19,
     307,   152,   145,    20,   108,   109,   110,    21,   261,   125,
      22,   127,    23,   367,   368,   126,    24,   133,    25,    26,
      27,    28,    29,   106,   107,   278,   279,   157,   158,   162,
     163,   134,   335,   382,    30,   139,   146,   149,    31,   150,
     151,   388,   389,   390,   344,   153,   137,   169,   294,   170,
     297,   395,   236,   172,    80,   286,   400,   116,    81,   180,
     181,  -174,  -174,  -174,  -174,  -174,  -174,   120,    84,    85,
     182,  -174,  -174,   183,    79,   187,    80,   132,    14,    15,
      81,   188,   189,   297,    82,  -174,   190,   137,  -174,    83,
      84,    85,    87,   193,  -174,    22,  -174,  -174,  -174,  -174,
    -174,   100,   194,    88,    26,    27,    28,    29,   346,   132,
     195,   196,  -174,   207,   209,   102,  -174,    23,   213,    30,
     353,    75,   215,   -52,   -52,   -52,   -52,   -52,   -52,   -52,
     -52,   -52,   297,   216,   -52,   -52,   -52,   220,   -52,   -52,
     -52,   -52,     6,     7,     8,     9,    10,   128,   222,   -52,
     227,   228,   -52,   229,   -52,    76,   380,   231,   -52,   230,
     -52,   -52,   -52,   -52,   -52,   233,   235,   237,   244,   240,
     392,   238,   239,   242,   241,   263,   -52,   285,   245,     3,
     -52,     4,     5,     6,     7,     8,     9,    10,   128,    12,
     255,   264,    13,    14,    15,   265,    16,    17,    18,    19,
     266,   267,   269,   270,   275,   272,   273,    21,   281,   289,
      22,   287,    23,   184,   288,   293,    24,   291,   129,    26,
      27,    28,    29,   290,   295,   298,   299,   300,   303,   302,
     305,   306,   310,   313,    30,   322,   314,     3,    31,     4,
       5,     6,     7,     8,     9,    10,   128,    12,   311,   323,
      13,    14,    15,   321,    16,    17,    18,    19,   326,   330,
     339,   341,   340,   343,   342,    21,   345,   347,    22,   349,
      23,   334,   350,   351,    24,   355,   129,    26,    27,    28,
      29,   356,   360,   362,   363,   364,   369,   370,   371,   374,
     376,   379,    30,   383,   381,     3,    31,     4,     5,     6,
       7,     8,     9,    10,   128,    12,   384,   385,    13,    14,
      15,   282,    16,    17,    18,    19,   391,   396,     0,   320,
     403,     0,     0,    21,     0,     0,    22,     0,    23,     0,
       0,     0,    24,     0,   129,    26,    27,    28,    29,     0,
       0,     6,     7,     8,     9,    10,   128,     0,     0,     0,
      30,    14,    15,     0,    31,     0,    14,    15,   140,     0,
       0,     0,     0,     0,     0,    21,     0,     0,    22,     0,
      21,     0,     0,    22,     0,     0,    68,    26,    27,    28,
      29,    68,    26,    27,    28,    29,     0,     0,    14,    15,
       0,     0,    30,    14,    15,   202,    31,    30,     0,   191,
       0,    31,    21,     0,     0,    22,     0,    21,     0,     0,
      22,     0,     0,    68,    26,    27,    28,    29,   203,    26,
      27,    28,    29,    14,    15,     0,     0,     0,     0,    30,
      14,    15,     0,    31,    30,     0,     0,    21,    31,     0,
      22,     0,     0,     0,    87,     0,     0,    22,    68,    26,
      27,    28,    29,     0,     0,    88,    26,    27,    28,    29,
       0,     0,     0,     0,    30,     0,     0,     0,    31,     0,
       0,    30,     0,     0,     0,    31
};

static const yytype_int16 yycheck[] =
{
      17,    77,     1,    38,    31,    22,    83,    84,    85,   238,
       1,   300,    38,   263,    22,    23,    30,    31,    24,    32,
      30,    98,    32,    30,    15,    16,    36,   397,   170,    32,
     172,   401,    40,   175,    40,    45,    46,    47,    29,    54,
      24,    32,    45,   120,    39,    62,    32,    38,    65,    40,
      41,    42,    43,    44,   304,   131,    40,    38,    53,    36,
     349,    36,    36,    80,    81,    56,    27,    28,    45,    60,
      45,    45,    99,   100,    35,    30,    35,    32,    77,    40,
      39,    36,    48,    49,   313,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   132,   115,    32,
     117,   108,   109,   110,    27,    28,   183,    24,    50,    51,
     187,     1,    35,    30,    32,    32,    35,    40,   117,    36,
      39,    33,   139,    40,    36,    15,    16,    38,    45,    46,
      47,   148,   131,    35,   151,   277,    40,    39,    30,    29,
     369,   370,    32,    24,    35,    38,    39,   289,    39,   226,
      40,    41,    42,    43,    44,     5,     6,     7,     8,     9,
      10,    45,   179,   305,   306,    38,    56,   103,   104,   105,
      60,    40,   189,    52,    53,    54,    55,    56,     0,     1,
      38,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      40,    38,    14,    15,    16,    31,    18,    19,    20,    21,
     276,    38,    39,    25,    57,    58,    59,    29,   225,    40,
      32,    35,    34,   355,   356,    33,    38,    40,    40,    41,
      42,    43,    44,    55,    56,   242,   243,   101,   102,   106,
     107,    40,   308,   375,    56,    45,    30,    40,    60,    40,
      45,   383,   384,   385,   321,    40,   263,    33,   265,    33,
     267,   393,    30,    33,    32,   254,   398,     1,    36,    40,
      34,     5,     6,     7,     8,     9,    10,    45,    46,    47,
      34,    15,    16,    45,    30,    45,    32,   276,    15,    16,
      36,    33,    39,   300,    40,    29,    37,   304,    32,    45,
      46,    47,    29,    32,    38,    32,    40,    41,    42,    43,
      44,    49,    40,    40,    41,    42,    43,    44,   325,   308,
      40,    40,    56,    40,    38,    51,    60,    34,    40,    56,
     337,     1,    38,     3,     4,     5,     6,     7,     8,     9,
      10,    11,   349,    38,    14,    15,    16,     1,    18,    19,
      20,    21,     5,     6,     7,     8,     9,    10,    32,    29,
      40,    38,    32,    41,    34,    35,   373,    42,    38,    37,
      40,    41,    42,    43,    44,    42,    32,    38,    37,    34,
     387,    32,    32,    45,    33,    32,    56,    40,    34,     1,
      60,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      35,    33,    14,    15,    16,    36,    18,    19,    20,    21,
      37,    34,    37,    34,     1,    41,    40,    29,    33,    33,
      32,    40,    34,    35,    40,    38,    38,    37,    40,    41,
      42,    43,    44,    39,    38,    38,    36,    34,    33,    38,
      33,    33,    38,    32,    56,    33,    32,     1,    60,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    38,    37,
      14,    15,    16,    45,    18,    19,    20,    21,    42,    38,
       1,    38,    40,    38,    40,    29,    38,    37,    32,    34,
      34,    35,    33,    12,    38,    33,    40,    41,    42,    43,
      44,    33,    38,    38,    32,    13,    32,    32,    32,    11,
      33,     1,    56,    33,    32,     1,    60,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    33,    33,    14,    15,
      16,   245,    18,    19,    20,    21,    33,    33,    -1,   290,
     402,    -1,    -1,    29,    -1,    -1,    32,    -1,    34,    -1,
      -1,    -1,    38,    -1,    40,    41,    42,    43,    44,    -1,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      56,    15,    16,    -1,    60,    -1,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    32,    -1,
      29,    -1,    -1,    32,    -1,    -1,    40,    41,    42,    43,
      44,    40,    41,    42,    43,    44,    -1,    -1,    15,    16,
      -1,    -1,    56,    15,    16,    17,    60,    56,    -1,    26,
      -1,    60,    29,    -1,    -1,    32,    -1,    29,    -1,    -1,
      32,    -1,    -1,    40,    41,    42,    43,    44,    40,    41,
      42,    43,    44,    15,    16,    -1,    -1,    -1,    -1,    56,
      15,    16,    -1,    60,    56,    -1,    -1,    29,    60,    -1,
      32,    -1,    -1,    -1,    29,    -1,    -1,    32,    40,    41,
      42,    43,    44,    -1,    -1,    40,    41,    42,    43,    44,
      -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    60,    -1,
      -1,    56,    -1,    -1,    -1,    60
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    62,     0,     1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    14,    15,    16,    18,    19,    20,    21,
      25,    29,    32,    34,    38,    40,    41,    42,    43,    44,
      56,    60,    63,    64,    65,    80,    83,    86,    88,    89,
      96,   100,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   131,   132,   133,   137,   139,   149,
      38,    54,    32,    32,    32,    32,     1,    38,    40,   106,
      38,    38,    40,    30,   106,     1,    35,    84,    24,    30,
      32,    36,    40,    45,    46,    47,    87,    29,    40,   107,
     114,   109,    22,    23,    40,    87,    24,    38,    45,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    40,   138,   106,     1,   116,     1,   140,   106,    38,
      45,    38,    31,    67,    68,    40,    33,    35,    10,    40,
      64,    85,    88,    40,    40,    81,    82,   106,   106,    45,
      17,   108,   108,   108,    38,    39,    30,    30,    36,    40,
      40,    45,    38,    40,   108,   109,   109,   110,   110,   112,
     112,   112,   113,   113,   114,   114,   114,    39,    53,    33,
      33,   106,    33,    88,   106,   141,   145,   148,    33,    36,
      40,    34,    34,    45,    35,    64,    98,    45,    33,    39,
      37,    26,   106,    32,    40,    40,    40,   106,    36,    45,
      36,    45,    17,    40,   106,    97,    99,    40,   134,    38,
      83,   117,    83,    40,    83,    38,    38,   106,    66,    69,
       1,   108,    32,   108,   106,    36,    45,    40,    38,    41,
      37,    42,    90,    42,    91,    32,    30,    38,    32,    32,
      34,    33,    45,   142,    37,    34,    27,    28,    35,    40,
      70,    71,    74,    78,    79,    35,    40,    88,   101,   102,
     103,   106,   108,    32,    33,    36,    37,    34,    92,    37,
      34,    93,    41,    40,   101,     1,   135,   118,   106,   106,
     146,    33,    69,    72,    73,    40,    88,    40,    40,    33,
      39,    37,    81,    38,   106,    38,    95,   106,    38,    36,
      34,    94,    38,    33,    32,    33,    33,    64,   136,    83,
      38,    38,    35,    32,    32,    24,    40,    24,    40,    83,
     103,    45,    33,    37,    35,    39,    42,    95,    35,    39,
      38,    81,    83,    83,    35,    64,   119,   143,   101,     1,
      40,    38,    40,    38,   108,    38,   106,    37,    35,    34,
      33,    12,   120,   106,   147,    33,    33,    76,    75,    77,
      38,    95,    38,    32,    13,   125,   144,    83,    83,    32,
      32,    32,    35,   121,    11,   126,    33,   101,   101,     1,
     106,    32,    83,    33,    33,    33,   122,   127,    83,    83,
      83,    33,   106,   123,   128,    83,    33,   124,   129,   120,
      83,   130,   120,   125
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    62,    63,    63,    63,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    66,    65,    67,    65,    68,    65,    69,    69,    70,
      70,    70,    72,    71,    73,    71,    75,    74,    76,    74,
      77,    74,    78,    78,    79,    79,    80,    80,    81,    81,
      82,    82,    84,    83,    83,    83,    85,    85,    86,    86,
      86,    86,    87,    87,    88,    88,    88,    88,    88,    88,
      89,    89,    90,    89,    91,    89,    92,    93,    94,    94,
      95,    95,    97,    96,    98,    96,    99,    96,   100,   100,
     101,   101,   102,   102,   103,   103,   104,   104,   104,   105,
     105,   106,   107,   107,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   109,   109,   109,   109,   110,   110,   110,
     111,   111,   111,   111,   112,   112,   112,   113,   113,   113,
     113,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   116,   117,   118,   119,   115,   115,
     121,   122,   123,   124,   120,   120,   126,   125,   127,   128,
     129,   130,   125,   125,   131,   132,   134,   135,   133,   136,
     136,   137,   138,   138,   140,   139,   139,   142,   143,   144,
     141,   145,   145,   145,   146,   146,   147,   147,   148,   148,
     149,   149,   149,   149
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     0,     8,     0,     6,     0,     6,     2,     0,     1,
       1,     1,     0,     6,     0,     6,     0,     9,     0,     9,
       0,     9,     4,     4,     1,     1,     9,    10,     1,     0,
       3,     1,     0,     4,     2,     3,     2,     1,     3,     5,
       3,     5,     3,     1,     1,     1,     1,     1,     1,     1,
       7,    10,     0,     7,     0,     7,     3,     3,     5,     3,
       3,     1,     0,     8,     0,     8,     0,     8,     1,     1,
       1,     0,     3,     1,     2,     2,     3,     2,     3,     2,
       1,     1,     4,     7,     3,     3,     3,     6,     9,     3,
       5,     5,     1,     3,     3,     2,     1,     3,     3,     1,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     2,     4,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     0,     0,     0,    11,     5,
       0,     0,     0,     0,    10,     0,     0,     3,     0,     0,
       0,     0,    12,     0,     2,     2,     0,     0,     9,     2,
       1,     5,     3,     1,     0,     5,     5,     0,     0,     0,
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
  case 20: /* statement: error SEMICOLON  */
#line 197 "parser.y"
            {
                printf("Invalid statement at line %d\n", yylineno);
                yyerrok;
            }
#line 1853 "y.tab.c"
    break;

  case 21: /* $@1: %empty  */
#line 206 "parser.y"
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
#line 1917 "y.tab.c"
    break;

  case 22: /* entity_decl: ENTITY IDENTIFIER EXTENDS IDENTIFIER $@1 LBRACE entity_body RBRACE  */
#line 266 "parser.y"
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
#line 1934 "y.tab.c"
    break;

  case 23: /* $@2: %empty  */
#line 280 "parser.y"
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
#line 1958 "y.tab.c"
    break;

  case 24: /* entity_decl: ENTITY IDENTIFIER $@2 LBRACE entity_body RBRACE  */
#line 300 "parser.y"
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
#line 1981 "y.tab.c"
    break;

  case 25: /* $@3: %empty  */
#line 319 "parser.y"
            { emit("entity", (yyvsp[0].sval), "", ""); }
#line 1987 "y.tab.c"
    break;

  case 26: /* entity_decl: ENTITY IDENTIFIER $@3 LBRACE error RBRACE  */
#line 321 "parser.y"
            {
                printf("Invalid entity body at line %d\n", yylineno);
                yyerrok;
                emit("end_entity", (yyvsp[-4].sval), "", "");
            }
#line 1997 "y.tab.c"
    break;

  case 32: /* $@4: %empty  */
#line 340 "parser.y"
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
#line 2023 "y.tab.c"
    break;

  case 33: /* constructor_decl: IDENTIFIER $@4 LPAREN param_list_opt RPAREN block  */
#line 362 "parser.y"
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
#line 2094 "y.tab.c"
    break;

  case 34: /* $@5: %empty  */
#line 429 "parser.y"
            { emit("constr", (yyvsp[0].sval), "", ""); }
#line 2100 "y.tab.c"
    break;

  case 35: /* constructor_decl: IDENTIFIER $@5 LPAREN error RPAREN block  */
#line 431 "parser.y"
            {
                printf("Invalid constructor parameters at line %d\n", yylineno);
                yyerrok;
                emit("end_constr", (yyvsp[-5].sval), "", "");
            }
#line 2110 "y.tab.c"
    break;

  case 36: /* $@6: %empty  */
#line 441 "parser.y"
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
#line 2139 "y.tab.c"
    break;

  case 37: /* method_decl: access_modifier type FUNC IDENTIFIER $@6 LPAREN param_list_opt RPAREN block  */
#line 466 "parser.y"
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
#line 2209 "y.tab.c"
    break;

  case 38: /* $@7: %empty  */
#line 534 "parser.y"
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
#line 2234 "y.tab.c"
    break;

  case 39: /* method_decl: access_modifier IDENTIFIER FUNC IDENTIFIER $@7 LPAREN param_list_opt RPAREN block  */
#line 555 "parser.y"
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
#line 2299 "y.tab.c"
    break;

  case 40: /* $@8: %empty  */
#line 616 "parser.y"
            { emit("method", (yyvsp[0].sval), "", ""); }
#line 2305 "y.tab.c"
    break;

  case 41: /* method_decl: access_modifier type FUNC IDENTIFIER $@8 LPAREN error RPAREN block  */
#line 618 "parser.y"
            {
                printf("Invalid method parameters at line %d\n", yylineno);
                yyerrok;
                emit("end_method", (yyvsp[-5].sval), "", "");
            }
#line 2315 "y.tab.c"
    break;

  case 42: /* access_var_decl: access_modifier type IDENTIFIER SEMICOLON  */
#line 627 "parser.y"
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
#line 2332 "y.tab.c"
    break;

  case 43: /* access_var_decl: access_modifier IDENTIFIER IDENTIFIER SEMICOLON  */
#line 641 "parser.y"
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
#line 2349 "y.tab.c"
    break;

  case 44: /* access_modifier: PUBLIC  */
#line 656 "parser.y"
                  { (yyval.access) = ACC_PUBLIC;  }
#line 2355 "y.tab.c"
    break;

  case 45: /* access_modifier: PRIVATE  */
#line 657 "parser.y"
                  { (yyval.access) = ACC_PRIVATE; }
#line 2361 "y.tab.c"
    break;

  case 46: /* object_decl: IDENTIFIER IDENTIFIER ASSIGN NEW IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 662 "parser.y"
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
#line 2412 "y.tab.c"
    break;

  case 47: /* object_decl: type IDENTIFIER ASSIGN IDENTIFIER DOT IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON  */
#line 709 "parser.y"
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
#line 2489 "y.tab.c"
    break;

  case 49: /* arg_list_opt: %empty  */
#line 785 "parser.y"
                        { call_arg_count = 0; }
#line 2495 "y.tab.c"
    break;

  case 50: /* arg_list: arg_list COMMA expression  */
#line 790 "parser.y"
        {
            if(call_arg_count < 64){
                call_arg_types[call_arg_count++] = last_expr_type;
            } 
            emit("arg", (yyvsp[0].sval), "", ""); 
        }
#line 2506 "y.tab.c"
    break;

  case 51: /* arg_list: expression  */
#line 797 "parser.y"
        { 
            call_arg_count = 0;
            call_arg_types[call_arg_count++] = last_expr_type;
            emit("arg", (yyvsp[0].sval), "", ""); 
        }
#line 2516 "y.tab.c"
    break;

  case 52: /* $@9: %empty  */
#line 806 "parser.y"
            {
                SymTable* bs = create_scope(SCOPE_BLOCK, "block", current_scope);
                current_scope = bs;
            }
#line 2525 "y.tab.c"
    break;

  case 53: /* block: LBRACE $@9 stmt_list RBRACE  */
#line 811 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 2534 "y.tab.c"
    break;

  case 55: /* block: LBRACE error RBRACE  */
#line 817 "parser.y"
            {
                printf("Error inside block at line %d\n", yylineno);
                yyerrok;
            }
#line 2543 "y.tab.c"
    break;

  case 58: /* var_decl: type id_list SEMICOLON  */
#line 831 "parser.y"
            {
                insert_var_list((yyvsp[-1].sval), (yyvsp[-2].dtype));
                free((yyvsp[-1].sval));
            }
#line 2552 "y.tab.c"
    break;

  case 59: /* var_decl: type IDENTIFIER ASSIGN expression SEMICOLON  */
#line 838 "parser.y"
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
#line 2578 "y.tab.c"
    break;

  case 60: /* var_decl: IDENTIFIER id_list SEMICOLON  */
#line 862 "parser.y"
            {
                insert_var_list((yyvsp[-1].sval), DT_ENTITY);
                free((yyvsp[-1].sval));
            }
#line 2587 "y.tab.c"
    break;

  case 61: /* var_decl: IDENTIFIER IDENTIFIER ASSIGN expression SEMICOLON  */
#line 869 "parser.y"
            {
            fprintf(stderr, "ERROR line %d: cannot initialize entity variable '%s' with value of type %s.\n", yylineno, (yyvsp[-3].sval), dt_names[last_expr_type]);
            parse_error_count++;
		emit("=", (yyvsp[-1].sval), "", (yyvsp[-3].sval));
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-3].sval),
                            KIND_VAR, DT_ENTITY, yylineno);
            if (sym) sym->is_initialized = 1;
            }
#line 2600 "y.tab.c"
    break;

  case 62: /* id_list: id_list COMMA IDENTIFIER  */
#line 881 "parser.y"
            {
                char* buf = malloc(strlen((yyvsp[-2].sval)) + strlen((yyvsp[0].sval)) + 2);
                sprintf(buf, "%s,%s", (yyvsp[-2].sval), (yyvsp[0].sval));
                free((yyvsp[-2].sval));
                (yyval.sval) = buf;
            }
#line 2611 "y.tab.c"
    break;

  case 63: /* id_list: IDENTIFIER  */
#line 888 "parser.y"
            {
                (yyval.sval) = strdup((yyvsp[0].sval));
            }
#line 2619 "y.tab.c"
    break;

  case 64: /* type: INT  */
#line 894 "parser.y"
                 { (yyval.dtype) = DT_INT;    }
#line 2625 "y.tab.c"
    break;

  case 65: /* type: FP  */
#line 895 "parser.y"
                 { (yyval.dtype) = DT_FLOAT;  }
#line 2631 "y.tab.c"
    break;

  case 66: /* type: CHR  */
#line 896 "parser.y"
                 { (yyval.dtype) = DT_CHAR;   }
#line 2637 "y.tab.c"
    break;

  case 67: /* type: STRING  */
#line 897 "parser.y"
                 { (yyval.dtype) = DT_STRING; }
#line 2643 "y.tab.c"
    break;

  case 68: /* type: BOOL  */
#line 898 "parser.y"
                 { (yyval.dtype) = DT_BOOL;   }
#line 2649 "y.tab.c"
    break;

  case 69: /* type: VOID  */
#line 899 "parser.y"
                 { (yyval.dtype) = DT_VOID;   }
#line 2655 "y.tab.c"
    break;

  case 70: /* array_decl: type SEQ1 IDENTIFIER LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 904 "parser.y"
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
#line 2675 "y.tab.c"
    break;

  case 71: /* array_decl: type SEQ2 IDENTIFIER LBRACKET INT_LITERAL RBRACKET LBRACKET INT_LITERAL RBRACKET SEMICOLON  */
#line 923 "parser.y"
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
#line 2692 "y.tab.c"
    break;

  case 72: /* $@10: %empty  */
#line 939 "parser.y"
        {
            current_array_elem_type = (yyvsp[-3].dtype);
            array_type_errors = 0;
        }
#line 2701 "y.tab.c"
    break;

  case 73: /* array_decl: type SEQ1 IDENTIFIER ASSIGN $@10 array_init SEMICOLON  */
#line 944 "parser.y"
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
#line 2744 "y.tab.c"
    break;

  case 74: /* $@11: %empty  */
#line 983 "parser.y"
        {
            current_array_elem_type = (yyvsp[-3].dtype);
            array_type_errors = 0;
            arr2d_rows = 0;
            arr2d_cols = 0;
        }
#line 2755 "y.tab.c"
    break;

  case 75: /* array_decl: type SEQ2 IDENTIFIER ASSIGN $@11 array_init2d SEMICOLON  */
#line 990 "parser.y"
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
#line 2790 "y.tab.c"
    break;

  case 76: /* array_init: LBRACE expr_list RBRACE  */
#line 1023 "parser.y"
                                  { (yyval.ival) = (yyvsp[-1].ival); }
#line 2796 "y.tab.c"
    break;

  case 77: /* array_init2d: LBRACE row_list RBRACE  */
#line 1027 "parser.y"
                                   { (yyval.ival) = (yyvsp[-1].ival); }
#line 2802 "y.tab.c"
    break;

  case 78: /* row_list: row_list COMMA LBRACE expr_list RBRACE  */
#line 1032 "parser.y"
        { 
            arr2d_rows++;
            arr2d_cols = (yyvsp[-1].ival);
            (yyval.ival) = (yyvsp[-4].ival) + (yyvsp[-1].ival); 
        }
#line 2812 "y.tab.c"
    break;

  case 79: /* row_list: LBRACE expr_list RBRACE  */
#line 1038 "parser.y"
        { 
            arr2d_rows = 1;
            arr2d_cols = (yyvsp[-1].ival);
            (yyval.ival) = (yyvsp[-1].ival); 
        }
#line 2822 "y.tab.c"
    break;

  case 80: /* expr_list: expr_list COMMA expression  */
#line 1047 "parser.y"
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
#line 2839 "y.tab.c"
    break;

  case 81: /* expr_list: expression  */
#line 1060 "parser.y"
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
#line 2857 "y.tab.c"
    break;

  case 82: /* $@12: %empty  */
#line 1077 "parser.y"
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
#line 2884 "y.tab.c"
    break;

  case 83: /* function_decl: func_type FUNC IDENTIFIER $@12 LPAREN param_list_opt RPAREN block  */
#line 1100 "parser.y"
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
#line 2953 "y.tab.c"
    break;

  case 84: /* $@13: %empty  */
#line 1166 "parser.y"
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
#line 2979 "y.tab.c"
    break;

  case 85: /* function_decl: IDENTIFIER FUNC IDENTIFIER $@13 LPAREN param_list_opt RPAREN block  */
#line 1188 "parser.y"
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
#line 3046 "y.tab.c"
    break;

  case 86: /* $@14: %empty  */
#line 1252 "parser.y"
            { emit("func", (yyvsp[0].sval), "", ""); }
#line 3052 "y.tab.c"
    break;

  case 87: /* function_decl: func_type FUNC IDENTIFIER $@14 LPAREN error RPAREN block  */
#line 1254 "parser.y"
            {
                printf("Invalid parameter list at line %d\n", yylineno);
                yyerrok;
                emit("endfunc", "", "", "");
            }
#line 3062 "y.tab.c"
    break;

  case 88: /* func_type: type  */
#line 1262 "parser.y"
               { (yyval.dtype) = (yyvsp[0].dtype);      }
#line 3068 "y.tab.c"
    break;

  case 89: /* func_type: VOID  */
#line 1263 "parser.y"
               { (yyval.dtype) = DT_VOID; }
#line 3074 "y.tab.c"
    break;

  case 94: /* param: type IDENTIFIER  */
#line 1279 "parser.y"
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
#line 3098 "y.tab.c"
    break;

  case 95: /* param: IDENTIFIER IDENTIFIER  */
#line 1300 "parser.y"
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
#line 3122 "y.tab.c"
    break;

  case 96: /* return_stmt: RETURN expression SEMICOLON  */
#line 1323 "parser.y"
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
#line 3151 "y.tab.c"
    break;

  case 97: /* return_stmt: RETURN SEMICOLON  */
#line 1348 "parser.y"
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
#line 3167 "y.tab.c"
    break;

  case 98: /* return_stmt: RETURN error SEMICOLON  */
#line 1361 "parser.y"
            {
                printf("Invalid return statement at line %d\n", yylineno);
                yyerrok;
            }
#line 3176 "y.tab.c"
    break;

  case 101: /* expression: assignment  */
#line 1373 "parser.y"
                     { (yyval.sval) = (yyvsp[0].sval); }
#line 3182 "y.tab.c"
    break;

  case 102: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET  */
#line 1377 "parser.y"
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
#line 3198 "y.tab.c"
    break;

  case 103: /* indexed_id: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET  */
#line 1389 "parser.y"
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
#line 3220 "y.tab.c"
    break;

  case 104: /* assignment: IDENTIFIER ASSIGN assignment  */
#line 1410 "parser.y"
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
#line 3238 "y.tab.c"
    break;

  case 105: /* assignment: IDENTIFIER ADD_ASSIGN assignment  */
#line 1424 "parser.y"
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
#line 3255 "y.tab.c"
    break;

  case 106: /* assignment: IDENTIFIER SUB_ASSIGN assignment  */
#line 1437 "parser.y"
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
#line 3273 "y.tab.c"
    break;

  case 107: /* assignment: IDENTIFIER LBRACKET expression RBRACKET ASSIGN assignment  */
#line 1451 "parser.y"
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
#line 3290 "y.tab.c"
    break;

  case 108: /* assignment: IDENTIFIER LBRACKET expression RBRACKET LBRACKET expression RBRACKET ASSIGN assignment  */
#line 1464 "parser.y"
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
#line 3311 "y.tab.c"
    break;

  case 109: /* assignment: indexed_id ASSIGN assignment  */
#line 1481 "parser.y"
            { (yyval.sval) = (yyvsp[0].sval); }
#line 3317 "y.tab.c"
    break;

  case 110: /* assignment: THIS DOT IDENTIFIER ASSIGN assignment  */
#line 1483 "parser.y"
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
#line 3348 "y.tab.c"
    break;

  case 111: /* assignment: IDENTIFIER DOT IDENTIFIER ASSIGN assignment  */
#line 1510 "parser.y"
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
#line 3382 "y.tab.c"
    break;

  case 112: /* assignment: logic_expr  */
#line 1539 "parser.y"
                     { (yyval.sval) = (yyvsp[0].sval); }
#line 3388 "y.tab.c"
    break;

  case 113: /* logic_expr: logic_expr OR logic_expr  */
#line 1544 "parser.y"
            { char* t = genVar(); emit("||", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3394 "y.tab.c"
    break;

  case 114: /* logic_expr: logic_expr AND logic_expr  */
#line 1546 "parser.y"
            { char* t = genVar(); emit("&&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3400 "y.tab.c"
    break;

  case 115: /* logic_expr: NOT logic_expr  */
#line 1548 "parser.y"
            { char* t = genVar(); emit("!", (yyvsp[0].sval), "", t); (yyval.sval) = t; }
#line 3406 "y.tab.c"
    break;

  case 116: /* logic_expr: bitwise_expr  */
#line 1549 "parser.y"
                       { (yyval.sval) = (yyvsp[0].sval); }
#line 3412 "y.tab.c"
    break;

  case 117: /* bitwise_expr: bitwise_expr BITAND bitwise_expr  */
#line 1554 "parser.y"
            { char* t = genVar(); emit("&", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3418 "y.tab.c"
    break;

  case 118: /* bitwise_expr: bitwise_expr BITOR bitwise_expr  */
#line 1556 "parser.y"
            { char* t = genVar(); emit("|", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t; }
#line 3424 "y.tab.c"
    break;

  case 119: /* bitwise_expr: rel_expr  */
#line 1557 "parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 3430 "y.tab.c"
    break;

  case 120: /* rel_expr: arith_expr GT arith_expr  */
#line 1562 "parser.y"
            {
            char* t = genVar(); emit(">", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
                
            }
#line 3439 "y.tab.c"
    break;

  case 121: /* rel_expr: arith_expr LT arith_expr  */
#line 1567 "parser.y"
            {
                    char* t = genVar(); emit("<", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
            }
#line 3447 "y.tab.c"
    break;

  case 122: /* rel_expr: arith_expr EQ arith_expr  */
#line 1571 "parser.y"
            {
                    char* t = genVar(); emit("==", (yyvsp[-2].sval), (yyvsp[0].sval), t); (yyval.sval) = t;
            }
#line 3455 "y.tab.c"
    break;

  case 123: /* rel_expr: arith_expr  */
#line 1574 "parser.y"
                     { (yyval.sval) = (yyvsp[0].sval); }
#line 3461 "y.tab.c"
    break;

  case 124: /* arith_expr: arith_expr PLUS term  */
#line 1579 "parser.y"
        {
                    char* t = genVar();
                    emit("+", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
                
            }
#line 3472 "y.tab.c"
    break;

  case 125: /* arith_expr: arith_expr MINUS term  */
#line 1586 "parser.y"
            {  
                    char* t = genVar();
                    emit("-", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
            }
#line 3482 "y.tab.c"
    break;

  case 126: /* arith_expr: term  */
#line 1591 "parser.y"
               { (yyval.sval) = (yyvsp[0].sval); }
#line 3488 "y.tab.c"
    break;

  case 127: /* term: term MUL factor  */
#line 1596 "parser.y"
            {
                    char* t = genVar();
                    emit("*", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
            }
#line 3498 "y.tab.c"
    break;

  case 128: /* term: term DIV factor  */
#line 1602 "parser.y"
            {
                    char* t = genVar();
                    emit("/", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
            }
#line 3508 "y.tab.c"
    break;

  case 129: /* term: term MOD factor  */
#line 1608 "parser.y"
            {
                    char* t = genVar();
                    emit("%", (yyvsp[-2].sval), (yyvsp[0].sval), t);
                    (yyval.sval) = t;
            }
#line 3518 "y.tab.c"
    break;

  case 130: /* term: factor  */
#line 1613 "parser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 3524 "y.tab.c"
    break;

  case 131: /* factor: MINUS factor  */
#line 1618 "parser.y"
            {
                char* t = genVar();
                emit("-", "0", (yyvsp[0].sval), t);
                (yyval.sval) = t;
                last_expr_type = DT_INT;
            }
#line 3535 "y.tab.c"
    break;

  case 132: /* factor: IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 1625 "parser.y"
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
#line 3621 "y.tab.c"
    break;

  case 133: /* factor: THIS DOT IDENTIFIER  */
#line 1707 "parser.y"
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
#line 3654 "y.tab.c"
    break;

  case 134: /* factor: IDENTIFIER DOT IDENTIFIER  */
#line 1737 "parser.y"
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
#line 3733 "y.tab.c"
    break;

  case 135: /* factor: IDENTIFIER  */
#line 1812 "parser.y"
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
#line 3761 "y.tab.c"
    break;

  case 136: /* factor: indexed_id  */
#line 1835 "parser.y"
                          { (yyval.sval) = (yyvsp[0].sval); }
#line 3767 "y.tab.c"
    break;

  case 137: /* factor: INT_LITERAL  */
#line 1836 "parser.y"
                          { char b[20]; sprintf(b, "%d",   (yyvsp[0].ival)); (yyval.sval) = strdup(b); last_expr_type = DT_INT; }
#line 3773 "y.tab.c"
    break;

  case 138: /* factor: FLOAT_LITERAL  */
#line 1837 "parser.y"
                          { char b[20]; sprintf(b, "%f",   (yyvsp[0].fval)); (yyval.sval) = strdup(b);  last_expr_type = DT_FLOAT; }
#line 3779 "y.tab.c"
    break;

  case 139: /* factor: CHAR_LITERAL  */
#line 1838 "parser.y"
                          { char b[20]; sprintf(b, "'%c'", (yyvsp[0].cval)); (yyval.sval) = strdup(b); last_expr_type = DT_CHAR; }
#line 3785 "y.tab.c"
    break;

  case 140: /* factor: STRING_LITERAL  */
#line 1839 "parser.y"
                          { (yyval.sval) = strdup((yyvsp[0].sval));  last_expr_type = DT_STRING; }
#line 3791 "y.tab.c"
    break;

  case 141: /* factor: TRUE  */
#line 1840 "parser.y"
                          { (yyval.sval) = strdup("1"); last_expr_type = DT_BOOL; }
#line 3797 "y.tab.c"
    break;

  case 142: /* factor: FALSE  */
#line 1841 "parser.y"
                          { (yyval.sval) = strdup("0"); last_expr_type = DT_BOOL; }
#line 3803 "y.tab.c"
    break;

  case 143: /* factor: LPAREN expression RPAREN  */
#line 1842 "parser.y"
                                   { (yyval.sval) = (yyvsp[-1].sval); }
#line 3809 "y.tab.c"
    break;

  case 144: /* $@15: %empty  */
#line 1847 "parser.y"
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
#line 3830 "y.tab.c"
    break;

  case 145: /* $@16: %empty  */
#line 1864 "parser.y"
            { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3836 "y.tab.c"
    break;

  case 146: /* $@17: %empty  */
#line 1866 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 3845 "y.tab.c"
    break;

  case 147: /* $@18: %empty  */
#line 1871 "parser.y"
            { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3851 "y.tab.c"
    break;

  case 148: /* if_stmt: IF LPAREN $@15 expression $@16 RPAREN $@17 block $@18 elif_list else_opt  */
#line 1873 "parser.y"
            { emit("label", "", "", topEnd()); popIfLabels(); }
#line 3857 "y.tab.c"
    break;

  case 149: /* if_stmt: IF LPAREN error RPAREN block  */
#line 1876 "parser.y"
            {
                printf("Invalid IF condition at line %d\n", yylineno);
                yyerrok;
            }
#line 3866 "y.tab.c"
    break;

  case 150: /* $@19: %empty  */
#line 1884 "parser.y"
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
#line 3887 "y.tab.c"
    break;

  case 151: /* $@20: %empty  */
#line 1901 "parser.y"
            { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3893 "y.tab.c"
    break;

  case 152: /* $@21: %empty  */
#line 1903 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 3902 "y.tab.c"
    break;

  case 153: /* $@22: %empty  */
#line 1908 "parser.y"
            { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3908 "y.tab.c"
    break;

  case 156: /* $@23: %empty  */
#line 1915 "parser.y"
            {
                char else_name[32];
                snprintf(else_name, 32, "else_%d", if_cnt++);
                SymTable* es = create_scope(SCOPE_ELSE, else_name, current_scope);
                current_scope = es;
            }
#line 3919 "y.tab.c"
    break;

  case 157: /* else_opt: ELSE $@23 block  */
#line 1922 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 3928 "y.tab.c"
    break;

  case 158: /* $@24: %empty  */
#line 1927 "parser.y"
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
#line 3949 "y.tab.c"
    break;

  case 159: /* $@25: %empty  */
#line 1944 "parser.y"
            { emit("ifFalse", (yyvsp[0].sval), "", topFalse()); }
#line 3955 "y.tab.c"
    break;

  case 160: /* $@26: %empty  */
#line 1946 "parser.y"
            {
                print_table(current_scope);
                current_scope = current_scope->parent;
            }
#line 3964 "y.tab.c"
    break;

  case 161: /* $@27: %empty  */
#line 1951 "parser.y"
            { emit("goto", "", "", topEnd()); emit("label", "", "", topFalse()); }
#line 3970 "y.tab.c"
    break;

  case 164: /* break_stmt: BREAK SEMICOLON  */
#line 1958 "parser.y"
        {
            if(loopTop < 0){
                fprintf(stderr, "ERROR line %d: 'break' outside loop.\n", yylineno);
            } else {
                emit("goto", "", "", loopEndStack[loopTop]);
            }
        }
#line 3982 "y.tab.c"
    break;

  case 165: /* continue_stmt: CONTINUE SEMICOLON  */
#line 1969 "parser.y"
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
#line 4002 "y.tab.c"
    break;

  case 166: /* $@28: %empty  */
#line 1988 "parser.y"
        {
            emit("snapshot_begin", (yyvsp[-1].sval), "", "");
            /* emit one snapshot_track per variable so optimizer
               treats each as live and never folds/propagates them */
            char buf[256];
            strncpy(buf, (yyvsp[-1].sval), 255);
            char* tok = strtok(buf, ",");
            while(tok) {
                while(*tok == ' ') tok++;
                emit("snapshot_track", tok, "", "");
                tok = strtok(NULL, ",");
            }
            free((yyvsp[-1].sval));
        }
#line 4021 "y.tab.c"
    break;

  case 167: /* $@29: %empty  */
#line 2003 "parser.y"
        {
            SymTable* bs = create_scope(SCOPE_BLOCK, "snapshot_block", current_scope);
            current_scope = bs;
        }
#line 4030 "y.tab.c"
    break;

  case 168: /* snapshot_stmt: SNAPSHOT LT snapshot_var_list GT $@28 LBRACE $@29 snapshot_stmt_list RBRACE  */
#line 2008 "parser.y"
        {
            emit("snapshot_end", "", "", "");
            print_table(current_scope);
            current_scope = current_scope->parent;
        }
#line 4040 "y.tab.c"
    break;

  case 169: /* snapshot_stmt_list: snapshot_stmt_list statement  */
#line 2017 "parser.y"
            { emit("snapshot_capture", "", "", ""); }
#line 4046 "y.tab.c"
    break;

  case 170: /* snapshot_stmt_list: statement  */
#line 2019 "parser.y"
            { emit("snapshot_capture", "", "", ""); }
#line 4052 "y.tab.c"
    break;

  case 171: /* rewind_stmt: REWIND LPAREN expression RPAREN SEMICOLON  */
#line 2024 "parser.y"
            {
                emit("rewind", (yyvsp[-2].sval), "", "");
            }
#line 4060 "y.tab.c"
    break;

  case 172: /* snapshot_var_list: snapshot_var_list COMMA IDENTIFIER  */
#line 2031 "parser.y"
            {
                char* buf = malloc(strlen((yyvsp[-2].sval)) + strlen((yyvsp[0].sval)) + 2);
                sprintf(buf, "%s,%s", (yyvsp[-2].sval), (yyvsp[0].sval));
                free((yyvsp[-2].sval));
                (yyval.sval) = buf;
            }
#line 4071 "y.tab.c"
    break;

  case 173: /* snapshot_var_list: IDENTIFIER  */
#line 2038 "parser.y"
            {
                (yyval.sval) = strdup((yyvsp[0].sval));
            }
#line 4079 "y.tab.c"
    break;

  case 174: /* $@30: %empty  */
#line 2045 "parser.y"
            {
                char for_name[32];
                snprintf(for_name, 32, "for_%d", for_cnt++);
                SymTable* fs = create_scope(SCOPE_FOR, for_name, current_scope);
                current_scope = fs;
            }
#line 4090 "y.tab.c"
    break;

  case 175: /* for_stmt: FOR LPAREN $@30 for_header block  */
#line 2052 "parser.y"
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
#line 4113 "y.tab.c"
    break;

  case 176: /* for_stmt: FOR LPAREN error RPAREN block  */
#line 2072 "parser.y"
            {
                printf("Invalid FOR header at line %d\n", yylineno);
                yyerrok;
            }
#line 4122 "y.tab.c"
    break;

  case 177: /* $@31: %empty  */
#line 2080 "parser.y"
            {
                char* b = getLabel();
                char* e = getLabel();
                pushIfLabels(b, e);
                loopEndStack[++loopTop] = strdup(e); 
                loopCondStack[loopTop]  = strdup(b);
                emit("label", "", "", b);
            }
#line 4135 "y.tab.c"
    break;

  case 178: /* $@32: %empty  */
#line 2089 "parser.y"
            {
                emit("ifFalse", (yyvsp[-1].sval) ? (yyvsp[-1].sval) : "1", "", topEnd());
                forDepth++;
                forIncIdx[forDepth]      = 0;
                inForIncrement[forDepth] = 1;
            }
#line 4146 "y.tab.c"
    break;

  case 179: /* $@33: %empty  */
#line 2096 "parser.y"
            { inForIncrement[forDepth] = 0; }
#line 4152 "y.tab.c"
    break;

  case 181: /* for_init_opt: var_decl_no_semi  */
#line 2101 "parser.y"
                                { printf("Hit var_decl_no_semi");(yyval.sval) = NULL; }
#line 4158 "y.tab.c"
    break;

  case 182: /* for_init_opt: expression  */
#line 2102 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 4164 "y.tab.c"
    break;

  case 183: /* for_init_opt: %empty  */
#line 2103 "parser.y"
                        { printf("Matching with empty string wala in for_init\n");(yyval.sval) = NULL; }
#line 4170 "y.tab.c"
    break;

  case 184: /* for_cond_opt: expression  */
#line 2107 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 4176 "y.tab.c"
    break;

  case 185: /* for_cond_opt: %empty  */
#line 2108 "parser.y"
                        { (yyval.sval) = strdup("1"); }
#line 4182 "y.tab.c"
    break;

  case 186: /* for_inc_opt: expression  */
#line 2112 "parser.y"
                        { (yyval.sval) = (yyvsp[0].sval); }
#line 4188 "y.tab.c"
    break;

  case 187: /* for_inc_opt: %empty  */
#line 2113 "parser.y"
                        { (yyval.sval) = NULL; }
#line 4194 "y.tab.c"
    break;

  case 188: /* var_decl_no_semi: type IDENTIFIER  */
#line 2118 "parser.y"
            {
                insert_symbol(current_scope, (yyvsp[0].sval), KIND_VAR, (yyvsp[-1].dtype), yylineno);
            }
#line 4202 "y.tab.c"
    break;

  case 189: /* var_decl_no_semi: type IDENTIFIER ASSIGN expression  */
#line 2122 "parser.y"
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
#line 4218 "y.tab.c"
    break;

  case 190: /* io_stmt: IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 2137 "parser.y"
            {
            Symbol* var = require_declared(current_scope, (yyvsp[-6].sval), yylineno);
            if(!var){
		parse_error_count++;
                fprintf(stderr, "ERROR line %d: assignment to undeclared variable '%s'.\n", yylineno, (yyvsp[-6].sval));
            }
            emit("out", (yyvsp[-2].sval), "", "");
            emit("in", "", "", (yyvsp[-6].sval)); 
        }
#line 4232 "y.tab.c"
    break;

  case 191: /* io_stmt: type IDENTIFIER ASSIGN FEED LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 2147 "parser.y"
            { 
            Symbol* sym = insert_symbol(current_scope, (yyvsp[-6].sval), KIND_VAR, (yyvsp[-7].dtype), yylineno);
            if(sym){
                sym->is_initialized = 1;
            }
            emit("out", (yyvsp[-2].sval), "", "");
            emit("in", "", "", (yyvsp[-6].sval)); 
        }
#line 4245 "y.tab.c"
    break;

  case 192: /* io_stmt: SHOW LPAREN expression RPAREN SEMICOLON  */
#line 2156 "parser.y"
            { emit("out", (yyvsp[-2].sval), "", ""); }
#line 4251 "y.tab.c"
    break;

  case 193: /* io_stmt: SHOW LPAREN expression LBRACKET expression RBRACKET RPAREN SEMICOLON  */
#line 2158 "parser.y"
            {
                char* t = genVar();
                emit("[]",  (yyvsp[-5].sval), (yyvsp[-3].sval), t);
                emit("out", t,  "", "");
            }
#line 4261 "y.tab.c"
    break;


#line 4265 "y.tab.c"

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

#line 2165 "parser.y"


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

