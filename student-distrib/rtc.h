/* keyboard.h - Defines used in interactions with the keyboard devices and its interrupts
 * vim:ts=4 noexpandtab
 */

#ifndef _RTC_H_
#define _RTC_H_

#include "types.h"

#define    RTC_REG_NUM_PORT    0x70     // specify an index and to disable NMI
#define    RTC_DATA_PORT       0x71     // R/W DATA port

/* NMI DISABLED REGS */
#define    RTC_NMIDIS_REG_A    0x8A
#define    RTC_NMIDIS_REG_B    0x8B
#define    RTC_NMIDIS_REG_C    0x8C

/* REGS */
#define    RTC_STATUS_REG_A    0x0A
#define    RTC_STATUS_REG_B    0x0B
#define    RTC_STATUS_REG_C    0x0C
#define    RTC_INTERRUPT       0x40
#define    RTC_IRQ             8


/* initialize the keyboard */
void rtc_init(void);

/* interrupt handler for rtc */
void rtc_interrupt_handler(void);

#endif
