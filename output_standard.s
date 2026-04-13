    .data

    # -- Global Scalar Variables --
        .align 2
    x:    .word  1
        .align 2
    y:    .word  2
        .align 2
    n1:    .word  10
        .align 2
    n2:    .word  5

    # -- global arrays --

    # -- string literals --

    .text
    .globl main
    main:
    # -- global scope --
        addi sp, sp, -288
        sw ra, 284(sp)
        sw s0, 280(sp)
        addi s0, sp, 288
        j global_body
    #  -- global scope end --


    global_body:
    # -- Global body --
    # assign
        li   t1, 1
    # assign
        li   t2, 2
    # assign
        li   t3, 10
    # assign
        li   t4, 5
    # >
        slt  t5, t2, t1
        beqz   t5, L0
        sw t5, -24(s0)
    # assign
        li   t5, 0
    # spill all registers
        sw t1, -8(s0)
        sw t2, -12(s0)
        sw t3, -16(s0)
        sw t4, -20(s0)
        sw t5, -8(s0)

    L2:
    # <
    lw t1, -8(s0)
    lw t1, -8(s0)
    lw t2, -16(s0)
    la   t0, n1
    lw   t2, 0(t0)
        slt  t3, t1, t2
        beqz   t3, L3
        sw t3, -28(s0)
    # spill all registers
        la     a0, str_0
        li     a7, 4
        ecall
    # spill all registers
        la     a0, str_1
        li     a7, 4
        ecall
    # +
    lw t1, -8(s0)
    lw t1, -8(s0)
    li t2, 1
        add  t3, t1, t2
    # assign
        mv   t1, t3
    # spill all registers
        sw t1, -8(s0)
        j      L2
    # spill all registers

    L3:
    # spill all registers
        j      L1
    # spill all registers

    L0:
    # <
    lw t1, -8(s0)
    la   t0, x
    lw   t1, 0(t0)
    lw t2, -12(s0)
    la   t0, y
    lw   t2, 0(t0)
        slt  t3, t1, t2
        beqz   t3, L4
        sw t3, -32(s0)
    # assign
        li   t3, 0
    # spill all registers
        sw t3, -8(s0)

    L5:
    # <
    lw t1, -8(s0)
    lw t1, -8(s0)
    lw t2, -20(s0)
    la   t0, n2
    lw   t2, 0(t0)
        slt  t3, t1, t2
        beqz   t3, L6
        sw t3, -36(s0)
    # spill all registers
        la     a0, str_2
        li     a7, 4
        ecall
    # spill all registers
        la     a0, str_1
        li     a7, 4
        ecall
    # +
    lw t1, -8(s0)
    lw t1, -8(s0)
    li t2, 1
        add  t3, t1, t2
    # assign
        mv   t1, t3
    # spill all registers
        sw t1, -8(s0)
        j      L5
    # spill all registers

    L6:
    # spill all registers
        j      L1
    # spill all registers

    L4:
    # spill all registers
        la     a0, str_3
        li     a7, 4
        ecall
    # spill all registers

    L1:

    # -- global scope epilogue --
        lw ra, 284(sp)
        lw s0, 280(sp)
        addi sp, sp, 288
        li a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 27
# Stores (sw) : 12
# Total : 39
# --------------------------------------
