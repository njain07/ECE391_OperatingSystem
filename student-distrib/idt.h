#ifndef _IDT_H
#define _IDT_H

//importing files
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"

// Initalising the IDT
void init_idt();
extern void DE();
// taken from the linux kernel
typedef struct pt_regs {
	long ebx;
	long ecx;
	long edx;
	long esi;
	long edi;
	long ebp;
	long eax;
	int  xds;
	int  xes;
	int  xfs;
	long orig_eax;
	long eip;
	int  xcs;
	long eflags;
	long esp;
	int  xss;
}pt_regs;
char *error_messages[21];
extern void interrupt_rtc();
extern void DE();
extern void DB();
extern void NMI();
extern void BP();
extern void OF();
extern void BR();
extern void UD();
extern void NM();
extern void DF();
extern void CSO();
extern void TS();
extern void NP();
extern void SS();
extern void GP();
extern void PF();
extern void IntelR();
extern void MF();
extern void AC();
extern void MC();
extern void XF();
//extern void Sys_Error();
extern void unknown();

#endif
