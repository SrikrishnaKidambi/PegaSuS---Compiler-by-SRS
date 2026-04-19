    .data

    # -- Global Scalar Variables --

    # -- global arrays --
        .align 2
    arr:    .word  1
           .word  2
           .word  3

    # -- string literals --
    str_0:    .asciz "Printing the array"
    str_1:    .asciz "\n"

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
        addi sp, sp, -208
        sd   ra, 200(sp)
        sd   s0, 192(sp)
        addi s0, sp, 208
    # array_init
    # spill all registers
        la   a0, str_0
        call puts
    # spill all registers
        la   a0, str_1
        call puts
        li   t1, 0
    # spill all registers
        sw   t1, -36(s0)

L0:
    # <
        lw   t1, -36(s0)
        slti t2, t1, 3
        beqz   t2, L1
        sw   t2, -104(s0)
    li t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, arr
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -108(s0)
        mv     a1, t5
    # spill all registers
        sw   t5, -112(s0)
        la a0, .fmt_int
        call printf
        lw   t1, -36(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -36(s0)
        sw   t2, -116(s0)
        j      L0
    # spill all registers

L1:

    # -- global scope epilogue --
        ld   ra, 200(sp)
        ld   s0, 192(sp)
        addi sp, sp, 208
        ret
