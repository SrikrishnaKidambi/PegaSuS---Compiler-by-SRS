    .data

    # -- Global Scalar Variables --
        .align 2
    n:    .word  6
        .align 2
    flag:    .word  0

    # -- global arrays --
        .align 2
    arr1:    .word  1
           .word  2
           .word  4
           .word  5
           .word  6
           .word  7
        .align 2
    arr:    .word  2
           .word  3
           .word  4
           .word  4
           .word  3
           .word  2

    # -- string literals --
    str_0:    .asciz "The first array is: "
    str_1:    .asciz "Not a palindraome"
    str_2:    .asciz "It is a palindrome"
    str_3:    .asciz "The second array is: "

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
        addi sp, sp, -464
        sd   ra, 456(sp)
        sd   s0, 448(sp)
        addi s0, sp, 464
    # array_init
    # array_init
        li   t1, 6
    # spill all registers
        la   t0, n
        sw   t1, 0(t0)
        la   a0, str_0
        call puts
        li   t1, 0
        li   t2, 0
    # spill all registers
        la   t0, flag
        sw   t1, 0(t0)
        sw   t2, -80(s0)

L0:
        la   t0, n
        lw   t1, 0(t0)
        li   t2, 2
        div  t3, t1, t2
    # <
        lw   t4, -80(s0)
        slt  t5, t4, t3
    # spill all registers
        sw   t3, -148(s0)
        sw   t5, -152(s0)
        beqz   t5, L1
        lw   t1, -80(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, arr
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -156(s0)
        la   t0, n
        lw   t3, 0(t0)
        sub  t4, t3, t1
        addi t6, t4, -1
        mul  s1, t6, t2
    # array access []
        la   s2, arr
        add  s3, s2, s1
        lw   s3, 0(s3)
        sw   s1, -160(s0)
    # ==
        sub  s1, t5, s3
        seqz s1, s1
    # !
        seqz s2, s1
    # spill all registers
        sw   t4, -164(s0)
        sw   t5, -168(s0)
        sw   t6, -172(s0)
        sw   s1, -176(s0)
        sw   s2, -180(s0)
        sw   s3, -184(s0)
        beqz   s2, L2
        li   t1, 1
    # spill all registers
        la   t0, flag
        sw   t1, 0(t0)
        j      L1
    # spill all registers
        j      L3
    # spill all registers

L2:
    # spill all registers

L3:
        lw   t1, -80(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -80(s0)
        sw   t2, -188(s0)
        j      L0
    # spill all registers

L1:
    # ==
        la   t0, flag
        lw   t1, 0(t0)
        li   t2, 1
        sub  t3, t1, t2
        seqz t3, t3
    # spill all registers
        sw   t3, -192(s0)
        beqz   t3, L4
    # spill all registers
        la   a0, str_1
        call puts
    # spill all registers
        j      L5
    # spill all registers

L4:
    # spill all registers
        la   a0, str_2
        call puts
    # spill all registers

L5:
    # spill all registers
        la   a0, str_3
        call puts
        li   t1, 0
        li   t2, 0
    # spill all registers
        la   t0, flag
        sw   t1, 0(t0)
        sw   t2, -80(s0)

L6:
        la   t0, n
        lw   t1, 0(t0)
        li   t2, 2
        div  t3, t1, t2
    # <
        lw   t4, -80(s0)
        slt  t5, t4, t3
    # spill all registers
        sw   t3, -196(s0)
        sw   t5, -200(s0)
        beqz   t5, L7
        lw   t1, -80(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, arr1
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -204(s0)
        la   t0, n
        lw   t3, 0(t0)
        sub  t4, t3, t1
        addi t6, t4, -1
        mul  s1, t6, t2
    # array access []
        la   s2, arr1
        add  s3, s2, s1
        lw   s3, 0(s3)
        sw   s1, -208(s0)
    # ==
        sub  s1, t5, s3
        seqz s1, s1
    # !
        seqz s2, s1
    # spill all registers
        sw   t4, -212(s0)
        sw   t5, -216(s0)
        sw   t6, -220(s0)
        sw   s1, -224(s0)
        sw   s2, -228(s0)
        sw   s3, -232(s0)
        beqz   s2, L8
        li   t1, 1
    # spill all registers
        la   t0, flag
        sw   t1, 0(t0)
        j      L7
    # spill all registers
        j      L9
    # spill all registers

L8:
    # spill all registers

L9:
        lw   t1, -80(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -80(s0)
        sw   t2, -236(s0)
        j      L6
    # spill all registers

L7:
    # ==
        la   t0, flag
        lw   t1, 0(t0)
        li   t2, 1
        sub  t3, t1, t2
        seqz t3, t3
    # spill all registers
        sw   t3, -240(s0)
        beqz   t3, L10
    # spill all registers
        la   a0, str_1
        call puts
    # spill all registers
        j      L11
    # spill all registers

L10:
    # spill all registers
        la   a0, str_2
        call puts
    # spill all registers

L11:

    # -- global scope epilogue --
        ld   ra, 456(sp)
        ld   s0, 448(sp)
        addi sp, sp, 464
        ret
