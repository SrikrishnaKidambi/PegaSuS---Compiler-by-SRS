    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "Saiman"

    # -- global objects (pointer slots) --
        .align 3
    P:    .dword 0
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


Patient_sii:
    # -- constructor prologue --
        addi sp, sp, -256
        sd   ra, 248(sp)
        sd   s0, 240(sp)
        addi s0, sp, 256
    # save 'this' pointer
        sd   a0, -24(s0)
        sw   a1, -40(s0)
        sw   a2, -48(s0)
        sw   a3, -52(s0)
    # -- initialize local arrays --
    # -- constructor prologue end --

    # name
    # fee
    # days
    # set_field
        ld  t1, -24(s0)
        ld   t2, -40(s0)
        sd   t2, 0(t1)
    # set_field
        ld  t1, -24(s0)
        lw   t2, -48(s0)
        sw   t2, 8(t1)
    # set_field
        ld  t1, -24(s0)
        lw   t2, -52(s0)
        sw   t2, 12(t1)
Lepi_Patient_sii:
    # -- epilogue --
        ld     ra, 248(sp)
        ld     s0, 240(sp)
        addi   sp, sp, 256
        ret
    # -- epilogue end --


total_fee_:
    # -- method prologue -- 
        addi sp, sp, -240
        sd   ra, 232(sp)
        sd   s0, 224(sp)
        addi s0, sp, 240
        sd   a0, -24(s0)
    # -- initialize local arrays --
    # -- method prologue end --

        lw   t1, -32(s0)
        lw   t2, -36(s0)
        mul  t3, t1, t2
        mv     a0, t3
        j      Lepi_total_fee_
Lepi_total_fee_:
    # -- epilogue --
        ld     ra, 232(sp)
        ld     s0, 224(sp)
        addi   sp, sp, 240
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -288
        sd   ra, 280(sp)
        sd   s0, 272(sp)
        addi s0, sp, 288
    # new object
    # spill all registers
        li   a0, 16
        call malloc
        la   t0, P
        sd   a0, 0(t0)
    # push_ptr: load obj pointer into a0
        la   t0, P
        ld   a0, 0(t0)
    # call_constr
        addi sp, sp, -16
        sd   a0, 0(sp)
    # spill all registers
        li   a1, "Saiman"
        li   a2, 200
        li   a3, 5
        ld   a0, 0(sp)
        addi sp, sp, 16
        call Patient_sii
    # push_ptr: load obj pointer into a0
        la   t0, P
        ld   a0, 0(t0)
    # call method
        addi sp, sp, -16
        sd   a0, 0(sp)
    # spill all registers
        ld   a0, 0(sp)
        addi sp, sp, 16
        call total_fee_
        sw   a0, -132(s0)
        lw   t1, -132(s0)
        mv   t2, t1
    # spill all registers
        la   t0, result
        sw   t2, 0(t0)
        la t0, result
        lw a1, 0(t0)
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 280(sp)
        ld   s0, 272(sp)
        addi sp, sp, 288
        ret
