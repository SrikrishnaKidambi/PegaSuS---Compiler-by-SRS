.data

    # -- Global Scalar Variables --
        .align 2
    z:    .word  0

    # -- global arrays --

    # -- string literals --

    .text
    .globl main
    main:
    # -- global scope --
        addi sp, sp, -240
        sw ra, 236(sp)
        sw s0, 232(sp)
        addi s0, sp, 240
        j global_body
    #  -- global scope end --


    add_ii:
    # -- prologue --
        addi   sp, sp, -240
        sw     ra, 236(sp)
        sw     s0, 232(sp)
        addi   s0, sp, 240
        sw     a0, -12(s0)
        sw     a1, -16(s0)
    # --initialize local arrays --
    # -- prologue end --

    # x
    # y
        lw   t1, -12(s0)
        lw   t2, -16(s0)
        add  t3, t1, t2
        mv     a0, t3
        j      Lepi_add_ii
    Lepi_add_ii:
    # -- epilogue --
        lw     ra, 236(sp)
        lw     s0, 232(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --


    helper_ii:
    # -- prologue --
        addi   sp, sp, -240
        sw     ra, 236(sp)
        sw     s0, 232(sp)
        addi   s0, sp, 240
        sw     a0, -12(s0)
        sw     a1, -16(s0)
    # --initialize local arrays --
    # -- prologue end --

    # x
    # y
    # spill all registers
        lw     a0, -12(s0)
        lw     a1, -16(s0)
        call   add_ii
        sw     a0, -84(s0)
        lw   t1, -84(s0)
        mv   t2, t1
        lw   t3, -12(s0)
        add  t4, t3, t2
        mv     a0, t4
        j      Lepi_helper_ii
    Lepi_helper_ii:
    # -- epilogue --
        lw     ra, 236(sp)
        lw     s0, 232(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --


    global_body:
    # -- Global body --
    # spill all registers
        li     a0, 3
        li     a1, 5
        call   helper_ii
        sw     a0, -84(s0)
        lw   t1, -84(s0)
        mv   t2, t1
        mv     a0, t2
    # spill all registers
        la   t0, z
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
# Loads (lw/li): 15
# Stores (sw) : 11
# Total : 26
# --------------------------------------
