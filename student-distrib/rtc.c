/* rtc.c - Functions to interact with the rtc and its interrupts
 * vim:ts=4 noexpandtab
 */

#include "rtc.h"
#include "
#include "lib.h"

/* Init helper for keyboard initialization */
void rtc_init(void){
    
    /* variable holding the old register value */
    unsigned char old_reg_val = inb(RTC_DATA_PORT);
    
    outb(RTC_NMIDIS_REG_B, RTC_REG_NUM_PORT);             // outportb(0x70, 0x8B), select register b, disable NMI
    
    /* periodic interrupt, turning on IRQ8 */
    outb(old_reg_val | RTC_INTERRUPT, RTC_DATA_PORT);    // outportb(0x71, old_reg_val | 0x40) -- write the old register value ORed y99with 0x40, turning on bit 6 of register B
    enable_irq(RTC_IRQ);
}

/* RTC handler for interrupts */
void rtc_interrupt_handler(void){
    /* critical section */
    cli();
    
    outb(RTC_STATUS_REG_C, RTC_REG_NUM_PORT);            // select Register C, read from register C
    inb(RTC_DATA_PORT);                                  // just throw away contents
    send_eoi(RTC_IRQ);                                   // done int, send EOI to IRQ8
    
    /* critical section ended */
    sti();
}
