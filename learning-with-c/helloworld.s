	.file	"helloworld.c"
	.text
	.globl	addNumbers
	.def	addNumbers;	.scl	2;	.type	32;	.endef
addNumbers:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp

	; move 5 to register rbp ;
	movl	$5, -4(%rbp)

	; move 10 to register ;
	movl	$10, -8(%rbp)

	; moving contents of 5 and 10 to edx and eax, respectively ;
	movl	-4(%rbp), %edx
	movl	-8(%rbp), %eax

	; add the values within the two registers ;
	addl	%edx, %eax
	movl	%eax, -12(%rbp)
	nop
	leave
	ret
	.ident	"GCC: (Rev5, Built by MSYS2 project) 16.1.0"
