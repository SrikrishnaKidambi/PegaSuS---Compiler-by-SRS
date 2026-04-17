    .data

    # -- Global Scalar Variables --
        .align 2
<<<<<<< HEAD
    result:    .word  0
=======
    a:    .word  100
        .align 2
<<<<<<< HEAD
    num1:    .word  48
        .align 2
    num2:    .word  18
=======
    b:    .word  200
        .align 2
    res:    .word  0
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4
>>>>>>> d92a61429a812fe59b93974fbde1d554efa2563d

    # -- global arrays --

    # -- string literals --
<<<<<<< HEAD
=======
<<<<<<< HEAD

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
=======
    str_0:    .asciz "The maximum is:"
>>>>>>> d92a61429a812fe59b93974fbde1d554efa2563d

    # -- I/O format strings --
    .fmt_int:    .asciz  "%d\n"
    .fmt_uint:    .asciz  "%u\n"
    .fmt_float:    .asciz  "%f\n"
    .fmt_str:    .asciz  "%s\n"
    .fmt_char:    .asciz  "%c\n"
    .fmt_scan_int:    .asciz  "%d"
    .fmt_scan_float:    .asciz  "%f\n"
    .fmt_scan_str:    .asciz  "%s\n"

    .text
    .globl main

main:
    # -- main function --
        addi sp, sp, -16
        sd   ra, 8(sp)
        call global_body
        ld   ra, 8(sp)
        addi sp, sp, 16
        li a0, 0
        ret


binarySearch_i:
    # -- prologue --
        addi   sp, sp, -320
        sd     ra, 312(sp)
        sd     s0, 304(sp)
        addi   s0, sp, 320
        sw     a0, -20(s0)
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

<<<<<<< HEAD
    # tgt
        li   t1, 0
        li   t2, 4
        lw   t3, -20(s0)
        mv   t4, t3
=======
    # x
    # y
    # >
        lw   t1, -20(s0)
        lw   t2, -24(s0)
        slt  t3, t2, t1
        beqz   t3, L0
        sw   t3, -80(s0)
        mv     a0, t1
        j      Lepi_max_ii
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4
    # spill all registers
        j      L1
>>>>>>> d92a61429a812fe59b93974fbde1d554efa2563d
    # spill all registers
        sw   t1, -24(s0)
        sw   t2, -28(s0)
        sw   t4, -36(s0)

L0:
    # <
        lw   t1, -24(s0)
        lw   t2, -28(s0)
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
        addi s5, s0, -40
        add  s6, s5, s4
        lw   s6, 0(s6)
        sw   s4, -116(s0)
    # ==
        lw   s4, -36(s0)
        sub  s5, s6, s4
        seqz s5, s5
        beqz   s5, L2
        sw   s5, -120(s0)
        mv     a0, s2
        j      Lepi_binarySearch_i
    # spill all registers
        sw   s2, -32(s0)
        j      L3
    # spill all registers

L2:
    # spill all registers

L3:
        lw   t1, -32(s0)
    li t2, 4
        mul  t3, t1, t2
    # array access []
        addi t4, s0, -40
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -124(s0)
    # <
        lw   t3, -36(s0)
        slt  t4, t5, t3
        beqz   t4, L4
        sw   t4, -128(s0)
        addi t4, t1, 1
        mv   t6, t4
    # spill all registers
        sw   t6, -24(s0)
        j      L5
    # spill all registers

L4:
        lw   t1, -32(s0)
        addi t2, t1, -1
        mv   t3, t2
    # spill all registers
        sw   t3, -28(s0)

L5:
    # spill all registers
        j      L0
    # spill all registers

<<<<<<< HEAD
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
=======
L1:
    li t1, 0
        addi t2, t1, -1
        mv     a0, t2
        j      Lepi_binarySearch_i
Lepi_binarySearch_i:
    # -- epilogue --
<<<<<<< HEAD
        ld     ra, 312(sp)
        ld     s0, 304(sp)
        addi   sp, sp, 320
=======
        ld     ra, 216(sp)
        ld     s0, 208(sp)
        addi   sp, sp, 224
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4
>>>>>>> d92a61429a812fe59b93974fbde1d554efa2563d
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
<<<<<<< HEAD
        addi sp, sp, -352
        sd   ra, 344(sp)
        sd   s0, 336(sp)
        addi s0, sp, 352
    # spill all registers
        li     a0, 1
        call   binarySearch_i
        sw     a0, -112(s0)
        lw   t1, -112(s0)
=======
<<<<<<< HEAD
        li   t1, 48
        li   t2, 18
        mv     a0, t1
        mv     a1, t2
=======
        addi sp, sp, -224
        sd   ra, 216(sp)
        sd   s0, 208(sp)
        addi s0, sp, 224
        li   t1, 100
        li   t2, 200
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4
    # spill all registers
        la   t0, num1
        sw   t1, 0(t0)
        la   t0, num2
        sw   t2, 0(t0)
<<<<<<< HEAD
        call   gcd_ii
=======
        la     t0, a
        lw     a0, 0(t0)
        la     t0, b
        lw     a1, 0(t0)
        call   max_ii
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4
        sw     a0, -84(s0)
        lw   t1, -84(s0)
>>>>>>> d92a61429a812fe59b93974fbde1d554efa2563d
        mv   t2, t1
        mv     a0, t2
    # spill all registers
        la   t0, result
        sw   t2, 0(t0)
<<<<<<< HEAD
        la t0, result
=======
<<<<<<< HEAD
        li     a7, 1
        ecall

    # -- global scope epilogue --
        lw ra, 236(sp)
        lw s0, 232(sp)
        addi sp, sp, 240
        li a7, 10
        ecall
=======
        la a0, str_0
        call puts
    # spill all registers
        la t0, res
>>>>>>> d92a61429a812fe59b93974fbde1d554efa2563d
        lw a1, 0(t0)
        la a0, .fmt_int
        call printf
    # spill all registers
        li     a1, 10
        la     a0, .fmt_int
        call   printf

    # -- global scope epilogue --
        ld   ra, 344(sp)
        ld   s0, 336(sp)
        addi sp, sp, 352
        ret
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
<<<<<<< HEAD
# Loads (lw/li): 32
# Stores (sw) : 23
# Total : 55
=======
# Loads (lw/li): 13
<<<<<<< HEAD
# Stores (sw) : 10
# Total : 23
=======
# Stores (sw) : 9
# Total : 22
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4
>>>>>>> d92a61429a812fe59b93974fbde1d554efa2563d
# --------------------------------------
