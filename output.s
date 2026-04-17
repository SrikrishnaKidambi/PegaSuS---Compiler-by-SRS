    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0
        .align 2
<<<<<<< HEAD
    a:    .word  1
        .align 2
    b:    .word  2
=======
    n:    .word  3
>>>>>>> 4069438bf062e17b842d1611d57e3a519cc8175f

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Going to return the"
    str_1:    .asciz "The result is:"

    .text
    .globl main
    main:
    # -- global scope --
        addi sp, sp, -208
        sw ra, 204(sp)
        sw s0, 200(sp)
        addi s0, sp, 208
        j global_body
    #  -- global scope end --


    add_ii:
    # -- prologue --
        addi   sp, sp, -208
        sw     ra, 204(sp)
        sw     s0, 200(sp)
        addi   s0, sp, 208
        sw     a0, -8(s0)
<<<<<<< HEAD
        sw     a1, -12(s0)
=======
    # --initialize local arrays --
>>>>>>> 4069438bf062e17b842d1611d57e3a519cc8175f
    # -- prologue end --

    # x
    # y
    # spill all registers
        la     a0, str_0
        li     a7, 4
        ecall
        lw   t1, -8(s0)
<<<<<<< HEAD
        lw   t2, -12(s0)
        add  t3, t1, t2
=======
    li t2, 0
        sub  t3, t1, t2
        seqz t3, t3
        beqz   t3, L0
        sw t3, -76(s0)
        li     a0, 1
        j      Lepi_fact_i
    # spill all registers
        j      L1
    # spill all registers

    L0:
    # spill all registers

    L1:
        lw   t1, -8(s0)
        addi t2, t1, -1
    # spill all registers
        lw     a0, -80(s0)
        call   fact
        sw     a0, -84(s0)
        lw   t1, -84(s0)
        lw   t2, -8(s0)
        mul  t3, t1, t2
>>>>>>> 4069438bf062e17b842d1611d57e3a519cc8175f
        mv     a0, t3
        j      Lepi_add_ii
    Lepi_add_ii:
    # -- epilogue --
        lw     ra, 204(sp)
        lw     s0, 200(sp)
        addi   sp, sp, 208
        ret
    # -- epilogue end --


    global_body:
    # -- Global body --
<<<<<<< HEAD
        li   t1, 1
        li   t2, 2
    # spill all registers
        la   t0, a
        sw   t1, 0(t0)
        la   t0, b
        sw   t2, 0(t0)
        la     t0, a
        lw     a0, 0(t0)
        la     t0, b
        lw     a1, 0(t0)
        call   add_ii
        sw     a0, -84(s0)
        lw   t1, -84(s0)
=======
        li   t2, 3
    # spill all registers
        sw t2, -8(s0)
        lw     a0, -8(s0)
        call   fact_i
        sw     a0, -80(s0)
        lw   t1, -80(s0)
>>>>>>> 4069438bf062e17b842d1611d57e3a519cc8175f
        mv   t2, t1
    # spill all registers
        la   t0, result
        sw   t2, 0(t0)
        la     a0, str_1
        li     a7, 4
        ecall
    # spill all registers
        la     t0, result
        lw     a0, 0(t0)
        li     a7, 1
        ecall

    # -- global scope epilogue --
        lw ra, 204(sp)
        lw s0, 200(sp)
        addi sp, sp, 208
        li a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 16
# Stores (sw) : 8
# Total : 24
# --------------------------------------
