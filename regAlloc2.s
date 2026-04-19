    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0
        .align 2
    a:    .word  1
        .align 2
    b:    .word  2
        .align 2
    c:    .word  3
        .align 2
    d:    .word  4
        .align 2
    e:    .word  5
        .align 2
    u1:    .word  0
        .align 2
    f:    .word  6
        .align 2
    u2:    .word  0
        .align 2
    g:    .word  7
        .align 2
    w1:    .word  0
        .align 2
    u3:    .word  0
        .align 2
    h:    .word  8
        .align 2
    w2:    .word  0
        .align 2
    i:    .word  9
        .align 2
    w3:    .word  0
        .align 2
    j:    .word  10
        .align 2
    w4:    .word  0
        .align 2
    k:    .word  11
        .align 2
    w5:    .word  0
        .align 2
    l:    .word  12
        .align 2
    p1:    .word  0
        .align 2
    p2:    .word  0
        .align 2
    p3:    .word  0
        .align 2
    p4:    .word  0
        .align 2
    p5:    .word  0
        .align 2
    p6:    .word  0
        .align 2
    v1:    .word  0
        .align 2
    v2:    .word  0

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


global_body:
    # -- Global body --
        addi sp, sp, -448
        sd   ra, 440(sp)
        sd   s0, 432(sp)
        addi s0, sp, 448
        li   t1, 1
        li   t2, 2
        li   t3, 3
        li   t4, 4
        li   t5, 5
        li   t6, 6
        li   s1, 7
        li   s2, 8
        li   s3, 9
        li   s4, 10
        li   s5, 11
        li   s6, 12
        add  s7, t1, t2
        mv   s8, s7
        add  s9, t3, t4
        mv   s10, s9
        add  s11, t5, t6
        mv   s7, s11
        add  s9, s1, s2
        mv   s11, s9
        add  s9, s3, s4
        la   t0, b
        sw   t2, 0(t0)
        mv   t2, s9
        add  s9, s5, s6
        la   t0, d
        sw   t4, 0(t0)
        mv   t4, s9
        add  s9, s8, s10
        la   t0, f
        sw   t6, 0(t0)
        mv   t6, s9
        add  s9, s7, s11
        la   t0, h
        sw   s2, 0(t0)
        mv   s2, s9
        add  s9, t2, t4
        la   t0, p5
        sw   t2, 0(t0)
        mv   t2, s9
        add  s9, t6, s2
        la   t0, p6
        sw   t4, 0(t0)
        mv   t4, s9
        add  s9, t4, t2
        la   t0, u3
        sw   t2, 0(t0)
        mv   t2, s9
        add  s9, t1, t3
        la   t0, a
        sw   t1, 0(t0)
        mv   t1, s9
        add  s9, t5, s1
        la   t0, c
        sw   t3, 0(t0)
        mv   t3, s9
        add  s9, s3, s5
        la   t0, v1
        sw   t4, 0(t0)
        mv   t4, s9
        add  s9, t1, t3
        la   t0, w1
        sw   t1, 0(t0)
        mv   t1, s9
        add  s9, t4, t1
        la   t0, w4
        sw   t1, 0(t0)
        mv   t1, s9
        add  s9, t2, t1
        la   t0, w5
        sw   t1, 0(t0)
        mv   t1, s9
    # spill all registers
        la   t0, result
        sw   t1, 0(t0)
        la   t0, v2
        sw   t2, 0(t0)
        la   t0, w2
        sw   t3, 0(t0)
        la   t0, w3
        sw   t4, 0(t0)
        la   t0, e
        sw   t5, 0(t0)
        la   t0, u1
        sw   t6, 0(t0)
        la   t0, g
        sw   s1, 0(t0)
        la   t0, u2
        sw   s2, 0(t0)
        la   t0, i
        sw   s3, 0(t0)
        la   t0, j
        sw   s4, 0(t0)
        la   t0, k
        sw   s5, 0(t0)
        la   t0, l
        sw   s6, 0(t0)
        la   t0, p3
        sw   s7, 0(t0)
        la   t0, p1
        sw   s8, 0(t0)
        la   t0, p2
        sw   s10, 0(t0)
        la   t0, p4
        sw   s11, 0(t0)
        la     t0, result
        lw     a1, 0(t0)
        la     a0, .fmt_int
        call   printf

    # -- global scope epilogue --
        ld   ra, 440(sp)
        ld   s0, 432(sp)
        addi sp, sp, 448
        ret
