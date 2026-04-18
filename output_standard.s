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
    # assign
        li   t1, 1
    # assign
        li   t2, 2
    # assign
        li   t3, 3
    # assign
        li   t4, 4
    # assign
        li   t5, 5
    # assign
        li   t6, 6
    # assign
        li   s1, 7
    # assign
        li   s2, 8
    # assign
        li   s3, 9
    # assign
        li   s4, 10
    # assign
        li   s5, 11
    # assign
        li   s6, 12
    # +
        add  s7, t1, t2
    # assign
        mv   s8, s7
    # +
        add  s9, t3, t4
    # assign
        mv   s10, s9
    # +
        add  s11, t5, t6
    # assign
        la   t0, a
        sw   t1, 0(t0)
        mv   t1, s11
    # +
        la   t0, p3
        sw   t1, 0(t0)
        add  t1, s1, s2
    # assign
        la   t0, b
        sw   t2, 0(t0)
        mv   t2, t1
    # +
        la   t0, p4
        sw   t2, 0(t0)
        add  t2, s3, s4
    # assign
        la   t0, c
        sw   t3, 0(t0)
        mv   t3, t2
    # +
        la   t0, p5
        sw   t3, 0(t0)
        add  t3, s5, s6
    # assign
        la   t0, d
        sw   t4, 0(t0)
        mv   t4, t3
    # +
        la   t0, p6
        sw   t4, 0(t0)
        add  t4, s8, s10
    # assign
        la   t0, e
        sw   t5, 0(t0)
        mv   t5, t4
    # +
        la   t0, u1
        sw   t5, 0(t0)
        la   t0, p3
        lw   t5, 0(t0)
        la   t0, f
        sw   t6, 0(t0)
        la   t0, p4
        lw   t6, 0(t0)
        la   t0, g
        sw   s1, 0(t0)
        add  s1, t5, t6
    # assign
        la   t0, h
        sw   s2, 0(t0)
        mv   s2, s1
    # +
        la   t0, u2
        sw   s2, 0(t0)
        la   t0, p5
        lw   s2, 0(t0)
        la   t0, i
        sw   s3, 0(t0)
        la   t0, p6
        lw   s3, 0(t0)
        la   t0, j
        sw   s4, 0(t0)
        add  s4, s2, s3
    # assign
        la   t0, k
        sw   s5, 0(t0)
        mv   s5, s4
    # +
        la   t0, u3
        sw   s5, 0(t0)
        la   t0, u1
        lw   s5, 0(t0)
        la   t0, l
        sw   s6, 0(t0)
        la   t0, u2
        lw   s6, 0(t0)
        la   t0, p1
        sw   s8, 0(t0)
        add  s8, s5, s6
    # assign
        la   t0, p2
        sw   s10, 0(t0)
        mv   s10, s8
    # +
        la   t0, v1
        sw   s10, 0(t0)
        la   t0, u3
        lw   s10, 0(t0)
        sw   t1, -188(s0)
        add  t1, s10, s10
    # assign
        sw   t1, -192(s0)
        mv   t1, t1
    # +
        la   t0, v2
        sw   t1, 0(t0)
        la   t0, a
        lw   t1, 0(t0)
        la   t0, c
        lw   t1, 0(t0)
        add  t1, t1, t1
    # assign
        sw   t1, -196(s0)
        mv   t1, t1
    # +
        la   t0, w1
        sw   t1, 0(t0)
        la   t0, e
        lw   t1, 0(t0)
        la   t0, g
        lw   t1, 0(t0)
        add  t1, t1, t1
    # assign
        sw   t1, -200(s0)
        mv   t1, t1
    # +
        la   t0, w2
        sw   t1, 0(t0)
        la   t0, i
        lw   t1, 0(t0)
        la   t0, k
        lw   t1, 0(t0)
        add  t1, t1, t1
    # assign
        sw   t1, -204(s0)
        mv   t1, t1
    # +
        la   t0, w3
        sw   t1, 0(t0)
        la   t0, w1
        lw   t1, 0(t0)
        la   t0, w2
        lw   t1, 0(t0)
        add  t1, t1, t1
    # assign
        sw   t1, -208(s0)
        mv   t1, t1
    # +
        la   t0, w4
        sw   t1, 0(t0)
        la   t0, w3
        lw   t1, 0(t0)
        la   t0, w4
        lw   t1, 0(t0)
        add  t1, t1, t1
    # assign
        sw   t1, -212(s0)
        mv   t1, t1
    # +
        la   t0, w5
        sw   t1, 0(t0)
        la   t0, v2
        lw   t1, 0(t0)
        la   t0, w5
        lw   t1, 0(t0)
        add  t1, t1, t1
    # assign
        sw   t1, -216(s0)
        mv   t1, t1
    # spill all registers
        la   t0, result
        sw   t1, 0(t0)
        la t0, result
        lw a1, 0(t0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 440(sp)
        ld   s0, 432(sp)
        addi sp, sp, 448
        ret
