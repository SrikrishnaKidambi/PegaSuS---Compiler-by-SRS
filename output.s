    .section .data

    .section .text
    .globl main
<<<<<<< HEAD
    # assign
        li   t1, "raghavendra"
    sw t1, 0(s0)
    # assign
        li   t2, 20
    sw t2, -8(s0)
    # +
    li t3, 20
        add  t4, t2, t3
    # ISSUE: symbol not found for addres lookup
    sw t4, 0(s0) # unknown: t0
    # assign
        mv   t2, t4
    sw t2, -12(s0)
    # *
    lw t3, -8(s0)
        mul  t4, t2, t3
    # ISSUE: symbol not found for addres lookup
    sw t4, 0(s0) # unknown: t1
    # assign
        mv   t2, t4
    sw t2, -16(s0)
    # spill all registers
=======
    # assign
        li   t1, 20
    # +
    li t2, 20
        add  t3, t1, t2
    # assign
        mv   t4, t3
    # *
        mul  t5, t4, t1
    # assign
        mv   t6, t5
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -8(s0)
    # (debug) store ins. to spill
    sw t4, -12(s0)
    # (debug) store ins. to spill
    sw t6, -16(s0)
>>>>>>> 362cfa83a4183c39fdf4dfb772e627e03de77b25
        lw     a0, -16(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall
