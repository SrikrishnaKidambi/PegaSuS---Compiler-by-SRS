.section .text
.globl main

    # assign
    li   t0, 20
    # +
    add  t0, t0, t0
    # assign
    mv   t0, t0
    # *
    mul  t0, t0, t0
    # assign
    mv   t0, t0
