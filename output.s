    .section .data

    .section .text
    .globl main

    add:
    # -- prologue --
        addi   sp, sp, -80
        sw     ra, 76(sp)
        sw     s0, 72(sp)
        addi   s0, sp, 80
    # ISSUE: symbol not found for addres lookup
        sw     a0, 0(s0)
    # ISSUE: symbol not found for addres lookup
        sw     a1, 0(s0)
    # -- prologue end --

    # x
    # y
    # +
    # ISSUE: symbol not found for addres lookup
    lw t1, 0(s0) # unknown: x
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: y
        add  t3, t1, t2
    # ISSUE: symbol not found for addres lookup
    sw t3, 0(s0) # unknown: t0
    # ISSUE: symbol not found for addres lookup
        lw     a0, 0(s0)
        j      .Lepi_add
    .Lepi_add:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --

    # assign
        li   t1, 0
    sw t1, 0(s0)
    # spill all registers
        li     a0, 5
        li     a1, 6
        call   add
    # ISSUE: symbol not found for addres lookup
        sw     a0, 0(s0)
    # assign
    # ISSUE: symbol not found for addres lookup
    lw t1, 0(s0) # unknown: t1
        mv   t2, t1
    sw t2, 0(s0)
    # spill all registers
        lw     a0, 0(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall
