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
    # -- snapshot variable name strings --

    # -- global objects (pointer slots) --

    # -- I/O format strings --
    .fmt_int:        .asciz  "%d\n"
    .fmt_uint:       .asciz  "%u\n"
    .fmt_float:      .asciz  "%f\n"
    .fmt_str:        .asciz  "%s\n"
    .fmt_char:       .asciz  "%c\n"
    .fmt_scan_int:   .asciz  "%d"
    .fmt_scan_float: .asciz  "%f\n"
    .fmt_scan_str:   .asciz  "%s\n"
    .fmt_int_bare:   .asciz  "%d"

    # -- linefreq report strings --
    .lf_prefix:    .asciz  "Blocks Spanning line numbers "
    .lf_sep:       .asciz  "-"
    .lf_mid:       .asciz  ": Executed "
    .lf_suffix:    .asciz  " times\n"
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
        addi   sp, sp, -336
        sd     ra, 328(sp)
        sd     s0, 320(sp)
        addi   s0, sp, 336
    # --initialize local arrays --
    # --initialize local scalars --
    # init local scalar i at offset -48
    # init local scalar key at offset -52
    # init local scalar j at offset -56
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
    # spill all registers
        sw   t3, -124(s0)
        beqz   t3, L1
        lw   t1, -48(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, arr
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -128(s0)
        mv   t3, t5
        addi t4, t1, -1
        mv   t6, t4
    # spill all registers
        sw   t3, -52(s0)
        sw   t4, -132(s0)
        sw   t5, -136(s0)
        sw   t6, -56(s0)

L2:
    # >
        lw   t1, -56(s0)
        li   t2, 0
        slt  t3, t2, t1
    # ==
        sub  t4, t1, t2
        seqz t4, t4
    # ||
        or   t5, t3, t4
    # spill all registers
        sw   t3, -140(s0)
        sw   t4, -144(s0)
        sw   t5, -148(s0)
        beqz   t5, L3
        lw   t1, -56(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, arr
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -152(s0)
    # <
        lw   t3, -52(s0)
        slt  t4, t5, t3
        mul  t6, t1, t2
    # array access []
        la   s1, arr
        add  s2, s1, t6
        lw   s2, 0(s2)
        sw   t6, -156(s0)
    # ==
        sub  t6, s2, t3
        seqz t6, t6
    # ||
        or   s1, t4, t6
    # spill all registers
        sw   t4, -160(s0)
        sw   t5, -164(s0)
        sw   t6, -168(s0)
        sw   s1, -172(s0)
        sw   s2, -176(s0)
        beqz   s1, L4
    # spill all registers
        j      L3
    # spill all registers
        j      L5
    # spill all registers

L4:
    # spill all registers

L5:
        lw   t1, -56(s0)
        addi t2, t1, 1
        li   t3, 4
        mul  t4, t1, t3
    # array access []
        la   t5, arr
        add  t6, t5, t4
        lw   t6, 0(t6)
        sw   t4, -180(s0)
        mul  t4, t2, t3
    # array store []=
        la   t5, arr
        add  s1, t5, t4
        sw   t6, 0(s1)
        addi t5, t1, -1
        mv   t1, t5
    # spill all registers
        sw   t1, -56(s0)
        sw   t2, -184(s0)
        sw   t4, -188(s0)
        sw   t5, -192(s0)
        sw   t6, -196(s0)
        j      L2
    # spill all registers

L3:
        lw   t1, -56(s0)
        addi t2, t1, 1
        li   t3, 4
        mul  t4, t2, t3
    # array store []=
        la   t5, arr
        lw   t6, -52(s0)
        add  s1, t5, t4
        sw   t6, 0(s1)
        lw   t5, -48(s0)
        addi s1, t5, 1
        mv   t5, s1
    # spill all registers
        sw   t2, -200(s0)
        sw   t4, -204(s0)
        sw   t5, -48(s0)
        sw   s1, -208(s0)
        j      L0
    # spill all registers

L1:
Lepi_insertion_sort_:
    # -- epilogue --
        ld     ra, 328(sp)
        ld     s0, 320(sp)
        addi   sp, sp, 336
        ret
    # -- epilogue end --


main_:
    # -- prologue --
        addi   sp, sp, -336
        sd     ra, 328(sp)
        sd     s0, 320(sp)
        addi   s0, sp, 336
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
    # spill all registers
        sw   t3, -120(s0)
        beqz   t3, L7
        lw   t1, -48(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, arr
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -124(s0)
        mv     a1, t5
    # spill all registers
        sw   t5, -128(s0)
        la a0, .fmt_int
        call printf
        lw   t1, -48(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -48(s0)
        sw   t2, -132(s0)
        j      L6
    # spill all registers

L7:
Lepi_main_:
    # -- epilogue --
        ld     ra, 328(sp)
        ld     s0, 320(sp)
        addi   sp, sp, 336
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -416
        sd   ra, 408(sp)
        sd   s0, 400(sp)
        addi s0, sp, 416
    # array_init
        li   t1, 5
    # spill all registers
        la   t0, n
        sw   t1, 0(t0)
        call   main_
        sw     a0, -124(s0)

    # -- global scope epilogue --
        ld   ra, 408(sp)
        ld   s0, 400(sp)
        addi sp, sp, 416
        ret
