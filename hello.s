.section .data
msg: .asciz "Hello, RISC-V!\n"

.section .text
.globl _start

_start:
    # write syscall
    li a7, 64          # syscall: write
    li a0, 1           # fd = stdout
    la a1, msg         # address of string
    li a2, 15          # length of string
    ecall

    # exit syscall
    li a7, 93          # syscall: exit
    li a0, 0           # exit code
    ecall
