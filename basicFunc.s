    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0
        .align 2
    a:    .word  1
        .align 2
    b:    .word  2

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Going to return the result"
    str_1:    .asciz "The result is:"

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


add_ii:
    # -- prologue --
        addi   sp, sp, -192
        sd     ra, 184(sp)
        sd     s0, 176(sp)
        addi   s0, sp, 192
        sw   a0, -24(s0)
        sw   a1, -28(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # x
    # y
    # spill all registers
        la   a0, str_0
        call puts
        lw   t1, -24(s0)
        lw   t2, -28(s0)
        add  t3, t1, t2
        mv     a0, t3
        j      Lepi_add_ii
Lepi_add_ii:
    # -- epilogue --
        ld     ra, 184(sp)
        ld     s0, 176(sp)
        addi   sp, sp, 192
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -208
        sd   ra, 200(sp)
        sd   s0, 192(sp)
        addi s0, sp, 208
        li     a0, 1
        li     a1, 2
    # spill all registers
        call   add_ii
        sw     a0, -100(s0)
    # spill all registers
        la   a0, str_1
        call puts
    # spill all registers
        lw     a1, -100(s0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 200(sp)
        ld   s0, 192(sp)
        addi sp, sp, 208
        ret
