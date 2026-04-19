    .data

    # -- Global Scalar Variables --

    # -- global arrays --

    # -- string literals --

    # -- global objects (pointer slots) --
        .align 3
    p1:    .dword 0
        .align 3
    p2:    .dword 0
        .align 3
    p3:    .dword 0
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


Point_:
    # -- constructor prologue --
        addi sp, sp, -320
        sd   ra, 312(sp)
        sd   s0, 304(sp)
        addi s0, sp, 320
    # save 'this' pointer
        sd   a0, -24(s0)
    # -- initialize local arrays --
    # -- constructor prologue end --

    # set_field
        ld  t1, -24(s0)
        li   t2, 0
        sw   t2, 0(t1)
    # set_field
        ld  t1, -24(s0)
        li   t2, 0
        sw   t2, 4(t1)
Lepi_Point_:
    # -- epilogue --
        ld     ra, 312(sp)
        ld     s0, 304(sp)
        addi   sp, sp, 320
        ret
    # -- epilogue end --


Point_i:
    # -- constructor prologue --
        addi sp, sp, -320
        sd   ra, 312(sp)
        sd   s0, 304(sp)
        addi s0, sp, 320
    # save 'this' pointer
        sd   a0, -24(s0)
        sw   a1, -32(s0)
    # -- initialize local arrays --
    # -- constructor prologue end --

    # a
    # set_field
        ld  t1, -24(s0)
        lw   t2, -32(s0)
        sw   t2, 0(t1)
    # set_field
        ld  t1, -24(s0)
        li   t2, 0
        sw   t2, 4(t1)
Lepi_Point_i:
    # -- epilogue --
        ld     ra, 312(sp)
        ld     s0, 304(sp)
        addi   sp, sp, 320
        ret
    # -- epilogue end --


Point_ii:
    # -- constructor prologue --
        addi sp, sp, -320
        sd   ra, 312(sp)
        sd   s0, 304(sp)
        addi s0, sp, 320
    # save 'this' pointer
        sd   a0, -24(s0)
        sw   a1, -32(s0)
        sw   a2, -36(s0)
    # -- initialize local arrays --
    # -- constructor prologue end --

    # a
    # b
    # set_field
        ld  t1, -24(s0)
        lw   t2, -32(s0)
        sw   t2, 0(t1)
    # set_field
        ld  t1, -24(s0)
        lw   t2, -36(s0)
        sw   t2, 4(t1)
Lepi_Point_ii:
    # -- epilogue --
        ld     ra, 312(sp)
        ld     s0, 304(sp)
        addi   sp, sp, 320
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -368
        sd   ra, 360(sp)
        sd   s0, 352(sp)
        addi s0, sp, 368
    # new object
    # spill all registers
        li   a0, 8
        call malloc
        la   t0, p1
        sd   a0, 0(t0)
    # push_ptr: load obj pointer into a0
        la   t0, p1
        ld   a0, 0(t0)
    # call_constr
        addi sp, sp, -16
        sd   a0, 0(sp)
    # spill all registers
        ld   a0, 0(sp)
        addi sp, sp, 16
        call Point_
    # new object
    # spill all registers
        li   a0, 8
        call malloc
        la   t0, p2
        sd   a0, 0(t0)
    # push_ptr: load obj pointer into a0
        la   t0, p2
        ld   a0, 0(t0)
    # call_constr
        addi sp, sp, -16
        sd   a0, 0(sp)
    # spill all registers
        li   a1, 5
        ld   a0, 0(sp)
        addi sp, sp, 16
        call Point_i
    # new object
    # spill all registers
        li   a0, 8
        call malloc
        la   t0, p3
        sd   a0, 0(t0)
    # push_ptr: load obj pointer into a0
        la   t0, p3
        ld   a0, 0(t0)
    # call_constr
        addi sp, sp, -16
        sd   a0, 0(sp)
    # spill all registers
        li   a1, 3
        li   a2, 4
        ld   a0, 0(sp)
        addi sp, sp, 16
        call Point_ii
    # get_field
        ld  t1, 0(s0)
        lw  t2, 0(t1)
    # ISSUE: symbol not found for address lookup
        sw   t2, 0(s0) # unknown: t0
        mv     a1, t2
    # spill all registers
        la a0, .fmt_int
        call printf
    # get_field
        ld  t1, 0(s0)
        lw  t2, 4(t1)
    # ISSUE: symbol not found for address lookup
        sw   t2, 0(s0) # unknown: t1
        mv     a1, t2
    # spill all registers
        la a0, .fmt_int
        call printf
    # get_field
        ld  t1, 0(s0)
        lw  t2, 0(t1)
    # ISSUE: symbol not found for address lookup
        sw   t2, 0(s0) # unknown: t2
        mv     a1, t2
    # spill all registers
        la a0, .fmt_int
        call printf
    # get_field
        ld  t1, 0(s0)
        lw  t2, 4(t1)
    # ISSUE: symbol not found for address lookup
        sw   t2, 0(s0) # unknown: t3
        mv     a1, t2
    # spill all registers
        la a0, .fmt_int
        call printf
    # get_field
        ld  t1, 0(s0)
        lw  t2, 0(t1)
    # ISSUE: symbol not found for address lookup
        sw   t2, 0(s0) # unknown: t4
        mv     a1, t2
    # spill all registers
        la a0, .fmt_int
        call printf
    # get_field
        ld  t1, 0(s0)
        lw  t2, 4(t1)
    # ISSUE: symbol not found for address lookup
        sw   t2, 0(s0) # unknown: t5
        mv     a1, t2
    # spill all registers
        la a0, .fmt_int
        call printf

    # -- global scope epilogue --
        ld   ra, 360(sp)
        ld   s0, 352(sp)
        addi sp, sp, 368
        ret
