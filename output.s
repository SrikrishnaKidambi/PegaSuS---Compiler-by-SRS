    .section .data

    .section .text
    .globl main
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
        lw     a0, -16(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall
