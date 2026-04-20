    .data

    # -- Global Scalar Variables --

    # -- global arrays --
        .align 2
    matmul:    .word  0
           .word  0
           .word  0
           .word  0
           .word  0
           .word  0
           .word  0
           .word  0
           .word  0
        .align 2
    mat1:    .word  1
           .word  0
           .word  0
           .word  0
           .word  1
           .word  0
           .word  0
           .word  0
           .word  1
        .align 2
    mat2:    .word  1
           .word  2
           .word  3
           .word  4
           .word  5
           .word  6
           .word  7
           .word  8
           .word  9

    # -- string literals --
    str_0:    .asciz "Printing the result\n"
    str_1:    .asciz "\nDone with printing the result"

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
        addi sp, sp, -528
        sd   ra, 520(sp)
        sd   s0, 512(sp)
        addi s0, sp, 528
        li   t1, 0
    # spill all registers
        sw   t1, -132(s0)

L0:
    # <
        lw   t1, -132(s0)
        slti t2, t1, 3
    # spill all registers
        sw   t2, -208(s0)
        beqz   t2, L1
        li   t1, 0
    # spill all registers
        sw   t1, -136(s0)

L2:
    # <
        lw   t1, -136(s0)
        slti t2, t1, 3
    # spill all registers
        sw   t2, -212(s0)
        beqz   t2, L3
        li   t1, 0
    # spill all registers
        sw   t1, -140(s0)

L4:
    # <
        lw   t1, -140(s0)
        slti t2, t1, 3
    # spill all registers
        sw   t2, -216(s0)
        beqz   t2, L5
        lw   t1, -132(s0)
        li   t2, 3
        mul  t3, t1, t2
        lw   t4, -136(s0)
        add  t5, t3, t4
        li   t6, 4
        mul  s1, t5, t6
    # array access []
        la   s2, matmul
        sub  s3, s2, s1
        lw   s3, 0(s3)
        sw   s1, -220(s0)
        mul  s1, t1, t2
        add  s2, s1, t4
        mul  s4, s2, t6
    # array access []
        la   s5, matmul
        sub  s6, s5, s4
        lw   s6, 0(s6)
        sw   s4, -224(s0)
        mul  s4, t1, t2
        lw   s5, -140(s0)
        add  s7, s4, s5
        mul  s8, s7, t6
    # array access []
        la   s9, mat1
        sub  s10, s9, s8
        lw   s10, 0(s10)
        sw   s8, -228(s0)
        mul  s8, s5, t2
        add  s9, s8, t4
        mul  s11, s9, t6
    # array access []
        la   t3, mat2
        sub  t5, t3, s11
        lw   t5, 0(t5)
        sw   s11, -232(s0)
        mul  t3, s10, t5
        add  s11, s6, t3
        addi t3, s5, 1
        mv   s5, t3
    # spill all registers
        sw   t3, -236(s0)
        sw   t5, -240(s0)
        sw   s1, -244(s0)
        sw   s2, -248(s0)
        sw   s3, -252(s0)
        sw   s4, -256(s0)
        sw   s5, -140(s0)
        sw   s6, -260(s0)
        sw   s7, -264(s0)
        sw   s8, -268(s0)
        sw   s9, -272(s0)
        sw   s10, -276(s0)
        sw   s11, -280(s0)
        j      L4
    # spill all registers

L5:
        lw   t1, -136(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -136(s0)
        sw   t2, -284(s0)
        j      L2
    # spill all registers

L3:
        lw   t1, -132(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -132(s0)
        sw   t2, -288(s0)
        j      L0
    # spill all registers

L1:
    # spill all registers
        la   a0, str_0
        call puts
        li   t1, 0
    # spill all registers
        sw   t1, -132(s0)

L6:
    # <
        lw   t1, -132(s0)
        slti t2, t1, 3
    # spill all registers
        sw   t2, -292(s0)
        beqz   t2, L7
        li   t1, 0
    # spill all registers
        sw   t1, -136(s0)

L8:
    # <
        lw   t1, -136(s0)
        slti t2, t1, 3
    # spill all registers
        sw   t2, -296(s0)
        beqz   t2, L9
        lw   t1, -132(s0)
        li   t2, 3
        mul  t3, t1, t2
        lw   t4, -136(s0)
        add  t5, t3, t4
        li   t6, 4
        mul  s1, t5, t6
    # array access []
        la   s2, matmul
        sub  s3, s2, s1
        lw   s3, 0(s3)
        sw   s1, -300(s0)
        mv     a1, s3
    # spill all registers
        sw   t3, -304(s0)
        sw   t5, -308(s0)
        sw   s3, -312(s0)
        la a0, .fmt_int
        call printf
        lw   t1, -136(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -136(s0)
        sw   t2, -316(s0)
        j      L8
    # spill all registers

L9:
        lw   t1, -132(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -132(s0)
        sw   t2, -320(s0)
        j      L6
    # spill all registers

L7:
    # spill all registers
        la   a0, str_1
        call puts

    # -- global scope epilogue --
        ld   ra, 520(sp)
        ld   s0, 512(sp)
        addi sp, sp, 528
        ret
