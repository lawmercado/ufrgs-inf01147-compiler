	.globl	x
	.data
	.size	x, 4
x:
	.long	5
	.globl	a
	.data
	.size	a, 4
a:
	.long	4
	.globl	c
	.data
	.size	c, 4
c:
	.long	2
	.globl	xx
	.data
	.size	xx, 12
xx:
	.long	5
	.long	1
	.long	0
__TEmpP0:
	.long	0
__TEmpP1:
	.long	0
.LC0:
	.string	"%d"
	.globl	main
main:
	.cfi_startproc
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$4, %eax
	movl	$3, %ecx
	cltd
	idivl	%ecx
	movl	%eax, __TEmpP1(%rip)
	movl	__TEmpP1(%rip), %eax 
	movl	%eax, x(%rip) 
	movl	x(%rip), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	popq	%rbp
	ret
	.cfi_endproc
