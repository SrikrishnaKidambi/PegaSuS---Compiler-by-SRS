#include "asm_gen.h"
#include "symtab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

/* ---some info collected
 * The flow:
 *
 * - Classifies every IR operand (temp/var/constant/label)
 * - Manages a register file (which register holds which operand)
 * - Implements load() and store() API
 * - Has symbol table helpers like getVarAddress, isConstant, isTemp
 * - Has assembly output helpers like asmEmit, asmComment etc.
 *
 * RISC V register usage:
 * t0-t6 -> 7 caller saved temporaries
 * s0 -> frame pointer(fp) 
 * a0 -> return value/first arg
 * sp -> stack pointer
 *
 * Memory layout:
 * Every local variable/param has an offset computed by symtab.c so using that offset we are creating like -8(s0) etc.
 */

//write assembly to file and display on terminal
static FILE* asm_out = NULL; 

void asmSetOutput(FILE* fp){
	asm_out = fp ? fp: stdout;
}

static FILE* out(void){
	if(!asm_out) asm_out = stdout;
	return asm_out;
}

//Assembly output helpers

//asmEmit will emit the assembly instruction in file or stdout
//Eg: asmEmit("add to, t1, t2");
//asmEmit("lw %s, %s",reg,addr);
//asmEmit(ins) is the signature
//while printing indentation is given so labels standout properly.
void asmEmit(const char* fmt, ...){
	fprintf(out(),"    "); //for indentation to keep the labels visible
	va_list args;
	va_start(args,fmt);
	vfprintf(out(),fmt,args);
	va_end(args);
	fprintf(out(), "\n");
}

void asmBlank(void){
	fprintf(out(),"\n");
}

void asmComment(const char* text){
	fprintf(out(),"    # %s\n",text);
}

//Operand classification

//isConstant function will return 1 if operand is a literal value

int isConstant(const char*  operand){
	if(!operand || operand[0] == '\0') return 0;

	//char literal
	if(operand[0] == '\'') return 1;

	//string literal
	if(operand[0]=='"') return 1;

	//integer and boolean (0 or 1)
	const char* p = operand;
	if(*p == '-') p++; //skip minus
	if(!*p) return 0; //no constant just minus
	if(!isdigit((unsigned char)*p)) return 0; //not a digit then not constant here
	
	//floating point number
	int dots = 0;
	while(*p){
		//if dots increment and exceed 1 then it is not fp as fp has only 1 decimal point
		if(*p == '.') {dots++; if(dots > 1) return 0; }
		else if(!isdigit((unsigned char)*p)) return 0;
		p++;
	}
	return 1;
}

//isTemp returns 1 if operand is compiler generated temporary

int isTemp(const char* operand){
	if(!operand || operand[0] != 't')return 0;
	//as first not t or no operand it returns 0
	//check if it starts with t and everything that comes after t are digits
	const char* p = operand + 1;
	if(*p == '\0') return 0; //just t - not a temp
	while(*p) {
		if(!isdigit((unsigned char)*p)) return 0;
		p++;
	}

	//check if it is actually temp variable or the user defined variable by doing the lookup in symbol table
	extern SynTable* current_scope;
	if(current_scope && lookup(current_scope,operand)) return 0;

	return 1;
}

//isLabel is internal function to say if it is label or not
static int isLabel(const char* operand){
	if(!operand || operand[0] != 'L') return 0;
	const char* p = operand + 1;
	if(*p == '\0') return 0;
	while(*p){
		if(!isdigit((unsigned char)*p)) return 0;
		p++;
	}
	return 1;
}

//getOperandType classifies the operands which we use later for register allocation and to know where to load the register from

OperandType getOperandType(const char* operand){
	if(!operand || operand[0] == '\0') return OT_UNKNOWN;

	//use symbol table for lookup for user defined variables
	extern SymTable* current_scope;
	if(current_scope && lookup(current_scope, operand))
		return OT_VAR;

	//for temp variables
	if(isTemp(operand)) return OT_TEMP;

	//for label variables
	if(isLabel(operand)) return OT_LABEL;

	//for literal constant
	if(isConstant(operand)) return OT_CONST;

	//for unknown
	return OT_UNKNOWN;
}

//getVarAddress will give the string "-offset(s0)" based on the offset in the symbol table
//offset is negative as stack grows downward
const char* getVarAddress(const char* name){
	static char addr_buf[64];

	extern SymTable* current_scope;
	if(!current_scope){
		snprintf(addr_buf,sizeof(addr_buf),"0(s0)");
		return addr_buf;
	}

	Symbol* sym = lookup(current_scope,name);
	if(!sym){
		asmComment("ISSUE: symbol not found for addres lookup");
		snprintf(addr_buf, sizeof(addr_buf),  "0(s0) # unknown: %s",name);
		return addr_buf;
	}

	if(sym->offset==0)
		snprintf(addr_buf,sizeof(addr_buf),"0(s0)");
	else
		snprintf(addr_buf,sizeof(addr_buf),"-%d(s0)",sym->offset);
	return addr_buf;
}

//Register allocation

//A register descriptor table is maintained:
//1) reg_name[i] -> RISC V register name, e.g. "t0"
//2) reg_contents[i]-> which operand currently lives here, or just empty if free
//3) reg_dirty[i] -> 1 if the value has been written but not yet stored back to memory.

//When a register is needed but nothing is free then spill happens accordingly.


// We use the registers t0-t6.(7 caller-saved registers) remaining are used by OS and functions(stack)
// t0 may be used for control
// so we first go from t1 to t6 and in imp case we use t1

