    .data

    # -- Global Scalar Variables --
        .align 2
    ans:    .word  0

    # -- global arrays --

    # -- string literals --

    .text
    .globl main
main:
    # -- global scope --
        addi sp, sp, -224
        sw ra, 220(sp)
        sw s0, 216(sp)
        addi s0, sp, 224
        j global_body
    #  -- global scope end --


Calculator_:
    # -- constructor prologue --
        addi sp, sp, -224
        sw   ra, 220(sp)
        sw   s0, 216(sp)
        addi s0, sp, 224
    # save 'this' pointer
        sw   a0, -4(s0)
    # -- initialize local arrays --
    # -- constructor prologue end --

Lepi_Calculator_:
    # -- epilogue --
        lw     ra, 220(sp)
        lw     s0, 216(sp)
        addi   sp, sp, 224
        ret
    # -- epilogue end --


add_ii:
    # -- method prologue -- 
        addi sp, sp, -224
        sw   ra, 220(sp)
        sw   s0, 216(sp)
        addi s0, sp, 224
        sw   a0, -4(s0)
        sw   a1, -8(s0)
        sw   a2, -12(s0)
    # -- initialize local arrays --
    # -- method prologue end --

    # a
    # b
        lw   t1, -8(s0)
        lw   t2, -12(s0)
        add  t3, t1, t2
        mv     a0, t3
        j      Lepi_add_ii
Lepi_add_ii:
    # -- epilogue --
        lw     ra, 220(sp)
        lw     s0, 216(sp)
        addi   sp, sp, 224
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
    # new object
    # spill all registers
        li   a0, 0
        call malloc
        sw   a0, 0(s0)
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call_constr
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Calculator_
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 3
        li   a2, 5
        lw a0, 0(sp)
        addi sp, sp, 4
        call add_ii
        sw     a0, -80(s0)
        lw   t1, -80(s0)
        mv   t2, t1
        mv     a0, t2
    # spill all registers
        la   t0, ans
        sw   t2, 0(t0)
        li     a7, 1
        ecall
        li     a0, 10
        li     a7, 11
        ecall

    # -- global scope epilogue --
        lw ra, 220(sp)
        lw s0, 216(sp)
        addi sp, sp, 224
        li a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 16
# Stores (sw) : 13
# Total : 29
# --------------------------------------
