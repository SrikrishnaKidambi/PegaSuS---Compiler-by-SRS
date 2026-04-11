    .section .data

    # -- Global Scalar Variables --
        .align 2
    r1:    .word  t3
        .align 2
    r2:    .word  t4

    # -- global arrays --

    # -- string literals --

    .section .text
    .globl main

    add$ii:
    # -- prologue --
        addi   sp, sp, -80
        sw     ra, 76(sp)
        sw     s0, 72(sp)
        addi   s0, sp, 80
        sw     a0, 0(s0)
        sw     a1, 0(s0)
    # -- prologue end --

    # a
    # b
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: a
        addi t1, t2, b
        lw     a0, 0(s0)
        j      .Lepi_add$ii
    .Lepi_add$ii:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --


    add$iii:
    # -- prologue --
        addi   sp, sp, -80
        sw     ra, 76(sp)
        sw     s0, 72(sp)
        addi   s0, sp, 80
        sw     a0, 0(s0)
        sw     a1, 0(s0)
        sw     a2, 0(s0)
    # -- prologue end --

    # a
    # b
    # c
        addi t3, t2, b
        addi t4, t3, c
        lw     a0, 0(s0)
        j      .Lepi_add$iii
    .Lepi_add$iii:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --

    # spill all registers
        li     a0, 3
        li     a1, 5
        call   add$ii
        sw     a0, 0(s0)
        li   t1, t3
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -8(s0)
        li     a0, 1
        li     a1, 2
        li     a2, 3
        call   add$iii
        sw     a0, 0(s0)
        li   t1, t4
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -12(s0)
        lw     a0, -4(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -8(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: OPTIMIZED (next use aware)
# Loads (lw/li): 19
# Stores (sw) : 13
# Total : 32
# --------------------------------------
