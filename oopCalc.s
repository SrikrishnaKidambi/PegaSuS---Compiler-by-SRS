    .data

    # -- Global Scalar Variables --
        .align 2
    ans:    .word  0

    # -- global arrays --

    # -- string literals --

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


Calculator_:
    # -- constructor prologue --
        addi sp, sp, -224
        sd   ra, 216(sp)
        sd   s0, 208(sp)
        addi s0, sp, 224
    # save 'this' pointer
        sw   a0, -4(s0)
    # -- initialize local arrays --
    # -- constructor prologue end --

Lepi_Calculator_:
    # -- epilogue --
        ld     ra, 216(sp)
        ld     s0, 208(sp)
        addi   sp, sp, 224
        ret
    # -- epilogue end --


add_ii:
    # -- method prologue -- 
        addi sp, sp, -224
        sd   ra, 216(sp)
        sd   s0, 208(sp)
        addi s0, sp, 224
        sw   a0, -4(s0)
        sw   a1, -20(s0)
        sw   a2, -24(s0)
    # -- initialize local arrays --
    # -- method prologue end --

    # a
    # b
        lw   t1, -20(s0)
        lw   t2, -24(s0)
        add  t3, t1, t2
        mv     a0, t3
        j      Lepi_add_ii
Lepi_add_ii:
    # -- epilogue --
        ld     ra, 216(sp)
        ld     s0, 208(sp)
        addi   sp, sp, 224
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -224
        sd   ra, 216(sp)
        sd   s0, 208(sp)
        addi s0, sp, 224
    # new object
    # spill all registers
        li   a0, 0
        call malloc
        sw   a0, 0(s0)
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call_constr
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        lw   a0, 0(sp)
        addi sp, sp, 4
        call Calculator_
    # push_ptr: load obj pointer into a0
        lw   a0, 0(s0)
    # call method
        addi sp, sp, -4
        sw   a0, 0(sp)
    # spill all registers
        li   a1, 3
        li   a2, 5
        lw a0, 0(sp)
        addi sp, sp, 4
        call add_ii
        sw     a0, -92(s0)
        lw   t1, -92(s0)
        mv   t2, t1
    # spill all registers
        la   t0, ans
        sw   t2, 0(t0)
        la t0, ans
        lw a1, 0(t0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 216(sp)
        ld   s0, 208(sp)
        addi sp, sp, 224
        ret
