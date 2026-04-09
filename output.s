    .section .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0
    b:    .byte  0
        .align 2
    weight:    .word  
        .align 2
    i:    .word  0
        .align 2
    sum:    .word  0
        .align 2
    y:    .word  3
        .align 2
    heavy:    .word  
        .align 2
    r1:    .word  t6
        .align 2
    r2:    .word  t7
        .align 2
    r3:    .word  t8
        .align 2
    r4:    .word  t9
        .align 2
    r5:    .word  t10
        .align 2
    age:    .word  
    str:    .asciz "hello"

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "hello"

    .section .text
    .globl main
    # get_field
        lw  t1, 0(s0)
        lw  t2, 0(t1)
    # ISSUE: symbol not found for addres lookup
    sw t2, 0(s0) # unknown: t0
    # set_field
        lw  t1, -4(s0)
        lw  t3, 0(s0)
        sw  t3, 0(t1)
        li   t1, 0
        li   t3, 0
        li   t1, 0
    # spill all registers
    # (debug) store ins. to spill
    sw t1, 0(s0)
    # (debug) store ins. to spill
    sw t3, -4(s0)

    L0:
    # <
    lw t1, 0(s0)
    li t2, 5
        slt  t3, t1, t2
    # ISSUE: symbol not found for addres lookup
    sw t3, 0(s0) # unknown: t1
        beqz   t3, L1
    lw t2, 0(s0)
    # ISSUE: symbol not found for addres lookup
    lw t3, 0(s0) # unknown: type.width
        mul  t1, t2, t3
    # array access []
    # ISSUE: symbol not found for addres lookup
        la   t4, 0(s0) # unknown: arr
        add  t5, t4, t1
        lw   t5, 0(t5)
    # ISSUE: symbol not found for addres lookup
    sw t5, 0(s0) # unknown: t4
    lw t4, -4(s0)
        addi t1, t4, t4
        li   t4, t5
        addi t6, t2, 1
        li   t2, t2
        j      L0
    # spill all registers
    # (debug) store ins. to spill
    sw t2, 0(s0)
    # (debug) store ins. to spill
    sw t4, -4(s0)

    L1:
    # spill all registers
        lw     a0, -8(s0)
        li     a7, 1
        ecall
        li   t1, 3
    # ISSUE: symbol not found for addres lookup
    lw t3, 0(s0) # unknown: x
        addi t2, t3, y
        li   t4, t6
        addi t5, t3, 0
        li   t6, t7
        mul  t0, t3, t1
        li   s1, t8
    # (debug) store ins. to spill
    sw t1, -8(s0)
    # (debug) store ins. to spill
    sw t4, -12(s0)
    lw t4, -8(s0)
        div  t1, t3, t4
    # (debug) store ins. to spill
    sw t6, -16(s0)
        li   t6, t9
    # (debug) store ins. to spill
    sw t6, -24(s0)
        rem  t6, t3, t4
    # (debug) store ins. to spill
    sw s1, -20(s0)
        li   s1, t10
    # spill all registers
    # (debug) store ins. to spill
    sw s1, -28(s0)
        lw     a0, -16(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -20(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -24(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -28(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -32(s0)
        li     a7, 1
        ecall
        li   t1, false
    # &&
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: a
        and  t3, t2, t1
    # (debug) store ins. to spill
    sw t1, -32(s0)
    # ISSUE: symbol not found for addres lookup
    sw t3, 0(s0) # unknown: t11
        li   t1, t11
    # ||
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: a
    lw t4, -32(s0)
        or   t5, t2, t4
    # ISSUE: symbol not found for addres lookup
    sw t5, 0(s0) # unknown: t12
        li   t2, t12
    # !
    # ISSUE: symbol not found for addres lookup
    lw t4, 0(s0) # unknown: a
        seqz t6, t4
    # ISSUE: symbol not found for addres lookup
    sw t6, 0(s0) # unknown: t13
        li   t4, t13
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -12(s0)
    # (debug) store ins. to spill
    sw t2, -16(s0)
    # (debug) store ins. to spill
    sw t4, -20(s0)
        lw     a0, -16(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -20(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -24(s0)
        li     a7, 1
        ecall
        la   t1, str_0
        li   t2, 0
    # >
    # ISSUE: symbol not found for addres lookup
    lw t3, 0(s0) # unknown: x
    li t4, 3
        slt  t5, t4, t3
    # ISSUE: symbol not found for addres lookup
    sw t5, 0(s0) # unknown: t14
        beqz   t5, L2
    # >
    # ISSUE: symbol not found for addres lookup
    lw t3, 0(s0) # unknown: x
    li t4, 4
        slt  t5, t4, t3
    # ISSUE: symbol not found for addres lookup
    sw t5, 0(s0) # unknown: t15
        beqz   t5, L4
        li   t2, 100
        j      L5
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -33(s0)
    # (debug) store ins. to spill
    sw t2, -41(s0)

    L4:
    # >
    # ISSUE: symbol not found for addres lookup
    lw t1, 0(s0) # unknown: x
    li t2, 2
        slt  t3, t2, t1
    # ISSUE: symbol not found for addres lookup
    sw t3, 0(s0) # unknown: t16
        beqz   t3, L6
        li   t1, 50
        j      L5
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -41(s0)

    L6:
        li   t1, 10
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -41(s0)

    L5:
        j      L3
    # spill all registers

    L2:
        li   t1, 0
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -41(s0)

    L3:
    # spill all registers
        lw     a0, -45(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, 0(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, 0(s0)
        li     a7, 1
        ecall
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        lw a0, 0(sp)
        addi sp, sp, 4
        call getAge$
        sw  a0, 0(s0)
        li   t1, t17
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -45(s0)
        lw a0, 0(sp)
        addi sp, sp, 4
        call getWeight$
        sw  a0, 0(s0)
        li   t1, t18
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -49(s0)
        lw     a0, -49(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -53(s0)
        li     a7, 1
        ecall
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 50
        lw a0, 0(sp)
        addi sp, sp, 4
        call isHeavierThan$i
        sw  a0, 0(s0)
        li   t1, t19
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -53(s0)
        lw     a0, -57(s0)
        li     a7, 1
        ecall
    # get_field
        lw  t1, 0(s0)
        lw  t2, 0(t1)
    # ISSUE: symbol not found for addres lookup
    sw t2, 0(s0) # unknown: t20
    li t3, 0
    # ISSUE: symbol not found for addres lookup
    lw t4, 0(s0) # unknown: array.cols
        mul  t1, t3, t4
        addi t5, t1, 0
    # ISSUE: symbol not found for addres lookup
    lw t0, 0(s0) # unknown: type.width
        mul  t6, t5, t0
    # array access []
    # ISSUE: symbol not found for addres lookup
        la   s1, 0(s0) # unknown: mat
        add  t1, s1, t6
        lw   t1, 0(t1)
    # ISSUE: symbol not found for addres lookup
    sw t1, 0(s0) # unknown: t24
    # spill all registers
        lw     a0, 0(s0)
        li     a7, 1
        ecall
    li t2, 1
    # ISSUE: symbol not found for addres lookup
    lw t3, 0(s0) # unknown: array.cols
        mul  t1, t2, t3
        addi t4, t1, 1
    # ISSUE: symbol not found for addres lookup
    lw t6, 0(s0) # unknown: type.width
        mul  t5, t4, t6
    # array access []
    # ISSUE: symbol not found for addres lookup
        la   t0, 0(s0) # unknown: mat
        add  s1, t0, t5
        lw   s1, 0(s1)
    # ISSUE: symbol not found for addres lookup
    sw s1, 0(s0) # unknown: t28
    # spill all registers
        lw     a0, 0(s0)
        li     a7, 1
        ecall
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: c
        addi t1, t2, 5
        li   t2, t29
    # spill all registers
        lw     a0, 0(s0)
        li     a7, 1
        ecall
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: c
        addi t1, t2, -3
        li   t2, t30
    # spill all registers
        lw     a0, 0(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall
