    .section .data

    .section .text
    .globl main
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
        lw     a0, -16(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall
