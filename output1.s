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
    # +
    # (debug) store ins. to spill
    sw t2, -12(s0)
        add  t2, t1, t3
    # +
    # (debug) store ins. to spill
    sw t3, -16(s0)
        add  t3, t2, t4
    # +
    # (debug) store ins. to spill
    sw t4, -20(s0)
        add  t4, t3, t5
    # +
    # (debug) store ins. to spill
    sw t5, -24(s0)
        add  t5, t4, t6
    # +
    # (debug) store ins. to spill
    sw t6, -28(s0)
        add  t6, t5, t0
    # +
    # (debug) store ins. to spill
    sw t0, -32(s0)
        add  t0, t6, s1
    # assign
    # (debug) store ins. to spill
    sw s1, -36(s0)
        mv   s1, t0
    # spill all registers
    # (debug) store ins. to spill
    sw s1, -40(s0)
        lw     a0, -40(s0)
        li     a7, 1
        ecall

        li     a7, 10
        ecall

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 0
# Stores (sw) : 9
# Total : 9
# --------------------------------------
