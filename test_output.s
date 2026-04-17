.section .data
a:      .word 100
b:      .word 200
res:    .word 0

str_0:  .asciz "The maximum is: "
newline:.asciz "\n"

.section .bss
buffer: .space 32     # buffer for integer to string

.section .text
.globl main

# -------------------------
# max_ii(a, b)
# returns max in a0
# -------------------------
max_ii:
    addi sp, sp, -16
    sw ra, 12(sp)
    sw s0, 8(sp)
    addi s0, sp, 16

    # a0 = x, a1 = y
    bgt a0, a1, ret_x
    mv a0, a1
    j done

ret_x:
    # a0 already has x
    nop

done:
    lw ra, 12(sp)
    lw s0, 8(sp)
    addi sp, sp, 16
    ret

# -------------------------
# int_to_str
# a0 = integer
# returns:
#   a0 = pointer to string
#   a1 = length
# -------------------------
int_to_str:
    la t0, buffer
    addi t1, t0, 31     # end of buffer
    li t2, 10

    sb zero, 0(t1)      # null terminator
    addi t1, t1, -1

    beqz a0, zero_case

convert_loop:
    rem t3, a0, t2
    addi t3, t3, 48     # digit → ASCII
    sb t3, 0(t1)
    addi t1, t1, -1
    div a0, a0, t2
    bnez a0, convert_loop

    addi t1, t1, 1
    sub a1, t0, t1
    addi a1, a1, 31
    mv a0, t1
    ret

zero_case:
    li t3, '0'
    sb t3, 0(t1)
    mv a0, t1
    li a1, 1
    ret

# -------------------------
# main
# -------------------------
main:
    # load values
    lw a0, a
    lw a1, b

    call max_ii
    mv s1, a0       # save result

    # print "The maximum is: "
    li a7, 64       # write
    li a0, 1        # stdout
    la a1, str_0
    li a2, 17
    ecall

    # convert int → string
    mv a0, s1
    call int_to_str

    # print number
    li a7, 64
    li a0, 1
    mv a2, a1
    mv a1, a0
    ecall

    # print newline
    li a7, 64
    li a0, 1
    la a1, newline
    li a2, 1
    ecall

    # exit
    li a7, 93
    li a0, 0
    ecall
