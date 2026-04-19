    .data

    # -- Global Scalar Variables --
        .align 2
    n:    .word  3

    # -- global arrays --
        .align 2
    arr:    .word  12
           .word  11
           .word  13

    # -- string literals --
    str_0:    .asciz "\n"

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


merge_iii:
    # -- prologue --
        addi   sp, sp, -352
        sd     ra, 344(sp)
        sd     s0, 336(sp)
        addi   s0, sp, 352
        sw   a0, -40(s0)
        sw   a1, -44(s0)
        sw   a2, -48(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # init local scalar n1 at offset -52
    # init local scalar n2 at offset -56
    # -- prologue end --

    # l
    # m
    # r
        lw   t1, -44(s0)
        lw   t2, -40(s0)
        sub  t3, t1, t2
        addi t4, t3, 1
        mv   t5, t4
        lw   t6, -48(s0)
        sub  s1, t6, t1
        mv   s2, s1
        li   s3, 0
    # spill all registers
        sw   t5, -52(s0)
        sw   s2, -56(s0)
        sw   s3, -84(s0)

L0:
    # <
        lw   t1, -84(s0)
        lw   t2, -52(s0)
        slt  t3, t1, t2
        beqz   t3, L1
        sw   t3, -160(s0)
        lw   t3, -40(s0)
        add  t4, t3, t1
    li t5, 4
        mul  t6, t4, t5
    # array access []
        la   s1, arr
        add  s2, s1, t6
        lw   s2, 0(s2)
        sw   t6, -164(s0)
        mul  t6, t1, t5
    # array store []=
        addi s1, s0, -60
        add  s3, s1, t6
        sw   s2, 0(s3)
        addi s1, t1, 1
        mv   t1, s1
    # spill all registers
        sw   t1, -84(s0)
        j      L0
    # spill all registers

L1:
        li   t1, 0
    # spill all registers
        sw   t1, -88(s0)

L2:
    # <
        lw   t1, -88(s0)
        lw   t2, -56(s0)
        slt  t3, t1, t2
        beqz   t3, L3
        sw   t3, -168(s0)
        lw   t3, -44(s0)
        addi t4, t3, 1
        add  t5, t4, t1
    li t6, 4
        mul  s1, t5, t6
    # array access []
        la   s2, arr
        add  s3, s2, s1
        lw   s3, 0(s3)
        sw   s1, -172(s0)
        mul  s1, t1, t6
    # array store []=
        addi s2, s0, -72
        add  s4, s2, s1
        sw   s3, 0(s4)
        addi s2, t1, 1
        mv   t1, s2
    # spill all registers
        sw   t1, -88(s0)
        j      L2
    # spill all registers

L3:
        li   t1, 0
        li   t2, 0
        lw   t3, -40(s0)
    # spill all registers
        sw   t1, -84(s0)
        sw   t2, -88(s0)
        sw   t3, -92(s0)

L4:
    # <
        lw   t1, -84(s0)
        lw   t2, -52(s0)
        slt  t3, t1, t2
    # <
        lw   t4, -88(s0)
        lw   t5, -56(s0)
        slt  t6, t4, t5
    # &&
        and  s1, t3, t6
        beqz   s1, L5
        sw   s1, -176(s0)
    li s1, 4
        mul  s2, t1, s1
    # array access []
        addi s3, s0, -60
        add  s4, s3, s2
        lw   s4, 0(s4)
        sw   s2, -180(s0)
        mul  s2, t4, s1
    # array access []
        addi s3, s0, -72
        add  s5, s3, s2
        lw   s5, 0(s5)
        sw   s2, -184(s0)
    # <
        slt  s2, s4, s5
        mul  s3, t1, s1
    # array access []
        addi s6, s0, -60
        add  s7, s6, s3
        lw   s7, 0(s7)
        sw   s3, -188(s0)
        mul  s3, t4, s1
    # array access []
        addi s6, s0, -72
        add  s8, s6, s3
        lw   s8, 0(s8)
        sw   s3, -192(s0)
    # ==
        sub  s3, s7, s8
        seqz s3, s3
    # ||
        or   s6, s2, s3
        beqz   s6, L6
        sw   s6, -196(s0)
        mul  s6, t1, s1
    # array access []
        addi s9, s0, -60
        add  s10, s9, s6
        lw   s10, 0(s10)
        sw   s6, -200(s0)
        lw   s6, -92(s0)
        mul  s9, s6, s1
    # array store []=
        la   s11, arr
        add  t3, s11, s9
        sw   s10, 0(t3)
        addi t3, t1, 1
        mv   t1, t3
    # spill all registers
        sw   t1, -84(s0)
        j      L7
    # spill all registers

L6:
        lw   t1, -88(s0)
    li t2, 4
        mul  t3, t1, t2
    # array access []
        addi t4, s0, -72
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -204(s0)
        lw   t3, -92(s0)
        mul  t4, t3, t2
    # array store []=
        la   t6, arr
        add  s1, t6, t4
        sw   t5, 0(s1)
        addi t6, t1, 1
        mv   t1, t6
    # spill all registers
        sw   t1, -88(s0)

L7:
        lw   t1, -92(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -92(s0)
        j      L4
    # spill all registers

L5:
    # spill all registers

L8:
    # <
        lw   t1, -84(s0)
        lw   t2, -52(s0)
        slt  t3, t1, t2
        beqz   t3, L9
        sw   t3, -208(s0)
    li t3, 4
        mul  t4, t1, t3
    # array access []
        addi t5, s0, -60
        add  t6, t5, t4
        lw   t6, 0(t6)
        sw   t4, -212(s0)
        lw   t4, -92(s0)
        mul  t5, t4, t3
    # array store []=
        la   s1, arr
        add  s2, s1, t5
        sw   t6, 0(s2)
        addi s1, t1, 1
        mv   t1, s1
        addi s2, t4, 1
        mv   t4, s2
    # spill all registers
        sw   t1, -84(s0)
        sw   t4, -92(s0)
        j      L8
    # spill all registers

L9:
    # spill all registers

L10:
    # <
        lw   t1, -88(s0)
        lw   t2, -56(s0)
        slt  t3, t1, t2
        beqz   t3, L11
        sw   t3, -216(s0)
    li t3, 4
        mul  t4, t1, t3
    # array access []
        addi t5, s0, -72
        add  t6, t5, t4
        lw   t6, 0(t6)
        sw   t4, -220(s0)
        lw   t4, -92(s0)
        mul  t5, t4, t3
    # array store []=
        la   s1, arr
        add  s2, s1, t5
        sw   t6, 0(s2)
        addi s1, t1, 1
        mv   t1, s1
        addi s2, t4, 1
        mv   t4, s2
    # spill all registers
        sw   t1, -88(s0)
        sw   t4, -92(s0)
        j      L10
    # spill all registers

L11:
Lepi_merge_iii:
    # -- epilogue --
        ld     ra, 344(sp)
        ld     s0, 336(sp)
        addi   sp, sp, 352
        ret
    # -- epilogue end --


mergeSort_ii:
    # -- prologue --
        addi   sp, sp, -336
        sd     ra, 328(sp)
        sd     s0, 320(sp)
        addi   s0, sp, 336
        sw   a0, -40(s0)
        sw   a1, -44(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # init local scalar m at offset -48
    # -- prologue end --

    # l
    # r
    # <
        lw   t1, -40(s0)
        lw   t2, -44(s0)
        slt  t3, t1, t2
        beqz   t3, L12
        sw   t3, -116(s0)
        sub  t3, t2, t1
    li t4, 2
        div  t5, t3, t4
        add  t6, t1, t5
        mv   s1, t6
        mv     a0, t1
        mv     a1, s1
    # spill all registers
        sw   s1, -48(s0)
        call   mergeSort_ii
        sw     a0, -120(s0)
        lw   t1, -48(s0)
        addi t2, t1, 1
        mv     a0, t2
    # spill all registers
        lw     a1, -44(s0)
        call   mergeSort_ii
        sw     a0, -124(s0)
    # spill all registers
        lw     a0, -40(s0)
        lw     a1, -48(s0)
        lw     a2, -44(s0)
        call   merge_iii
        sw     a0, -128(s0)
    # spill all registers
        j      L13
    # spill all registers

L12:
    # spill all registers

L13:
Lepi_mergeSort_ii:
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
    # init local scalar i at offset -40
    # -- prologue end --

        la   t0, n
        lw   t1, 0(t0)
        addi t2, t1, -1
        li     a0, 0
        mv     a1, t2
    # spill all registers
        call   mergeSort_ii
        sw     a0, -108(s0)
        li   t1, 0
    # spill all registers
        sw   t1, -40(s0)

L14:
    # <
        lw   t1, -40(s0)
        la   t0, n
        lw   t2, 0(t0)
        slt  t3, t1, t2
        beqz   t3, L15
        sw   t3, -112(s0)
    li t3, 4
        mul  t4, t1, t3
    # array access []
        la   t5, arr
        add  t6, t5, t4
        lw   t6, 0(t6)
        sw   t4, -116(s0)
        mv     a1, t6
    # spill all registers
        la a0, .fmt_int
        call printf
    # spill all registers
        la   a0, str_0
        call puts
        lw   t1, -40(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -40(s0)
        j      L14
    # spill all registers

L15:
Lepi_main_:
    # -- epilogue --
        ld     ra, 328(sp)
        ld     s0, 320(sp)
        addi   sp, sp, 336
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -800
        sd   ra, 792(sp)
        sd   s0, 784(sp)
        addi s0, sp, 800
    # array_init
        li   t1, 3
    # spill all registers
        la   t0, n
        sw   t1, 0(t0)
        call   main_
        sw     a0, -160(s0)

    # -- global scope epilogue --
        ld   ra, 792(sp)
        ld   s0, 784(sp)
        addi sp, sp, 800
        ret
