    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0
        .align 2
    num1:    .word  48
        .align 2
    num2:    .word  18

    # -- global arrays --

    # -- string literals --

    .text
    .globl main
    main:
    # -- global scope --
        addi sp, sp, -240
        sw ra, 236(sp)
        sw s0, 232(sp)
        addi s0, sp, 236
        j global_body
    #  -- global scope end --


    gcd_ii:
    # -- prologue --
        addi   sp, sp, -240
        sw     ra, 236(sp)
        sw     s0, 232(sp)
        addi   s0, sp, 236
        sw     a0, -8(s0)
        sw     a1, -12(s0)
    # --initialize local arrays --
    # -- prologue end --

    # a
    # b
    # ==
        lw   t1, -12(s0)
    li t2, 0
        sub  t3, t1, t2
        seqz t3, t3
        beqz   t3, L0
        sw   t3, -80(s0)
        lw     a0, -8(s0)
        j      Lepi_gcd_ii
    # spill all registers
        j      L1
    # spill all registers

    L0:
    # spill all registers

    L1:
        lw   t1, -8(s0)
        lw   t2, -12(s0)
        rem  t3, t1, t2
        mv     a0, t2
        mv     a1, t3
    # spill all registers
        call   gcd_ii
        sw     a0, -84(s0)
        lw     a0, -84(s0)
        j      Lepi_gcd_ii
    Lepi_gcd_ii:
    # -- epilogue --
        lw     ra, 236(sp)
        lw     s0, 232(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --


    global_body:
    # -- Global body --
        li   t1, 48
        li   t2, 18
        mv     a0, t1
        mv     a1, t2
    # spill all registers
        la   t0, num1
        sw   t1, 0(t0)
        la   t0, num2
        sw   t2, 0(t0)
        call   gcd_ii
        sw     a0, -84(s0)
        lw   t1, -84(s0)
        mv   t2, t1
        mv     a0, t2
    # spill all registers
        la   t0, result
        sw   t2, 0(t0)
        li     a7, 1
        ecall

    # -- global scope epilogue --
        lw ra, 236(sp)
        lw s0, 232(sp)
        addi sp, sp, 240
        li a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 13
# Stores (sw) : 10
# Total : 23
# --------------------------------------
