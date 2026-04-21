    .data

    # -- Global Scalar Variables --
        .align 2
    n:    .word  10

    # -- global arrays --
        .align 2
    arr:    .word  12
           .word  11
           .word  13
           .word  5
           .word  6
           .word  7
           .word  1
           .word  3
           .word  9
           .word  2
        .align 2
    L:    .space 40   # array [10] elem_size=4
        .align 2
    R:    .space 40   # array [10] elem_size=4

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
        sw   a0, -148(s0)
        sw   a1, -152(s0)
        sw   a2, -156(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # init local scalar n1 at offset -160
    # init local scalar n2 at offset -164
    # -- prologue end --

    # l
    # m
    # r
        lw   t1, -152(s0)
        lw   t2, -148(s0)
        sub  t3, t1, t2
        addi t4, t3, 1
        mv   t5, t4
        lw   t6, -156(s0)
        sub  s1, t6, t1
        mv   s2, s1
        li   s3, 0
    # spill all registers
        sw   t3, -244(s0)
        sw   t4, -248(s0)
        sw   t5, -160(s0)
        sw   s1, -252(s0)
        sw   s2, -164(s0)
        sw   s3, -168(s0)

L0:
    # <
        lw   t1, -168(s0)
        lw   t2, -160(s0)
        slt  t3, t1, t2
    # spill all registers
        sw   t3, -256(s0)
        beqz   t3, L1
        lw   t1, -148(s0)
        lw   t2, -168(s0)
        add  t3, t1, t2
        li   t4, 4
        mul  t5, t3, t4
    # array access []
        la   t6, arr
        add  s1, t6, t5
        lw   s1, 0(s1)
        sw   t5, -260(s0)
        mul  t5, t2, t4
    # array store []=
        la   t6, L
        add  s2, t6, t5
        sw   s1, 0(s2)
        addi t6, t2, 1
        mv   t2, t6
    # spill all registers
        sw   t2, -168(s0)
        sw   t3, -264(s0)
        sw   t5, -268(s0)
        sw   t6, -272(s0)
        sw   s1, -276(s0)
        j      L0
    # spill all registers

L1:
        li   t1, 0
    # spill all registers
        sw   t1, -172(s0)

L2:
    # <
        lw   t1, -172(s0)
        lw   t2, -164(s0)
        slt  t3, t1, t2
    # spill all registers
        sw   t3, -280(s0)
        beqz   t3, L3
        lw   t1, -152(s0)
        addi t2, t1, 1
        lw   t3, -172(s0)
        add  t4, t2, t3
        li   t5, 4
        mul  t6, t4, t5
    # array access []
        la   s1, arr
        add  s2, s1, t6
        lw   s2, 0(s2)
        sw   t6, -284(s0)
        mul  t6, t3, t5
    # array store []=
        la   s1, R
        add  s3, s1, t6
        sw   s2, 0(s3)
        addi s1, t3, 1
        mv   t3, s1
    # spill all registers
        sw   t2, -288(s0)
        sw   t3, -172(s0)
        sw   t4, -292(s0)
        sw   t6, -296(s0)
        sw   s1, -300(s0)
        sw   s2, -304(s0)
        j      L2
    # spill all registers

L3:
        li   t1, 0
        li   t2, 0
        lw   t3, -148(s0)
    # spill all registers
        sw   t1, -168(s0)
        sw   t2, -172(s0)
        sw   t3, -176(s0)

L4:
    # <
        lw   t1, -168(s0)
        lw   t2, -160(s0)
        slt  t3, t1, t2
    # <
        lw   t4, -172(s0)
        lw   t5, -164(s0)
        slt  t6, t4, t5
    # &&
        and  s1, t3, t6
    # spill all registers
        sw   t3, -308(s0)
        sw   t6, -312(s0)
        sw   s1, -316(s0)
        beqz   s1, L5
        lw   t1, -168(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, L
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -320(s0)
        lw   t3, -172(s0)
        mul  t4, t3, t2
    # array access []
        la   t6, R
        add  s1, t6, t4
        lw   s1, 0(s1)
        sw   t4, -324(s0)
    # <
        slt  t4, t5, s1
        mul  t6, t1, t2
    # array access []
        la   s2, L
        add  s3, s2, t6
        lw   s3, 0(s3)
        sw   t6, -328(s0)
        mul  t6, t3, t2
    # array access []
        la   s2, R
        add  s4, s2, t6
        lw   s4, 0(s4)
        sw   t6, -332(s0)
    # ==
        sub  t6, s3, s4
        seqz t6, t6
    # ||
        or   s2, t4, t6
    # spill all registers
        sw   t4, -336(s0)
        sw   t5, -340(s0)
        sw   t6, -344(s0)
        sw   s1, -348(s0)
        sw   s2, -352(s0)
        sw   s3, -356(s0)
        sw   s4, -360(s0)
        beqz   s2, L6
        lw   t1, -168(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, L
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -364(s0)
        lw   t3, -176(s0)
        mul  t4, t3, t2
    # array store []=
        la   t6, arr
        add  s1, t6, t4
        sw   t5, 0(s1)
        addi t6, t1, 1
        mv   t1, t6
    # spill all registers
        sw   t1, -168(s0)
        sw   t4, -368(s0)
        sw   t5, -372(s0)
        sw   t6, -376(s0)
        j      L7
    # spill all registers

L6:
        lw   t1, -172(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, R
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -380(s0)
        lw   t3, -176(s0)
        mul  t4, t3, t2
    # array store []=
        la   t6, arr
        add  s1, t6, t4
        sw   t5, 0(s1)
        addi t6, t1, 1
        mv   t1, t6
    # spill all registers
        sw   t1, -172(s0)
        sw   t4, -384(s0)
        sw   t5, -388(s0)
        sw   t6, -392(s0)

L7:
        lw   t1, -176(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -176(s0)
        sw   t2, -396(s0)
        j      L4
    # spill all registers

L5:
    # spill all registers

L8:
    # <
        lw   t1, -168(s0)
        lw   t2, -160(s0)
        slt  t3, t1, t2
    # spill all registers
        sw   t3, -400(s0)
        beqz   t3, L9
        lw   t1, -168(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, L
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -404(s0)
        lw   t3, -176(s0)
        mul  t4, t3, t2
    # array store []=
        la   t6, arr
        add  s1, t6, t4
        sw   t5, 0(s1)
        addi t6, t1, 1
        mv   t1, t6
        addi s1, t3, 1
        mv   t3, s1
    # spill all registers
        sw   t1, -168(s0)
        sw   t3, -176(s0)
        sw   t4, -408(s0)
        sw   t5, -412(s0)
        sw   t6, -416(s0)
        sw   s1, -420(s0)
        j      L8
    # spill all registers

L9:
    # spill all registers

L10:
    # <
        lw   t1, -172(s0)
        lw   t2, -164(s0)
        slt  t3, t1, t2
    # spill all registers
        sw   t3, -424(s0)
        beqz   t3, L11
        lw   t1, -172(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, R
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -428(s0)
        lw   t3, -176(s0)
        mul  t4, t3, t2
    # array store []=
        la   t6, arr
        add  s1, t6, t4
        sw   t5, 0(s1)
        addi t6, t1, 1
        mv   t1, t6
        addi s1, t3, 1
        mv   t3, s1
    # spill all registers
        sw   t1, -172(s0)
        sw   t3, -176(s0)
        sw   t4, -432(s0)
        sw   t5, -436(s0)
        sw   t6, -440(s0)
        sw   s1, -444(s0)
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
        sw   a0, -148(s0)
        sw   a1, -152(s0)
    # --initialize local arrays --
    # --initialize local scalars --
    # init local scalar m at offset -156
    # -- prologue end --

    # l
    # r
    # <
        lw   t1, -148(s0)
        lw   t2, -152(s0)
        slt  t3, t1, t2
    # spill all registers
        sw   t3, -224(s0)
        beqz   t3, L12
        lw   t1, -152(s0)
        lw   t2, -148(s0)
        sub  t3, t1, t2
        li   t4, 2
        div  t5, t3, t4
        add  t6, t2, t5
        mv   s1, t6
        mv     a0, t2
        mv     a1, s1
    # spill all registers
        sw   t3, -228(s0)
        sw   t5, -232(s0)
        sw   t6, -236(s0)
        sw   s1, -156(s0)
        call   mergeSort_ii
        sw     a0, -240(s0)
        lw   t1, -156(s0)
        addi t2, t1, 1
        mv     a0, t2
    # spill all registers
        sw   t2, -244(s0)
        lw     a1, -152(s0)
        call   mergeSort_ii
        sw     a0, -248(s0)
    # spill all registers
        lw     a0, -148(s0)
        lw     a1, -156(s0)
        lw     a2, -152(s0)
        call   merge_iii
        sw     a0, -252(s0)
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
    # -- prologue end --

        la   t0, n
        lw   t1, 0(t0)
        addi t2, t1, -1
        li     a0, 0
        mv     a1, t2
    # spill all registers
        sw   t2, -216(s0)
        call   mergeSort_ii
        sw     a0, -220(s0)
        li   t1, 0
    # spill all registers
        sw   t1, -148(s0)

L14:
    # <
        lw   t1, -148(s0)
        la   t0, n
        lw   t2, 0(t0)
        slt  t3, t1, t2
    # spill all registers
        sw   t3, -224(s0)
        beqz   t3, L15
        lw   t1, -148(s0)
        li   t2, 4
        mul  t3, t1, t2
    # array access []
        la   t4, arr
        add  t5, t4, t3
        lw   t5, 0(t5)
        sw   t3, -228(s0)
        mv     a1, t5
    # spill all registers
        sw   t5, -232(s0)
        la a0, .fmt_int
        call printf
    # spill all registers
        la   a0, str_0
        call puts
        lw   t1, -148(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -148(s0)
        sw   t2, -236(s0)
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
        addi sp, sp, -880
        sd   ra, 872(sp)
        sd   s0, 864(sp)
        addi s0, sp, 880
    # array_init
        li   t1, 10
    # spill all registers
        la   t0, n
        sw   t1, 0(t0)
        call   main_
        sw     a0, -244(s0)

    # -- global scope epilogue --
        ld   ra, 872(sp)
        ld   s0, 864(sp)
        addi sp, sp, 880
        ret
