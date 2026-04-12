    .section .data

    # -- Global Scalar Variables --
        .align 2
    ans:    .word  

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Casio"

    .section .text
    .globl main

    Calculator_s:
    # -- constructor prologue --
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
    # save 'this' pointer
        sw   a0, -4(s0)
        sw   a1, 0(s0)
    # -- constructor prologue end --

    # m
    # set_field
        lw  t1, -4(s0)
        lw  t2, 0(s0)
        sw  t2, 0(t1)
    .Lepi_Calculator_s:
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

    # new object
    # spill all registers
        li   a0, 8
        call malloc
        sw   a0, -16(s0)
    # push_ptr: load obj pointer into a0
        lw   a0, -16(s0)
    # call_constr
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, "Casio"
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Calculator_s
    # push_ptr: load obj pointer into a0
        lw   a0, -16(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 3
        li   a2, 5
        lw a0, 0(sp)
        addi sp, sp, 4
        call add_ii
        sw  a0, 0(s0)
        li   t1, t1
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -24(s0)
        lw     a0, -20(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 20
# Stores (sw) : 15
# Total : 35
# --------------------------------------
