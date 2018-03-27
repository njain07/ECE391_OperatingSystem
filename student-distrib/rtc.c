/* rtc.c - Functions to interact with the rtc and its interrupts
 * vim:ts=4 noexpandtab
 */


/*
 * Author: Hyun Do Jung, 03.10.18
 * Reference materials: OSDEV website
 * Filename: rtc.c
 * History:
 *    SL    1    Sun Mar 11 14:20:50 2018
 *        First written for MP3.1
 *    SL    2    Thu Mar 22 10:13:12 2018
 *        Modified, commented for MP3.1 and added firstly for MP3.2
 */


#include "rtc.h"
#include "i8259.h"
#include "lib.h"

/* Flag used in rtc_read prevent other possible interrupts occured during it, until it gets cleared */
volatile int rtc_interrupt_flag = 0;


/*
 * rtc_init
 *   DESCRIPTION: Initialize Real Time Clock(RTC)
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: rtc is initialized
 */

void rtc_init(void){
    
    /* variable holding the old register value */
    unsigned char old_reg_val = inb(RTC_DATA_PORT);
    
    outb(RTC_NMIDIS_REG_B, RTC_REG_NUM_PORT);             // outportb(0x70, 0x8B), select register b, disable NMI
    
    /* periodic interrupt, turning on IRQ8 */
    outb(old_reg_val | RTC_INTERRUPT, RTC_DATA_PORT);     // outportb(0x71, old_reg_val | 0x40) -- write the old register value ORed with 0x40, turning on bit 6 of register B
    enable_irq(RTC_IRQ);
}


/*
 * rtc_interrupt_handler
 *   DESCRIPTION: helper function; handles rtc interrupt; If we want to have another interrupt,
 *   we need to set this to have a periodic interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: handles with RTC interrupts
 */

void rtc_interrupt_handler(void){
    /* critical section */
    cli();
    
    outb(RTC_STATUS_REG_C, RTC_REG_NUM_PORT);            // select Register C, read from register C
    inb(RTC_DATA_PORT);                                  // just throw away contents
    send_eoi(RTC_IRQ);                                   // done int, send EOI to IRQ8
    
    rtc_interrupt_flag = 0;
    /* critical section ended */
    sti();
}


/*
 * rtc_open
 *   DESCRIPTION: helper function for opening the RTC
 *   INPUTS: uint8_t* filename(uint8_t ~= unsigned char(0~255)), pointer
 *   OUTPUTS: none
 *   RETURN VALUE: always return 0
 *   SIDE EFFECTS: handles with opening RTC, set frequency to 2 Hz.
 */

int32_t rtc_open(const uint8_t* filename){
     
     /* SET rtc frequnecy = 2 Hz */
     //set_rtc_frequency(2);
     return 0;
}

/*
 * rtc_close
 *   DESCRIPTION: helper function for closing the RTC
 *   INPUTS: pointer to the file descriptor that will be closed
 *   OUTPUTS: none
 *   RETURN VALUE: always return 0
 *   SIDE EFFECTS: handles with closing RTC, reset frequency to 2 Hz.
 */

int32_t rtc_close(int32_t fd){
    
    /* RESET rtc frequnecy = 2 Hz */
    //set_rtc_frequency(2);
    return 0;
}

/*
 * rtc_read
 *   DESCRIPTION: helper function for reading the RTC
 *   INPUTS: pointer to the file descriptor that will be closed
 *   OUTPUTS: none
 *   RETURN VALUE: always return 0
 *   SIDE EFFECTS: flag is cleared after an interrupt
 */

/* handler for reading RTC */
int32_t rtc_read(int32_t fd){
    
    rtc_interrupt_flag = 1;
    
    /* during interrupt, do nothing */
    while(rtc_interrupt_flag){   }
    
    /* clear the interrupt flag */
    rtc_interrupt_flag = 0;
    return 0;
}

/*
 * rtc_write
 *   DESCRIPTION: helper function for reading the RTC
 *   INPUTS: pointer to the file descriptor that will be closed
 *   OUTPUTS: none
 *   RETURN VALUE: always return 0
 *   SIDE EFFECTS: flag is cleared after an interrupt
 */

/* handler for reading RTC */
int32_t rtc_write(int32_t* buf, int32_t nbytes){
    
    /* during interrupt, do nothing */
    while(!rtc_interrupt_flag){   }
    
    /* clear the interrupt flag */
    rtc_interrupt_flag = 0;
    return 0;
}
