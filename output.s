    .section .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Hello"

    .section .text
    .globl main
    # m
    # set_field
        lw  t1, -4(s0)
        lw  t2, 0(s0)
        sw  t2, 0(t1)
    # spill all registers
        lw     a0, 0(s0)
        li     a7, 1
        ecall

    Calculator$s:
    # -- constructor prologue --
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
    # save 'this' pointer
        sw   a0, -4(s0)
        sw   a1, 0(s0)
    # -- constructor prologue end --

    .Lepi_Calculator$s:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --

    # x
    # y
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: x
        addi t1, t2, y
        li   t3, t0
        lw     a0, 0(s0)

    add$ii:
    # -- method prologue -- 
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
        sw   a0, -4(s0)
        sw   a1, 0(s0)
        sw   a2, 0(s0)
    # -- method prologue end --

    .Lepi_add$ii:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --

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
        li   a1, "Hello"
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Calculator$s
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 5
        li   a2, 6
        lw a0, 0(sp)
        addi sp, sp, 4
        call add$ii
        sw  a0, 0(s0)
        li   t1, t1
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -8(s0)
        lw     a0, -4(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 23
# Stores (sw) : 15
# Total : 38
# --------------------------------------