static const char* reg_name[NUM_REGS] = {
	"t1", "t2", "t3", "t4", "t5", "t6", "t0", "s1"
	//s1 callee saved (used in functions and control) and we use it if needed
}

static char reg_contents[NUM_REGS][64]; // operand name int this register
static int reg_dirty[NUM_REGS]; //1 if the value is modifed and need to be stored.
				
//initRegs is used to initialize registers at the start
static void initRegs(void){
	for(int i=0;i<NUM_REGS; i++){
		reg_contents[i][0] = '\0';
		reg_dirty[i] = 0;
	}
}

//findRegFor is used to return the index of the register already holding operand or -1 if not found.

static int findRegFor(const char* operand){
	for(int i=0;i<NUM_REGS; i++){
		if(reg_contents[i][0] != '\0' && strcmp(reg_contents[i], operand) ==0)
			return i;
	}
	return -1;
}

//findFreeReg - will look if there is any free register if not already stored

static int findFreeReg(void) {
	for(int i=0;i<NUM_REGS;i++){
		if(reg_contents[i][0] == '\0')return i;
	}
	return -1;
}

//spillOne is the function that spills the register at index i.

static void spillOne(int i){
	if(reg_contents[i][0]=='\0') return; // already free ntg to spill
	if(reg_dirty[i]){
		const char* occupant = reg_contents[i];
		OperandType ot = getOperandType(occupant);

		if(ot == OT_VAR){
			const char* addr = getVarAddress(occupant);
			asmComment("(debug) store ins. to spill");
			asmEmit("sw %s, %s", reg_name[i], addr);
		}
	}
	reg_contents[i][0] = '\0';
	reg_dirty[i] = 0;
}

//getReg - reg allocation algorithm
//the basic logic for this is:
//1) If in register then use the same register
//2) If free register available then use it
//3) Spill a candidate register and use it(need to be optimized in next implementation)

const char* getReg(const char* operand){
	//if already in register continue in it
	int idx = findRegFor(operand);
	if(idx >= 0) return reg_name[idx];

	//if there is free slot
	idx = findFreeReg();
	if(idx<0){
		//no reg free now need to spill
		//can be optimized later********
		//currently trying to pick a variable that is recently modified so we can change it in memory and load correct value from next time
		int victim = 0;
		for(int i=0;i<NUM_REGS;i++){
			if(reg_dirty[i] && getOperandType(reg_contents[i]) == OT_VAR){
				victim = i;
				break;
			}
		}
		spillOne(victim);
		idx = victim;
	}
	strncpy(reg_contents[idx],operand,63); //occupy the register
	reg_dirty[idx]=0;
	return reg_name[idx];
}


//freeReg - release the register holding 'operand'

void freeReg(const char* operand){
	int idx = findRegFor(operand);
	if(idx < 0)return; // not in any register
 	spillOne(idx);
	reg_contents[idx][0] = '\0';
	reg_dirty[idx] = 0;
}

//spillAllRegs - write all dirty registers back to memory and free registers

void spillAllRegs(void){
	asmComment("spill all registers");
	for(int i=0;i<NUM_REGS;i++){
		spillOne(i);
	}
}

//-----Load and Store implementation-----

//Load - bring an operand into a register and return the register name.

const char* load(const char* operand){
	if(!operand || operand[0]=='\0') return "zero"; //zero register
	OperandType ot = getOperandType(operand);

	//if constant
	if(ot == OT_CONST){
		//use li - load immediate
		//get some reg to hold the const

		//if already a register with same value exist then use it
		int already = findRegFor(operand);
		if(already >= 0) return reg_name[already];

		const char* reg = getReg(operand);

		if(operand[0] == '\''){
			//char literal - use ascii value to retrieve/extract it.
			char ch = operand[1];
			asmEmit("li %s, %d",reg,(int)ch);
		}
		else{
			asmEmit("li %s, %s",reg,operand);
		}
		reg_dirty[findRegFor(operand)] = 0;
		return reg;
	}

	//if label
	if(ot == OT_LABEL){
		//need not to load into register, so just return the label string
		static char label_buf[32];
		strncpy(label_buf, operand, 31);
		return label_buf;
	}


	//if temp or variable
	//if there in register return the reg else load it from stack
	
	int idx = findRegFor(operand);
	if(idx >= 0){
		return reg_name[idx];
	}

	//not in register
	const char* reg = getReg(operand);
	const char* addr = getVarAddress(operand);

	asmEmit("lw %s, %s",reg,addr);

	int new_idx = findRegFor(operand);
	if(new_idx>=0) reg_dirty[new_idx] = 0;

	return reg;
}

//store- write the value in reg back to variable var in memory

void store(const char* var, const char* reg){
	if(!var || !reg) return;

	const char* addr = getVarAddress(var);
	asmEmit("sw %s, %s",reg,addr);

	for(int i=0;i<NUM_REGS;i++){
		if(strcmp(reg_name[i],reg)==0){
			strncpy(reg_contents[i],var,63);
			reg_dirty[i]=0;
			return;
		}
	}
}

//markDirty - when we write we need to mark dirty
static void markDirty(const char* regname){
	for(int i=0;i<NUM_REGS;i++){
		if(strcmp(reg_name[i],regname)==0){
			reg_dirty[i]=1;
			return;
		}
	}
}

//driver code to integrate

//generateASM - this is called by the main in the parser

void genQuad(const Quad* q);

extern Quad OPT_IR[];
extern int OPT_IR_idx;

void generateASM(void){
	initRegs();
	fprintf(out(), "    .text\n");
	fprintf(otu(), "    .globl main\n");
	asmBlank();

	for(int i=0;i<OPT_IR_idx;i++){
		genQuad(&OPT_IR[i]);
	}
}
