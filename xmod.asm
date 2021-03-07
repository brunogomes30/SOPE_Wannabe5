	.file	"xmod.c"
	.text
	.local	nModif
	.comm	nModif,8,8
	.local	nTotal
	.comm	nTotal,8,8
	.comm	stop,8,8
	.section	.rodata
.LC0:
	.string	"rwx"
	.text
	.globl	getSymbolic
	.type	getSymbolic, @function
getSymbolic:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -8(%rbp)
	movl	$8, -16(%rbp)
	movl	$2, -12(%rbp)
	jmp	.L2
.L5:
	movl	-20(%rbp), %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L3
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movl	-16(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -16(%rbp)
	movslq	%eax, %rdx
	movq	-32(%rbp), %rax
	addq	%rax, %rdx
	movzbl	(%rcx), %eax
	movb	%al, (%rdx)
	jmp	.L4
.L3:
	movl	-16(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -16(%rbp)
	movslq	%eax, %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movb	$45, (%rax)
.L4:
	shrl	-20(%rbp)
	subl	$1, -12(%rbp)
	cmpl	$0, -12(%rbp)
	jns	.L2
	movl	$2, -12(%rbp)
.L2:
	cmpl	$0, -20(%rbp)
	jne	.L5
	movq	-32(%rbp), %rax
	addq	$9, %rax
	movb	$0, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	getSymbolic, .-getSymbolic
	.section	.rodata
.LC1:
	.string	"%d ; ; %d ; %d\n"
	.text
	.globl	sigintHandler
	.type	sigintHandler, @function
sigintHandler:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movl	%edi, -20(%rbp)
	movq	nModif(%rip), %rax
	movl	(%rax), %r12d
	movq	nTotal(%rip), %rax
	movl	(%rax), %ebx
	call	getpid@PLT
	movl	%r12d, %ecx
	movl	%ebx, %edx
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	stop(%rip), %rax
	movl	$1, (%rax)
	nop
	addq	$16, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	sigintHandler, .-sigintHandler
	.section	.rodata
.LC2:
	.string	"Queres continuar? (y or n)"
.LC3:
	.string	"%c"
.LC4:
	.string	"Path = %s\n"
.LC5:
	.string	"Error stat() %s\n"
.LC6:
	.string	"Invalid type of user - %c\n"
.LC7:
	.string	"Error\n"
	.align 8
.LC8:
	.string	"modo de '%s' alterado de %04o (%s) para %04o (%s)\n"
	.align 8
.LC9:
	.string	"modo de '%s' mantido como %04o (%s)\n"
.LC10:
	.string	"."
.LC11:
	.string	".."
	.text
	.globl	xmod
	.type	xmod, @function
xmod:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$368, %rsp
	movq	%rdi, -344(%rbp)
	movq	%rsi, -352(%rbp)
	movl	%edx, %eax
	movw	%ax, -356(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	stop(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	je	.L8
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	leaq	-332(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movzbl	-332(%rbp), %eax
	cmpb	$121, %al
	setne	%dl
	movq	stop(%rip), %rax
	movzbl	%dl, %edx
	movl	%edx, (%rax)
	movzbl	-332(%rbp), %eax
	cmpb	$110, %al
	jne	.L8
	movl	$9, %esi
	movl	$0, %edi
	call	kill@PLT
.L8:
	movq	-344(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	nTotal(%rip), %rax
	movl	(%rax), %edx
	movq	nTotal(%rip), %rax
	addl	$1, %edx
	movl	%edx, (%rax)
	movq	-352(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -333(%rbp)
	leaq	sigintHandler(%rip), %rsi
	movl	$2, %edi
	call	signal@PLT
	movl	$1, %edi
	call	sleep@PLT
	movl	$144, %edi
	call	malloc@PLT
	movq	%rax, -320(%rbp)
	movq	-320(%rbp), %rdx
	movq	-344(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	stat@PLT
	testl	%eax, %eax
	je	.L10
	movq	stderr(%rip), %rax
	movq	-344(%rbp), %rdx
	leaq	.LC5(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L28
.L10:
	movq	-320(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, -328(%rbp)
	andl	$511, -328(%rbp)
	cmpb	$48, -333(%rbp)
	jne	.L12
	movq	-352(%rbp), %rax
	movl	$8, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	strtol@PLT
	movl	%eax, -332(%rbp)
	jmp	.L13
.L12:
	cmpb	$117, -333(%rbp)
	je	.L14
	cmpb	$103, -333(%rbp)
	je	.L14
	cmpb	$111, -333(%rbp)
	je	.L14
	cmpb	$97, -333(%rbp)
	jne	.L15
.L14:
	movl	-328(%rbp), %eax
	movl	%eax, -332(%rbp)
	leaq	-332(%rbp), %rdx
	movq	-352(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	symbolicChmod
	jmp	.L13
.L15:
	movsbl	-333(%rbp), %edx
	movq	stderr(%rip), %rax
	leaq	.LC6(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L28
.L13:
	movl	-332(%rbp), %edx
	movq	-344(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	chmod@PLT
	testl	%eax, %eax
	je	.L16
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$6, %edx
	movl	$1, %esi
	leaq	.LC7(%rip), %rdi
	call	fwrite@PLT
	movl	$-1, %eax
	jmp	.L28
.L16:
	movl	-332(%rbp), %eax
	cmpl	%eax, -328(%rbp)
	je	.L17
	movq	nModif(%rip), %rax
	movl	(%rax), %edx
	movq	nModif(%rip), %rax
	addl	$1, %edx
	movl	%edx, (%rax)
.L17:
	movswl	-356(%rbp), %eax
	andl	$272, %eax
	testl	%eax, %eax
	je	.L18
	leaq	-282(%rbp), %rdx
	movl	-328(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	getSymbolic
	movl	-332(%rbp), %eax
	leaq	-272(%rbp), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	getSymbolic
	andl	$511, -328(%rbp)
	movl	-332(%rbp), %eax
	cmpl	%eax, -328(%rbp)
	je	.L19
	movl	-332(%rbp), %esi
	leaq	-272(%rbp), %rdi
	leaq	-282(%rbp), %rcx
	movl	-328(%rbp), %edx
	movq	-344(%rbp), %rax
	movq	%rdi, %r9
	movl	%esi, %r8d
	movq	%rax, %rsi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L18
.L19:
	movswl	-356(%rbp), %eax
	andl	$256, %eax
	testl	%eax, %eax
	je	.L18
	movl	-332(%rbp), %edx
	leaq	-272(%rbp), %rcx
	movq	-344(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC9(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L18:
	movswl	-356(%rbp), %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L21
	movq	-320(%rbp), %rax
	movl	24(%rax), %eax
	andl	$61440, %eax
	cmpl	$16384, %eax
	jne	.L21
	movq	-344(%rbp), %rax
	movq	%rax, %rdi
	call	opendir@PLT
	movq	%rax, -312(%rbp)
	cmpq	$0, -312(%rbp)
	je	.L21
	jmp	.L22
.L27:
	movq	-304(%rbp), %rax
	addq	$19, %rax
	leaq	.LC10(%rip), %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	je	.L22
	movq	-304(%rbp), %rax
	addq	$19, %rax
	leaq	.LC11(%rip), %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L24
	jmp	.L22
.L24:
	call	fork@PLT
	movl	%eax, -324(%rbp)
	cmpl	$-1, -324(%rbp)
	je	.L25
	cmpl	$0, -324(%rbp)
	jne	.L26
	movq	-344(%rbp), %rdx
	leaq	-272(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	leaq	-272(%rbp), %rax
	movq	$-1, %rcx
	movq	%rax, %rdx
	movl	$0, %eax
	movq	%rdx, %rdi
	repnz scasb
	movq	%rcx, %rax
	notq	%rax
	leaq	-1(%rax), %rdx
	leaq	-272(%rbp), %rax
	addq	%rdx, %rax
	movw	$47, (%rax)
	movq	-304(%rbp), %rax
	leaq	19(%rax), %rdx
	leaq	-272(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcat@PLT
	movswl	-356(%rbp), %edx
	movq	-352(%rbp), %rcx
	leaq	-272(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	xmod
	movl	$0, %edi
	call	exit@PLT
.L25:
	movl	$-1, %eax
	jmp	.L28
.L26:
	movq	-296(%rbp), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	wait@PLT
	nop
.L22:
	movq	-312(%rbp), %rax
	movq	%rax, %rdi
	call	readdir@PLT
	movq	%rax, -304(%rbp)
	cmpq	$0, -304(%rbp)
	jne	.L27
	movq	-312(%rbp), %rax
	movq	%rax, %rdi
	call	closedir@PLT
.L21:
	movl	$0, %eax
.L28:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L29
	call	__stack_chk_fail@PLT
.L29:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	xmod, .-xmod
	.section	.rodata
	.align 8
.LC12:
	.string	"Invalid permission - %c, fullString = %s\n"
.LC13:
	.string	"Invalid operator - %c\n"
	.text
	.globl	symbolicChmod
	.type	symbolicChmod, @function
symbolicChmod:
.LFB9:
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
	movq	-40(%rbp), %rax
	movzbl	1(%rax), %eax
	movb	%al, -17(%rbp)
	movl	$0, -16(%rbp)
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movl	$2, -12(%rbp)
	jmp	.L31
.L37:
	movl	-12(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -12(%rbp)
	movslq	%eax, %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	cmpl	$120, %eax
	je	.L32
	cmpl	$120, %eax
	jg	.L33
	cmpl	$114, %eax
	je	.L34
	cmpl	$119, %eax
	je	.L35
	jmp	.L33
.L34:
	addl	$4, -16(%rbp)
	jmp	.L31
.L35:
	addl	$2, -16(%rbp)
	jmp	.L31
.L32:
	addl	$1, -16(%rbp)
	jmp	.L31
.L33:
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movq	-40(%rbp), %rdx
	movl	%eax, %esi
	leaq	.LC12(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L30
.L31:
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L37
	movq	-40(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	cmpl	$117, %eax
	je	.L38
	cmpl	$117, %eax
	jg	.L39
	cmpl	$111, %eax
	je	.L40
	cmpl	$111, %eax
	jg	.L39
	cmpl	$97, %eax
	je	.L41
	cmpl	$103, %eax
	je	.L42
	jmp	.L39
.L38:
	sall	$6, -16(%rbp)
	movl	$63, -8(%rbp)
	jmp	.L43
.L42:
	sall	$3, -16(%rbp)
	movl	$455, -8(%rbp)
	jmp	.L43
.L40:
	movl	$504, -8(%rbp)
	jmp	.L43
.L41:
	movl	$0, -8(%rbp)
	movl	-16(%rbp), %edx
	movl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	movl	%eax, -16(%rbp)
	jmp	.L43
.L39:
	movq	-40(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %edx
	movq	stderr(%rip), %rax
	leaq	.LC6(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L30
.L43:
	addl	$32768, -8(%rbp)
	movsbl	-17(%rbp), %eax
	cmpl	$61, %eax
	je	.L44
	cmpl	$61, %eax
	jg	.L45
	cmpl	$43, %eax
	je	.L46
	cmpl	$45, %eax
	jne	.L45
	movl	-16(%rbp), %eax
	notl	%eax
	andl	-4(%rbp), %eax
	movl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L47
.L46:
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	orl	-16(%rbp), %eax
	movl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L47
.L44:
	movl	-4(%rbp), %eax
	andl	-8(%rbp), %eax
	orl	-16(%rbp), %eax
	movl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L47
.L45:
	movsbl	-17(%rbp), %edx
	movq	stderr(%rip), %rax
	leaq	.LC13(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L30
.L47:
.L30:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	symbolicChmod, .-symbolicChmod
	.section	.rodata
.LC14:
	.string	"Invalid flag %s"
	.align 8
.LC15:
	.string	"\n\t\tFINAL nModif = %d, nTotal = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%rdx, -56(%rbp)
	movl	$0, %r9d
	movl	$-1, %r8d
	movl	$33, %ecx
	movl	$3, %edx
	movl	$4, %esi
	movl	$0, %edi
	call	mmap@PLT
	movq	%rax, nTotal(%rip)
	movl	$0, %r9d
	movl	$-1, %r8d
	movl	$33, %ecx
	movl	$3, %edx
	movl	$4, %esi
	movl	$0, %edi
	call	mmap@PLT
	movq	%rax, nModif(%rip)
	movl	$0, %r9d
	movl	$-1, %r8d
	movl	$33, %ecx
	movl	$3, %edx
	movl	$4, %esi
	movl	$0, %edi
	call	mmap@PLT
	movq	%rax, stop(%rip)
	movw	$0, -30(%rbp)
	cmpl	$2, -36(%rbp)
	jle	.L49
	movl	$1, -28(%rbp)
	jmp	.L50
.L59:
	movl	-28(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$45, %al
	jne	.L51
	movq	-8(%rbp), %rax
	addq	$2, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L52
.L51:
	movq	stderr(%rip), %rax
	movq	-8(%rbp), %rdx
	leaq	.LC14(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L53
.L52:
	movq	-8(%rbp), %rax
	addq	$1, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	cmpl	$118, %eax
	je	.L54
	cmpl	$118, %eax
	jg	.L55
	cmpl	$82, %eax
	je	.L56
	cmpl	$99, %eax
	je	.L57
	jmp	.L55
.L54:
	orw	$256, -30(%rbp)
	jmp	.L58
.L57:
	orw	$16, -30(%rbp)
	jmp	.L58
.L56:
	orw	$1, -30(%rbp)
	jmp	.L58
.L55:
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC14(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L53
.L58:
	addl	$1, -28(%rbp)
.L50:
	movl	-36(%rbp), %eax
	subl	$2, %eax
	cmpl	%eax, -28(%rbp)
	jl	.L59
	movl	-36(%rbp), %eax
	cltq
	salq	$3, %rax
	leaq	-16(%rax), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -24(%rbp)
	movl	-36(%rbp), %eax
	cltq
	salq	$3, %rax
	leaq	-8(%rax), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	movswl	-30(%rbp), %edx
	movq	-24(%rbp), %rcx
	movq	-16(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	xmod
.L49:
	movq	nTotal(%rip), %rax
	movl	(%rax), %edx
	movq	nModif(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC15(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L53:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
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
