    .data

    # -- Global Scalar Variables --
        .align 2
    n:    .word  5

    # -- global arrays --

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
        addi sp, sp, -144
        sd   ra, 136(sp)
        sd   s0, 128(sp)
        addi s0, sp, 144
        li   t1, 5

    # -- global scope epilogue --
        ld   ra, 136(sp)
        ld   s0, 128(sp)
        addi sp, sp, 144
        ret
