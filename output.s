    .data

    # -- Global Scalar Variables --

    # -- global arrays --

    # -- string literals --

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


compute:
    # -- prologue --
        addi   sp, sp, -192
        sd     ra, 184(sp)
        sd     s0, 176(sp)
        addi   s0, sp, 192
        sw     a0, -20(s0)
        sw     a1, -24(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # x
    # y
        lw   t1, -20(s0)
        lw   t2, -24(s0)
        add  t3, t1, t2
    # <
    # load: unknown operand, loading 0
        li   t4, 0
    # load: unknown operand, loading 0
        li   t5, 0
        slt  t6, t4, t5
        addi s1, t4, 1
        mv   t4, s1
    # <
    # load: unknown operand, loading 0
        li   s2, 0
    # load: unknown operand, loading 0
        li   s3, 0
        slt  s4, s2, s3
        addi s5, s2, 1
        mv   s2, s5

global_body:
    # -- Global body --
        addi sp, sp, -192
        sd   ra, 184(sp)
        sd   s0, 176(sp)
        addi s0, sp, 192

    # -- global scope epilogue --
        ld   ra, 184(sp)
        ld   s0, 176(sp)
        addi sp, sp, 192
        ret
