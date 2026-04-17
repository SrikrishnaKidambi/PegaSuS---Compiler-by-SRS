
#ifndef ASM_GEN_H
#define ASM_GEN_H

#include "symtab.h"   /* DataType, Symbol, SymTable — already defined */
#include <stdio.h>
#include <limits.h>
#include <string.h>
#define MAX_VARS 256
#define IR_SIZE 10000

#ifndef QUAD_DEFINED
#define QUAD_DEFINED
typedef struct {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
} Quad;
#endif

/* Shared IR array — defined in parser.y, extern here */
extern Quad   IR[];
extern int    IR_idx;

// A global variable that is used for checking if the template mathing is turned on or not. By default it is always turned on
extern int use_template_matching;

typedef enum {
    OT_TEMP,    /* t0, t1, t2 …  (genVar temporaries) */
    OT_VAR,     /* named user variable / parameter    */
    OT_CONST,   /* integer / float / char literal     */
    OT_LABEL,   /* L0, L1 …  (getLabel labels)        */
    OT_UNKNOWN
} OperandType;

OperandType getOperandType(const char* operand);

#define NUM_REGS 17   /* number of general-purpose registers available */

/* Map a temp/var name to a physical register name (e.g. "rax").
   Returns the register string; never NULL. */
const char* getReg(const char* operand);

/* Release the register currently holding 'operand'. */
void freeReg(const char* operand);

/* Spill all registers (called at label boundaries). */
void spillAllRegs(void);

/* Ensure 'operand' is in a register; return that register's name.
   Emits a MOV / load instruction if not already loaded. */
const char* load(const char* operand);

/* Write the value in 'reg' back to memory location 'var'.
   Emits a store / MOV instruction. */
void store(const char* var, const char* reg);

/* Set the output file (default: stdout). */
void asmSetOutput(FILE* fp);

/* Emit one line of assembly text (adds newline automatically). */
void asmEmit(const char* fmt, ...);

/* Emit a blank line (readability). */
void asmBlank(void);

/* Emit a comment line. */
void asmComment(const char* text);


/* Top-level driver: iterates IR[] and dispatches each quad. */
void generateASM(void);

/* Per-quad dispatcher — calls the handlers below. */
void genQuad(const Quad* q);

/* Section handlers (implement in separate .c files or below): */
void genArith(const Quad* q);          /* +  -  *  /  %          */
void genLogic(const Quad* q);          /* &&  ||  !  &  |         */
void genRelational(const Quad* q);     /* >  <  ==               */
void genAssign(const Quad* q);         /* =                       */
void genArrayAccess(const Quad* q);    /* []                      */
void genIfGoto(const Quad* q);         /* ifFalse / goto / label  */
void genFunctionCall(const Quad* q);   /* call / arg / param      */
void genFunctionPrologue(const Quad* q); /* func                  */
void genFunctionEpilogue(const Quad* q); /* endfunc / return      */
void genIO(const Quad* q);             /* in / out                */
void genObjectOps(const Quad* q);      /* new / call_constr /
                                          call_method / get_field /
                                          set_field / push_ptr    */
void genEntityBlock(const Quad* q);    /* entity / end_entity     */

// Symbol-Table Helpers for Code Generation

/* Return the stack-frame offset string for a named variable,
   e.g.  "-8(%rbp)"  — uses symtab lookup. */
const char* getVarAddress(const char* name);

/* True if the operand string looks like a numeric constant. */
int isConstant(const char* operand);

/* True if the operand is a compiler-generated temporary (t0…). */
int isTemp(const char* operand);
void markDirty(const char* regname);

extern int use_optimized_regalloc;
#endif /* ASM_GEN_H */
