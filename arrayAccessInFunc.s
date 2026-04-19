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


global_body:
    # -- Global body --
        addi sp, sp, -192
        sd   ra, 184(sp)
        sd   s0, 176(sp)
        addi s0, sp, 192
    # array_init
        li   t1, 5
    li t2, 0
    li t3, 4
        mul  t4, t2, t3
    # array access []
        la   t5, arr
        add  t6, t5, t4
        lw   t6, 0(t6)
        sw   t4, -112(s0)
        mv     a1, t6
    # spill all registers
        la   t0, n
        sw   t1, 0(t0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 184(sp)
        ld   s0, 176(sp)
        addi sp, sp, 192
        ret
