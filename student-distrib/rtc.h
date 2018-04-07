/* rtc.h - Defines used in interactions with the rtc and its interrupts
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

/* others */
#define    RS_MASK             0xF0

/* Flag used in rtc_read prevent other possible interrupts occured during it, until it gets cleared */
volatile int rtc_interrupt_flag;

/* initialize the keyboard */
void rtc_init(void);

/* interrupt handler for rtc */
void rtc_interrupt_handler(void);

/* handler for opening RTC */
int32_t rtc_open(const uint8_t* filename);

/* handler for reading RTC */
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);

/* handler for writing RTC */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);

/* handler for closing RTC */
int32_t rtc_close(int32_t fd);

/* handler for setting an interrupt frequency of rtc */
void rtc_set_int_freq(int32_t target_freq);

#endif /* _RTC_H_ */
