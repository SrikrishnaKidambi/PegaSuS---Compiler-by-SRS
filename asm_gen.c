#include "asm_gen.h"
#include "symtab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

// Add near the top of asm_gen.c, after the includes:
static void genConstructorPrologue(const Quad* q);
static void genMethodPrologue(const Quad* q);
static int getVarOffset(const char* var_name);

// A pointer that points to the current scope of the function
// Set in the genFunctionPrologue and cleared in the genFunctionEpilogue
// If the global scope is being parsed then we make it NULL
static SymTable* current_func_scope = NULL;

// A global variable that decides if we need to use template matching based algorithm for instruction selection phase in assembly code generation
int use_template_matching = 1;	// By default it is 1 and it is turned off when the stats mode is used while compiling the input code

typedef enum{
	MODE_REG,	// Value currently in register, so use it directly (no load instruction to be generated
	MODE_IMM,	// Compile time constant - so use the instructions like li or immediate instruction
	MODE_MEM,	// the operand lives in memory and hence requries a lw instruction
	MODE_NONE	// Opearand is absent
}OperandMode;



// lookupForCodeGen - This function can be used for performing the resolution of correct symbol
// This function first seaches for local that is current function scope and then global scope (returns NULL if found nowhere)
Symbol* lookupForCodeGen(const char* name){
	Symbol* sym = NULL;

	// Search at the local scope using the above defined pointer
	if(current_func_scope){
		sym = lookup(current_func_scope, name);
		if(sym){
			return sym;
		}
	}

	// Serach at the global scope using the global_scope defined pointer in the symbol table implementation
	if(global_scope){
		sym = lookup_local(global_scope, name);
		if(sym){
			return sym;
		}
	}

	// if not found in current function scope or global scope then return NULL
	return NULL;
}

// setCurrentFuncScope, clearCurrentFuncScope - These functions are called by the genFunctionPrologue and genFunctionEpilogue respectively to ensure that current_func_scope is in sync with the current function for which the generation is happening

void setCurrentFuncScope(SymTable* scope){
	current_func_scope = scope;
}

void clearCurrentFuncScope(void){
	current_func_scope = NULL;
}

#define MAX_STRING_LITERALS 256
#define MAX_STR_CONTENT 512

typedef struct{
	char label[32];			// like str_0, str_1....
	char content[MAX_STR_CONTENT];	// Stores raw string without double quotes
}StrLitEntry;

static StrLitEntry str_lit_table[MAX_STRING_LITERALS];
static int str_lit_count = 0;

// Removes the double quotes from the string literal to store only the raw string
static void stripQuotes(char* dst, const char* src, int dst_size){
	if(!src || src[0] != '"'){
		strncpy(dst, src ? src : "", dst_size - 1);
		dst[dst_size - 1] = '\0';
		return;
	}

	// If the double quotes are there then follow this
	strncpy(dst, src+1, dst_size-1);
	dst[dst_size - 1] = '\0';
	int len = strlen(dst);
	if(len > 0 && dst[len-1] == '"'){
		dst[len - 1] = '\0';
	}
}

// Checks if the operand is a string literal
static int isStringLiteral(const char* operand){
	return operand && operand[0] == '"';
}

// registerStringLiteral - creates a mapping for the string literal and name of the label assigned for that string literal, this function does not emit anything, all the emission happens in the genDataSection
static const char* registerStringLiteral(const char* operand){
	char content[MAX_STR_CONTENT];
	stripQuotes(content, operand, sizeof(content));

	// check if the string literal is already registered
	for(int i = 0; i < str_lit_count; i++){
		if(strcmp(str_lit_table[i].content, content) == 0){
			return str_lit_table[i].label;
		}
	}

	if(str_lit_count >= MAX_STRING_LITERALS){
		return NULL;
	}

	int idx = str_lit_count++;
    	snprintf(str_lit_table[idx].label, 32, "str_%d", idx);
     	strncpy(str_lit_table[idx].content, content, MAX_STR_CONTENT - 1);
    	str_lit_table[idx].content[MAX_STR_CONTENT - 1] = '\0';
    	return str_lit_table[idx].label;
}

// getStringLabel - gets a label for the string using the registerStringLiteral function by sending the argument as the operand with quotes
const char* getStringLabel(const char* operand){
	char content[MAX_STR_CONTENT];
	stripQuotes(content, operand, sizeof(content));

	for(int i = 0; i < str_lit_count; i++){
		if(strcmp(str_lit_table[i].content, content) == 0){
			return str_lit_table[i].label;
		}
	}
	return NULL;
}


// emitAlign - generates a directive that will be enforcing the alignment with the of the variables defined in the data section
static void emitAlign(int bytes){
	if(bytes >= 4){
		asmEmit("    .align 2");
	}
	else if(bytes >= 2){
		asmEmit("    .align 1");
	}
}

// emitGlobalScalar - emits an entry in the .data section for every single global variable
static void emitGlobalScalar(Symbol* sym){
	switch(sym->datatype){
		case DT_INT:
			emitAlign(4);
			if(sym->is_initialized){
				asmEmit("%s:    .word  %s", sym->name, sym->init_value);
			}
			else{
				asmEmit("%s:    .word  0", sym->name);
			}
			break;
		case DT_FLOAT:
			emitAlign(4);
			if(sym->is_initialized){
                                asmEmit("%s:    .float %s", sym->name, sym->init_value);
                        }
                        else{
                                asmEmit("%s:    .float 0.0", sym->name);
			}
			break;
		case DT_BOOL:
			if(sym->is_initialized){
				int val = (strcmp(sym->init_value, "true") == 0 || 						strcmp(sym->init_value, "1") == 0) ? 1: 0;
				asmEmit("%s:    .byte  %d", sym->name, val);
			}
			else{
				asmEmit("%s:    .byte  0", sym->name);
			}
			break;
		case DT_CHAR:
			if(sym->is_initialized){
				const char* v = sym->init_value;
				if (v[0] == '\'' && v[2] == '\'') {
					asmEmit("%s:    .byte  '%c'", sym->name, v[1]);
				}
				else{
					asmEmit("%s:    .byte  %s", sym->name, v);
				}
			}
			else{
				asmEmit("%s:    .byte  0", sym->name);
			}
			break;
		default:
			break;
	}
}


