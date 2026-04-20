    .data

    # -- Global Scalar Variables --
        .align 2
    result:    .word  0
    res:    .space 8

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
        addi sp, sp, -320
        sd   ra, 312(sp)
        sd   s0, 304(sp)
        addi s0, sp, 320
    # save 'this' pointer
        sd   a0, -24(s0)
        sd a1, -32(s0)
        sw   a2, -40(s0)
        sw   a3, -44(s0)
    # -- initialize local arrays --
    # -- constructor prologue end --

    # name
    # fee
    # days
    # set_field
        ld  t1, -24(s0)
        ld   t2, -32(s0)
        sd   t2, 0(t1)
    # set_field
        ld  t1, -24(s0)
        lw   t2, -40(s0)
        sw   t2, 8(t1)
    # set_field
        ld  t1, -24(s0)
        lw   t2, -44(s0)
        sw   t2, 12(t1)
Lepi_Patient_sii:
    # -- epilogue --
        ld     ra, 312(sp)
        ld     s0, 304(sp)
        addi   sp, sp, 320
        ret
    # -- epilogue end --


total_fee_:
    # -- method prologue -- 
        addi sp, sp, -304
        sd   ra, 296(sp)
        sd   s0, 288(sp)
        addi s0, sp, 304
        sd   a0, -24(s0)
    # -- initialize local arrays --
    # -- method prologue end --

    # get_field
        ld  t1, -24(s0)
        lw  t2, 8(t1)
        mv   t1, t2
    # get_field
        ld  t3, -24(s0)
        lw  t4, 12(t3)
        mv   t3, t4
        mul  t5, t1, t3
        mv   t6, t5
        mv     a0, t6
    # spill all registers
        sw   t1, -32(s0)
        sw   t3, -36(s0)
        sw   t5, -108(s0)
        sw   t6, -40(s0)
        j      Lepi_total_fee_
Lepi_total_fee_:
    # -- epilogue --
        ld     ra, 296(sp)
        ld     s0, 288(sp)
        addi   sp, sp, 304
        ret
    # -- epilogue end --


getName_:
    # -- method prologue -- 
        addi sp, sp, -304
        sd   ra, 296(sp)
        sd   s0, 288(sp)
        addi s0, sp, 304
        sd   a0, -24(s0)
    # -- initialize local arrays --
    # -- method prologue end --

    # get_field
        ld  t1, -24(s0)
        ld  t2, 0(t1)
        mv     a0, t2
    # spill all registers
        j      Lepi_getName_
Lepi_getName_:
    # -- epilogue --
        ld     ra, 296(sp)
        ld     s0, 288(sp)
        addi   sp, sp, 304
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -336
        sd   ra, 328(sp)
        sd   s0, 320(sp)
        addi s0, sp, 336
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
        la     a1, str_0
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
        sw   a0, -140(s0)
        lw   t1, -140(s0)
    # spill all registers
        la   t0, result
        sw   t1, 0(t0)
        la     t0, result
        lw     a1, 0(t0)
        la a0, .fmt_int
        call printf
    # push_ptr: load obj pointer into a0
        la   t0, P
        ld   a0, 0(t0)
    # call method
        addi sp, sp, -16
        sd   a0, 0(sp)
    # spill all registers
        ld   a0, 0(sp)
        addi sp, sp, 16
        call getName_
        sd   a0, -144(s0)
        ld   t1, -144(s0)
    # spill all registers
        la   t0, res
        sd   t1, 0(t0)
        la     t0, res
        ld     a0, 0(t0)
        call puts

    # -- global scope epilogue --
        ld   ra, 328(sp)
        ld   s0, 320(sp)
        addi sp, sp, 336
        ret
