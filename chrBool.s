    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0
        .align 2
    b1:    .word  1
        .align 2
    b2:    .word  0
    ch:    .byte  'X'

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
        addi sp, sp, -208
        sd   ra, 200(sp)
        sd   s0, 192(sp)
        addi s0, sp, 208
        li   t1, 1
        li   t2, 0
        li   t3, 88
    # spill all registers
        la   t0, b1
        sw   t1, 0(t0)
        la   t0, b2
        sw   t2, 0(t0)
        la   t0, ch
        sw   t3, 0(t0)
        la     t0, b1
        lw     a1, 0(t0)
        la a0, .fmt_int
        call printf
    # spill all registers
        la     t0, b2
        lw     a1, 0(t0)
        la a0, .fmt_int
        call printf
    # &&
        la   t0, b1
        lw   t1, 0(t0)
        la   t0, b2
        lw   t2, 0(t0)
        and  t3, t1, t2
        mv   t4, t3
    # spill all registers
        la   t0, result
        sw   t4, 0(t0)
        la     t0, result
        lw     a1, 0(t0)
        la a0, .fmt_int
        call printf
    # ||
        la   t0, b1
        lw   t1, 0(t0)
        la   t0, b2
        lw   t2, 0(t0)
        or   t3, t1, t2
        mv   t4, t3
    # spill all registers
        la   t0, result
        sw   t4, 0(t0)
        la     t0, result
        lw     a1, 0(t0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 200(sp)
        ld   s0, 192(sp)
        addi sp, sp, 208
        ret
