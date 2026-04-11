    .section .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  t10

    # -- global arrays --

    # -- string literals --

    .section .text
    .globl main

    helper:
    # -- prologue --
        addi   sp, sp, -80
        sw     ra, 76(sp)
        sw     s0, 72(sp)
        addi   s0, sp, 80
        sw     a0, 0(s0)
        sw     a1, 0(s0)
    # -- prologue end --

    # x
    # y
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: x
        addi t1, t2, y
        lw     a0, 0(s0)
        j      .Lepi_helper
    .Lepi_helper:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --


    add:
    # -- prologue --
        addi   sp, sp, -96
        sw     ra, 92(sp)
        sw     s0, 88(sp)
        addi   s0, sp, 96
        sw     a0, 0(s0)
        sw     a1, 0(s0)
        sw     a2, 0(s0)
        sw     a3, 0(s0)
        sw     a4, 0(s0)
        sw     a5, 0(s0)
        sw     a6, 0(s0)
        sw     a7, 0(s0)
    # -- prologue end --

    # a
    # b
    # c
    # d
    # e
    # f
    # g
    # h
    # ISSUE: symbol not found for addres lookup
    lw t4, 0(s0) # unknown: a
        addi t3, t4, b
        li   t5, t1
    # ISSUE: symbol not found for addres lookup
    lw t0, 0(s0) # unknown: c
        addi t6, t0, d
        li   s1, t2
    # ISSUE: symbol not found for addres lookup
    lw s3, 0(s0) # unknown: e
        addi s2, s3, f
        li   s4, t3
    # ISSUE: symbol not found for addres lookup
    lw s6, 0(s0) # unknown: g
        addi s5, s6, h
        li   s7, t4
        addi s8, t5, p2
        li   s9, t5
        addi s10, s4, p4
        li   s11, t6
        addi t1, s9, p6
        li   t1, t7
    # spill all registers
        lw     a0, 0(s0)
        lw     a1, 0(s0)
        call   helper
        sw     a0, 0(s0)
        li   t1, t8
        addi t2, t1, b
        li   t3, t9
        lw     a0, 0(s0)
        j      .Lepi_add
    .Lepi_add:
    # -- epilogue --
        lw     ra, 92(sp)
        lw     s0, 88(sp)
        addi   sp, sp, 96
        ret
    # -- epilogue end --

    # spill all registers
        li     a0, 1
        li     a1, 2
        li     a2, 3
        li     a3, 4
        li     a4, 5
        li     a5, 6
        li     a6, 7
        li     a7, 8
        call   add
        sw     a0, 0(s0)
        li   t1, t10
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -8(s0)
        lw     a0, -4(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 34
# Stores (sw) : 17
# Total : 51
# --------------------------------------
