    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Hello"

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


    Calculator_s:
    # -- constructor prologue --
        addi sp, sp, -240
        sw   ra, 236(sp)
        sw   s0, 232(sp)
        addi s0, sp, 240
    # save 'this' pointer
        sw   a0, -4(s0)
        sw   a1, -8(s0)
    # -- constructor prologue end --

    # m
    # set_field
        lw  t1, -4(s0)
        lw  t2, -8(s0)
        sw  t2, 0(t1)
    # spill all registers
        addi   a0, s0, -8
        li     a7, 4
        ecall
    Lepi_Calculator_s:
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

    # x
    # y
        lw   t1, -8(s0)
        lw   t2, -12(s0)
        add  t3, t1, t2
        lw     a0, -16(s0)
        j      Lepi_add_ii
    Lepi_add_ii:
    # -- epilogue --
        lw     ra, 236(sp)
        lw     s0, 232(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --


    global_body:
    # -- Global body --
    # new object
    # spill all registers
        li   a0, 0
        call malloc
        sw   a0, -8(s0)
    # push_ptr: load obj pointer into a0
        lw   a0, -8(s0)
    # call_constr
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, "Hello"
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Calculator_s
    # push_ptr: load obj pointer into a0
        lw   a0, -8(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 5
        li   a2, 6
        lw a0, 0(sp)
        addi sp, sp, 4
        call add_ii
        sw  a0, -84(s0)
        lw   t1, -84(s0)
        mv   t2, t1
        mv     a0, t2
    # spill all registers
        sw t2, -8(s0)
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
# Loads (lw/li): 21
# Stores (sw) : 15
# Total : 36
# --------------------------------------
