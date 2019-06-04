	.text
	.file	"main"
	.type	a,@object
	.bss
	.globl	a
	.p2align	2
a:
	.long	0
	.size	a, 4

	.type	d,@object
	.globl	d
	.p2align	2
d:
	.long	0
	.size	d, 4

	.type	b,@object
	.globl	b
	.p2align	3
b:
	.quad	0
	.size	b, 8

	.type	c,@object
	.globl	c
	.p2align	3
c:
	.quad	0
	.size	c, 8


	.section	".note.GNU-stack","",@progbits
