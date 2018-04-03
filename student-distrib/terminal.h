/* terminal.c - Defines used in interactions with the terminals including basic file syste
 *              functions
 * vim:ts=4 noexpandtab
 */


/*
 * Editor: Hyun Do Jung, 03.19.18
 * Reference materials: OSDEV website, appendix B, MP3 document
 * Filename: terminal.c
 * History:
 *    SL    1    Thu Sep 10 22:08:20 2018
 *        First made for MP3.3
 */

#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "types.h"
#include "keyboard.h"
#include "lib.h"
#include "rtc.h"
#include "file_system.h"


#define  BUFFER_SIZE    128
#define  TERMINAL_NUM   3

#define  FAIL           -1
#define  READ           0
#define  READ_DONE      1
#define  WRITE          2
#define  WAIT           3


/*  */
char keyboard_buffer[BUFFER_SIZE];
int b_index;
int read_index;
int read_flag;

/* TERMINAL DATA STRUCTURE
typedef struct terminals{
    int terminal_id;
    int current_process;
    
    uint8_t x;
    uint8_t y;
    
    volatile
    
}*/


/* initialize terminal */
void terminal_init(void);

/* Terminal System Calls */
int32_t terminal_open(const uint8_t* filename);
int32_t terminal_close(int32_t fd);
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);

/* Switch to the next terminal */
void terminal_switch(uint8_t terminal_id)
#endif /* _TERMINAL_H_ */
