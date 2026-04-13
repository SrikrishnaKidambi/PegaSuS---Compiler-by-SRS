    .data

    # -- Global Scalar Variables --
        .align 2
    sum:    .word  0

    # -- global arrays --

    # -- string literals --

    .text
    .globl main
    main:
    # -- global scope --
        addi sp, sp, -224
        sw ra, 220(sp)
        sw s0, 216(sp)
        addi s0, sp, 224
        j global_body
    #  -- global scope end --


    mul_ii:
    # -- prologue --
        addi   sp, sp, -224
        sw     ra, 220(sp)
        sw     s0, 216(sp)
        addi   s0, sp, 224
        sw     a0, -8(s0)
        sw     a1, -12(s0)
    # -- prologue end --

    # x
    # y
    lw t2, -8(s0)
    lw t2, -8(s0)
    lw t3, -12(s0)
    lw t3, -12(s0)
        mul  t1, t2, t3
        mv     a0, t1
        j      Lepi_mul_ii
    Lepi_mul_ii:
    # -- epilogue --
        lw     ra, 220(sp)
        lw     s0, 216(sp)
        addi   sp, sp, 224
        ret
    # -- epilogue end --


    global_body:
    # -- Global body --
        li   t4, 0
        li   t5, 1
    # spill all registers
        sw t4, -8(s0)
        sw t5, -8(s0)

    L0:
    # <
    lw t1, -8(s0)
    lw t1, -8(s0)
    li t2, 5
        slt  t3, t1, t2
        beqz   t3, L1
        sw t3, -16(s0)
        addi t3, t1, 1
        mul  t4, t1, t3
    lw t6, -8(s0)
    la   t0, sum
    lw   t6, 0(t0)
        add  t5, t6, t4
        mv   t6, t5
        addi t0, t1, 1
        mv   t1, t0
    # spill all registers
        sw t1, -8(s0)
        sw t6, -8(s0)
        j      L0
    # spill all registers

    L1:
    # spill all registers
        lw     a0, -8(s0)
        li     a7, 1
        ecall

    # -- global scope epilogue --
        lw ra, 220(sp)
        lw s0, 216(sp)
        addi sp, sp, 224
        li a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 12
# Stores (sw) : 9
# Total : 21
# --------------------------------------
