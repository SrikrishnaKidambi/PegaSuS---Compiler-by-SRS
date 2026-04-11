    .data

    # -- Global Scalar Variables --
        .align 2
    n:    .word  10

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Good Afternoon"
    str_1:    .asciz "\n"

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


    greet:
    # -- prologue --
        addi   sp, sp, -208
        sw     ra, 204(sp)
        sw     s0, 200(sp)
        addi   s0, sp, 208
    # -- prologue end --

        la   t1, str_0
        lw     a0, -8(s0)
        j      Lepi_greet
    Lepi_greet:
    # -- epilogue --
        lw     ra, 204(sp)
        lw     s0, 200(sp)
        addi   sp, sp, 208
        ret
    # -- epilogue end --


    global_body:
    # -- Global body --
        li   t2, 10
        li   t3, 0
    # spill all registers
        sw t2, -8(s0)
        sw t3, -8(s0)

    L0:
    # <
    lw t1, -8(s0)
    la   t0, n
    lw   t2, 0(t0)
        slt  t3, t1, t2
        beqz   t3, L1
        sw t3, -16(s0)
    # spill all registers
        call   greet
        sw     a0, -20(s0)
        lw t1, -20(s0)
        mv   t2, t1
    # spill all registers
        addi   a0, s0, -8
        li     a7, 4
        ecall
    # spill all registers
        la     a0, str_1
        li     a7, 4
        ecall
    lw t2, -8(s0)
        addi t1, t2, 1
        mv   t2, t1
    # spill all registers
        sw t1, -24(s0)
        sw t2, -8(s0)
        j      L0
    # spill all registers

    L1:

    # -- global scope epilogue --
        lw ra, 220(sp)
        lw s0, 216(sp)
        addi sp, sp, 224
        li a7, 10
        ecall
