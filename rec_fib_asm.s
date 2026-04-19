    .data

    # -- Global Scalar Variables --
        .align 2
    ans:    .word  0

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


fib_i:
    # -- prologue --
        addi   sp, sp, -256
        sd     ra, 248(sp)
        sd     s0, 240(sp)
        addi   s0, sp, 256
        sw   a0, -24(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # -- prologue end --

    # n
    # ==
        lw   t1, -24(s0)
    li t2, 0
        sub  t3, t1, t2
        seqz t3, t3
        beqz   t3, L0
        sw   t3, -92(s0)
    # spill all registers
        li     a0, 1
        j      Lepi_fib_i
    # spill all registers
        j      L1
    # spill all registers

L0:
    # spill all registers

L1:
    # ==
        lw   t1, -24(s0)
    li t2, 1
        sub  t3, t1, t2
        seqz t3, t3
        beqz   t3, L2
        sw   t3, -96(s0)
        mv     a0, t2
    # spill all registers
        j      Lepi_fib_i
    # spill all registers
        j      L3
    # spill all registers

L2:
    # spill all registers

L3:
        lw   t1, -24(s0)
        addi t2, t1, -1
        mv     a0, t2
    # spill all registers
        sw   t2, -100(s0)
        call   fib_i
        sw     a0, -104(s0)
        lw   t1, -24(s0)
        addi t2, t1, -2
        mv     a0, t2
    # spill all registers
        sw   t2, -108(s0)
        call   fib_i
        sw     a0, -112(s0)
        lw   t1, -104(s0)
        lw   t2, -112(s0)
        add  t3, t1, t2
        mv     a0, t3
    # spill all registers
        sw   t3, -116(s0)
        j      Lepi_fib_i
Lepi_fib_i:
    # -- epilogue --
        ld     ra, 248(sp)
        ld     s0, 240(sp)
        addi   sp, sp, 256
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -256
        sd   ra, 248(sp)
        sd   s0, 240(sp)
        addi s0, sp, 256
        li     a0, 5
    # spill all registers
        call   fib_i
        sw     a0, -92(s0)
        lw   t1, -92(s0)
    # spill all registers
        la   t0, ans
        sw   t1, 0(t0)
        la     t0, ans
        lw     a1, 0(t0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 248(sp)
        ld   s0, 240(sp)
        addi sp, sp, 256
        ret
