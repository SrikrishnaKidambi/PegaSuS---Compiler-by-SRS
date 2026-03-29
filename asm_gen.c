#include "asm_gen.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

static FILE* asm_out = NULL;	// File for storing the output assembly code generated

// Register descriptor table used by load, store, getReg, freeReg functions

typedef struct {
	char name[8];
	char holds[64];
	int in_use;
} RegDesc;

// Defining the general purpose temporary registers available to the register allocator
// Caller Saved temporaries: t0-t6 only
static RegDesc regs[NUM_REGS] = {
    { "t0", "", 0 }, { "t1", "", 0 },
    { "t2", "", 0 }, { "t3", "", 0 },
    { "t4", "", 0 }, { "t5", "", 0 },
    { "t6", "", 0 },
};

// Defining argument registers for function calls
static const char* arg_regs[] = { "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7" };
#define MAX_ARG_REGS 8

// Tracks how many arguments are collected before a function call
static int pending_arg_count = 0;
static char pending_args[MAX_ARG_REGS][64];

// tracks the name of the current function we are implementing
static char current_func_name[64] = "";

// Defining the size of the frame that is used by the functions - genFunctionPrologue and genFunctionEpilogue
static int current_frame_size = 64;

// Output helper functions
void asmEmit(const char* fmt, ...) {
    if (!asm_out) asm_out = stdout;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(asm_out, fmt, ap);
    va_end(ap);
    fprintf(asm_out, "\n");
}

void asmBlank(void) {
    if (!asm_out) asm_out = stdout;
    fprintf(asm_out, "\n");
}

void asmComment(const char* text) {
    asmEmit("    # %s", text);
}

// Single entry point from the main function after the optimizations
// Writes the boilerplate ELF code (like .data, .text) 
// Iteraters through all IR Quads and calls genQuad for each of the quad
// Minimal .data section for the string literals using string formatters

void generateASM(void)
{
	if(!asm_out){
		asm_out = stdout;
	}

	// generating .data section with format of strings specified for outputting the strings or taking input
	// String literals are defined here using the format specifiers using the .asciz directive for internally generating a NULL-terminated string.
	asmEmit(".section .data");
	asmBlank();

	// Generating .text section
	asmEmit(".section .text");
	asmEmit(".globl main");		// Entry point of the asm file
	
	for(int i = 0; i < IR_idx; i++){
		genQuad(&IR[i]);
	}

	// Emit a program exit at the end of the .text section so that simulator does not fall off the end of main and crash. The service for the exit program ecall is 10.
	asmBlank();
	asmEmit("    li     a7, 10");
	asmEmit("    ecall");
}

