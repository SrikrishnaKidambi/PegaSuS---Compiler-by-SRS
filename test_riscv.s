	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.file	"test.cpp"
	.p2align	1
	.type	__cxx_global_var_init,@function
__cxx_global_var_init:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)
	sd	s0, 0(sp)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	addi	s0, sp, 16
	.cfi_def_cfa s0, 0
	lui	a0, %hi(a)
	addi	a0, a0, %lo(a)
	call	_ZN6AnimalC2Ev
	ld	ra, 8(sp)
	ld	s0, 0(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end0:
	.size	__cxx_global_var_init, .Lfunc_end0-__cxx_global_var_init
	.cfi_endproc

	.section	.text._ZN6AnimalC2Ev,"axG",@progbits,_ZN6AnimalC2Ev,comdat
	.weak	_ZN6AnimalC2Ev
	.p2align	1
	.type	_ZN6AnimalC2Ev,@function
_ZN6AnimalC2Ev:
	addi	sp, sp, -32
	sd	ra, 24(sp)
	sd	s0, 16(sp)
	addi	s0, sp, 32
	sd	a0, -24(s0)
	ld	a1, -24(s0)
	li	a0, 0
	sw	a0, 0(a1)
	ld	ra, 24(sp)
	ld	s0, 16(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end1:
	.size	_ZN6AnimalC2Ev, .Lfunc_end1-_ZN6AnimalC2Ev

	.text
	.p2align	1
	.type	_GLOBAL__sub_I_test.cpp,@function
_GLOBAL__sub_I_test.cpp:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)
	sd	s0, 0(sp)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	addi	s0, sp, 16
	.cfi_def_cfa s0, 0
	call	__cxx_global_var_init
	ld	ra, 8(sp)
	ld	s0, 0(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end2:
	.size	_GLOBAL__sub_I_test.cpp, .Lfunc_end2-_GLOBAL__sub_I_test.cpp
	.cfi_endproc

	.type	a,@object
	.section	.sbss,"aw",@nobits
	.globl	a
	.p2align	2
a:
	.zero	8
	.size	a, 8

	.section	.init_array,"aw",@init_array
	.p2align	3
	.quad	_GLOBAL__sub_I_test.cpp
	.ident	"Ubuntu clang version 14.0.0-1ubuntu1.1"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __cxx_global_var_init
	.addrsig_sym _GLOBAL__sub_I_test.cpp
	.addrsig_sym a
