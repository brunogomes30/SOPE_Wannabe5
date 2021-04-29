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
	.string	"Invalid type of user - %c\n"
.LC6:
	.string	"Error\n"
	.align 8
.LC7:
	.string	"modo de '%s' alterado de %04o (%s) para %04o (%s)\n"
	.align 8
.LC8:
	.string	"modo de '%s' mantido como %04o (%s)\n"
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
	subq	$80, %rsp
	movq	%rdi, -56(%rbp)
	movq	%rsi, -64(%rbp)
	movl	%edx, %eax
	movl	%ecx, -72(%rbp)
	movw	%ax, -68(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	stop(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	je	.L8
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movzbl	-32(%rbp), %eax
	cmpb	$121, %al
	setne	%dl
	movq	stop(%rip), %rax
	movzbl	%dl, %edx
	movl	%edx, (%rax)
	movzbl	-32(%rbp), %eax
	cmpb	$110, %al
	jne	.L8
	movl	$9, %esi
	movl	$0, %edi
	call	kill@PLT
.L8:
	movq	-56(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	nTotal(%rip), %rax
	movl	(%rax), %edx
	movq	nTotal(%rip), %rax
	addl	$1, %edx
	movl	%edx, (%rax)
	movq	-64(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -33(%rbp)
	leaq	sigintHandler(%rip), %rsi
	movl	$2, %edi
	call	signal@PLT
	movl	$1, %edi
	call	sleep@PLT
	andl	$511, -72(%rbp)
	cmpb	$48, -33(%rbp)
	jne	.L10
	movq	-64(%rbp), %rax
	movl	$8, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	strtol@PLT
	movl	%eax, -32(%rbp)
	jmp	.L11
.L10:
	cmpb	$117, -33(%rbp)
	je	.L12
	cmpb	$103, -33(%rbp)
	je	.L12
	cmpb	$111, -33(%rbp)
	je	.L12
	cmpb	$97, -33(%rbp)
	jne	.L13
.L12:
	movl	-72(%rbp), %eax
	movl	%eax, -32(%rbp)
	leaq	-32(%rbp), %rdx
	movq	-64(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbolicChmod
	jmp	.L11
.L13:
	movsbl	-33(%rbp), %edx
	movq	stderr(%rip), %rax
	leaq	.LC5(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L20
.L11:
	movl	-32(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	chmod@PLT
	testl	%eax, %eax
	je	.L15
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$6, %edx
	movl	$1, %esi
	leaq	.LC6(%rip), %rdi
	call	fwrite@PLT
	movl	$-1, %eax
	jmp	.L20
.L15:
	movl	-32(%rbp), %eax
	cmpl	%eax, -72(%rbp)
	je	.L16
	movq	nModif(%rip), %rax
	movl	(%rax), %edx
	movq	nModif(%rip), %rax
	addl	$1, %edx
	movl	%edx, (%rax)
.L16:
	movswl	-68(%rbp), %eax
	andl	$272, %eax
	testl	%eax, %eax
	je	.L17
	leaq	-28(%rbp), %rdx
	movl	-72(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	getSymbolic
	movl	-32(%rbp), %eax
	leaq	-18(%rbp), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	getSymbolic
	andl	$511, -72(%rbp)
	movl	-32(%rbp), %eax
	cmpl	%eax, -72(%rbp)
	je	.L18
	movl	-32(%rbp), %esi
	leaq	-18(%rbp), %rdi
	leaq	-28(%rbp), %rcx
	movl	-72(%rbp), %edx
	movq	-56(%rbp), %rax
	movq	%rdi, %r9
	movl	%esi, %r8d
	movq	%rax, %rsi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L17
.L18:
	movswl	-68(%rbp), %eax
	andl	$256, %eax
	testl	%eax, %eax
	je	.L17
	movl	-32(%rbp), %edx
	leaq	-18(%rbp), %rcx
	movq	-56(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L17:
	movl	$0, %eax
.L20:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L21
	call	__stack_chk_fail@PLT
.L21:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	xmod, .-xmod
	.section	.rodata
	.align 8
.LC9:
	.string	"Invalid permission - %c, fullString = %s\n"
.LC10:
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
	jmp	.L23
.L29:
	movl	-12(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -12(%rbp)
	movslq	%eax, %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	cmpl	$120, %eax
	je	.L24
	cmpl	$120, %eax
	jg	.L25
	cmpl	$114, %eax
	je	.L26
	cmpl	$119, %eax
	je	.L27
	jmp	.L25
.L26:
	addl	$4, -16(%rbp)
	jmp	.L23
.L27:
	addl	$2, -16(%rbp)
	jmp	.L23
.L24:
	addl	$1, -16(%rbp)
	jmp	.L23
.L25:
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movq	-40(%rbp), %rdx
	movl	%eax, %esi
	leaq	.LC9(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L28
.L23:
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L29
	movq	-40(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	cmpl	$117, %eax
	je	.L30
	cmpl	$117, %eax
	jg	.L31
	cmpl	$111, %eax
	je	.L32
	cmpl	$111, %eax
	jg	.L31
	cmpl	$97, %eax
	je	.L33
	cmpl	$103, %eax
	je	.L34
	jmp	.L31
.L30:
	sall	$6, -16(%rbp)
	movl	$63, -8(%rbp)
	jmp	.L35
.L34:
	sall	$3, -16(%rbp)
	movl	$455, -8(%rbp)
	jmp	.L35
.L32:
	movl	$504, -8(%rbp)
	jmp	.L35
.L33:
	movl	$0, -8(%rbp)
	movl	-16(%rbp), %edx
	movl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	movl	%eax, -16(%rbp)
	jmp	.L35
.L31:
	movq	-40(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %edx
	movq	stderr(%rip), %rax
	leaq	.LC5(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L28
.L35:
	addl	$32768, -8(%rbp)
	movsbl	-17(%rbp), %eax
	cmpl	$61, %eax
	je	.L36
	cmpl	$61, %eax
	jg	.L37
	cmpl	$43, %eax
	je	.L38
	cmpl	$45, %eax
	jne	.L37
	movl	-16(%rbp), %eax
	notl	%eax
	andl	-4(%rbp), %eax
	movl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L39
.L38:
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	orl	-16(%rbp), %eax
	movl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L39
.L36:
	movl	-4(%rbp), %eax
	andl	-8(%rbp), %eax
	orl	-16(%rbp), %eax
	movl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L39
.L37:
	movsbl	-17(%rbp), %edx
	movq	stderr(%rip), %rax
	leaq	.LC10(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L28
.L39:
	movl	$0, %eax
.L28:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	symbolicChmod, .-symbolicChmod
	.section	.rodata
.LC11:
	.string	"Invalid flag %s"
.LC12:
	.string	"Error stat() %s\n"
.LC13:
	.string	"."
.LC14:
	.string	".."
.LC15:
	.string	"./xmod"
	.align 8
.LC16:
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
	subq	$608, %rsp
	movl	%edi, -596(%rbp)
	movq	%rsi, -608(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
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
	movw	$0, -586(%rbp)
	cmpl	$2, -596(%rbp)
	jle	.L41
	movl	$1, -584(%rbp)
	jmp	.L42
.L51:
	movl	-584(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-608(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -536(%rbp)
	movq	-536(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$45, %al
	jne	.L43
	movq	-536(%rbp), %rax
	addq	$2, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L44
.L43:
	movq	stderr(%rip), %rax
	movq	-536(%rbp), %rdx
	leaq	.LC11(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L60
.L44:
	movq	-536(%rbp), %rax
	addq	$1, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	cmpl	$118, %eax
	je	.L46
	cmpl	$118, %eax
	jg	.L47
	cmpl	$82, %eax
	je	.L48
	cmpl	$99, %eax
	je	.L49
	jmp	.L47
.L46:
	orw	$256, -586(%rbp)
	jmp	.L50
.L49:
	orw	$16, -586(%rbp)
	jmp	.L50
.L48:
	orw	$1, -586(%rbp)
	jmp	.L50
.L47:
	movq	-536(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC11(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L60
.L50:
	addl	$1, -584(%rbp)
.L42:
	movl	-596(%rbp), %eax
	subl	$2, %eax
	cmpl	%eax, -584(%rbp)
	jl	.L51
	movl	-596(%rbp), %eax
	cltq
	salq	$3, %rax
	leaq	-16(%rax), %rdx
	movq	-608(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -576(%rbp)
	movl	-596(%rbp), %eax
	cltq
	salq	$3, %rax
	leaq	-8(%rax), %rdx
	movq	-608(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rdx
	leaq	-528(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	movl	$144, %edi
	call	malloc@PLT
	movq	%rax, -568(%rbp)
	movq	-568(%rbp), %rdx
	leaq	-528(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	stat@PLT
	testl	%eax, %eax
	je	.L52
	movq	stderr(%rip), %rax
	leaq	-528(%rbp), %rdx
	leaq	.LC12(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$-1, %eax
	jmp	.L60
.L52:
	movq	-568(%rbp), %rax
	movl	24(%rax), %ecx
	movswl	-586(%rbp), %edx
	movq	-576(%rbp), %rsi
	leaq	-528(%rbp), %rax
	movq	%rax, %rdi
	call	xmod
	movswl	-586(%rbp), %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L41
	movq	-568(%rbp), %rax
	movl	24(%rax), %eax
	andl	$61440, %eax
	cmpl	$16384, %eax
	jne	.L41
	leaq	-528(%rbp), %rax
	movq	%rax, %rdi
	call	opendir@PLT
	movq	%rax, -560(%rbp)
	movq	$0, -552(%rbp)
	cmpq	$0, -560(%rbp)
	je	.L41
	jmp	.L54
.L59:
	movq	-544(%rbp), %rax
	addq	$19, %rax
	leaq	.LC13(%rip), %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	je	.L54
	movq	-544(%rbp), %rax
	addq	$19, %rax
	leaq	.LC14(%rip), %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L56
	jmp	.L54
.L56:
	call	fork@PLT
	movl	%eax, -580(%rbp)
	cmpl	$-1, -580(%rbp)
	je	.L57
	cmpl	$0, -580(%rbp)
	jne	.L58
	leaq	-528(%rbp), %rdx
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
	movq	-544(%rbp), %rax
	leaq	19(%rax), %rdx
	leaq	-272(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcat@PLT
	movl	-596(%rbp), %eax
	cltq
	salq	$3, %rax
	leaq	-8(%rax), %rdx
	movq	-608(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	leaq	-272(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	movl	-596(%rbp), %eax
	cltq
	salq	$3, %rax
	leaq	-8(%rax), %rdx
	movq	-608(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-608(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC15(%rip), %rdi
	call	execvp@PLT
	movl	$0, %edi
	call	exit@PLT
.L57:
	movl	$-1, %eax
	jmp	.L60
.L58:
	movq	-552(%rbp), %rax
	movq	%rax, %rdi
	call	wait@PLT
	nop
.L54:
	movq	-560(%rbp), %rax
	movq	%rax, %rdi
	call	readdir@PLT
	movq	%rax, -544(%rbp)
	cmpq	$0, -544(%rbp)
	jne	.L59
	movq	-560(%rbp), %rax
	movq	%rax, %rdi
	call	closedir@PLT
.L41:
	movq	nTotal(%rip), %rax
	movl	(%rax), %edx
	movq	nModif(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC16(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L60:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L61
	call	__stack_chk_fail@PLT
.L61:
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
