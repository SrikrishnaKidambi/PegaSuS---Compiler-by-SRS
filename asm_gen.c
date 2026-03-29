#include "asm_gen.h"
#include "symtab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>


/*
  genArith function handles + ,- ,*,/ and %
  quad form:  op  arg1  arg2  result
  e.g.        +   a     b     t0
  RISC-V assembly code:
    add  rd, rs1, rs2   integer add
    sub  rd, rs1, rs2   integer sub
    mul  rd, rs1, rs2   integer mul   
    div  rd, rs1, rs2   integer div
    rem  rd, rs1, rs2   integer mod
    fadd.s / fsub.s / fmul.s / fdiv.s  for floats 
    li   rd, imm        load immediate 
 */
void genArith(const Quad* q) {
    asmComment(q->op);   // commenting the line with quad->op

    // loading both operands into regs 
    const char* r1  = load(q->arg1);
    const char* r2  = load(q->arg2);
    const char* dst = getReg(q->result);

    if (strcmp(q->op, "+") == 0) {
        asmEmit("    add  %s, %s, %s", dst, r1, r2);

    } else if (strcmp(q->op, "-") == 0) {
        asmEmit("    sub  %s, %s, %s", dst, r1, r2);

    } else if (strcmp(q->op, "*") == 0) {
        asmEmit("    mul  %s, %s, %s", dst, r1, r2);

    } else if (strcmp(q->op, "/") == 0) {
        asmEmit("    div  %s, %s, %s", dst, r1, r2);

    } else if (strcmp(q->op, "%") == 0) {
        asmEmit("    rem  %s, %s, %s", dst, r1, r2);

    } else {
        asmComment("unknown arith op — skipped");
        freeReg(q->arg1);
        freeReg(q->arg2);
        return;
    }

    freeReg(q->arg1);
    freeReg(q->arg2);
    store(q->result, dst);
}


/*
  genLogic function handles &&,||,!,& and |
  !x  →  seqz  dst, src   (set if equal zero)
  &   →  and   rd, r1, r2
  |   →  or    rd, r1, r2
  &&  →  and   rd, r1, r2   (values are already 0 or 1)
  ||  →  or    rd, r1, r2
 */
void genLogic(const Quad* q) {
    asmComment(q->op);

    if (strcmp(q->op, "!") == 0) {
        // unary NOT:only arg1 required
        const char* src = load(q->arg1);
        const char* dst = getReg(q->result);
        asmEmit("    seqz %s, %s", dst, src);
        freeReg(q->arg1);
        store(q->result, dst);
        return;
    }

    const char* r1  = load(q->arg1);
    const char* r2  = load(q->arg2);
    const char* dst = getReg(q->result);

    if (strcmp(q->op, "&&") == 0) {
        asmEmit("    and  %s, %s, %s", dst, r1, r2);

    } else if (strcmp(q->op, "||") == 0) {
        asmEmit("    or   %s, %s, %s", dst, r1, r2);

    } else if (strcmp(q->op, "&") == 0) {
        asmEmit("    and  %s, %s, %s", dst, r1, r2);

    } else if (strcmp(q->op, "|") == 0) {
        asmEmit("    or   %s, %s, %s", dst, r1, r2);

    } else {
        asmComment("unknown logic op — skipped");
        freeReg(q->arg1);
        freeReg(q->arg2);
        return;
    }

    freeReg(q->arg1);
    freeReg(q->arg2);
    store(q->result, dst);
}


/*
  genRelational function handles >,<,==
  RISC-V has no "greater than" instruction, but:
    a > b  →  slt  dst, b, a   (set if b < a)
    a < b  →  slt  dst, a, b
    a == b →  sub  tmp, a, b ; seqz dst, tmp
  result is 1 (true) or 0 (false) in an integer register.
 */
void genRelational(const Quad* q) {
    asmComment(q->op);

    const char* r1  = load(q->arg1);
    const char* r2  = load(q->arg2);
    const char* dst = getReg(q->result);

    if (strcmp(q->op, "<") == 0) {
        asmEmit("    slt  %s, %s, %s", dst, r1, r2);

    } else if (strcmp(q->op, ">") == 0) {
        // swap the operands: slt dst, r2, r1  means  r1 > r2
        asmEmit("    slt  %s, %s, %s", dst, r2, r1);

    } else if (strcmp(q->op, "==") == 0) {
        // subtract and then seqz 
        asmEmit("    sub  %s, %s, %s", dst, r1, r2);
        asmEmit("    seqz %s, %s",     dst, dst);

    } else {
        asmComment("unknown relational op — skipped");
        freeReg(q->arg1);
        freeReg(q->arg2);
        return;
    }

    freeReg(q->arg1);
    freeReg(q->arg2);
    store(q->result, dst);
}


