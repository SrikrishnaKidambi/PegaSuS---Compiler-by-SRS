    .data

    # -- Global Scalar Variables --
        .align 2
    n:    .word  5

    # -- global arrays --
        .align 2
    arr:    .word  5
           .word  3
           .word  8
           .word  1
           .word  2

    # -- string literals --

    # -- global objects (pointer slots) --
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


main_:
    # -- prologue --
        addi   sp, sp, -208
        sd     ra, 200(sp)
        sd     s0, 192(sp)
        addi   s0, sp, 208
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    li t1, 2
    li t2, 4
        mul  t3, t1, t2
    # array store []=
        la   t4, arr
        add  t5, t4, t3
        sw   t2, 0(t5)
    li t4, 0
        mul  t5, t4, t2
    # array access []
        la   t6, arr
        add  s1, t6, t5
        lw   s1, 0(s1)
        sw   t5, -112(s0)
        mv     a1, s1
    # spill all registers
        la a0, .fmt_int
        call printf
    li t1, 2
    li t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, arr
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -116(s0)
        mv     a1, t5
    # spill all registers
        la a0, .fmt_int
        call printf
Lepi_main_:
    # -- epilogue --
        ld     ra, 200(sp)
        ld     s0, 192(sp)
        addi   sp, sp, 208
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -224
        sd   ra, 216(sp)
        sd   s0, 208(sp)
        addi s0, sp, 224
    # array_init
        li   t1, 5
    # spill all registers
        la   t0, n
        sw   t1, 0(t0)
        call   main_
        sw     a0, -112(s0)

    # -- global scope epilogue --
        ld   ra, 216(sp)
        ld   s0, 208(sp)
        addi sp, sp, 224
        ret
