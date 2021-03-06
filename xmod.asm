	.file	"xmod.c"
	.text
	.section	.rodata
.LC0:
	.string	"path = %s ; mode = %d\n"
.LC1:
	.string	"Error"
.LC2:
	.string	"Invalid permission - %c\n"
.LC3:
	.string	"Invalid type of user - %c\n"
.LC4:
	.string	"Invalid operator - %c\n"
.LC5:
	.string	"Error chmod "
.LC6:
	.string	"Invalid input"
	.text
	.globl	xmod
	.type	xmod, @function
xmod:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -30(%rbp)
	cmpb	$48, -30(%rbp)
	jne	.L2
	movq	-48(%rbp), %rax
	movl	$8, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	strtol@PLT
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %edx
	movq	-40(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-12(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	chmod@PLT
	testl	%eax, %eax
	je	.L3
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
	movl	$1, %eax
	jmp	.L4
.L2:
	cmpb	$117, -30(%rbp)
	je	.L5
	cmpb	$103, -30(%rbp)
	je	.L5
	cmpb	$111, -30(%rbp)
	je	.L5
	cmpb	$97, -30(%rbp)
	jne	.L6
.L5:
	movq	-48(%rbp), %rax
	movzbl	1(%rax), %eax
	movb	%al, -29(%rbp)
	movl	$0, -28(%rbp)
	movl	$1, -24(%rbp)
	jmp	.L7
.L12:
	movl	-24(%rbp), %eax
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	cmpl	$120, %eax
	je	.L8
	cmpl	$120, %eax
	jg	.L9
	cmpl	$114, %eax
	je	.L10
	cmpl	$119, %eax
	je	.L11
	jmp	.L9
.L10:
	addl	$4, -28(%rbp)
	jmp	.L7
.L11:
	addl	$2, -28(%rbp)
	jmp	.L7
.L8:
	addl	$1, -28(%rbp)
	jmp	.L7
.L9:
	movl	-24(%rbp), %eax
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L4
.L7:
	addl	$1, -24(%rbp)
	movl	-24(%rbp), %eax
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L12
	movsbl	-30(%rbp), %eax
	cmpl	$117, %eax
	je	.L13
	cmpl	$117, %eax
	jg	.L14
	cmpl	$111, %eax
	je	.L15
	cmpl	$111, %eax
	jg	.L14
	cmpl	$97, %eax
	je	.L16
	cmpl	$103, %eax
	je	.L17
	jmp	.L14
.L13:
	sall	$6, -28(%rbp)
	movl	$63, -20(%rbp)
	jmp	.L18
.L17:
	sall	$3, -28(%rbp)
	movl	$455, -20(%rbp)
	jmp	.L18
.L15:
	movl	$504, -20(%rbp)
	jmp	.L18
.L16:
	movl	$0, -20(%rbp)
	movl	-28(%rbp), %edx
	movl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	movl	%eax, -28(%rbp)
	jmp	.L18
.L14:
	movsbl	-30(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L4
.L18:
	movq	-8(%rbp), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	stat@PLT
	movq	-8(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, -16(%rbp)
	movsbl	-29(%rbp), %eax
	cmpl	$61, %eax
	je	.L19
	cmpl	$61, %eax
	jg	.L20
	cmpl	$43, %eax
	je	.L21
	cmpl	$45, %eax
	jne	.L20
	movl	-28(%rbp), %eax
	notl	%eax
	andl	-16(%rbp), %eax
	movl	%eax, -28(%rbp)
	jmp	.L22
.L21:
	movl	-16(%rbp), %eax
	orl	%eax, -28(%rbp)
	jmp	.L22
.L19:
	movl	-16(%rbp), %eax
	andl	-20(%rbp), %eax
	orl	%eax, -28(%rbp)
	jmp	.L22
.L20:
	movsbl	-29(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L4
.L22:
	movl	-28(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	chmod@PLT
	testl	%eax, %eax
	je	.L24
	leaq	.LC5(%rip), %rdi
	call	puts@PLT
	movl	$-1, %eax
	jmp	.L4
.L6:
	leaq	.LC6(%rip), %rdi
	call	puts@PLT
	movl	$-1, %eax
	jmp	.L4
.L24:
	nop
.L3:
	movl	$0, %eax
.L4:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	xmod, .-xmod
	.globl	symbolicChmod
	.type	symbolicChmod, @function
symbolicChmod:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	symbolicChmod, .-symbolicChmod
	.section	.rodata
.LC7:
	.string	"Teste"
.LC8:
	.string	"nargs > 2"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	leaq	.LC7(%rip), %rdi
	call	puts@PLT
	cmpl	$2, -36(%rbp)
	jle	.L27
	leaq	.LC8(%rip), %rdi
	call	puts@PLT
	movl	$0, -20(%rbp)
	jmp	.L28
.L29:
	addl	$1, -20(%rbp)
.L28:
	movl	-36(%rbp), %eax
	subl	$2, %eax
	cmpl	%eax, -20(%rbp)
	jl	.L29
	movl	-36(%rbp), %eax
	cltq
	salq	$3, %rax
	leaq	-16(%rax), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	movl	-36(%rbp), %eax
	cltq
	salq	$3, %rax
	leaq	-8(%rax), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	xmod
.L27:
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
