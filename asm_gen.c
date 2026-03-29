#include "asm_gen.h"
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
