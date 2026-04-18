    .data

    # -- Global Scalar Variables --
        .align 2
    a:    .word  5
        .align 2
    res:    .word  0

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


fact_i:
    # -- prologue --
        addi   sp, sp, -224
        sd     ra, 216(sp)
        sd     s0, 208(sp)
        addi   s0, sp, 224
        sw     a0, -20(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # n
    # ==
        lw   t1, -20(s0)
    li t2, 0
        sub  t3, t1, t2
        seqz t3, t3
        beqz   t3, L0
        sw   t3, -88(s0)
        li     a0, 1
        j      Lepi_fact_i
    # spill all registers
        j      L1
    # spill all registers

L0:
    # spill all registers

L1:
        lw   t1, -20(s0)
        addi t2, t1, -1
        mv     a0, t2
    # spill all registers
    # spill all registers
        call   fact_i
        sw     a0, -92(s0)
        lw   t1, -20(s0)
        lw   t2, -92(s0)
        mul  t3, t1, t2
        mv     a0, t3
        j      Lepi_fact_i
Lepi_fact_i:
    # -- epilogue --
        ld     ra, 216(sp)
        ld     s0, 208(sp)
        addi   sp, sp, 224
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -224
        sd   ra, 216(sp)
        sd   s0, 208(sp)
        addi s0, sp, 224
        li   t1, 5
        mv     a0, t1
    # spill all registers
        la   t0, a
        sw   t1, 0(t0)
    # spill all registers
        call   fact_i
        sw     a0, -92(s0)
        lw   t1, -92(s0)
        mv   t2, t1
    # spill all registers
        la   t0, res
        sw   t2, 0(t0)
        la t0, res
        lw a1, 0(t0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 216(sp)
        ld   s0, 208(sp)
        addi sp, sp, 224
        ret
