    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0

    # -- global arrays --

    # -- string literals --

    .text
    .globl main
main:
    # -- global scope --
        addi sp, sp, -352
        sw ra, 348(sp)
        sw s0, 344(sp)
        addi s0, sp, 352
        j global_body
    #  -- global scope end --


binarySearch_i:
    # -- prologue --
        addi   sp, sp, -320
        sw     ra, 316(sp)
        sw     s0, 312(sp)
        addi   s0, sp, 320
        sw     a0, -8(s0)
    # --initialize local arrays --
    # init local array
       li    t0,1
       sw    t0,-28(s0)
       li    t0,2
       sw    t0,-24(s0)
       li    t0,3
       sw    t0,-20(s0)
       li    t0,4
       sw    t0,-16(s0)
       li    t0,5
       sw    t0,-12(s0)
    # --initialize local scalars --
    # init local scalar search_target at offset -24
    # init local scalar l at offset -12
        li    t0, 0
        sw    t0, -12(s0)
    # init local scalar r at offset -16
        li    t0, 4
        sw    t0, -16(s0)
    # -- prologue end --

    # tgt
        li   t1, 0
        li   t2, 4
        lw   t3, -8(s0)
        mv   t4, t3
    # spill all registers
        sw   t1, -12(s0)
        sw   t2, -16(s0)
        sw   t4, -24(s0)

L0:
    # <
        lw   t1, -12(s0)
        lw   t2, -16(s0)
        slt  t3, t1, t2
    # ==
        sub  t4, t1, t2
        seqz t4, t4
    # ||
        or   t5, t3, t4
        beqz   t5, L1
        sw   t5, -112(s0)
        add  t5, t1, t2
    li t6, 2
        div  s1, t5, t6
        mv   s2, s1
    li s3, 4
        mul  s4, s2, s3
    # array access []
        addi s5, s0, -28
        add  s6, s5, s4
        lw   s6, 0(s6)
        sw   s4, -116(s0)
    # ==
        lw   s4, -24(s0)
        sub  s5, s6, s4
        seqz s5, s5
        beqz   s5, L2
        sw   s5, -120(s0)
        mv     a0, s2
        j      Lepi_binarySearch_i
    # spill all registers
        sw   s2, -20(s0)
        j      L3
    # spill all registers

L2:
    # spill all registers

L3:
        lw   t1, -20(s0)
    li t2, 4
        mul  t3, t1, t2
    # array access []
        addi t4, s0, -28
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -124(s0)
    # <
        lw   t3, -24(s0)
        slt  t4, t5, t3
        beqz   t4, L4
        sw   t4, -128(s0)
        addi t4, t1, 1
        mv   t6, t4
    # spill all registers
        sw   t6, -12(s0)
        j      L5
    # spill all registers

L4:
        lw   t1, -20(s0)
        addi t2, t1, -1
        mv   t3, t2
    # spill all registers
        sw   t3, -16(s0)

L5:
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
        lw     ra, 316(sp)
        lw     s0, 312(sp)
        addi   sp, sp, 320
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
    # spill all registers
        li     a0, 1
        call   binarySearch_i
        sw     a0, -112(s0)
        lw   t1, -112(s0)
        mv   t2, t1
        mv     a0, t2
    # spill all registers
        la   t0, result
        sw   t2, 0(t0)
        li     a7, 1
        ecall
        li     a0, 10
        li     a7, 11
        ecall
        li     a0, 10
        li     a7, 1
        ecall
        li     a0, 10
        li     a7, 11
        ecall

    # -- global scope epilogue --
        lw ra, 348(sp)
        lw s0, 344(sp)
        addi sp, sp, 352
        li a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 32
# Stores (sw) : 23
# Total : 55
# --------------------------------------