/*
  genAssign function handles (result = arg1)
  quad form:  =  arg1  ""  result
  If arg1 is a constant we use  li  (load immediate) otherwise we load arg1 into a reg and store into result.
  Special case: char literal like 'a'  →  li dst, 97
 */
void genAssign(const Quad* q) {
    asmComment("assign");

    const char* src = q->arg1;
    const char* dst_name = q->result;

    // char literal  'x'
    if (src[0] == '\'' && src[2] == '\'') {
        const char* dst = getReg(dst_name);
        asmEmit("    li   %s, %d", dst, (int)src[1]);
        store(dst_name, dst);
        return;
    }

    // numeric constant 
    if (isConstant(src)) {
        const char* dst = getReg(dst_name);
        asmEmit("    li   %s, %s", dst, src);
        store(dst_name, dst);
        return;
    }
    // string literal
    if (src[0] == '"') {
        const char* dst = getReg(dst_name);
        asmEmit("    la   %s, str_literal", dst);
        store(dst_name, dst);
        return;
    }

    // temp variable
    const char* r   = load(src);
    const char* dst = getReg(dst_name);
    asmEmit("    mv   %s, %s", dst, r);
    freeReg(src);
    store(dst_name, dst);
}


/*
  genArrayAccess funtion handles arrays.
  The parser emits two quads for arr[i]:
    *   i    width   t1     ->  byte offset = i * elem_size
    []  arr  t1      t2     ->  address of arr[i]
 For the  []  quad we:
    1. load base address of arr  (la  r_base, arr OR lw from symtab)
    2. load offset (already a byte count in t1)
    3. add them → effective address
    4. load the value at that address into result
 */
void genArrayAccess(const Quad* q) {
    if (strcmp(q->op, "[]") != 0) {
        asmComment("genArrayAccess called with non-[] op");
        return;
    }

    asmComment("array access []");

    // r_base = address of the array's first element 
    const char* r_base   = getReg("_arr_base_tmp");
    const char* addr_str = getVarAddress(q->arg1);
    asmEmit("    la   %s, %s", r_base, addr_str);   // base address 

    // r_off = byte offset (already computed by the * quad before this) 
    const char* r_off = load(q->arg2);

    // effective address = base + offset 
    const char* r_ea = getReg(q->result);
    asmEmit("    add  %s, %s, %s", r_ea, r_base, r_off);

    // load the 4-byte value from that address into result 
    asmEmit("    lw   %s, 0(%s)", r_ea, r_ea);

    freeReg("_arr_base_tmp");
    freeReg(q->arg2);
    store(q->result, r_ea);
}


/*
  genObjectOps function handles new,call_constr,call_method,get_field,set_field and push_ptr
  Quad forms from parser.y:
    new          ClassName  ""        obj_result
    call_constr  CtorLabel  ""        ""
    call_method  label      ""        t_result
    get_field    obj        fieldname t_result
    set_field    obj        fieldname value
    push_ptr     obj        ""        ""
  RISC-V calling conventions are used here:
    a0–a7  argument registers
    a0     return value
    Objects are passed as pointers in a0.
 */
