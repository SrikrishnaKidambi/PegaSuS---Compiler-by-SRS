    .data

    # -- Global Scalar Variables --
        .align 2
    r1:    .word  0
        .align 2
    r2:    .word  0

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


add_ii:
    # -- prologue --
        addi   sp, sp, -256
        sd     ra, 248(sp)
        sd     s0, 240(sp)
        addi   s0, sp, 256
        sw     a0, -20(s0)
        sw     a1, -24(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # a
    # b
        lw   t1, -20(s0)
        lw   t2, -24(s0)
        add  t3, t1, t2
        mv     a0, t3
        j      Lepi_add_ii
Lepi_add_ii:
    # -- epilogue --
        ld     ra, 248(sp)
        ld     s0, 240(sp)
        addi   sp, sp, 256
        ret
    # -- epilogue end --


add_iii:
    # -- prologue --
        addi   sp, sp, -272
        sd     ra, 264(sp)
        sd     s0, 256(sp)
        addi   s0, sp, 272
        sw     a0, -20(s0)
        sw     a1, -24(s0)
        sw     a2, -28(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # a
    # b
    # c
        lw   t1, -20(s0)
        lw   t2, -24(s0)
        add  t3, t1, t2
        lw   t4, -28(s0)
        add  t5, t3, t4
        mv     a0, t5
        j      Lepi_add_iii
Lepi_add_iii:
    # -- epilogue --
        ld     ra, 264(sp)
        ld     s0, 256(sp)
        addi   sp, sp, 272
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -256
        sd   ra, 248(sp)
        sd   s0, 240(sp)
        addi s0, sp, 256
    # spill all registers
        addi sp, sp, -16
        sd   ra, 8(sp)
        li     a0, 3
        li     a1, 5
        call   add_ii
        ld   ra, 8(sp)
        addi sp, sp, 16
        sw     a0, -84(s0)
        lw   t1, -84(s0)
        mv   t2, t1
    # spill all registers
        la   t0, r1
        sw   t2, 0(t0)
        addi sp, sp, -16
        sd   ra, 8(sp)
        li     a0, 1
        li     a1, 2
        li     a2, 3
        call   add_iii
        ld   ra, 8(sp)
        addi sp, sp, 16
        sw     a0, -88(s0)
        lw   t1, -88(s0)
        mv   t2, t1
    # spill all registers
        la   t0, r2
        sw   t2, 0(t0)
        addi sp, sp, -16
        sd ra, 8(sp)
        la t0, r1
        lw a1, 0(t0)
        la a0, .fmt_int
        call printf
        ld ra, 8(sp)
        addi sp, sp, 16
    # spill all registers
        addi sp, sp, -16
        sd ra, 8(sp)
        la t0, r2
        lw a1, 0(t0)
        la a0, .fmt_int
        call printf
        ld ra, 8(sp)
        addi sp, sp, 16

    # -- global scope epilogue --
        ld   ra, 248(sp)
        ld   s0, 240(sp)
        addi sp, sp, 256
        ret

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 20
# Stores (sw) : 13
# Total : 33
# --------------------------------------
