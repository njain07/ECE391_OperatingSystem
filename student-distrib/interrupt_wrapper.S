.global interrupt_rtc, interrupt_keyboard, interrupt_sys
.global DE, DB, NMI, BP, OF, BR, UD, NM, DF, CSO, TS, NP, SS, GP, PF, IntelR, MF, AC, MC, XF, unknown

interrupt_rtc:
	pushal
	pushl $40
	jmp common_interrupt

interrupt_keyboard:
	pushal
	pushl $33
	jmp common_interrupt

DE:
	pushal
	pushl $0
	jmp common_interrupt

DB:
	pushal
	pushl $1
	jmp common_interrupt

NMI:
	pushal
	pushl $2
	jmp common_interrupt

BP:
	pushal
	pushl $3
	jmp common_interrupt

OF:
	pushal
	pushl $4
	jmp common_interrupt

BR:
	pushal
	pushl $5
	jmp common_interrupt

UD:
	pushal
	pushl $6
	jmp common_interrupt

NM:
	pushal
	pushl $7
	jmp common_interrupt

DF:
	pushal
	pushl $8
	jmp common_interrupt

CSO:
	pushal
	pushl $9
	jmp common_interrupt

TS:
	pushal
	pushl $10
	jmp common_interrupt

NP:
	pushal
	pushl $11
	jmp common_interrupt

SS:
	pushal
	pushl $12
	jmp common_interrupt

GP:
	pushal
	pushl $13
	jmp common_interrupt

PF:
	pushal
	pushl $14
	jmp common_interrupt

IntelR:
	pushal
	pushl $15
	jmp common_interrupt

MF:
	pushal
	pushl $16
	jmp common_interrupt

AC:
	pushal
	pushl $17
	jmp common_interrupt

MC:
	pushal
	pushl $18
	jmp common_interrupt

XF:
	pushal
	pushl $19
	jmp common_interrupt

unknown:
	pushal
	pushl $20
	jmp common_interrupt

common_interrupt:
	# save all the registers 
	# as in the pt_regs struct
	#pushw $0
	#pushl %fs
	#pushw $0
 	#pushl %es
	#pushw $0
	#pushl %ds
	# cli	
	#push %eax
	#push %ebp
	#push %edi
	#push %esi
	#push %edx
	#push %ecx
	#push %ebx
 	#push %esp
	#pushl 44(%esp)	

    call do_irq

	#addl $8,%esp
	#pop  %ebx
	#pop  %ecx
	#pop  %edx
	#pop  %esi
	#pop  %edi
	#pop  %ebp
	#pop  %eax
	# sti	
	#popl %ds
	#popl %es
	#popl %fs
    addl $4,%esp
    popal
    iret

 #  CP 3 
interrupt_sys:
	pushal 

	push	%es
	push	%fs
	push	%gs

	pushl 	%edx
	pushl 	%ecx
	pushl 	%ebx

	subl 	$1, %eax
	
	# syscall value checking
	cmpl	$0, %eax	
	jl 		ERROR

	cmpl	$5, %eax	#change to 9 for cp4
	jg		ERROR

	call 	*jmp_table( ,%eax, 4)
	jmp 	POP

ERROR:
	movl	$-1, %eax

POP:
	popl 	%ebx
	popl 	%ecx
	popl 	%edx

	pop 	%gs
	pop		%fs
	pop		%es

jmp_table:
	.long halt, execute, read, write, open, close #, getargs, vidmap, set_handler, sigreturn 