void genObjectOps(const Quad* q) {

    /*
     "new"
      allocates memory for a new object on the heap.We call malloc(class_size),class_size comes from the symbol table; we use a placeholder for now.
     example quad: new  ClassName  ""  result
     */
    if (strcmp(q->op, "new") == 0) {
        asmComment("new object");

        // symtab for the class size 
        Symbol* cls = lookup(global_scope, q->arg1);
        int class_size = 0;
        if (cls && cls->kind == KIND_ENTITY)
            class_size = cls->attr.entity.class_size;

        asmEmit("    li   a0, %d", class_size);
        asmEmit("    call malloc");
        const char* dst = getReg(q->result);
        asmEmit("    mv   %s, a0", dst);
        store(q->result, dst);
        return;
    }

    /*
    "push_ptr"
     push object pointer into a0 before calling a method.
     example quad: push_ptr  obj  ""  ""
     */
    if (strcmp(q->op, "push_ptr") == 0) {
        asmComment("push obj ptr → a0");
        const char* r = load(q->arg1);
        asmEmit("    mv   a0, %s", r);
        freeReg(q->arg1);
        return;
    }

    /*
      "call_constr"
      call a constructor; object ptr is already in a0 via push_ptr.
      example quad: call_constr  ctor_label  ""  ""
     */
    if (strcmp(q->op, "call_constr") == 0) {
        asmComment("call constructor");
        spillAllRegs();
        asmEmit("    call %s", q->arg1);
        return;
    }

    /*
     "call_method"
     call an instance method; obj ptr is in a0 via push_ptr,args were pushed as "arg" quads.
     example quad: call_method  mangled_label  ""  t_result
     */
    if (strcmp(q->op, "call_method") == 0) {
        asmComment("call method");
        spillAllRegs();
        asmEmit("    call %s", q->arg1);
        if (q->result[0] != '\0') {
            // store the return value 
            const char* dst = getReg(q->result);
            asmEmit("    mv   %s, a0", dst);
            store(q->result, dst);
        }
        return;
    }

    /* 
     "get_field"
      load obj.field into result.
      example quad: get_field  obj  fieldname  t_result
      We look up field offset from symtab then: lw  dst, offset(obj_reg)
     */
    if (strcmp(q->op, "get_field") == 0) {
        asmComment("get_field");

        const char* r_obj = load(q->arg1);

        // finding the  field offset in the entity scope level through symbol table
        int field_off = 0;
        Symbol* obj_sym = lookup(global_scope, q->arg1);
        if (obj_sym && obj_sym->kind == KIND_OBJECT) {
            SymTable* esc = find_entity_scope(obj_sym->attr.object.entity_name);
            if (esc) {
                Symbol* f = lookup_local(esc, q->arg2);
                if (f) field_off = f->offset;
            }
        }

        const char* dst = getReg(q->result);
        asmEmit("    lw   %s, %d(%s)", dst, field_off, r_obj);
        freeReg(q->arg1);
        store(q->result, dst);
        return;
    }

    /* 
    "set_field"
     storing the value into obj.field
     example quad: set_field  obj  fieldname  value
    */
    if (strcmp(q->op, "set_field") == 0) {
        asmComment("set_field");

        const char* r_obj = load(q->arg1);
        const char* r_val = load(q->result);  // "result" field holds value in parser

        int field_off = 0;
        Symbol* obj_sym = lookup(global_scope, q->arg1);
        if (obj_sym && obj_sym->kind == KIND_OBJECT) {
            SymTable* esc = find_entity_scope(obj_sym->attr.object.entity_name);
            if (esc) {
                Symbol* f = lookup_local(esc, q->arg2);
                if (f) field_off = f->offset;
            }
        }

        asmEmit("    sw   %s, %d(%s)", r_val, field_off, r_obj);
        freeReg(q->arg1);
        freeReg(q->result);
        return;
    }

    asmComment("unknown object op — skipped");
}
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
	extern SymTable* current_scope;
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
};

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

// //driver code to integrate

// //generateASM - this is called by the main in the parser

// void genQuad(const Quad* q);

// extern Quad OPT_IR[];
// extern int OPT_IR_idx;

// void generateASM(void){
// 	initRegs();
// 	fprintf(out(), "    .text\n");
// 	fprintf(otu(), "    .globl main\n");
// 	asmBlank();

// 	for(int i=0;i<OPT_IR_idx;i++){
// 		genQuad(&OPT_IR[i]);
// 	}
// }

// // Register descriptor table used by load, store, getReg, freeReg functions

// typedef struct {
// 	char name[8];
// 	char holds[64];
// 	int in_use;
// } RegDesc;

// // Defining the general purpose temporary registers available to the register allocator
// // Caller Saved temporaries: t0-t6 only
// static RegDesc regs[NUM_REGS] = {
//     { "t0", "", 0 }, { "t1", "", 0 },
//     { "t2", "", 0 }, { "t3", "", 0 },
//     { "t4", "", 0 }, { "t5", "", 0 },
//     { "t6", "", 0 },
// };

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
	// else if(strcmp(op, "entity") == 0 || strcmp(op, "end_entity") == 0
	// 	|| strcmp(op, "method") == 0 || strcmp(op, "end_method") == 0
	// 	|| strcmp(op, "constr") == 0 || strcmp(op, "end_constr") == 0){
	// 	genEntityBlock(q);
	// }

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
