    .data

    # -- Global Scalar Variables --
        .align 2
    x:    .word  0

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "x"
    # -- snapshot variable name strings --
    __snap_str_0:    .asciz "x"

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
        addi sp, sp, -208
        sd   ra, 200(sp)
        sd   s0, 192(sp)
        addi s0, sp, 208
        li   t1, 0
    # snapshot_begin
    # spill all registers
        la   t0, x
        sw   t1, 0(t0)
        li   a0, 1
        call __snapshot_init
    # spill all registers
        la   a0, x
        la   a1, __snap_str_0
        call __snapshot_register_var
    # snapshot_track
    # spill all registers
        la   a0, x
        la   a1, str_0
        call __snapshot_register_var
        li   t1, 1
    # spill all registers
        la   t0, x
        sw   t1, 0(t0)
        call __snapshot_capture
    # spill all registers
        call __snapshot_capture
        li   t1, 2
    # spill all registers
        la   t0, x
        sw   t1, 0(t0)
        call __snapshot_capture
    # spill all registers
        call __snapshot_capture
        li   t1, 3
    # spill all registers
        la   t0, x
        sw   t1, 0(t0)
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
    # snapshot_end
    # spill all registers
        call __snapshot_cleanup

    # -- global scope epilogue --
        ld   ra, 200(sp)
        ld   s0, 192(sp)
        addi sp, sp, 208
        ret
