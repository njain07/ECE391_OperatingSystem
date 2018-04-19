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

/*
 * rtc_init
 *   DESCRIPTION: Initialize Real Time Clock(RTC)
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: rtc is initialized
 */

void rtc_init(void){
    /* interrupt flag set */
    rtc_interrupt_flag = 0; // moved from rtc.h since any intialization in header files are inhibited.
    
    /* variable holding the old register value */
    outb(RTC_NMIDIS_REG_B, RTC_REG_NUM_PORT);             // outportb(0x70, 0x8B), select register b, disable NMI
    unsigned char old_reg_val = inb(RTC_DATA_PORT);        // read from RTC_DATA_PORT

    outb(RTC_NMIDIS_REG_B, RTC_REG_NUM_PORT);
    /* periodic interrupt, turning on IRQ8 */
    outb(old_reg_val | RTC_INTERRUPT, RTC_DATA_PORT);     // outportb(0x71, old_reg_val | 0x40) -- write the old register value ORed with 0x40, turning on bit 6 of register B
    rtc_set_int_freq(2); 
    enable_irq(RTC_IRQ);                                    // write to RTC_DATA_PORT
    // The last statement is confusing

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

void rtc_interrupt_handler(){
    /* critical section */
    // cli();
    outb(RTC_STATUS_REG_C, RTC_REG_NUM_PORT);            // select Register C, read from register C
    inb(RTC_DATA_PORT);                                  // just throw away contents
    putc('a');

    send_eoi(RTC_IRQ);                                   // done int, send EOI to IRQ8
    
    // test_interrupts();                                 // let us keep this one in hold
    rtc_interrupt_flag = 1;
    /* critical section ended */
    // sti();
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
     rtc_set_int_freq(2);
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
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes){
    
    /* during interrupt, do nothing */
    rtc_interrupt_flag = 0;
    while(rtc_interrupt_flag == 0){ /* SPIN, waiting until the next RTC interrupt */  
    }
    
    /* clear the interrupt flag */
    return 0;
}

/*
 * rtc_write
 *   DESCRIPTION: helper function for writing the RTC
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: return -1 on failure, return 0 on success
 *   SIDE EFFECTS: flag is cleared after an interrupt
 */

/* handler for reading RTC */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes){
    
    /* set local variable */
    int32_t target_freq;
    
    
    /* Boundary check, only accepts 4 bytes */
    if( (nbytes != 4) || ((int32_t)buf == NULL) )
        return -1;
    else
        target_freq = *((int32_t*)buf);
        
    /* set a interrupt freq as wanted */
    cli();
    rtc_set_int_freq(target_freq);
    sti();


    /* testing */
    //int i, count;
    // for(i = 0; i < target_freq; i++){
    //     count = 1;
    //     putc('1');
    // }

    return nbytes;
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
    //rtc_set_int_freq(2);
    return 0;
}

/*
 * rtc_set_int_freq
 *   DESCRIPTION: helper function for setting an interrupt frequency as wanted
 *   INPUTS: int32_t target_freq
 *   OUTPUTS: none
 *   RETURN VALUE:
 *   SIDE EFFECTS:
 */

/* handler for setting an interrupt frequency of rtc */
void rtc_set_int_freq(int32_t target_freq){
    
    /* set local variables */
    char freq;
    unsigned char old_a_val;
    
    /* backup; save previous reg A value */
    outb(RTC_NMIDIS_REG_A, RTC_REG_NUM_PORT);
    old_a_val = inb(RTC_DATA_PORT);
    
    /* Values defined in RTC Datasheet(new) */
    switch(target_freq){
        case 8192:
        case 4096:
        case 2048:
            return;
            
        case 1024: freq = 0x06; break;
        case 512: freq = 0x07; break;
        case 256: freq = 0x08; break;
        case 128: freq = 0x09; break;
        case 64:  freq = 0x0A; break;
        case 32:  freq = 0x0B; break;
        case 16:  freq = 0x0C; break;
        case 8:   freq = 0x0D; break;
        case 4:   freq = 0x0E; break;
        case 2:   freq = 0x0F; break;
        
        default: return;
    }
    
    /* set RS values */
    outb(RTC_NMIDIS_REG_A, RTC_REG_NUM_PORT);
    outb( (old_a_val & RS_MASK) | freq, RTC_DATA_PORT);
}

