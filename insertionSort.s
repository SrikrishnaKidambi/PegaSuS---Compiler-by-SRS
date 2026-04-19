    .data

    # -- Global Scalar Variables --
        .align 2
    n:    .word  5

    # -- global arrays --
        .align 2
    arr:    .word  5
           .word  3
           .word  8
           .word  1
           .word  2

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


insertion_sort_:
    # -- prologue --
        addi   sp, sp, -368
        sd     ra, 360(sp)
        sd     s0, 352(sp)
        addi   s0, sp, 368
    # --initialize local arrays --
    # --initialize local scalars --
    # init local scalar i at offset -48
    # init local scalar key at offset -52
    # init local scalar j at offset -56
    # init local scalar aj at offset -60
    # -- prologue end --

        li   t1, 1
    # spill all registers
        sw   t1, -48(s0)

L0:
    # <
        lw   t1, -48(s0)
        la   t0, n
        lw   t2, 0(t0)
        slt  t3, t1, t2
        beqz   t3, L1
        sw   t3, -128(s0)
    li t3, 4
        mul  t4, t1, t3
    # array access []
        la   t5, arr
        add  t6, t5, t4
        lw   t6, 0(t6)
        sw   t4, -132(s0)
        mv   t4, t6
        addi t5, t1, -1
        mv   s1, t5
    # spill all registers
        sw   t4, -52(s0)
        sw   s1, -56(s0)

L2:
    # >
        lw   t1, -56(s0)
    li t2, 0
        slt  t3, t2, t1
        beqz   t3, L3
        sw   t3, -136(s0)
    li t3, 4
        mul  t4, t1, t3
    # array access []
        la   t5, arr
        add  t6, t5, t4
        lw   t6, 0(t6)
        sw   t4, -140(s0)
        mv   t4, t6
    # >
        lw   t5, -52(s0)
        slt  s1, t5, t4
        beqz   s1, L4
        sw   s1, -144(s0)
        addi s1, t1, 1
        mul  s2, s1, t3
    # array access []
        la   s3, arr
        add  s4, s3, s2
        lw   s4, 0(s4)
        sw   s2, -148(s0)
    # spill all registers
        sw   t4, -60(s0)
        j      L5
    # spill all registers

L4:
        li   t1, 0
    # spill all registers
        sw   t1, -56(s0)

L5:
        lw   t1, -56(s0)
        addi t2, t1, -1
        mv   t1, t2
    # spill all registers
        sw   t1, -56(s0)
        j      L2
    # spill all registers

L3:
        lw   t1, -56(s0)
        addi t2, t1, 1
    li t3, 4
        mul  t4, t2, t3
    # array access []
        la   t5, arr
        add  t6, t5, t4
        lw   t6, 0(t6)
        sw   t4, -152(s0)
        lw   t4, -48(s0)
        addi t5, t4, 1
        mv   t4, t5
    # spill all registers
        sw   t4, -48(s0)
        j      L0
    # spill all registers

L1:
Lepi_insertion_sort_:
    # -- epilogue --
        ld     ra, 360(sp)
        ld     s0, 352(sp)
        addi   sp, sp, 368
        ret
    # -- epilogue end --


main_:
    # -- prologue --
        addi   sp, sp, -368
        sd     ra, 360(sp)
        sd     s0, 352(sp)
        addi   s0, sp, 368
    # --initialize local arrays --
    # --initialize local scalars --
    # init local scalar i at offset -48
    # -- prologue end --

    # spill all registers
        call   insertion_sort_
        sw     a0, -116(s0)
        li   t1, 0
    # spill all registers
        sw   t1, -48(s0)

L6:
    # <
        lw   t1, -48(s0)
        la   t0, n
        lw   t2, 0(t0)
        slt  t3, t1, t2
        beqz   t3, L7
        sw   t3, -120(s0)
    li t3, 4
        mul  t4, t1, t3
    # array access []
        la   t5, arr
        add  t6, t5, t4
        lw   t6, 0(t6)
        sw   t4, -124(s0)
        mv     a1, t6
    # spill all registers
        la a0, .fmt_int
        call printf
        lw   t1, -48(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -48(s0)
        j      L6
    # spill all registers

L7:
Lepi_main_:
    # -- epilogue --
        ld     ra, 360(sp)
        ld     s0, 352(sp)
        addi   sp, sp, 368
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -400
        sd   ra, 392(sp)
        sd   s0, 384(sp)
        addi s0, sp, 400
    # array_init
        li   t1, 5

    # -- global scope epilogue --
        ld   ra, 392(sp)
        ld   s0, 384(sp)
        addi sp, sp, 400
        ret
