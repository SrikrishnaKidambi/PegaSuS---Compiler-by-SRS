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
    str_0:    .asciz "Going to return the"
    str_1:    .asciz "The result is:"

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
        addi   sp, sp, -208
        sd     ra, 200(sp)
        sd     s0, 192(sp)
        addi   s0, sp, 208
        sw     a0, -20(s0)
        sw     a1, -24(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # x
    # y
    # spill all registers
        la a0, str_0
        call puts
        lw   t1, -20(s0)
        lw   t2, -24(s0)
        add  t3, t1, t2
        mv     a0, t3
        j      Lepi_add_ii
Lepi_add_ii:
    # -- epilogue --
        ld     ra, 200(sp)
        ld     s0, 192(sp)
        addi   sp, sp, 208
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -208
        sd   ra, 200(sp)
        sd   s0, 192(sp)
        addi s0, sp, 208
        li   t1, 1
        li   t2, 2
    # spill all registers
        la   t0, a
        sw   t1, 0(t0)
        la   t0, b
        sw   t2, 0(t0)
        la     t0, a
        lw     a0, 0(t0)
        la     t0, b
        lw     a1, 0(t0)
        call   add_ii
        sw     a0, -84(s0)
        lw   t1, -84(s0)
        mv   t2, t1
    # spill all registers
        la   t0, result
        sw   t2, 0(t0)
        la a0, str_1
        call puts
    # spill all registers
        la t0, result
        lw a1, 0(t0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 200(sp)
        ld   s0, 192(sp)
        addi sp, sp, 208
        ret

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 13
# Stores (sw) : 8
# Total : 21
# --------------------------------------
