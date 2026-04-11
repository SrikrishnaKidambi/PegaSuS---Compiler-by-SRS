    .section .data

    # -- Global Scalar Variables --
        .align 2
    x:    .word  
        .align 2
    y:    .word  
        .align 2
    z:    .word  

    # -- global arrays --

    # -- string literals --

    .section .text
    .globl main

    MathUtil_:
    # -- constructor prologue --
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
    # save 'this' pointer
        sw   a0, -4(s0)
    # -- constructor prologue end --

    .Lepi_MathUtil_:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --


    add_ii:
    # -- method prologue -- 
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
        sw   a0, -4(s0)
        sw   a1, 0(s0)
        sw   a2, 0(s0)
    # -- method prologue end --

    # a
    # b
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: a
        addi t1, t2, b
        lw     a0, 0(s0)
        j      .Lepi_add_ii
    .Lepi_add_ii:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --


    add_iii:
    # -- method prologue -- 
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
        sw   a0, -4(s0)
        sw   a1, 0(s0)
        sw   a2, 0(s0)
        sw   a3, 0(s0)
    # -- method prologue end --

    # a
    # b
    # c
        addi t3, t2, b
        addi t4, t3, c
        lw     a0, 0(s0)
        j      .Lepi_add_iii
    .Lepi_add_iii:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --


    add_i:
    # -- method prologue -- 
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
        sw   a0, -4(s0)
        sw   a1, 0(s0)
    # -- method prologue end --

    # a
        addi t5, t2, 10
        lw     a0, 0(s0)
        j      .Lepi_add_i
    .Lepi_add_i:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --

    # new object
    # spill all registers
        li   a0, 0
        call malloc
        sw   a0, 0(s0)
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call_constr
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        lw   a0, 0(sp)
        addi sp, sp, 4
        call MathUtil_
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 5
        li   a2, 6
        lw a0, 0(sp)
        addi sp, sp, 4
        call add_ii
        sw  a0, 0(s0)
        li   t1, t4
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -8(s0)
        li   a1, 1
        li   a2, 2
        li   a3, 3
        lw a0, 0(sp)
        addi sp, sp, 4
        call add_iii
        sw  a0, 0(s0)
        li   t1, t5
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -12(s0)
        li   a1, 10
        lw a0, 0(sp)
        addi sp, sp, 4
        call add_i
        sw  a0, 0(s0)
        li   t1, t6
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -16(s0)
        lw     a0, -4(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -8(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -12(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 37
# Stores (sw) : 29
# Total : 66
# --------------------------------------
