	.globl	x
	.data
	.size	x, 4
x:
	.globl	xx
	.data
	.size	xx, 8
xx:
	.long	5
	.long	1
__TEmpP7:
	.long	0
__TEmpP0:
	.long	0
__TEmpP1:
	.long	0
__TEmpP2:
	.long	0
__TEmpP3:
	.long	0
__TEmpP4:
	.long	0
__TEmpP5:
	.long	0
__TEmpP6:
	.long	0
l:
	.long	0
a:
	.long	0
.LC0:
	.string	"%d"
	.globl	ff
ff:
	.cfi_startproc
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, %a(%rip)
	movl	%esi, %l(%rip)
	movl	a(%rip), %edx
	movl	l(%rip), %eax
	addl	%edx, %eax
	movl	%eax, __TEmpP4(%rip)
	movl	__TEmpP4(%rip), %eax 
	movl	%eax, x(%rip) 
	movl	x(%rip), %eax 

	popq	%rbp
	ret
	.cfi_endproc

	.globl	main
main:
	.cfi_startproc
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$1, %esi
	movl	$55, %edi
	call	ff
	movl	%eax, __TEmpP6(%rip)
	movl	__TEmpP6(%rip), %eax 
	movl	%eax, x(%rip) 
	movl	x(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax

	popq	%rbp
	ret
	.cfi_endproc
