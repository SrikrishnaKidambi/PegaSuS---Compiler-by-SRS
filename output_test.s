.data

    # -- Global Scalar Variables --
        .align 2
    a:    .word  100
        .align 2
    b:    .word  200
        .align 2
    res:    .word  0

    # -- global arrays --

    # -- string literals --
    str_0:    .asciz "The maximum is:"

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
        sd   ra, 8(sp)          # Save 64-bit return address
        call global_body
        ld   ra, 8(sp)          # Restore 64-bit return address
        addi sp, sp, 16
        li a0, 0
        ret


max_ii:
    # -- prologue --
        addi   sp, sp, -224
        sd     ra, 216(sp)      # Save 64-bit return address (offset adjusted)
        sd     s0, 208(sp)      # Save 64-bit frame pointer (offset adjusted)
        addi   s0, sp, 224
        sw     a0, -8(s0)       # 32-bit integer parameter
        sw     a1, -12(s0)      # 32-bit integer parameter
    # --initialize local arrays --
    # -- prologue end --

    # x
    # y
    # >
        lw   t1, -8(s0)         # 32-bit integer load
        lw   t2, -12(s0)        # 32-bit integer load
        slt  t3, t2, t1
        beqz   t3, L0
        sw   t3, -80(s0)        # 32-bit integer store
        mv     a0, t1
        j      Lepi_max_ii
    # spill all registers
        j      L1
    # spill all registers

L0:
    # spill all registers

L1:
        lw     a0, -12(s0)      # 32-bit integer load
        j      Lepi_max_ii
        
Lepi_max_ii:
    # -- epilogue --
        ld     ra, 216(sp)      # Restore 64-bit return address
        ld     s0, 208(sp)      # Restore 64-bit frame pointer
        addi   sp, sp, 224
        ret
    # -- epilogue end --


global_body:
    # -- Global body --
        addi sp, sp, -224
        sd   ra, 216(sp)        # Save 64-bit return address
        sd   s0, 208(sp)        # Save 64-bit frame pointer
        addi s0, sp, 224
        li   t1, 100
        li   t2, 200
    # spill all registers
        la   t0, a
        sw   t1, 0(t0)          # 32-bit integer store
        la   t0, b
        sw   t2, 0(t0)          # 32-bit integer store
        la   t0, a
        lw   a0, 0(t0)          # 32-bit integer load
        la   t0, b
        lw   a1, 0(t0)          # 32-bit integer load
        call max_ii
        sw   a0, -84(s0)        # 32-bit integer store
        lw   t1, -84(s0)        # 32-bit integer load
        mv   t2, t1
    # spill all registers
        la   t0, res
        sw   t2, 0(t0)          # 32-bit integer store
        addi sp, sp, -16
        sd   ra, 8(sp)          # Save 64-bit return address
        la a0, str_0
        call puts
        ld   ra, 8(sp)          # Restore 64-bit return address
        addi sp, sp, 16
    # spill all registers
        addi sp, sp, -16
        sd   ra, 8(sp)          # Save 64-bit return address
        la t0, res
        lw a1, 0(t0)            # 32-bit integer load
        la a0, .fmt_int
        call printf
        ld   ra, 8(sp)          # Restore 64-bit return address
        addi sp, sp, 16

    # -- global scope epilogue --
        ld   ra, 216(sp)        # Restore 64-bit return address
        ld   s0, 208(sp)        # Restore 64-bit frame pointer
        addi sp, sp, 224
        ret

#--- Register Allocation Statistics -----
# Strategy: BASIC (first dirty VAR)
# Loads (lw/li): 13
# Stores (sw) : 9
# Total : 22
# --------------------------------------
