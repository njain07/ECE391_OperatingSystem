#include "idt.h"

// void DE()
// {
// 	cli();
// 	printf(" Divide by Zero error \n" );
// 	while(1);
// }

// void DB()
// {
// 	cli();
// 	printf(" Reserved error \n" );
// 	while(1);
// }
// void NMI()
// {
// 	cli();
// 	printf(" NMI Interrupt \n" );
// 	while(1);
// }
// void BP()
// {
// 	cli();
// 	printf(" Breakpoint error \n" );
// 	while(1);
// }
//  void OF()
//  {
//  	cli();
//  	printf(" Overflow error \n" );
//  	while(1);
//  }

//  void BR()
//  {
//  	cli();
//  	printf(" Bound Range Exceeded error \n" );
//  	while(1);
//  }

//  void UD()
//  {
//  	cli();
//  	printf(" Invalid Opcode \n" );
//  	while(1);
//  }

//  void NM()
//  {
//  	cli();
//  	printf(" Device not Available \n" );
//  	while(1);
//  }

//  void DF()
//  {
//  	cli();
//  	printf(" Double Fault \n" );
//  	while(1);
//  }

//  void CSO()
//  {
//  	cli();
//  	printf(" Coprocessor Segment Overrun\n" );
//  	while(1);
//  }

//  void TS()
//  {
//  	cli();
//  	printf(" Invalid TSS\n" );
//  	while(1);
//  }

//  void NP()
//  {
//  	cli();
//  	printf(" Segment not present\n" );
//  	while(1);
//  }

//  void SS()
//  {
//  	cli();
//  	printf(" Stack Segment fault\n" );
//  	while(1);
//  }

//  void GP()
//  {
//  	cli();
//  	printf(" General Protection\n" );
//  	while(1);
//  }

//  void PF()
//  {
//  	cli();
//  	printf(" Page Fault\n" );
//  	while(1);
//  }
//  void IntelR()
//  {
//  	cli();
//  	printf(" Intel Reserved\n" );
//  	while(1);
//  }

//  void MF()
//  {
//  	cli();
//  	printf(" Floating Point Error\n" );
//  	while(1);
//  }

//  void AC()
//  {
//  	cli();
//  	printf(" Alignment Check\n" );
//  	while(1);
//  }

//  void MC()
//  {
//  	cli();
//  	printf(" Machine Check\n" );
//  	while(1);
//  }

//  void XF()
//  {
//  	cli();
//  	printf(" SIMD Floating Point Exception\n" );
//  	while(1);
//  }

//  void Sys_Error()
//  {
//  	cli();
//  	printf(" System Error\n" );
//  	while(1);
//  }

// void unknown()
// {
// 	cli();
// 	printf(" unknown error\n" );
// 	while(1);
// }

void do_irq(int i){
	// get i to hold the vec num pushed by the assembly code
	// using the pt_regs struct
	printf("%x", i);

	if(i==40)
	{
		rtc_interrupt_handler();
	}
	else if (i == 33)
	{
		keyboard_interrupt_handler();
	}
	else if (i < 32)
	{
		printf("The exception raised was :%s", error_messages[i]);
		while(1);
	}
}

void init_idt()
{
	int i;
	//void (*idt_interrupts[22]) = {DE, DB, NMI, BP, OF, BR, UD, NM, DF, CSO, TS, NP, SS, GP, PF, IntelR, MF, AC, MC, XF, Sys_Error, unknown}; // first 19 interrupts stored in an array of function pointers
	//notice that the 15th one is some intel reserved interrupt I will just keep an intel reserved error
	for (i = 0; i < NUM_VEC; i++)
		{
			idt[i].seg_selector = KERNEL_CS;
			idt[i].reserved4 = 0;
			idt[i].reserved3 = 0; //when set to 1 -> trap gate
			idt[i].reserved2 = 1;
			idt[i].reserved1 = 1; // these three lines initialize the entry to be that of an interrupt gate since they are all 32 bits
			idt[i].reserved0 = 0; 
			idt[i].dpl = 0;
			idt[i].size = 1;
			idt[i].present = 1;

			if (i == 0x80) 		//remember to magic number this one
			{
				idt[i].dpl = 3;
				idt[i].reserved3 = 1;
			}

			else if (i == 0x28)
			{
				SET_IDT_ENTRY(idt[0x28], &interrupt_rtc);
			}
			
			else if (i == 0x21)
			{
				SET_IDT_ENTRY(idt[0x21], &interrupt_keyboard);
			}
			
			else
			{
			 	SET_IDT_ENTRY(idt[i], unknown);
			}

			// 	SET_IDT_ENTRY(idt[i], idt_interrupts[21]); // if this doesnt work try SET_IDT_ENTRY(idt[i], *unknown);
			// note that we could change the if else if to add more interrupts when needed 
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

	//initalizing strings 
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
