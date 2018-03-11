#include "idt.h"

void init_idt(){
int i;
void *idt_interrupts = {DE,DB,NMI,BP,OF,BR,UD,NM,DF,CSO,TS,NP,SS,GP,PF,IntelR,MF,AC,MC,XF,Sys_Error,unknown} // firt 19 interrupts 
//notice that the 15th one is some intel reserved interrupt I will just keep an intel reserved error
//kept an unknown function pointer in the 
for (i = 0; i < NUM_VEC; ++i)
	{
		idt[i].seg_selector = KERNEL_CS;
		idt[i].reserved3 = 0;
		idt[i].reserved2 = 1;
		idt[i].reserved1 = 1; // theese three lines initialises the entry to be that of an interrupt gate since they are all 32 bits
		idt[i].reserved0 = 0; 
		idt[i].dpl = 0;
		if( i == 0x80) 		//remember to magic number this one
			idt[i].dpl = 3;
		idt[i].present = 0;  // set to zero to show it is unused interrupt
		
if (i < 20)
	SET_IDT_ENTRY(idt[i],idt_interrupts[i]);
else if i == 0x80
	SET_IDT_ENTRY(idt[i],idt_interrupts[20]); // if this doesnt work try SET_IDT_ENTRY(idt[i],*Sys_Error);
else
	SET_IDT_ENTRY(idt[i],idt_interrupts[21]); // if this doesnt work try SET_IDT_ENTRY(idt[i],*unknown);
// note that we cold change the if else if to add more interrupts when needed 
}
}

void DE()
{
	cli();
	printf(" Divide by Zero error \n" );
	while(1);
}

void DB()
{
	cli();
	printf(" Reserved error \n" );
	while(1);
}
void NMI()
{
	cli();
	printf(" NMI Interrupt \n" );
	while(1);
}
void BP()
{
	cli();
	printf(" Breakpoint error \n" );
	while(1);
}

void OF()
{
	cli();
	printf(" Overflow error \n" );
	while(1);
}

void BR()
{
	cli();
	printf(" Bound Range Exceedded error \n" );
	while(1)
}

void UD()
{
	cli();
	printf(" Invalid Opcode \n" );
	while(1)
}

void NM()
{
	cli();
	printf(" Device not Available \n" );
	while(1);
}

void DF()
{
	cli();
	printf(" Double Fault \n" );
	while(1);
}

void CSO()
{
	cli();
	printf(" Coprocessor Segment Overrun\n" );
	while(1);
}

void TS()
{
	cli();
	printf(" Invalid TSS\n" );
	while(1);
}

void NP()
{
	cli();
	printf(" Segment not present\n" );
	while(1);
}

void SS()
{
	cli();
	printf(" Stack Segment fault\n" );
	while(1);
}

void GP()
{
	cli();
	printf(" General Protection\n" );
	while(1);
}

void PF()
{
	cli();
	printf(" Page Fault\n" );
	while(1);
}
void IntelR()
{
	cli();
	printf(" Intel Reserved\n" );
	while(1);
}

void MF()
{
	cli();
	printf(" Floating Point Error\n" );
	while(1);
}

void AC()
{
	cli();
	printf(" Alignment Check\n" );
	while(1);
}

void MC()
{
	cli();
	printf(" Machine Check\n" );
	while(1);
}

void XF()
{
	cli();
	printf(" SIMD Floating Point Exception\n" );
	while(1);
}

void Sys_Error()
{
	cli();
	printf(" System Error\n" );
	while(1);
}

void unknown()
{
	cli();
	printf(" unknown error\n" );
	while(1);
}