// emitGlobalString - emits an entry in .data section
static void emitGlobalString(Symbol* sym){
	if(sym->is_initialized && sym->init_value && sym->init_value[0] != '\0'){
		char content[MAX_STR_CONTENT];
		if(sym->init_value[0] == '"'){
			stripQuotes(content, sym->init_value, sizeof(content));
		}
		else{
			strncpy(content, sym->init_value, sizeof(content) - 1);
			content[sizeof(content) - 1] = '\0';
		}
		asmEmit("%s:    .asciz \"%s\"", sym->name, content);
	}
	else{
		int buf_size = (sym->size > 0) ? sym->size : 64;
		asmEmit("%s:    .space %d", sym->name, buf_size);
	}
}

// emitGlobalArray - Emits .data for every global array
static void emitGlobalArray(Symbol* sym){
	int dims = sym->attr.array.dimensions;
	int dim1 = sym->attr.array.dim1;
	int dim2 = sym->attr.array.dim2;
	int elem_size = datatype_size(sym->datatype);
	int is_init = sym->attr.array.is_initialized;
	int init_cnt = sym->attr.array.init_count;

	int total_elems = (dims == 2) ? (dim1*dim2) : dim1;
       	int total_bytes = total_elems * elem_size;

	if(!is_init || init_cnt == 0){
		if(sym->datatype == DT_STRING){
			int str_buf = 64;
			asmEmit("%s:    .space %d   # string array [%d] x %d bytes", sym->name, total_elems * str_buf, total_elems, str_buf);
		}else{
			emitAlign(elem_size);
			asmEmit("%s:    .space %d   # array [%d] elem_size=%d", sym->name, total_bytes, total_elems, elem_size);
		}
		return;
	}

	emitAlign(elem_size);
	int first = 1;
	for(int i = 0; i < total_elems; i++){
		const char* val = (i < init_cnt) ?
			sym->attr.array.init_values[i] : "0";
		const char* label_part = first ? sym->name : "";
		first = 0;
		switch(sym->datatype){
			case DT_INT:
				if(label_part[0]){
					asmEmit("%s:    .word  %s", label_part, val);
				}
				else{
					asmEmit("       .word  %s", val);
				}
				break;
			case DT_FLOAT:
				if(label_part[0]){
					asmEmit("%s:    .float %s", label_part, val);
				}
				else{
					asmEmit("       .float %s", val);
				}
				break;
			case DT_BOOL: {
				
                		int bval = (strcmp(val, "true") == 0 ||
                            	strcmp(val, "1")    == 0) ? 1 : 0;
                		if (label_part[0])
                    			asmEmit("%s:    .byte  %d", label_part, bval);
                		else
                    			asmEmit("       .byte  %d", bval);
               			 break;

            		}
			
			case DT_CHAR:
				      if(label_part[0]){
					      asmEmit("%s:    .byte  %s", label_part, val);
				      }
				      else{
					      asmEmit("       .byte  %s", val);
				      }
				      break;
			case DT_STRING:
				      {
					      char content[MAX_STR_CONTENT];
					      if(val[0] == '"'){
						      stripQuotes(content, val, sizeof(content));
					      }
					      else{
						      strncpy(content, val, sizeof(content) - 1);
						      content[sizeof(content) - 1] = '\0';
					      }
					      if(label_part[0]){
						      asmEmit("%s:    .asciz \"%s\"", label_part, content);
					      }
					      else{
						      asmEmit("       .asciz \"%s\"", content);
					      }
					      break;
					}
			default:
				      break;
		}
	}
}


// scanStringLiterals - this function iterates through the IR code and finds all the quoted strings and register them in the string literal table so that in future they can emit .asciz entry
// This function is called before the genDataSection is called so that all the string literals are defined in the .data section before program tries to access the strings using the "la" instruction
static void scanStringLiterals(void)
{
	for(int i = 0; i < IR_idx; i++){
		const Quad* q = &IR[i];

		// checking all three possible operand fields that is arg1, arg2 and result
		const char* fields[3] = {q->arg1, q->arg2, q->result};

		for(int f = 0; f < 3; f++){
			const char* field = fields[f];

			if(!field || !isStringLiteral(field)){
				continue;
			}

			int before = str_lit_count;
			const char* label = registerStringLiteral(field);
			int is_new = str_lit_count > before;
			if(label && is_new){
				asmEmit("%s:    .asciz \"%s\"",
						label,
						str_lit_table[str_lit_count - 1].content);
			}
		}
	}
}

// genDataSection - Emits the .data section completely emitting the labels for all the global scalar variabls, global arrays, and string literals by scanning IR quads
void genDataSection(void)
{
	asmEmit(".section .data");
	asmBlank();

	// Collect all global symbols to that first we can emit global scalars
	if(!global_scope){
		asmBlank();
		return;
	}

	asmComment("-- Global Scalar Variables --");
	
	for(int b = 0; b < HASH_SIZE; b++){
		for(Symbol* sym = global_scope->buckets[b]; sym; sym=sym->next){
			// 
			if (sym->kind == KIND_FUNCTION   ||
				sym->kind == KIND_METHOD     ||
				sym->kind == KIND_CONSTRUCTOR||
				sym->kind == KIND_ENTITY     ||
				sym->kind == KIND_OBJECT     ||
				sym->kind == KIND_FOR        ||
				sym->kind == KIND_IF         ||
				sym->kind == KIND_ELIF       ||
				sym->kind == KIND_ELSE) {
				continue;
			}

			// skip arrays also
			if(sym->kind == KIND_ARRAY){
				continue;
			}

			// Emit scalar variable
			if(sym->datatype == DT_STRING){
				emitGlobalString(sym);
			}
			else{
				emitGlobalScalar(sym);
			}
		}
	}

	asmBlank();
	asmComment("-- global arrays --");

	for(int b = 0; b < HASH_SIZE; b++){
		for(Symbol* sym = global_scope->buckets[b]; sym; sym = sym->next){
			if(sym->kind != KIND_ARRAY){
				continue;
			}
			emitGlobalArray(sym);
		}
	}

	asmBlank();
	asmComment("-- string literals --");

	scanStringLiterals();
	asmBlank();
}

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
    markDirty(dst);
    //freeReg(q->arg1);
    //freeReg(q->arg2);
    //store(q->result, dst);
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
	markDirty(dst);
       // freeReg(q->arg1);
       // store(q->result, dst);
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
    markDirty(dst);
   // freeReg(q->arg1);
   // freeReg(q->arg2);
    //store(q->result, dst);
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
    markDirty(dst);
    //freeReg(q->arg1);
    //freeReg(q->arg2);
    //store(q->result, dst);
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
	markDirty(dst);
       // store(dst_name, dst);
        return;
    }

    // numeric constant 
    if (isConstant(src)) {
        const char* dst = getReg(dst_name);
        asmEmit("    li   %s, %s", dst, src);
	markDirty(dst);
       // store(dst_name, dst);
        return;
    }
    // string literal
    if (src[0] == '"') {
        const char* dst = getReg(dst_name);
        asmEmit("    la   %s, str_literal", dst);
	markDirty(dst);
        //store(dst_name, dst);
        return;
    }

    // temp variable
    const char* r   = load(src);
    const char* dst = getReg(dst_name);
    asmEmit("    mv   %s, %s", dst, r);
    markDirty(dst);
   // freeReg(src);
    //store(dst_name, dst);
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

