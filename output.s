    .data

    # -- Global Scalar Variables --

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "I love Compiler design lab\n"
    str_1:    .asciz "I love IIT Tirupati\n"
    str_2:    .asciz "Raghavendra is hardworker\n"

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


print_s:
    # -- prologue --
        addi   sp, sp, -192
        sd     ra, 184(sp)
        sd     s0, 176(sp)
        addi   s0, sp, 188
        sw     a0, -20(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # m
    # spill all registers
        lw     a0, -20(s0)
        call puts
Lepi_print_s:
    # -- epilogue --
        ld     ra, 184(sp)
        ld     s0, 176(sp)
        addi   sp, sp, 192
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -192
        sd   ra, 184(sp)
        sd   s0, 176(sp)
        addi s0, sp, 192
        la     a0, str_0
    # spill all registers
        call   print_s
        sw     a0, -80(s0)
        la     a0, str_1
    # spill all registers
        call   print_s
        sw     a0, -84(s0)
        la     a0, str_2
    # spill all registers
        call   print_s
        sw     a0, -88(s0)

    # -- global scope epilogue --
        ld   ra, 184(sp)
        ld   s0, 176(sp)
        addi sp, sp, 192
        ret