// This function is called for all quads in the generated IR code and then based on the operator present in the Quad corresponding function handler is called
void genQuad(const Quad* q){
	const char* op = q->op;

	// Skipping any empty quads that might occur due to error recovery in parser
	if(!op || op[0] == '\0'){
		return;
	}

	// Arithmetic operations
	if(strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0
	   || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {
		genArith(q);
	}

	// Logical expressions (operations)
	else if(strcmp(op, "&&") == 0 || strcmp(op, "||") == 0 || 
		strcmp(op, "!") == 0 || strcmp(op, "&") == 0 || 
		strcmp(op, "|") == 0){
		genLogic(q);
	}

	// relational expressions
	else if(strcmp(op, ">") == 0 || strcmp(op, "<") == 0
		|| strcmp(op, "==") == 0){
		genRelational(q);
	}

	// assignment operations
	else if(strcmp(op, "=") == 0){
		genAssign(q);
	}

	// array access Quads
	else if(strcmp(op, "[]") == 0){
		genArrayAccess(q);
	}

	// Control flow Quads
	else if(strcmp(op, "ifFalse") == 0 || strcmp(op, "goto") == 0 
		|| strcmp(op, "label") == 0){
		genIfGoto(q);
	}

	// Functional prologue - starting of the function
	else if(strcmp(op, "func") == 0){
		genFunctionPrologue(q);
	}
	
	// Functional epilogue - end of the function
	else if(strcmp(op, "endfunc") == 0 || strcmp(op, "return") == 0){
		genFunctionEpilogue(q);
	}

	// call mechanism - arg, param, call
	else if(strcmp(op, "call") == 0 || strcmp(op, "arg") == 0
		|| strcmp(op, "param") == 0){
		genFunctionCall(q);
	}

	// IO operations
	else if(strcmp(op, "in") == 0 || strcmp(op, "out") == 0){
		genIO(q);
	}

	// OOP constructs - entity or method or constructor block markers
	else if(strcmp(op, "entity") == 0 || strcmp(op, "end_entity") == 0
		|| strcmp(op, "method") == 0 || strcmp(op, "end_method") == 0
		|| strcmp(op, "constr") == 0 || strcmp(op, "end_constr") == 0){
		genEntityBlock(q);
	}

	// OOP - runtime object operations
	else if(strcmp(op, "new") == 0 || strcmp(op, "call_constr") == 0
		|| strcmp(op, "call_method") == 0 || strcmp(op, "get_field") == 0
		|| strcmp(op, "set_field") == 0 || strcmp(op, "push_ptr") == 0){
		genObjectOps(q);
	}
	
	// If any constrcut not yet handled then emit it as a comment to prevent from crashing
	else{
		asmComment(op);
	}
}

// Used for handling the cases when when the getVarAddress returns invalid
static int getVarOffset(const char* var_name){
	const char* addr_str = getVarAddress(var_name);
	if(!addr_str || addr_str[0] == '\0'){
		asmComment("Warning: getVarOffset got empty address for variable");
		return 0;
	}

	char* end;
	long offset = strtol(addr_str, &end, 10);
	if (end == addr_str || (*end != '(' && *end != '\0')) {
       	 	asmComment("WARNING: getVarOffset could not parse address format");
        	return 0;
    	}
    	return (int)offset;
}

// genIfGoto - A function for handling the opcodes: label, goto and ifFalse in IR
// label - place a label in the assembly code and then spill all the registers (that is write back to the memory as it is not safe to assume same register contents after this control flow structure
// goto - generate an unconditional jump statement (using "j")
// ifFalse - jump if the condition register equals zero (using the "beqz" instruction
void genIfGoto(const Quad* q){
	// If this is a label quad then spill all the registers as multiple code paths may jump here and we cannot trust or assume which values the registers hold
	if(strcmp(q->op, "label") == 0){
		spillAllRegs();
		asmBlank();
		asmEmit("%s:", q->result);
	}
	// If the quad is just a goto quad then in assembly generate an unconditional jump statement
	else if(strcmp(q->op, "goto") == 0){
		asmEmit("    j      %s", q->result);
	}
	// If the quad contains ifFalse in the operator position then we need to generate the jump statement checking the condition 
	// condition to be checked is stored in the first argumet of the quad, so load it for checking the condition
	else if(strcmp(q->op, "ifFalse") == 0){
		const char* cond_reg = load(q->arg1);
		asmEmit("    beqz   %s, %s", cond_reg, q->result);
		freeReg(q->arg1);
	}
}


// genFunctionCall - This function handles generation of assembly code for three different IR opcodes: param, arg, call
// param: This is a formal parameter for the function declaration and definition. The actual value of this paramter is stored in the argument registers when parsing the function prologue quad
// arg: An argument that needs to be passed to the function at the call site. Here we just buffer the argument name and load the values of all these registers at the "call" time
// call: invoking the function
	// Spill all the caller saved registers (so that callee can use them freely)
	// Load the buffered arguments before writing the call statement
	// Emit the "call" <funcname> statement
	// Then store the return value in the a0 register

void genFunctionCall(const Quad* q){
	if(strcmp(q->op, "param") == 0){
		asmComment(q->arg1);
		return;
	}

	if(strcmp(q->op, "arg") == 0){
		if(pending_arg_count < MAX_ARG_REGS){
			strncpy(pending_args[pending_arg_count], q->arg1, 63);
			pending_args[pending_arg_count][63] = '\0';	// Terminate with NULL
			pending_arg_count++;
		}
		else{
			asmComment("stack-arg (>8 args) not allowed in current implementation");
		}
		return;
	}

	if(strcmp(q->op, "call") == 0){
		// Spill all the registers so that callee can use t0-t7 and a0-a7 properly
		spillAllRegs();

		// Load all the buffered arguments into the argument registers (a0 - a7). If it is constant operand using "li" else use "lw"
		for(int i = 0; i < pending_arg_count; i++){
			OperandType ot = getOperandType(pending_args[i]);
			if(ot == OT_CONST){
				asmEmit("    li     %s, %s", arg_regs[i], pending_args[i]);
			}
			else{
				asmEmit("    lw     %s, %d(s0)", arg_regs[i], getVarOffset(pending_args[i]));
			}
		}
		pending_arg_count = 0;	// Reset the count of the number of arguments for the next call

		asmEmit("    call   %s", q->arg1);

		// According to RISC-V convention the return value is captured in the register - a0 and this will be stored in the stack frame by emitting a "sw" instruction

		if(q->result[0] != '\0'){
			asmEmit("    sw     a0, %d(s0)", getVarOffset(q->result));
		}
		return;
	}
}

// genFunctionPrologue - creating the opening of the function, called for every "func" opcode.
// Action frame layout - this is a 64-byte frame with 16-byte alignment with the first 4 bytes for storing the return address of the caller, next one for the pointer to caller's sp and then store the s0 which might be over-written in future
// Actually s - registers are callee-saved registers that callee is responsible for storing these register values. Current implementation only stores s0 and loads it back before the exit of the function
// This limits the current implementation of the compiler from using 11 registers (s1-s11) which can be implemented in the future extension
// Also at the start of the function block we are required to store all the arguments in the a registers because they might be re-used or overwritten (use "load" functions for getting those values)
// Current top of the stack frame pointer is required because we are required because it is required for correct loading of the values from the stored ones in the stack frame
void genFunctionPrologue(const Quad* q){
	const char* fname = q->arg1;
	
	asmBlank();
	asmEmit("%s:", fname);
	asmComment("-- prologue --");

	// Perform a lookup in the symbol table for getting the parameter count
	Symbol* fsym = lookup(global_scope, fname);
	int frame_size = 64;

	if (fsym && (fsym->kind == KIND_FUNCTION || fsym->kind == KIND_METHOD)){
			int param_count = (fsym->kind == KIND_FUNCTION) ? fsym->attr.func.param_count : fsym->attr.method.param_count;
			// align to 16-byte boundary
			frame_size = param_count * 4 + 64;
			frame_size = (frame_size + 15) & ~15;
	}
	
	// Allocate the space for the new frame
	asmEmit("    addi   sp, sp, -%d", frame_size);
	
	// Save the return address and the caller's frame pointer
	asmEmit("    sw     ra, %d(sp)", frame_size - 4);
	asmEmit("    sw     s0, %d(sp)", frame_size - 8);
	
	// Load current frame's pointer
	asmEmit("    addi   s0, sp, %d", frame_size);

	// Copy the argument list from a registers to the stack slots
	if(fsym){
		ParamNode* p = (fsym->kind == KIND_FUNCTION)
				? fsym->attr.func.param_list
				: fsym->attr.method.param_list;
		int i = 0;
		while(p && i < MAX_ARG_REGS){
			asmEmit("    sw     %s, %d(s0)", arg_regs[i], getVarOffset(p->name));
			p = p->next;
			i++;
		}
	}

	asmComment("-- prologue end --");
	asmBlank();

	// Store the function name so that epilogue can emit correct function name
	strncpy(current_func_name, fname, sizeof(current_func_name) - 1);
	current_func_name[sizeof(current_func_name) - 1] = '\0';

	current_frame_size = frame_size;
}

// Handles the exit of the function
void genFunctionEpilogue(const Quad* q){
	if(strcmp(q->op, "return") == 0){
		// Load the return value to a0
		if(q->arg1[0] != '\0'){
			OperandType ot = getOperandType(q->arg1);
			if(ot == OT_CONST){
				asmEmit("    li     a0, %s", q->arg1);
			}
			else{
				asmEmit("    lw     a0, %d(s0)", getVarOffset(q->arg1));
			}
		}
		
		// Generate a jump instruction to the epilogue to perform the clean-up
		if(current_func_name[0] != '\0'){
			asmEmit("    j      .Lepi_%s", current_func_name);
		}
		return;
	}
	if(strcmp(q->op, "endfunc") == 0){
		// Emitting the shared epilogue label so that all return quads of the function would jump to this shared label
		if(current_func_name[0] != '\0'){
			asmEmit(".Lepi_%s:", current_func_name);
		}
		asmComment("-- epilogue --");

		int frame_size = current_frame_size;

		// Restore ra and s0 from the first and second 4 bytes of the stack frame (as saved in the prologue)
		asmEmit("    lw     ra, %d(sp)", frame_size - 4);
		asmEmit("    lw     s0, %d(sp)", frame_size - 8);


		// Now shrink the stack frame by adding frame_size to the current stack pointer (sp)
		asmEmit("    addi   sp, sp, %d", frame_size);

		// Generate a "ret" instruction that jumps to address stored in "ra" register that is set by caller and loaded back before this instruction
		asmEmit("    ret");
		asmComment("-- epilogue end --");
		asmBlank();

		// clear the current function name so that it cannot be leaking to the next function
		current_func_name[0] = '\0';
		current_frame_size = 64;	// reset to the safe default for the next function
		return;
	}
}


// genIO - a function that is used for handling the generation of equivalent assembly code for IO operations
void genIO(const Quad* q){
	// If the quad is a output quad that is "out"
	if(strcmp(q->op, "out") == 0){
		int service = 1;	// assign the default value (printing integer
		Symbol* sym = lookup(current_scope, q->arg1);
		if(sym){
			switch(sym->datatype){
				case DT_FLOAT: service = 2; break;
				case DT_STRING: service = 4; break;
				default: service = 1; break;
			}
		}

		// for safety, all the registers are spilled before performing the ecall which is similar to function call as simulator might clobber registers
		spillAllRegs();

		// Step 1: Load the value to be printed into a0 register
		// For constant use "li" instruction
		// For variable use "lw" instruction
		// For string address load the base address of the string using the name of the string label (defined in .data section). "la" is the type of instruction used
		// If it is a string pointer load it using "lw" with base address from the starting pointer of stack frame and offset computed from the symbol table
		OperandType ot = getOperandType(q->arg1);
		if(ot == OT_CONST){
			asmEmit("    li     a0, %s", q->arg1);
		}
		else if(sym && sym->datatype == DT_STRING){
			asmEmit("    la     a0, %s", q->arg1);
		}
		else{
			asmEmit("    lw     a0, %d(s0)", getVarOffset(q->arg1));
		}

		// load the service number into a7
		asmEmit("    li     a7, %d", service);
		// now emit the "ecall" instruction
		asmEmit("    ecall");
	}
	else if(strcmp(q->op, "in") == 0){
		// Compute the correct read service number
		int service = 5; 	// Set the default value to read integer
		Symbol* sym = lookup(current_scope, q->result);
		if(sym){
			switch(sym->datatype){
				case DT_FLOAT: service = 6; break;
				case DT_STRING: service = 8; break;
				default: service = 5; break;
			}
		}
		spillAllRegs();
		
		// In simulators like RARS for reading a 
		// string - a0 should contain address of the buffer to read into and a1 should contain the maximum number of characters to read
		// integer or float - no arguments are required and the result is direcrtly kept in a0
		if(sym && sym->datatype == DT_STRING){
			// Compute the offset of the string buffer on the stack and add it to the buffer base address that is a0
			int slot_offset = -(sym->offset + sym->size);
			asmEmit("    addi   a0, s0, %d", slot_offset);

			asmEmit("    li     a1, %d", sym->size);
		}

		asmEmit("    li     a7, %d", service);

		asmEmit("    ecall");
		
		// Now we need to store the input taken from the user in the required register 
		// ecall stores int input in a0, float input in fa0 and for string it is already stored in the desired buffer
		if(sym && sym->datatype != DT_STRING){
			if(sym->datatype == DT_FLOAT){
				asmEmit("    fsw    fa0, %d(s0)", getVarOffset(q->result));	// floating point store word is used for storing the floating point taken as input
			}
			else{
				asmEmit("    sw     a0, %d(s0)", getVarOffset(q->result));
			}
		}
	}
}

