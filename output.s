    .data

    # -- Global Scalar Variables --
        .align 2
<<<<<<< HEAD
    ans:    .word  
=======
    ans:    .word  0
>>>>>>> fc4295436a39c3c4f62e2f140d1707b1f1655e1d

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Casio"

    .text
    .globl main
    main:
    # -- global scope --
        addi sp, sp, -256
        sw ra, 252(sp)
        sw s0, 248(sp)
        addi s0, sp, 256
        j global_body
    #  -- global scope end --

<<<<<<< HEAD
=======

>>>>>>> fc4295436a39c3c4f62e2f140d1707b1f1655e1d
    Calculator_s:
    # -- constructor prologue --
        addi sp, sp, -240
        sw   ra, 236(sp)
        sw   s0, 232(sp)
        addi s0, sp, 240
    # save 'this' pointer
        sw   a0, -4(s0)
<<<<<<< HEAD
        sw   a1, 0(s0)
=======
        sw   a1, -8(s0)
>>>>>>> fc4295436a39c3c4f62e2f140d1707b1f1655e1d
    # -- constructor prologue end --

    # m
    # set_field
        lw  t1, -4(s0)
<<<<<<< HEAD
        lw  t2, 0(s0)
        sw  t2, 0(t1)
    .Lepi_Calculator_s:
=======
        lw  t2, -8(s0)
        sw  t2, 0(t1)
    Lepi_Calculator_s:
>>>>>>> fc4295436a39c3c4f62e2f140d1707b1f1655e1d
    # -- epilogue --
        lw     ra, 236(sp)
        lw     s0, 232(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --


    add_ii:
    # -- method prologue -- 
        addi sp, sp, -240
        sw   ra, 236(sp)
        sw   s0, 232(sp)
        addi s0, sp, 240
        sw   a0, -4(s0)
        sw   a1, -8(s0)
        sw   a2, -12(s0)
    # -- method prologue end --

    # a
    # b
    lw t2, -8(s0)
    lw t2, -8(s0)
    lw t3, -12(s0)
    lw t3, -12(s0)
        add  t1, t2, t3
        mv     a0, t1
        j      Lepi_add_ii
    Lepi_add_ii:
    # -- epilogue --
        lw     ra, 236(sp)
        lw     s0, 232(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --

<<<<<<< HEAD
=======

    global_body:
    # -- Global body --
>>>>>>> fc4295436a39c3c4f62e2f140d1707b1f1655e1d
    # new object
    # spill all registers
        li   a0, 8
        call malloc
        sw   a0, -16(s0)
    # push_ptr: load obj pointer into a0
        lw   a0, -16(s0)
    # call_constr
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, "Casio"
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Calculator_s
    # push_ptr: load obj pointer into a0
        lw   a0, -16(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 3
        li   a2, 5
        lw a0, 0(sp)
        addi sp, sp, 4
        call add_ii
<<<<<<< HEAD
        sw  a0, 0(s0)
        li   t1, t1
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -24(s0)
        lw     a0, -20(s0)
=======
        sw  a0, -28(s0)
    # ISSUE: symbol not found for addres lookup
    lw t1, 0(s0) # unknown: t1
        lw t1, -28(s0)
        mv   t2, t1
    # spill all registers
        sw t2, -24(s0)
        lw     a0, -24(s0)
>>>>>>> fc4295436a39c3c4f62e2f140d1707b1f1655e1d
        li     a7, 1
        ecall

    # -- global scope epilogue --
        lw ra, 252(sp)
        lw s0, 248(sp)
        addi sp, sp, 256
        li a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 20
# Stores (sw) : 15
# Total : 35
# --------------------------------------
