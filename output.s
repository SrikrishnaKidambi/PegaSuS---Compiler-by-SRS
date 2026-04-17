    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0
        .align 2
    n:    .word  3

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


    fact_i:
    # -- prologue --
        addi   sp, sp, -224
        sw     ra, 220(sp)
        sw     s0, 216(sp)
        addi   s0, sp, 224
        sw     a0, -8(s0)
    # --initialize local arrays --
    # -- prologue end --

    # n
    # ==
        lw   t1, -8(s0)
    li t2, 0
        sub  t3, t1, t2
        seqz t3, t3
        beqz   t3, L0
        sw t3, -76(s0)
        li     a0, 1
        j      Lepi_fact_i
    # spill all registers
        j      L1
    # spill all registers

    L0:
    # spill all registers

    L1:
        lw   t1, -8(s0)
        addi t2, t1, -1
    # spill all registers
        lw     a0, -80(s0)
        call   fact
        sw     a0, -84(s0)
        lw   t1, -84(s0)
        lw   t2, -8(s0)
        mul  t3, t1, t2
        mv     a0, t3
        j      Lepi_fact_i
    Lepi_fact_i:
    # -- epilogue --
        lw     ra, 220(sp)
        lw     s0, 216(sp)
        addi   sp, sp, 224
        ret
    # -- epilogue end --


    global_body:
    # -- Global body --
        li   t2, 3
    # spill all registers
        sw t2, -8(s0)
        lw     a0, -8(s0)
        call   fact_i
        sw     a0, -80(s0)
        lw   t1, -80(s0)
        mv   t2, t1
        mv     a0, t2
    # spill all registers
        sw t2, -12(s0)
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
# Loads (lw/li): 14
# Stores (sw) : 8
# Total : 22
# --------------------------------------
