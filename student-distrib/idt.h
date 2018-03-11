#ifndef _IDT_H
#define _IDT_H

//importing files
#include "x86_desc.h"
#include "lib.h"
// Initalising the IDT
void init_idt();

// all the interrupt functions
void DE();
void DB();
void NMI();
void BP();
void OF();
void BR();
void UD();
void NM();
void DF();
void CSO();
void TS();
void NP();
void SS();
void GP();
void PF();
void IntelR();
void MF();
void AC();
void MC();
void XF();
void Sys_Error();
void unknown();

#endif
