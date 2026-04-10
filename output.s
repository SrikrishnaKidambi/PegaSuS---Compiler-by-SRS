    .section .data

    # -- Global Scalar Variables --

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Hello"

    .section .text
    .globl main

    Calculator:
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
    # spill all registers
        addi   a0, s0, -8
        li     a7, 4
        ecall
    .Lepi_Calculator:
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
    # call_constr
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, "Hello"
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Calculator

        li     a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 0
# Stores (sw) : 0
# Total : 0
# --------------------------------------
