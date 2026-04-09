    .section .data

    .section .text
    .globl main
    # assign
        li   t1, 1
    # assign
        li   t2, 2
    # assign
        li   t3, 3
    # assign
        li   t4, 4
    # assign
        li   t5, 5
    # assign
        li   t6, 6
    # assign
        li   t0, 7
    # assign
        li   s1, 8
    # +
    # (debug) store ins. to spill
    sw t1, -8(s0)
        add  t1, t1, t2
    # assign
    # (debug) store ins. to spill
    sw t2, -12(s0)
        mv   t2, t1
    # +
        add  t2, t3, t4
    # assign
    # (debug) store ins. to spill
    sw t2, -40(s0)
        mv   t2, t2
    # +
        add  t2, t5, t6
    # assign
    # (debug) store ins. to spill
    sw t2, -44(s0)
        mv   t2, t2
    # +
        add  t2, t0, s1
    # assign
    # (debug) store ins. to spill
    sw t2, -48(s0)
        mv   t2, t2
    # assign
    # (debug) store ins. to spill
    sw t2, -52(s0)
        li   t2, 9
    # assign
    # (debug) store ins. to spill
    sw t2, -56(s0)
        li   t2, 10
    # +
    # (debug) store ins. to spill
    sw t2, -60(s0)
    lw t2, -40(s0)
    # (debug) store ins. to spill
    sw t3, -16(s0)
    lw t3, -44(s0)
    # (debug) store ins. to spill
    sw t4, -20(s0)
        add  t4, t2, t3
    # +
    # (debug) store ins. to spill
    sw t4, -52(s0)
    lw t4, -48(s0)
    # (debug) store ins. to spill
    sw t5, -24(s0)
        add  t5, t4, t4
    # +
    # (debug) store ins. to spill
    sw t6, -28(s0)
    lw t6, -52(s0)
    # (debug) store ins. to spill
    sw t0, -32(s0)
        add  t0, t5, t6
    # +
    # (debug) store ins. to spill
    sw s1, -36(s0)
    lw s1, -56(s0)
        add  t1, t0, s1
    # +
    lw t1, -60(s0)
        add  t1, t1, t1
    # assign
        mv   t1, t1
    # spill all registers
    # (debug) store ins. to spill
    sw t1, -64(s0)
        lw     a0, -64(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 6
# Stores (sw) : 16
# Total : 22
# --------------------------------------
