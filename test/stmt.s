	.text
	.file	"main"
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$1, d(%rip)
	cmpl	$1, a(%rip)
	je	.LBB0_2
# %bb.1:                                # %else
	movl	$.L__unnamed_1, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_2, %edi
	xorl	%eax, %eax
	callq	printf
.LBB0_2:                                # %end
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	a,@object               # @a
	.section	.rodata,"a",@progbits
	.globl	a
	.p2align	2
a:
	.long	1                       # 0x1
	.size	a, 4

	.type	str,@object             # @str
	.globl	str
str:
	.asciz	"'fuck'"
	.size	str, 7

	.type	b,@object               # @b
	.bss
	.globl	b
	.p2align	2
b:
	.long	0                       # 0x0
	.size	b, 4

	.type	c,@object               # @c
	.globl	c
	.p2align	2
c:
	.long	0                       # 0x0
	.size	c, 4

	.type	d,@object               # @d
	.globl	d
	.p2align	2
d:
	.long	0                       # 0x0
	.size	d, 4

	.type	.L__unnamed_1,@object   # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"str"
	.size	.L__unnamed_1, 4

	.type	.L__unnamed_2,@object   # @1
.L__unnamed_2:
	.asciz	"\n"
	.size	.L__unnamed_2, 2


	.section	".note.GNU-stack","",@progbits
