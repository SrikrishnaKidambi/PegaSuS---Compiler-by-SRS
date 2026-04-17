    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0
        .align 2
    a:    .word  1
        .align 2
    b:    .word  2

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Going to return the"
    str_1:    .asciz "The result is:"

    .text
    .globl main
    main:
    # -- global scope --
        addi sp, sp, -208
        sw ra, 204(sp)
        sw s0, 200(sp)
        addi s0, sp, 208
        j global_body
    #  -- global scope end --


    add_ii:
    # -- prologue --
        addi   sp, sp, -208
        sw     ra, 204(sp)
        sw     s0, 200(sp)
        addi   s0, sp, 208
        sw     a0, -8(s0)
        sw     a1, -12(s0)
    # -- prologue end --

    # x
    # y
    # spill all registers
        la     a0, str_0
        li     a7, 4
        ecall
        lw   t1, -8(s0)
        lw   t2, -12(s0)
        add  t3, t1, t2
        mv     a0, t3
        j      Lepi_add_ii
    Lepi_add_ii:
    # -- epilogue --
        lw     ra, 204(sp)
        lw     s0, 200(sp)
        addi   sp, sp, 208
        ret
    # -- epilogue end --


    global_body:
    # -- Global body --
        li   t1, 1
        li   t2, 2
    # spill all registers
        la   t0, a
        sw   t1, 0(t0)
        la   t0, b
        sw   t2, 0(t0)
        la     t0, a
        lw     a0, 0(t0)
        la     t0, b
        lw     a1, 0(t0)
        call   add_ii
        sw     a0, -84(s0)
        lw   t1, -84(s0)
        mv   t2, t1
    # spill all registers
        la   t0, result
        sw   t2, 0(t0)
        la     a0, str_1
        li     a7, 4
        ecall
    # spill all registers
        la     t0, result
        lw     a0, 0(t0)
        li     a7, 1
        ecall

    # -- global scope epilogue --
        lw ra, 204(sp)
        lw s0, 200(sp)
        addi sp, sp, 208
        li a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 16
# Stores (sw) : 8
# Total : 24
# --------------------------------------
