	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$1, d(%rip)
	cmpl	$1, a(%rip)
	je	.LBB0_2
	movl	$.L__unnamed_1, %edi
	movl	$str, %esi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_2, %edi
	xorl	%eax, %eax
	callq	printf
.LBB0_2:
	movl	a(%rip), %esi
	movl	$.L__unnamed_3, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_4, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_5, %edi
	movl	$str, %esi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_6, %edi
	xorl	%eax, %eax
	callq	printf
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
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

	.type	str,@object
	.globl	str
str:
	.asciz	"'fuck'"
	.size	str, 7

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

	.type	.L__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%s"
	.size	.L__unnamed_1, 3

	.type	.L__unnamed_2,@object
.L__unnamed_2:
	.asciz	"\n"
	.size	.L__unnamed_2, 2

	.type	.L__unnamed_3,@object
.L__unnamed_3:
	.asciz	"%d"
	.size	.L__unnamed_3, 3

	.type	.L__unnamed_4,@object
.L__unnamed_4:
	.asciz	"\n"
	.size	.L__unnamed_4, 2

	.type	.L__unnamed_5,@object
.L__unnamed_5:
	.asciz	"%s"
	.size	.L__unnamed_5, 3

	.type	.L__unnamed_6,@object
.L__unnamed_6:
	.asciz	"\n"
	.size	.L__unnamed_6, 2


	.section	".note.GNU-stack","",@progbits
