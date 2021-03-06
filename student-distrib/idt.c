/* Written by: Vismayak
 * Debugged by: Vismayak */

#include "idt.h"

#define RTC_VECTOR 		0x28
#define KEYBOARD_VECTOR 0x21
#define SYSTEM_CALL 	0x80

#define PAGE_FAULT 		14

/*
 * do_irq
 *   DESCRIPTION: 
 *   INPUTS: i -- the vector number pushed by the assembly code
 				  using the pt_regs struct
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void do_irq(int i){
	if(i == RTC_VECTOR)
		rtc_interrupt_handler();

	else if (i == KEYBOARD_VECTOR)
		keyboard_interrupt_handler();

	else if (i < (KEYBOARD_VECTOR-1))
	{
		if(i == PAGE_FAULT)
		{
			uint32_t page_fault_addr;
			asm volatile 
		    (
		        "movl %%cr2, %0;"
		        : "=r" (page_fault_addr)
		        :
		        : "ecx"
		    );
		    printf("The address being accessed was %x\n", page_fault_addr);
		}
		printf("The exception raised was :%s", error_messages[i]);
		while(1);
	}
}

/*
 * init_idt
 *   DESCRIPTION: initializes the IDT
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void idt_init()
{
	int i;
	for (i = 0; i < NUM_VEC; i++)
	{
		idt[i].seg_selector = KERNEL_CS;
		idt[i].reserved4 = 0;
		idt[i].reserved3 = 0; //when set to 1 -> trap gate (doesn't mask interrupts, unlike interrupt gates)
		idt[i].reserved2 = 1;
		idt[i].reserved1 = 1; // these three lines initialize the entry to be that of an interrupt gate since they are all 32 bits
		idt[i].reserved0 = 0; 
		idt[i].dpl = 0;
		idt[i].size = 1;
		idt[i].present = 1;

		if (i == SYSTEM_CALL)
		{
			idt[i].dpl = 3;
			idt[i].reserved3 = 1; // use trap gate so it doesn't mask interrupts
			SET_IDT_ENTRY(idt[SYSTEM_CALL], &interrupt_sys);
		}

		else if (i == RTC_VECTOR)
			SET_IDT_ENTRY(idt[RTC_VECTOR], &interrupt_rtc);
		
		else if (i == KEYBOARD_VECTOR)
			SET_IDT_ENTRY(idt[KEYBOARD_VECTOR], &interrupt_keyboard);
		
		else
		 	SET_IDT_ENTRY(idt[i], unknown);
	}

	SET_IDT_ENTRY(idt[0],DE);
	SET_IDT_ENTRY(idt[1],DB);
	SET_IDT_ENTRY(idt[2],NMI);
	SET_IDT_ENTRY(idt[3],BP);
	SET_IDT_ENTRY(idt[4],OF);
	SET_IDT_ENTRY(idt[5],BR);
	SET_IDT_ENTRY(idt[6],UD);
	SET_IDT_ENTRY(idt[7],NM);
	SET_IDT_ENTRY(idt[8],DF);
	SET_IDT_ENTRY(idt[9],CSO);
	SET_IDT_ENTRY(idt[10],TS);
	SET_IDT_ENTRY(idt[11],NP);
	SET_IDT_ENTRY(idt[12],SS);
	SET_IDT_ENTRY(idt[13],GP);
	SET_IDT_ENTRY(idt[14],PF);
	SET_IDT_ENTRY(idt[15],IntelR);
	SET_IDT_ENTRY(idt[16],MF);
	SET_IDT_ENTRY(idt[17],AC);
	SET_IDT_ENTRY(idt[18],MC);
	SET_IDT_ENTRY(idt[19],XF);
	lidt(idt_desc_ptr);

	//initalizing error_messages
	error_messages[0]= " Divide by Zero error \n";
	error_messages[1]= " Reserved error \n";
	error_messages[2]= " NMI Interrupt \n" ;
	error_messages[3]= " Breakpoint error \n"; 
	error_messages[4]= " Overflow error \n" ;
	error_messages[5]= " Bound Range Exceeded error \n" ;
	error_messages[6]= " Invalid Opcode \n" ;
	error_messages[7]= " Device not Available \n";
	error_messages[8]= " Double Fault \n" ;
	error_messages[9]= " Coprocessor Segment Overrun\n" ;
	error_messages[10]= " Invalid TSS\n";
	error_messages[11]= " Segment not present\n";
	error_messages[12]= " Stack Segment fault\n";
	error_messages[13]= " General Protection\n";
	error_messages[14]= " Page Fault\n";
	error_messages[15]= " Intel Reserved\n";
	error_messages[16]= " Floating Point Error\n";
	error_messages[17]= " Alignment Check\n";
	error_messages[18]= " Machine Check\n";
	error_messages[19]= " SIMD Floating Point Exception\n";
	error_messages[20]= " unknown error \n";
}
