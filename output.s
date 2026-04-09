    .section .data

    # -- Global Scalar Variables --

    # -- global arrays --

    # -- string literals --

    .section .text
    .globl main
        li   t1, 0
        li   t2, 0

    Point$:
    # -- constructor prologue --
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
    # save 'this' pointer
        sw   a0, -4(s0)
    # -- constructor prologue end --

    .Lepi_Point$:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --

    # a
    # m
        li   t1, a
        li   t2, 0

    Point$is:
    # -- constructor prologue --
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
    # save 'this' pointer
        sw   a0, -4(s0)
        sw   a1, 0(s0)
        sw   a2, 0(s0)
    # -- constructor prologue end --

    .Lepi_Point$is:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --

    # a
    # b
        li   t1, a
        li   t2, b

    Point$is:
    # -- constructor prologue --
        addi sp, sp, -80
        sw   ra, 76(sp)
        sw   s0, 72(sp)
        addi s0, sp, 80
    # save 'this' pointer
        sw   a0, -4(s0)
        sw   a1, 0(s0)
        sw   a2, 0(s0)
    # -- constructor prologue end --

    .Lepi_Point$is:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --

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
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Point$
    # new object
    # spill all registers
        li   a0, 8
        call malloc
        sw   a0, -24(s0)
    # push_ptr: load obj pointer into a0
        lw   a0, -24(s0)
    # call_constr
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 5
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Point$i
    # new object
    # spill all registers
        li   a0, 8
        call malloc
        sw   a0, -32(s0)
    # push_ptr: load obj pointer into a0
        lw   a0, -32(s0)
    # call_constr
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 3
        li   a2, 4
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Point$ii
    # spill all registers
        lw     a0, -16(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -24(s0)
        li     a7, 1
        ecall
    # spill all registers
        lw     a0, -32(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 31
# Stores (sw) : 19
# Total : 50
# --------------------------------------
