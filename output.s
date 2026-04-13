    .data

    # -- Global Scalar Variables --
        .align 2
    res:    .word  0
        .align 2
    target:    .word  1

    # -- global arrays --

    # -- string literals --

    .text
    .globl main
    main:
    # -- global scope --
        addi sp, sp, -336
        sw ra, 332(sp)
        sw s0, 328(sp)
        addi s0, sp, 336
        j global_body
    #  -- global scope end --


    binarySearch_i:
    # -- prologue --
        addi   sp, sp, -304
        sw     ra, 300(sp)
        sw     s0, 296(sp)
        addi   s0, sp, 304
        sw     a0, -8(s0)
    # -- prologue end --

    # tgt
        li   t1, 0
        li   t2, 4
    # spill all registers
        sw t1, -32(s0)
        sw t2, -36(s0)

    L0:
    # <
        lw   t1, -32(s0)
        lw   t2, -36(s0)
        slt  t3, t1, t2
        beqz   t3, L1
        sw t3, -108(s0)
        add  t3, t1, t2
    li t4, 2
        div  t5, t3, t4
    li t6, 4
        mul  t0, t5, t6
    # array access []
        addi s1, s0, -12
        add  s2, s1, t0
        lw   s2, 0(s2)
        sw t0, -112(s0)
    # ==
        lw   t0, -8(s0)
        sub  s1, s2, t0
        seqz s1, s1
        beqz   s1, L2
        sw s1, -116(s0)
        mv     a0, t5
        j      Lepi_binarySearch_i
    # spill all registers
        sw t5, -40(s0)
        j      L3
    # spill all registers

    L2:
        lw   t1, -40(s0)
    li t2, 4
        mul  t3, t1, t2
    # array access []
        addi t4, s0, -12
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw t3, -120(s0)
    # <
        lw   t3, -8(s0)
        slt  t4, t5, t3
        beqz   t4, L4
        sw t4, -124(s0)
        addi t4, t1, 1
    # spill all registers
        sw t4, -32(s0)
        j      L3
    # spill all registers

    L4:
        lw   t1, -40(s0)
        addi t2, t1, -1
    # spill all registers
        sw t2, -36(s0)

    L3:
    # spill all registers
        j      L0
    # spill all registers

    L1:
    li t1, 0
        addi t2, t1, -1
        mv     a0, t2
        j      Lepi_binarySearch_i
    Lepi_binarySearch_i:
    # -- epilogue --
        lw     ra, 300(sp)
        lw     s0, 296(sp)
        addi   sp, sp, 304
        ret
    # -- epilogue end --


    global_body:
    # -- Global body --
        li   t3, 1
    # spill all registers
        sw t3, -8(s0)
        lw     a0, -8(s0)
        call   binarySearch_i
        sw     a0, -108(s0)
        lw   t1, -108(s0)
        mv   t2, t1
        mv     a0, t2
    # spill all registers
        sw t2, -12(s0)
        li     a7, 1
        ecall

    # -- global scope epilogue --
        lw ra, 332(sp)
        lw s0, 328(sp)
        addi sp, sp, 336
        li a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 23
# Stores (sw) : 16
# Total : 39
# --------------------------------------
