    .data

    # -- Global Scalar Variables --
        .align 2
    x:    .word  10
        .align 2
    y:    .word  20

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "x"
    str_1:    .asciz "y"
    # -- snapshot variable name strings --
    __snap_str_0:    .asciz "x"
    __snap_str_1:    .asciz "y"

    # -- global objects (pointer slots) --

    # -- I/O format strings --
    .fmt_int:        .asciz  "%d\n"
    .fmt_uint:       .asciz  "%u\n"
    .fmt_float:      .asciz  "%f\n"
    .fmt_str:        .asciz  "%s\n"
    .fmt_char:       .asciz  "%c\n"
    .fmt_scan_int:   .asciz  "%d"
    .fmt_scan_float: .asciz  "%f\n"
    .fmt_scan_str:   .asciz  "%s\n"
    .fmt_int_bare:   .asciz  "%d"

    # -- linefreq report strings --
    .lf_prefix:    .asciz  "Blocks Spanning line numbers "
    .lf_sep:       .asciz  "-"
    .lf_mid:       .asciz  ": Executed "
    .lf_suffix:    .asciz  " times\n"
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


global_body:
    # -- Global body --
        addi sp, sp, -240
        sd   ra, 232(sp)
        sd   s0, 224(sp)
        addi s0, sp, 240
        li   t1, 10
        li   t2, 20
    # snapshot_begin
    # spill all registers
        la   t0, x
        sw   t1, 0(t0)
        la   t0, y
        sw   t2, 0(t0)
        li   a0, 2
        call __snapshot_init
    # spill all registers
        la   a0, x
        la   a1, __snap_str_0
        call __snapshot_register_var
    # spill all registers
        la   a0, y
        la   a1, __snap_str_1
        call __snapshot_register_var
    # snapshot_track
    # spill all registers
        la   a0, x
        la   a1, str_0
        call __snapshot_register_var
    # snapshot_track
    # spill all registers
        la   a0, y
        la   a1, str_1
        call __snapshot_register_var
        la   t0, x
        lw   t1, 0(t0)
        addi t2, t1, 1
        mv   t1, t2
    # spill all registers
        la   t0, x
        sw   t1, 0(t0)
        sw   t2, -96(s0)
        call __snapshot_capture
    # spill all registers
        call __snapshot_capture
        la   t0, y
        lw   t1, 0(t0)
        li   t2, 2
        mul  t3, t1, t2
        mv   t1, t3
    # spill all registers
        la   t0, y
        sw   t1, 0(t0)
        sw   t3, -100(s0)
        call __snapshot_capture
    # spill all registers
        call __snapshot_capture
        la   t0, x
        lw   t1, 0(t0)
        addi t2, t1, 5
        mv   t1, t2
    # spill all registers
        la   t0, x
        sw   t1, 0(t0)
        sw   t2, -104(s0)
        call __snapshot_capture
    # spill all registers
        call __snapshot_capture
    # rewind
    # spill all registers
        li   a0, 2
        call __snapshot_rewind
    # spill all registers
        call __snapshot_capture
    # spill all registers
        la     t0, x
        lw     a1, 0(t0)
        la a0, .fmt_int
        call printf
    # spill all registers
        call __snapshot_capture
    # spill all registers
        la     t0, y
        lw     a1, 0(t0)
        la a0, .fmt_int
        call printf
    # spill all registers
        call __snapshot_capture
    # snapshot_end
    # spill all registers
        call __snapshot_cleanup

    # -- global scope epilogue --
        ld   ra, 232(sp)
        ld   s0, 224(sp)
        addi sp, sp, 240
        ret