// Defining argument registers for function calls
static const char* arg_regs[] = { "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7" };
#define MAX_ARG_REGS 8

// Tracks how many arguments are collected before a function call
static int pending_arg_count = 0;
static char pending_args[MAX_ARG_REGS][64];

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
	int class_size = cls && cls->kind == KIND_ENTITY ? cls->attr.entity.class_size:4;
	spillAllRegs();
	asmEmit("    li   a0, %d", class_size);
	asmEmit("    call malloc");
	// Store the pointer directly instead of going through register allocator
	asmEmit("    sw   a0, %d(s0)", getVarOffset(q->result));
	return;
    }

    /*
    "push_ptr"
     push object pointer into a0 before calling a method.
     example quad: push_ptr  obj  ""  ""
     */
    if(strcmp(q->op, "push_ptr") == 0){
	    asmComment("push_ptr: load obj pointer into a0");
	    asmEmit("    lw   a0, %d(s0)", getVarOffset(q->arg1));
	    return;
    }

    /*
      "call_constr"
      call a constructor; object ptr is already in a0 via push_ptr.
      example quad: call_constr  ctor_label  ""  ""
     */
    if (strcmp(q->op, "call_constr") == 0) {
	    asmComment("call_constr");
	    // save "this" pointer below sp before spilling a0
	    asmEmit("    addi sp, sp, -4");
	    asmEmit("    sw   a0, 0(sp)");
	    spillAllRegs();
	    // load arguments in constructor arguments into a1...a7
	    for(int i = 0; i < pending_arg_count; i++){
		    OperandType ot = getOperandType(pending_args[i]);
		    if(ot == OT_CONST){
			    asmEmit("    li   %s, %s", arg_regs[i+1], pending_args[i]);
		    }
		    else{
			    asmEmit("    lw   %s, %d(s0)", arg_regs[i+1], getVarOffset(pending_args[i]));
		    }
	    }
	    pending_arg_count = 0;
	    asmEmit("    lw   a0, 0(sp)");
	    asmEmit("    addi sp, sp, 4");
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
	asmEmit("    addi sp, sp, -4");
	asmEmit("    sw   a0, 0(sp)");
        spillAllRegs();
        for(int i = 0; i < pending_arg_count; i++){
		OperandType ot = getOperandType(pending_args[i]);
		if(ot == OT_CONST){
			asmEmit("    li   %s, %s", arg_regs[i+1], pending_args[i]);
		}
		else{
			asmEmit("    lw   %s, %d(s0)", arg_regs[i+1], getVarOffset(pending_args[i]));
		}
	}
	pending_arg_count = 0;
	asmEmit("    lw a0, 0(sp)");
	asmEmit("    addi sp, sp, 4");
	asmEmit("    call %s", q->arg1);
	if(q->result[0] != '\0'){
		asmEmit("    sw  a0, %d(s0)", getVarOffset(q->result));
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
	    int field_off = 0;
	    Symbol* obj_sym = lookupForCodeGen(q->arg1);	// local-first lookup
	
	    if(obj_sym && obj_sym->kind == KIND_OBJECT){
		    SymTable* esc = find_entity_scope(obj_sym->attr.object.entity_name);
		    if(esc){
			    Symbol* f = lookup_local(esc, q->arg2);
			    if(f){
				    field_off = f->offset;
			    }
		    }
	    }
	    else if(strcmp(q->arg1, "this") == 0 && current_func_scope){
		    SymTable* sc = current_func_scope->parent;
		    while(sc){
			    if(sc->kind == SCOPE_ENTITY){
				    Symbol* f = lookup_local(sc, q->arg2);
				    if(f){
					    field_off = f->offset;
					    break;
				    }
			    }
			    sc = sc->parent;
		    }
	    }
	    const char* r_obj = getReg("_obj_ptr_tmp");
	    if(strcmp(q->arg1, "this") == 0){
		    asmEmit("    lw  %s, -4(s0)", r_obj);
	    }
	    else{
		    asmEmit("    lw  %s, %d(s0)", r_obj, getVarOffset(q->arg1));
	    }
	    const char* dst = getReg(q->result);
	    asmEmit("    lw  %s, %d(%s)", dst, field_off, r_obj);
	    freeReg("_obj_ptr_tmp");
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
	int field_off = 0;
	Symbol* obj_sym = lookupForCodeGen(q->arg1);
	if(obj_sym && obj_sym->kind == KIND_OBJECT){
		SymTable* esc = find_entity_scope(obj_sym->attr.object.entity_name);
		if(esc){
			Symbol* f = lookup_local(esc, q->arg2);
			if(f){
				field_off = f->offset;
			}
		}
	}
	else if(strcmp(q->arg1, "this") == 0 && current_func_scope){
		SymTable* sc = current_func_scope->parent;
		while(sc){
			if(sc->kind == SCOPE_ENTITY){
				Symbol* f = lookup_local(sc, q->arg2);
				if(f){
					field_off = f->offset;
					break;
				}
			}
			sc = sc->parent;
		}
	}
	const char* r_obj = getReg("_obj_ptr_tmp");
	if(strcmp(q->arg1, "this") == 0){
		asmEmit("    lw  %s, -4(s0)", r_obj);
	}
	else{
		asmEmit("    lw  %s, %d(s0)", r_obj, getVarOffset(q->arg1));
	}
	const char* r_val = getReg("_field_val_tmp");
	OperandType vt = getOperandType(q->result);
	if(vt == OT_CONST){
		asmEmit("    li  %s, %s", r_val, q->result);
	}
	else{
		asmEmit("    lw  %s, %d(s0)", r_val, getVarOffset(q->result));
	}
	asmEmit("    sw  %s, %d(%s)", r_val, field_off, r_obj);
	freeReg("_obj_ptr_tmp");
	freeReg("_field_val_tmp");
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

void genEntityBlock(const Quad* q){
	(void)q;
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
	if(strcmp(name, "this") == 0){
		snprintf(addr_buf, sizeof(addr_buf), "-4(s0)");
		return addr_buf;
	}

	Symbol* sym = lookup(current_scope,name);
	if(!sym){
		asmComment("ISSUE: symbol not found for addres lookup");
		snprintf(addr_buf, sizeof(addr_buf),  "0(s0) # unknown: %s",name);
		return addr_buf;
	}

	if(sym->offset==0)
		snprintf(addr_buf,sizeof(addr_buf),"-8(s0)");
	else
		snprintf(addr_buf,sizeof(addr_buf),"-%d(s0)",sym->offset+8);
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

//0 = basic spill, 1 = optimized next use spill coming from parser.y
int use_optimized_regalloc = 0;
//counts of loads and stores
static int count_loads = 0;
static int count_stores = 0;

/*--------Next Use Information-------------
For each instruction i and each variable v we store:
next_use => index of the next instruction that uses v and -1 means v is dead(never comes again)
is_live => 1 if v is live at instruction i, 0 if dead

Motive: The candidate variable that we select to evict when a new variable need to be allocated a register
we look to evict a variable that is used much futher or if it no more used that is dead.
All these are done to reduce the loads and stores
*/

typedef struct {
	int next_use; //instruction index of next use, -1 = dead
	int is_live; //1 = live at this point, 0 = dead
} NextUse;

//next_use_table[i][v] gives the next use info for the variable v at the instruction i
static NextUse next_use_table[IR_SIZE][MAX_VARS];

//flat array of variable names - gives each variable an index v
static int next_use_var_count = 0;
static char next_use_vars[MAX_VARS][64];

//which IR instruction we are currently looking into
//get Reg uses the above IR instruction we are currently on look at the next_use_table to get the info
static int current_ir_idx = 0;



//initRegs is used to initialize registers at the start
static void initRegs(void){
	for(int i=0;i<NUM_REGS; i++){
		reg_contents[i][0] = '\0';
		reg_dirty[i] = 0;
	}

	//reset next-use table state so previous function's info don't effect next one
	//next_use_var_count = 0;
	current_ir_idx = 0;
}

/*----isBlockEnd----
Returns 1 if quad q is the last instruction of a basic block

We use the next use information per basic block so this function identifies where bb ends so the next line is start of the next basic block
A basic block ends at any instruction that transfers control
(branch, jump, return , function end)

At the end of the basic block all user defined variables are assumed to be live
as they might be used in the next basic blocks.

All compiler temporaries are assumed to be dead at the end of basic block as they are locally used and created by compiler
*/

static int isBlockEnd(const Quad* q){
	return (strcmp(q->op, "ifFalse")==0 || //branch statement
			strcmp(q->op, "goto")  == 0 || //jump statement 
			strcmp(q->op, "return") == 0|| //jump from function
			strcmp(q->op, "endfunc") ==0); //ending of the function
}

//-----isBlockStart----
static int isBlockStart(const Quad* q){
	return (strcmp(q->op,"label")==0 ||
			strcmp(q->op,"func")==0);
}

/*--------getVarIdx-------
Maps a variable/temp name to a column index in next_use_table
If the name is new it is added to the next_use_vars array

We need a integer index aliasing the variable to use as index in next_use_table
*/
static int getVarIdx(const char* name){
	//search existing entries
	for(int i=0;i<next_use_var_count;i++){
		if(strcmp(next_use_vars[i],name)==0)return i;
	}

	if(next_use_var_count<MAX_VARS){
		strncpy(next_use_vars[next_use_var_count],name,63);
		return next_use_var_count++;
	}
	return -1; //table full
}

/*------computeNextUse---------
Scans instructions in reverse order that is from the 'end' to 'begin'.
Fills next_use_table[i][v] for every instruction i in [start,end] (basic block).

=> Initialize at the end of block:
	user variables -> live, next_use = -1
	temporaries -> dead, next_use=-1

For every instruction:
-> result = arg1 op arg2
1) copy liveness from instructions i+1
2) make result dead (not live) , next_use = -1
3) arg1 and arg2 set it live and next_use = i
*/

static void computeNextUse(int start, int end){
	extern Quad IR[]; // we need to pass the Quad to get the next use information 

	//initialize the bottom of the block as in above comments
	for(int v=0;v<next_use_var_count;v++){
		next_use_table[end][v].next_use = -1;
		next_use_table[end][v].is_live = !isTemp(next_use_vars[v]);
	}

	//scan backwards from end-1 down to start
	for(int i=end-1;i>=start;i--){
		//copy state from instructions just below
		for(int v = 0;v<next_use_var_count;v++){
			next_use_table[i][v] = next_use_table[i+1][v];
		}

		Quad* q = &IR[i];//current quad

		//kill the result - make it dead
		if(q->result[0] != '\0'){
			int v = getVarIdx(q->result);
			if(v>=0){
				next_use_table[i][v].next_use = -1;
				next_use_table[i][v].is_live = 0;
			}
		}

		//arg1 is used in this instruction - it is live and next use = i
		if(q->arg1[0] != '\0' && !isConstant(q->arg1)){
			int v = getVarIdx(q->arg1);
			if(v>=0){
				next_use_table[i][v].next_use = i;
				next_use_table[i][v].is_live = 1;
			}
		}

		//arg2 is used in this instruction - it is live and next use = i
		if(q->arg2[0] != '\0' && !isConstant(q->arg2)){
			int v = getVarIdx(q->arg2);
			if(v>=0){
				next_use_table[i][v].next_use = i;
				next_use_table[i][v].is_live = 1;
			}
		}
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
			count_stores++;
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

//the advanced logic uses next use information backward pass algorithm + reg desc table
const char* getRegOpt(const char* operand){
	//Already in a register return it immediately, no loading
	int idx = findRegFor(operand);
	if(idx>=0) return reg_name[idx];

	//free register available then claim it
	idx = findFreeReg();
	if(idx>=0) {
		strncpy(reg_contents[idx],operand,63);
		reg_dirty[idx]=0;
		return reg_name[idx];
	}

	//no empty registers -spill one using next use info
	/*Spill strategy as per the book

	Priority1 - dead temporary (no load and store)
		A compiler generated temporary variable which is dead is best choice to remove and occupy the position
	Priority2 - clean register (value already in memory) ( no store)
		As value is in register no need a sw
	Priority3 - variable with Farthest next use (no load for long time)
		If we must emit a store, pick the variable that is next used till long time

	We need to pick the candidate variable v whose next use is farthest/dead.
	*/

	//go through after covering all the register we get to get the best possible candidate

	int victim = 0; //default fallback slot 0
	int farthest = -2; //initialization

	for (int i=0;i<NUM_REGS;i++){
		if(reg_contents[i][0] == '\0') continue;  //already free no need to find just safe check
		
		const char* occupant = reg_contents[i];
		//Priority 1: dead temporary - best possible victim
		//isTemp confirms it is compiler generated
		//next_use = -1 means no instruction in this block uses it again.
		//No store needed as it is a temp variable
		if(isTemp(occupant)){
			int v = getVarIdx(occupant);
			int nu = (v>=0) ? next_use_table[current_ir_idx][v].next_use:-1;
			if(nu==-1){
				//dead temp so we can evict
				reg_contents[i][0] = '\0';
				reg_dirty[i] = 0;
				strncpy(reg_contents[i],operand,63);
				reg_dirty[i] = 0;
				return reg_name[i];
			}
		}

		//Priority 2: clean register already the data is in the memory
		// reg_dirty = 0 means this above case
		//we can use this register without emitting a store
		if(!reg_dirty[i] && farthest == -2){
			victim = i;
			farthest = -1; //mark as fount as clean candidate
			continue;
		}

		//Priority 3: farthest next use
		//Look up when this occupant will next be needed
		//next_use == -1 means dead -> used much much further
		int v = getVarIdx(occupant);
		int nu = (v>=0)? next_use_table[current_ir_idx][v].next_use:-1;
		int effective = (nu == -1)? INT_MAX: nu;
		if(effective > farthest){
			farthest = effective;
			victim = i;
		}
	}

	spillOne(victim);
	strncpy(reg_contents[victim],operand,63);
	reg_dirty[victim]=0;
	return reg_name[victim];
}
const char* getReg(const char* operand){
	//always check if already in register first - same for both strategies
	int idx = findRegFor(operand);
	if (idx >= 0) return reg_name[idx];

	//free slot available - same for both strategies
	idx = findFreeReg();
	if(idx>=0){
		strncpy(reg_contents[idx],operand,63);
		reg_dirty[idx]=0;
		return reg_name[idx];
	}

	//no free slot choose spill strategy based on flag from parser.y
	if(use_optimized_regalloc)
		return getRegOpt(operand); //use opt. strategy of next use information

	//basic starategy - spill the first dirty register
	int victim =  0;
	for(int i=0;i<NUM_REGS;i++){
		if(reg_dirty[i] && getOperandType(reg_contents[i]) == OT_VAR){
			victim = i;
			break;
		}
	}
	spillOne(victim);
	strncpy(reg_contents[victim],operand,63);
	reg_dirty[victim]=0;
	return reg_name[victim];
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
		if(reg_contents[i][0] == '\0') continue;

		//dead temporary - just discard, no store needed
		if(isTemp(reg_contents[i])){
			reg_contents[i][0] = '\0';
			reg_dirty[i] = 0;
			continue;
		}

		//live variables spillOne handles the dirty
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
			count_loads++;
		}
		else{
			asmEmit("li %s, %s",reg,operand);
			count_loads++;
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
	count_loads++;

	int new_idx = findRegFor(operand);
	if(new_idx>=0) reg_dirty[new_idx] = 0;

	return reg;
}

//store- write the value in reg back to variable var in memory

void store(const char* var, const char* reg){
	if(!var || !reg) return;

	const char* addr = getVarAddress(var);
	asmEmit("sw %s, %s",reg,addr);
	count_stores++;

	for(int i=0;i<NUM_REGS;i++){
		if(strcmp(reg_name[i],reg)==0){
			strncpy(reg_contents[i],var,63);
			reg_dirty[i]=0;
			return;
		}
	}
}

//markDirty - when we write we need to mark dirty
void markDirty(const char* regname){
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


// tracks the name of the current function we are implementing
static char current_func_name[64] = "";

// Defining the size of the frame that is used by the functions - genFunctionPrologue and genFunctionEpilogue
static int current_frame_size = 64;

// This function is useful for finding the mode of the operand to efficiently select the instructions

static OperandMode getMode(const char* operand){
	if(!operand || operand[0] == '\0'){
		return MODE_NONE;
	}
	if(isConstant(operand) >= 0){
		return MODE_IMM;
	}
	if(findRegFor(operand) >= 0){
		return MODE_REG;
	}
	return MODE_MEM;
}

static void emitArithWithMode(const Quad* q, OperandMode m1, OperandMode m2){
	const char* dst = getReg(q->result);

	// if addition operation and one of the operands is a constant, hence use addi
	if(strcmp(q->op, "+") == 0 && m2 == MODE_IMM){
		const char* r1 = load(q->arg1);
		asmEmit("    addi %s, %s, %s", dst, r1, q->arg2);
		//freeReg(q->arg1);
		//store(q->result, dst);
		markDirty(dst);
		return;
	}

	// Now if the argument 1 has constant value
	if(strcmp(q->op, "+") == 0 && m1 == MODE_IMM){
		const char* r2 = load(q->arg2);
		asmEmit("    addi %s, %s, %s", dst, r2, q->arg1);
		// freeReg(q->arg2);
		// store(q->result, dst);
		markDirty(dst);
		return;
	}

	// if the argument 2 is constant the operation is subtraction
	if(strcmp(q->op, "-") == 0 && m2 == MODE_IMM){
		const char* r1 = load(q->arg1);
		long imm = strtol(q->arg2, NULL, 10);
		asmEmit("    addi %s, %s, %ld", dst, r1, -imm);
		// freeReg(q->arg1);
		// store(q->result, dst);
		markDirty(dst);
		return;
	}

	// For any general expressions
	const char* r1 = load(q->arg1);
	const char* r2 = load(q->arg2);

	if(strcmp(q->op, "+") == 0){
		asmEmit("    add  %s, %s, %s", dst, r1, r2);
	}
	if(strcmp(q->op, "-") == 0){
		asmEmit("    sub  %s, %s, %s", dst, r1, r2);
	}
	if(strcmp(q->op, "*") == 0){
		asmEmit("    mul  %s, %s, %s", dst, r1, r2);
	}
	if(strcmp(q->op, "/") == 0){
		asmEmit("    div  %s, %s, %s", dst, r1, r2);
	}
	if(strcmp(q->op, "%") == 0){
		asmEmit("    rem  %s, %s, %s", dst, r1, r2);
	}
	// freeReg(q->arg1);
	// freeReg(q->arg2);
	markDirty(dst);
}

static void emitAssignWithMode(const Quad* q, OperandMode m1){
	// charactr literal
	if(q->arg1[0] == '\'' && q->arg1[2] == '\''){
		const char* dst = getReg(q->result);
		asmEmit("    li   %s, %d", dst, (int)q->arg1[1]);
		markDirty(dst);
		// store(q->result, dst);
		return;
	}

	// string literal
	if(q->arg1[0] == '"'){
		const char* lbl = getStringLabel(q->arg1);
		const char* dst = getReg(q->result);
		if(lbl){
			asmEmit("    la   %s, %s", dst, lbl);
		}
		markDirty(dst);
		// store(q->result, dst);
		return;
	}

	switch(m1){
		case MODE_IMM:
			// If a constant then just li is sufficient
		{
			const char* dst = getReg(q->result);
			asmEmit("    li   %s, %s", dst, q->arg1);
			// store(q->result, dst);
			markDirty(dst);
		}
		break;
		case MODE_REG:
			// If the variable is in register then generate mv
		{
			const char* src_reg = reg_name[findRegFor(q->arg1)];
			const char* dst = getReg(q->result);
			asmEmit("    mv   %s, %s", dst, src_reg);
			// store(q->result, dst);
			markDirty(dst);
		}
		break;
		case MODE_MEM:
			// Variable is in memory so use mv lw and then mv
		{
			const char* r = load(q->arg1);
			const char* dst = getReg(q->result);
			asmEmit("    mv   %s, %s", dst, r);
			// freeReg(q->arg1);
			// store(q->result, dst);
			markDirty(dst);
		}
		break;
		default:
			genAssign(q);
			break;
	}
}

// This function is called for all quads in the generated IR code and then based on the operator present in the Quad corresponding function handler is called
void genQuad(const Quad* q){
	const char* op = q->op;

	// Skipping any empty quads that might occur due to error recovery in parser
	if(!op || op[0] == '\0'){
		return;
	}
	
	if(use_template_matching){
		// Get the modes of the operand
		OperandMode m1 = getMode(q->arg1);
		OperandMode m2 = getMode(q->arg2);

		if(strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
			strcmp(op, "*") == 0 || strcmp(op, "/") == 0 ||
			strcmp(op, "%") == 0){
			emitArithWithMode(q, m1, m2);
			return;
		}

		if(strcmp(op, "=") == 0){
			emitAssignWithMode(q, m1);
			return;
		}
		(void)m1; (void)m2;
		if(strcmp(op, "&&") == 0 || strcmp(op, "||") == 0 ||
			strcmp(op, "!") == 0 || strcmp(op, "&") == 0 ||
			strcmp(op, "|") == 0){
			genLogic(q);
			return;
		}
		if(strcmp(op, ">") == 0 || strcmp(op, "<") == 0 ||
			strcmp(op, "==") == 0){
			genRelational(q);
			return;
		}
		if(strcmp(op, "[]") == 0){
			genArrayAccess(q);
			return;
		}
		if(strcmp(op, "ifFalse") == 0 || strcmp(op, "goto") == 0 ||
			strcmp(op, "label") == 0){
			genIfGoto(q);
			return;
		}
		if(strcmp(op, "func") == 0){
			genFunctionPrologue(q);
			return;
		}
		if(strcmp(op, "return") == 0 ||
			strcmp(op, "endfunc") == 0){
			genFunctionEpilogue(q);
			return;
		}
		if(strcmp(op, "call") == 0 || strcmp(op, "arg") == 0 ||
			strcmp(op, "param") == 0){
			genFunctionCall(q);
			return;
		}
		if(strcmp(op, "in") == 0 ||
			strcmp(op, "out") == 0){
			genIO(q);
			return;
		}
		if(strcmp(op, "entity") == 0 ||
			strcmp(op, "end_entity") == 0){
			return;
		}
		if(strcmp(op, "constr") == 0){
			genConstructorPrologue(q);
			return;
		}
		if(strcmp(op, "end_constr") == 0){
			genFunctionEpilogue(q);
			return;
		}
		if(strcmp(op, "method") == 0){
			genMethodPrologue(q);
			return;
		}
		if(strcmp(op, "end_method") == 0){
			genFunctionEpilogue(q);
			return;
		}
		if(strcmp(op, "new") == 0 || strcmp(op, "call_constr") == 0
		   || strcmp(op, "call_method") == 0 || strcmp(op, "get_field") == 0 || strcmp(op, "set_field") == 0 || strcmp(op, "push_ptr") == 0){
			genObjectOps(q);
			return;
		}
		asmComment(op);
		return;
	}

	// Arithmetic operations
	if(strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0
	   || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {
		genArith(q);
		return;
	}

	// Logical expressions (operations)
	else if(strcmp(op, "&&") == 0 || strcmp(op, "||") == 0 || 
		strcmp(op, "!") == 0 || strcmp(op, "&") == 0 || 
		strcmp(op, "|") == 0){
		genLogic(q);
		return;
	}

	// relational expressions
	else if(strcmp(op, ">") == 0 || strcmp(op, "<") == 0
		|| strcmp(op, "==") == 0){
		genRelational(q);
		return;
	}

	// assignment operations
	else if(strcmp(op, "=") == 0){
		genAssign(q);
		return;
	}

	// array access Quads
	else if(strcmp(op, "[]") == 0){
		genArrayAccess(q);
		return;
	}

	// Control flow Quads
	else if(strcmp(op, "ifFalse") == 0 || strcmp(op, "goto") == 0 
		|| strcmp(op, "label") == 0){
		genIfGoto(q);
		return;
	}

	// Functional prologue - starting of the function
	else if(strcmp(op, "func") == 0){
		genFunctionPrologue(q);
		return;
	}
	
	// Functional epilogue - end of the function
	else if(strcmp(op, "endfunc") == 0 || strcmp(op, "return") == 0){
		genFunctionEpilogue(q);
		return;
	}

	// call mechanism - arg, param, call
	else if(strcmp(op, "call") == 0 || strcmp(op, "arg") == 0
		|| strcmp(op, "param") == 0){
		genFunctionCall(q);
		return;
	}

	// IO operations
	else if(strcmp(op, "in") == 0 || strcmp(op, "out") == 0){
		genIO(q);
		return;
	}

	// OOP - runtime object operations
	else if(strcmp(op, "new") == 0 || strcmp(op, "call_constr") == 0
		|| strcmp(op, "call_method") == 0 || strcmp(op, "get_field") == 0
		|| strcmp(op, "set_field") == 0 || strcmp(op, "push_ptr") == 0){
		genObjectOps(q);
		return;

	}

	// entity block
	else if(strcmp(op, "entity") == 0 || strcmp(op, "end_entity") == 0){
		genEntityBlock(q);
		return;
	}

	// constructor
	else if(strcmp(op, "constr") == 0){
		genConstructorPrologue(q);
		return;
	}

	// end of the constructor
	else if(strcmp(op, "end_constr") == 0){
		genFunctionEpilogue(q);		// reuses the shared epilogue
		return;
	}

	// starting the method of the constrctor
	else if(strcmp(op, "method") == 0){
		genMethodPrologue(q);
	}

	// closing of the method
	else if(strcmp(op, "end_method") == 0){
		genFunctionEpilogue(q);		// reuses the shred epilogue
	}
	// If any constrcut not yet handled then emit it as a comment to prevent from crashing
	else{
		asmComment(op);
	}
}

/*
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
*/

static int getVarOffset(const char* var_name){
	extern SymTable* current_scope;
	if(!current_scope){
		return 0;
	}
	Symbol* sym = lookup(current_scope, var_name);
	if(!sym){
		return 0;
	}
	return -(sym->offset + sym->size);
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
	pending_arg_count = 0;
	const char* fname = q->arg1;
	
	asmBlank();
	asmEmit("%s:", fname);
	asmComment("-- prologue --");

	// Perform a lookup in the symbol table for getting the parameter count
	Symbol* fsym = lookup(global_scope, fname);

	if(fsym && fsym->attr.func.scope){
		setCurrentFuncScope(fsym->attr.func.scope);
	}
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

static void genConstructorPrologue(const Quad* q){
	const char* cname = q->arg1;

	asmBlank();
	asmEmit("%s:", cname);
	asmComment("-- constructor prologue --");

	// Search for the constructor symbol at the entity scope
	Symbol* csym = NULL;
	for(int b = 0; b < HASH_SIZE && !csym; b++){
		for(Symbol* s = global_scope->buckets[b]; s && !csym; s = s->next){
			if(s->kind == KIND_ENTITY){
				SymTable* es = s->attr.entity.scope;
				if(es){
					Symbol* c = lookup_local(es, cname);
					if(c && c->kind == KIND_CONSTRUCTOR){
						csym = c;
					}
				}
			}
		}
	}

	int param_count = csym ? csym->attr.ctor.param_count:0;
	int frame_size = 4 + param_count * 4 + 64;
	frame_size = (frame_size + 15) & ~15;

	asmEmit("    addi sp, sp, -%d", frame_size);
	asmEmit("    sw   ra, %d(sp)", frame_size - 4);
	asmEmit("    sw   s0, %d(sp)", frame_size - 8);
	asmEmit("    addi s0, sp, %d", frame_size);

	asmComment("save 'this' pointer");
	asmEmit("    sw   a0, -4(s0)");

	if(csym && csym->attr.ctor.param_list){
		ParamNode* p = csym->attr.ctor.param_list;
		int i = 1;
		while(p && i < MAX_ARG_REGS){
			asmEmit("    sw   %s, %d(s0)", arg_regs[i], getVarOffset(p->name));
			p = p->next;
			i++;
		}
	}

	if(csym && csym->attr.ctor.scope){
		setCurrentFuncScope(csym->attr.ctor.scope);
	}

	asmComment("-- constructor prologue end --");
	asmBlank();
	strncpy(current_func_name, cname, sizeof(current_func_name) - 1);
	current_frame_size = frame_size;
}

static void genMethodPrologue(const Quad* q){
	const char* mname = q->arg1;

	asmBlank();
	asmEmit("%s:",mname);
	asmComment("-- method prologue -- ");
	Symbol* msym = NULL;

	for (int b=0;b < HASH_SIZE && !msym; b++){
		for(Symbol* s = global_scope->buckets[b]; s && !msym; s= s->next) {
			if(s->kind == KIND_ENTITY) {
				SymTable* es= s->attr.entity.scope;
				if (es) {
					Symbol* m = lookup_local(es,mname);
					if(m && m->kind == KIND_METHOD)
						msym = m;
				}
			}
		}
	}
	int param_count = msym ? msym->attr.method.param_count: 0;
	int frame_size = 4 + param_count * 4 + 64;
	frame_size = (frame_size + 15) & ~15;

	asmEmit("    addi sp, sp, -%d", frame_size);
	asmEmit("    sw   ra, %d(sp)", frame_size - 4);
	asmEmit("    sw   s0, %d(sp)", frame_size - 8);
	asmEmit("    addi s0, sp, %d", frame_size);
	
	//save 'this' pointer
	asmEmit("    sw   a0, -4(s0)");

	if(msym && msym->attr.method.param_list){
		ParamNode* p = msym->attr.method.param_list;
		int i = 1;
		while(p && i < MAX_ARG_REGS) {
			asmEmit("    sw   %s, %d(s0)", arg_regs[i], getVarOffset(p->name));
			p = p->next;
			i++;
		}
	}

	if(msym && msym->attr.method.scope)
		setCurrentFuncScope(msym->attr.method.scope);

	asmComment("-- method prologue end --");
	asmBlank();

	strncpy(current_func_name, mname, sizeof(current_func_name) - 1);
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
	if(strcmp(q->op, "endfunc") == 0 || strcmp(q->op, "end_constr") == 0 || strcmp(q->op, "end_method") == 0){
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
		clearCurrentFuncScope();
		return;
	}
}


// genIO - a function that is used for handling the generation of equivalent assembly code for IO operations
void genIO(const Quad* q){
	// If the quad is a output quad that is "out"
	if(strcmp(q->op, "out") == 0){
		int service = 1;	// assign the default value (printing integer
		
		Symbol* sym = NULL;

		if(isStringLiteral(q->arg1)){
			service = 4;
		}
		else{
			sym = lookupForCodeGen(q->arg1);
			if(sym){
				switch(sym->datatype){
					case DT_FLOAT: service = 2; break;
					case DT_STRING: service = 4; break;
					default: service = 1; break;
				}
			}
		}

		// for safety, all the registers are spilled before performing the ecall which is similar to function call as simulator might clobber registers
		spillAllRegs();

		// Step 1: Load the value to be printed into a0 register
		// For constant use "li" instruction
		// For variable use "lw" instruction
		// For string address load the base address of the string using the name of the string label (defined in .data section). "la" is the type of instruction used
		// If it is a string pointer load it using "lw" with base address from the starting pointer of stack frame and offset computed from the symbol table
		if(isStringLiteral(q->arg1)){
			const char* lbl = getStringLabel(q->arg1);
			if(lbl){
				asmEmit("    la     a0, %s", lbl);
			}
			else{
				asmComment("String literal not found");
			}
		}
		else if(sym && sym->datatype == DT_STRING){
			if(sym->kind == KIND_ARRAY || sym->scope_level == 0){
				asmEmit("    la     a0, %s", sym->name);
			}
			else{
				int slot = -(sym->offset + sym->size);
				asmEmit("    addi   a0, s0, %d", slot);
			}
		}
		else{
			OperandType ot = getOperandType(q->arg1);
			if(ot == OT_CONST){
				asmEmit("    li     a0, %s", q->arg1);
			}
			else{
				asmEmit("    lw     a0, %d(s0)", getVarOffset(q->arg1));
			}
		}

		asmEmit("    li     a7, %d", service);
		asmEmit("    ecall");

	}
	else if(strcmp(q->op, "in") == 0){
		// Compute the correct read service number
		int service = 5; 	// Set the default value to read integer
		Symbol* sym = lookupForCodeGen(q->result);
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
			if(sym->scope_level == 0 || sym->kind == KIND_ARRAY){
				asmEmit("    la     a0, %s", sym->name);
			}
			else{
				int slot = -(sym->offset + sym->size);
				asmEmit("    addi   a0, s0, %d", slot);
			}
			asmEmit("    li     a1, %d", (sym->size > 0) ? sym->size : 64);
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
	initRegs();
	if(!asm_out){
		asm_out = stdout;
	}

	//pre pass register all names so getVarIdx works
	for(int i=0;i<IR_idx;i++){
		if(IR[i].arg1[0] && !isConstant(IR[i].arg1)) getVarIdx(IR[i].arg1);
		if(IR[i].arg2[0] && !isConstant(IR[i].arg2)) getVarIdx(IR[i].arg2);
		if(IR[i].result[0] && !isConstant(IR[i].result)) getVarIdx(IR[i].result);
	}

	//compute the next use block by block
	int block_start = 0;
	for(int i=0;i<IR_idx;i++){
		if(isBlockEnd(&IR[i])){
			computeNextUse(block_start,i);
			block_start=i+1;
		}else if(isBlockStart(&IR[i]) && i>0) {
			computeNextUse(block_start,i-1);
			block_start=i;
		}
	}
	if(block_start<IR_idx){
		computeNextUse(block_start,IR_idx-1);
	}
	//reset counters before generating
	count_loads=0;
	count_stores = 0;
	initRegs();


	// generating .data section with format of strings specified for outputting the strings or taking input
	// String literals are defined here using the format specifiers using the .asciz directive for internally generating a NULL-terminated string.
	genDataSection();

	// Generating .text section
	asmEmit(".section .text");
	asmEmit(".globl main");		// Entry point of the asm file
	
	for(int i = 0; i < IR_idx; i++){
		current_ir_idx = i; //tell getReg which instruction we are at
		genQuad(&IR[i]);
	}

	// Emit a program exit at the end of the .text section so that simulator does not fall off the end of main and crash. The service for the exit program ecall is 10.
	asmBlank();
	asmEmit("    li     a7, 10");
	asmEmit("    ecall");

	//print stats of register allocation
	fprintf(out(), "\n");
	fprintf(out(), "#--- Register Allocation Statistics -----\n");
	fprintf(out(), "# Strategy: %s\n", use_optimized_regalloc?"OPTIMIZED (next use aware)" : "BASIC (first dirty VAR)");
	fprintf(out(), "# Loads (lw/li): %d\n", count_loads);
	fprintf(out(), "# Stores (sw) : %d\n", count_stores);
	fprintf(out(), "# Total : %d\n", count_loads+count_stores);
	fprintf(out(), "# --------------------------------------\n");
}
