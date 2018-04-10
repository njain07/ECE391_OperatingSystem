.global interrupt_rtc ,DE,DB,NMI,BP,OF,BR,UD,NM,DF,CSO,TS,NP,SS,GP,PF,IntelR,MF,AC,MC,XF,unknown

# Ok so we need to write assembly code to ensure 
interrupt_rtc:
		pushl $40
		jmp common_interrupt


NMI_Exception:
	pushl $3
	jmp common_interrupt

DE:
	pushl $0
	jmp common_interrupt

DB:
	pushl $1
	jmp common_interrupt

NMI:
	pushl $2
	jmp common_interrupt

BP:
	pushl $3
	jmp common_interrupt

OF:
	pushl $4
	jmp common_interrupt

BR:
	pushl $5
	jmp common_interrupt

UD:
	pushl $6
	jmp common_interrupt

NM:
	pushl $7
	jmp common_interrupt

DF:
	pushl $8
	jmp common_interrupt

CSO:
	pushl $9
	jmp common_interrupt

TS:
	pushl $10
	jmp common_interrupt

NP:
	pushl $11
	jmp common_interrupt

SS:
	pushl $12
	jmp common_interrupt

GP:
	pushl $13
	jmp common_interrupt

PF:
	pushl $14
	jmp common_interrupt

IntelR:
	pushl $15
	jmp common_interrupt

MF:
	pushl $16
	jmp common_interrupt

AC:
	pushl $17
	jmp common_interrupt

MC:
	pushl $18
	jmp common_interrupt

XF:
	pushl $19
	jmp common_interrupt

unknown:
	pushl $20
	jmp common_interrupt

common_interrupt:
	# save all the registers 
	# as in the pt_regs struct
	#pushw $0
	pushw %fs
	#pushw $0
    pushw %es
    #pushw $0
    pushw %ds
    # cli	
    push %eax
    push %ebp
    push %edi
    push %esi
    push %edx
    push %ecx
    push %ebx
    push %esp
    pushl 38(%esp)	

    call do_irq

    addl $8,%esp
    pop  %ebx
    pop  %ecx
    pop  %edx
    pop  %esi
    pop  %edi
    pop  %ebp
    pop  %eax
    # sti	
    popw %ds
    popw %es
    popw %fs
    addl $4,%esp
    iret
	# call a common C based exception Handler

	# Pop all 
	# Then do IRET