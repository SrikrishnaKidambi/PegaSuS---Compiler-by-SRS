    .data

    # -- Global Scalar Variables --
        .align 2
    l:    .float 3.000000
        .align 2
    w:    .float 4.500000
        .align 2
    temp1:    .word  0
        .align 2
    temp2:    .float 0.0

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


area_i:
    # -- prologue --
        addi   sp, sp, -240
        sd     ra, 232(sp)
        sd     s0, 224(sp)
        addi   s0, sp, 240
        sw   a0, -24(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # side
        lw   t1, -24(s0)
        mul  t2, t1, t1
        mv     a0, t2
    # spill all registers
        sw   t2, -92(s0)
        j      Lepi_area_i
Lepi_area_i:
    # -- epilogue --
        ld     ra, 232(sp)
        ld     s0, 224(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --


area_ff:
    # -- prologue --
        addi   sp, sp, -240
        sd     ra, 232(sp)
        sd     s0, 224(sp)
        addi   s0, sp, 240
        fsw  fa0, -24(s0)
        fsw  fa1, -28(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # l
    # w
    # *
        flw  ft0, -24(s0)
        flw  ft1, -28(s0)
        fmul.s ft2, ft0, ft1
    # spill all registers
        fsw  ft2, -96(s0)
        lw     a0, -96(s0)
        j      Lepi_area_ff
Lepi_area_ff:
    # -- epilogue --
        ld     ra, 232(sp)
        ld     s0, 224(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -240
        sd   ra, 232(sp)
        sd   s0, 224(sp)
        addi s0, sp, 240
        li     a0, 4
    # spill all registers
        call   area_i
        sw     a0, -104(s0)
        lw   t1, -104(s0)
    # spill all registers
        la   t0, temp1
        sw   t1, 0(t0)
        la     t0, temp1
        lw     a1, 0(t0)
        la a0, .fmt_int
        call printf
        addi   sp, sp, -16
        li     t0, 1077936128
        sw     t0, 0(sp)
        flw    ft0, 0(sp)
        addi   sp, sp, 16
        addi   sp, sp, -16
        li     t0, 1083179008
        sw     t0, 0(sp)
        flw    ft1, 0(sp)
        addi   sp, sp, 16
    # spill all registers
        la   t0, l
        fsw  ft0, 0(t0)
        la   t0, w
        fsw  ft1, 0(t0)
        la     t0, l
        flw    fa0, 0(t0)
        la     t0, w
        flw    fa1, 0(t0)
        call   area_ff
        sw     a0, -108(s0)
        lw   t1, -108(s0)
    # spill all registers
        la   t0, temp2
        sw   t1, 0(t0)
        la     t0, temp2
        flw    fa0, 0(t0)
        fcvt.d.s fa0, fa0
        la     a0, .fmt_float
        call   printf

    # -- global scope epilogue --
        ld   ra, 232(sp)
        ld   s0, 224(sp)
        addi sp, sp, 240
        ret
