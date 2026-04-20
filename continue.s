    .data

    # -- Global Scalar Variables --

    # -- global arrays --

    # -- string literals --

    # -- global objects (pointer slots) --
    # -- I/O format strings --
    .fmt_int:    .asciz  "%d\n"
    .fmt_uint:    .asciz  "%u\n"
    .fmt_float:    .asciz  "%f\n"
    .fmt_str:    .asciz  "%s\n"
    .fmt_char:    .asciz  "%c\n"
    .fmt_scan_int:    .asciz  "%d"
    .fmt_scan_float:    .asciz  "%f\n"
    .fmt_scan_str:    .asciz  "%s\n"

    .text
    .globl main

main:
    # -- main function --
        addi sp, sp, -16
        sd   ra, 8(sp)
        call global_body
        ld   ra, 8(sp)
        addi sp, sp, 16
        li a0, 0
        ret


main_:
    # -- prologue --
        addi   sp, sp, -336
        sd     ra, 328(sp)
        sd     s0, 320(sp)
        addi   s0, sp, 336
    # --initialize local arrays --
    # --initialize local scalars --
    # init local scalar i at offset -24
    # -- prologue end --

        li   t1, 0
    # spill all registers
        sw   t1, -24(s0)

L0:
    # <
        lw   t1, -24(s0)
        slti t2, t1, 5
    # spill all registers
        sw   t2, -92(s0)
        beqz   t2, L1
    # ==
        lw   t1, -24(s0)
        li   t2, 2
        sub  t3, t1, t2
        seqz t3, t3
    # spill all registers
        sw   t3, -96(s0)
        beqz   t3, L2
        lw   t1, -24(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -24(s0)
        sw   t2, -100(s0)
        j      L0
    # spill all registers
        j      L3
    # spill all registers

L2:
    # spill all registers

L3:
    # spill all registers
        lw     a1, -24(s0)
        la a0, .fmt_int
        call printf
        lw   t1, -24(s0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        sw   t1, -24(s0)
        sw   t2, -100(s0)
        j      L0
    # spill all registers

L1:
Lepi_main_:
    # -- epilogue --
        ld     ra, 328(sp)
        ld     s0, 320(sp)
        addi   sp, sp, 336
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -224
        sd   ra, 216(sp)
        sd   s0, 208(sp)
        addi s0, sp, 224
    # spill all registers
        call   main_
        sw     a0, -92(s0)

    # -- global scope epilogue --
        ld   ra, 216(sp)
        ld   s0, 208(sp)
        addi sp, sp, 224
        ret
