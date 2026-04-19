    .data

    # -- Global Scalar Variables --
        .align 2
    r1:    .word  0
        .align 2
    r2:    .word  0

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


add_ii:
    # -- prologue --
        addi   sp, sp, -240
        sd     ra, 232(sp)
        sd     s0, 224(sp)
        addi   s0, sp, 240
        sw   a0, -24(s0)
        sw   a1, -28(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # a
    # b
        lw   t1, -24(s0)
        lw   t2, -28(s0)
        add  t3, t1, t2
        mv     a0, t3
        j      Lepi_add_ii
Lepi_add_ii:
    # -- epilogue --
        ld     ra, 232(sp)
        ld     s0, 224(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --


add_iii:
    # -- prologue --
        addi   sp, sp, -256
        sd     ra, 248(sp)
        sd     s0, 240(sp)
        addi   s0, sp, 256
        sw   a0, -24(s0)
        sw   a1, -28(s0)
        sw   a2, -32(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # a
    # b
    # c
        lw   t1, -100(s0)
        lw   t2, -32(s0)
        add  t3, t1, t2
        mv     a0, t3
        j      Lepi_add_iii
Lepi_add_iii:
    # -- epilogue --
        ld     ra, 248(sp)
        ld     s0, 240(sp)
        addi   sp, sp, 256
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -240
        sd   ra, 232(sp)
        sd   s0, 224(sp)
        addi s0, sp, 240
        li     a0, 3
        li     a1, 5
    # spill all registers
        call   add_ii
        sw     a0, -100(s0)
        li     a0, 1
        li     a1, 2
        li     a2, 3
    # spill all registers
        call   add_iii
        sw     a0, -104(s0)
    # spill all registers
        lw     a1, -100(s0)
        la a0, .fmt_int
        call printf
    # spill all registers
        lw     a1, -104(s0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 232(sp)
        ld   s0, 224(sp)
        addi sp, sp, 240
        ret
