#ifndef INTERRUPTS
#define INTERRUPTS

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
