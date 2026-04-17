    .data

    # -- Global Scalar Variables --
        .align 2
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

    # -- global arrays --

    # -- string literals --
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


max_ii:
    # -- prologue --
        addi   sp, sp, -224
        sd     ra, 216(sp)
        sd     s0, 208(sp)
        addi   s0, sp, 224
        sw     a0, -20(s0)
        sw     a1, -24(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

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
    # spill all registers

L0:
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
        lw     a0, -24(s0)
        j      Lepi_max_ii
Lepi_max_ii:
    # -- epilogue --
        ld     ra, 216(sp)
        ld     s0, 208(sp)
        addi   sp, sp, 224
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
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
        mv   t2, t1
        mv     a0, t2
    # spill all registers
        la   t0, res
        sw   t2, 0(t0)
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
        lw a1, 0(t0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 216(sp)
        ld   s0, 208(sp)
        addi sp, sp, 224
        ret
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 13
<<<<<<< HEAD
# Stores (sw) : 10
# Total : 23
=======
# Stores (sw) : 9
# Total : 22
>>>>>>> b6c6f75b10504544f9ebf22fc332fb85eeb03fb4
# --------------------------------------
