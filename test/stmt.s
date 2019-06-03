	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	movl	$1, d(%rip)
	cmpl	$1, a(%rip)
	xorl	%eax, %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	a,@object
	.section	.rodata,"a",@progbits
	.globl	a
	.p2align	2
a:
	.long	1
	.size	a, 4

	.type	b,@object
	.bss
	.globl	b
	.p2align	2
b:
	.long	0
	.size	b, 4

	.type	c,@object
	.globl	c
	.p2align	2
c:
	.long	0
	.size	c, 4

	.type	d,@object
	.globl	d
	.p2align	2
d:
	.long	0
	.size	d, 4


	.section	".note.GNU-stack","",@progbits
