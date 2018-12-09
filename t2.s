# TAC_BEGIN_FUNCTION
    .globl	main
main:
    .cfi_startproc
    pushq	%rbp
    movq	%rsp, %rbp


#TAC_END_FUNCTION
    popq	%rbp
    ret
    .cfi_endproc

#TAC_ATTRIB
    movl	c(%rip), %eax
	movl	%eax, a(%rip)

#TAC_PRINT (NUMERO)
    movl	$.LC0, %edi
	movl	$0, %eax
	call	printf

#TAC_PRINT (STRING)
