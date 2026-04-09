    .section .data

    .section .text
    .globl main
    # assign
        li   t1, 3.140000

    greater:
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
    # >
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: x
    # ISSUE: symbol not found for addres lookup
    lw t3, 0(s0) # unknown: y
        slt  t4, t3, t2
        beqz   t4, L0
    # ISSUE: symbol not found for addres lookup
        lw     a0, 0(s0)
        j      .Lepi_greater
        j      L1
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -8(s0)

    L0:
    # <
    # ISSUE: symbol not found for addres lookup
    lw t1, 0(s0) # unknown: x
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: y
        slt  t3, t1, t2
        beqz   t3, L2
    # ISSUE: symbol not found for addres lookup
        lw     a0, 0(s0)
        j      .Lepi_greater
        j      L1
    # spill all registers

    L2:
    # spill all registers
        li     a0, "equal numbers"
        li     a7, 1
        ecall
    # spill all registers

    L1:
    .Lepi_greater:
    # -- epilogue --
        lw     ra, 76(sp)
        lw     s0, 72(sp)
        addi   sp, sp, 80
        ret
    # -- epilogue end --

    # entity
    # constr
    # assign
        li   t1, 0
    # end_constr
    # a
    # b
    # +
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: a
    # ISSUE: symbol not found for addres lookup
    lw t3, 0(s0) # unknown: b
        add  t4, t2, t3
    # assign
        mv   t1, t4
    # ISSUE: symbol not found for addres lookup
        lw     a0, 0(s0)
    # method
    # end_method
    # end_entity

    main:
    # -- prologue --
        addi   sp, sp, -64
        sw     ra, 60(sp)
        sw     s0, 56(sp)
        addi   s0, sp, 64
    # -- prologue end --

    # spill all registers
        li     a7, 5
        ecall
    # spill all registers
        li     a7, 5
        ecall
    # +
    # ISSUE: symbol not found for addres lookup
    lw t1, 0(s0) # unknown: x
    # ISSUE: symbol not found for addres lookup
    lw t2, 0(s0) # unknown: y
        add  t3, t1, t2
    # assign
        mv   t4, t3
    # spill all registers
        li     a0, "Sum is: "
        li     a7, 1
        ecall
    # spill all registers
    # ISSUE: symbol not found for addres lookup
        lw     a0, 0(s0)
        li     a7, 1
        ecall
    # new object
        li   a0, 4
        call malloc
        mv   t1, a0
    # ISSUE: symbol not found for addres lookup
    sw t1, 0(s0) # unknown: calc
    # call constructor
    # spill all registers
        call Calculator
    # push obj ptr → a0
    # ISSUE: symbol not found for addres lookup
    lw t1, 0(s0) # unknown: calc
        mv   a0, t1
    # call method
    # spill all registers
        call add$ii
        mv   t1, a0
    # ISSUE: symbol not found for addres lookup
    sw t1, 0(s0) # unknown: t4
    # assign
        mv   t2, t1
    # *
    li t3, 0
    li t4, 4
        mul  t5, t3, t4
    # array access []
    # ISSUE: symbol not found for addres lookup
        la   t6, 0(s0) # unknown: numbers
        add  t0, t6, t5
        lw   t0, 0(t0)
    # ISSUE: symbol not found for addres lookup
    sw t0, 0(s0) # unknown: t6
    .Lepi_main:
    # -- epilogue --
        lw     ra, 60(sp)
        lw     s0, 56(sp)
        addi   sp, sp, 64
        ret
    # -- epilogue end --


        li     a7, 10
        ecall
