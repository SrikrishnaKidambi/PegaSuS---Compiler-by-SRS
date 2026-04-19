    .data

    # -- Global Scalar Variables --
        .align 2
    res:    .float 0.0
        .align 2
    x:    .float 5.500000
        .align 2
    y:    .float 7.500000

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

        .align 2
    .fconst_0:    .float 5.500000
        .align 2
    .fconst_1:    .float 7.500000
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
        addi sp, sp, -176
        sd   ra, 168(sp)
        sd   s0, 160(sp)
        addi s0, sp, 176
        la   t0, .fconst_0
        flw  ft0, 0(t0)
        la   t0, x
        fsw  ft0, 0(t0)
        la   t0, .fconst_1
        flw  ft0, 0(t0)
        la   t0, y
        fsw  ft0, 0(t0)
        la   t0, x
        flw  ft0, 0(t0)
        la   t0, y
        flw  ft1, 0(t0)
        fadd.s ft2, ft0, ft1
        fsw  ft2, -100(s0)
        flw  ft0, -100(s0)
        la   t0, res
        fsw  ft0, 0(t0)
    # spill all registers
        la     t0, res
        flw    fa0, 0(t0)
        fcvt.d.s fa0, fa0
        la     a0, .fmt_float
        call   printf

    # -- global scope epilogue --
        ld   ra, 168(sp)
        ld   s0, 160(sp)
        addi sp, sp, 176
        ret
