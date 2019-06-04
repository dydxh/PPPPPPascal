	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	cmpl	$1, b(%rip)
	jne	.LBB0_2
	movl	$6, f(%rip)
	xorl	%eax, %eax
	retq
.LBB0_2:
	movl	$9, f(%rip)
	xorl	%eax, %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3
.LCPI1_0:
	.quad	4607182418800017408
.LCPI1_1:
	.quad	4611686018427387904
	.text
	.globl	foo
	.p2align	4, 0x90
	.type	foo,@function
foo:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$3, foo_x(%rip)
	movl	$4, foo_test1(%rip)
	movsd	.LCPI1_0(%rip), %xmm0
	movsd	.LCPI1_1(%rip), %xmm1
	movl	$1, %edi
	movl	$2, %esi
	callq	foo2
	movl	%eax, foo_test1(%rip)
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	foo, .Lfunc_end1-foo
	.cfi_endproc

	.globl	foo2
	.p2align	4, 0x90
	.type	foo2,@function
foo2:
	.cfi_startproc
	movl	%edi, foo2_a(%rip)
	movl	%esi, foo2_b(%rip)
	movsd	%xmm0, foo2_e(%rip)
	movsd	%xmm1, foo2_c(%rip)
	movl	$4, foo_x(%rip)
	movl	$678, foo2_foo2(%rip)
	movl	$678, %eax
	retq
.Lfunc_end2:
	.size	foo2, .Lfunc_end2-foo2
	.cfi_endproc

	.type	b,@object
	.section	.rodata,"a",@progbits
	.globl	b
	.p2align	2
b:
	.long	1
	.size	b, 4

	.type	c,@object
	.globl	c
	.p2align	3
c:
	.quad	4612136378390124954
	.size	c, 8

	.type	f,@object
	.bss
	.globl	f
	.p2align	2
f:
	.long	0
	.size	f, 4

	.type	g,@object
	.globl	g
	.p2align	3
g:
	.quad	0
	.size	g, 8

	.type	h,@object
	.globl	h
h:
	.byte	0
	.size	h, 1

	.type	j,@object
	.globl	j
	.p2align	2
j:
	.long	0
	.size	j, 4

	.type	foo_test,@object
	.section	.rodata,"a",@progbits
	.globl	foo_test
	.p2align	3
foo_test:
	.quad	4608983858650965606
	.size	foo_test, 8

	.type	foo_test1,@object
	.globl	foo_test1
	.p2align	2
foo_test1:
	.long	1
	.size	foo_test1, 4

	.type	foo_x,@object
	.bss
	.globl	foo_x
	.p2align	2
foo_x:
	.long	0
	.size	foo_x, 4

	.type	foo2_a,@object
	.globl	foo2_a
	.p2align	2
foo2_a:
	.long	0
	.size	foo2_a, 4

	.type	foo2_b,@object
	.globl	foo2_b
	.p2align	2
foo2_b:
	.long	0
	.size	foo2_b, 4

	.type	foo2_e,@object
	.globl	foo2_e
	.p2align	3
foo2_e:
	.quad	0
	.size	foo2_e, 8

	.type	foo2_c,@object
	.globl	foo2_c
	.p2align	3
foo2_c:
	.quad	0
	.size	foo2_c, 8

	.type	foo2_foo2,@object
	.globl	foo2_foo2
	.p2align	2
foo2_foo2:
	.long	0
	.size	foo2_foo2, 4


	.section	".note.GNU-stack","",@progbits
