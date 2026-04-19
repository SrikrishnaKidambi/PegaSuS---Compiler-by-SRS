    .data

    # -- Global Scalar Variables --
        .align 2
    j:    .word  0
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
        addi sp, sp, -304
        sd   ra, 296(sp)
        sd   s0, 288(sp)
        addi s0, sp, 304
        li   t1, 5
    # <
    # load: unknown operand, loading 0
        li   t2, 0
        slt  t3, t2, t1
        addi t4, t2, 1
        mv   t2, t4
        addi t5, t2, -1
        mv   t6, t5
    # spill all registers
        la   t0, n
        sw   t1, 0(t0)
        la   t0, j
        sw   t6, 0(t0)

L0:
    # >
        la   t0, j
        lw   t1, 0(t0)
    li t2, 0
        slt  t3, t2, t1
        beqz   t3, L1
        sw   t3, -100(s0)
    # load: unknown operand, loading 0
        li   t3, 0
        mul  t4, t1, t3
    # array access []
    # ERROR: array symbol not found
        lw   t6, -104(s0)
    # >
    # load: unknown operand, loading 0
        li   s1, 0
        slt  s2, s1, t6
        beqz   s2, L2
        sw   s2, -108(s0)
        addi s2, t1, 1
        mul  s3, s2, t3
    # array access []
    # ERROR: array symbol not found
    # spill all registers
        sw   t6, -32(s0)
        j      L3
    # spill all registers

L2:
        li   t1, 0
    # spill all registers
        la   t0, j
        sw   t1, 0(t0)

L3:
        la   t0, j
        lw   t1, 0(t0)
        addi t2, t1, -1
        mv   t1, t2
    # spill all registers
        la   t0, j
        sw   t1, 0(t0)
        j      L0
    # spill all registers

L1:
        la   t0, j
        lw   t1, 0(t0)
        addi t2, t1, 1
    # load: unknown operand, loading 0
        li   t3, 0
        mul  t4, t2, t3
    # array access []
    # ERROR: array symbol not found
        li   t6, 0
    # spill all registers
        sw   t6, -32(s0)

L4:
    # <
        lw   t1, -32(s0)
        la   t0, n
        lw   t2, 0(t0)
        slt  t3, t1, t2
        beqz   t3, L5
        sw   t3, -112(s0)
    # load: unknown operand, loading 0
        li   t3, 0
        mul  t4, t1, t3
    # array access []
    # ERROR: array symbol not found
    # spill all registers
        lw     a1, -116(s0)
        la a0, .fmt_int
        call printf
        lw   t1, -32(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -32(s0)
        j      L4
    # spill all registers

L5:

    # -- global scope epilogue --
        ld   ra, 296(sp)
        ld   s0, 288(sp)
        addi sp, sp, 304
        ret
