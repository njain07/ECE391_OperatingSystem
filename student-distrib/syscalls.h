#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "lib.h"
#include "filesys.h"
#include "rtc.h"
#include "paging.h"
#include "pcb.h"
#include "x86_desc.h"

#define NULL        0
#define FD_SIZE     8
#define KB_8		0x2000
#define	MB_4		0x400000
#define MB_8		0x800000

extern int32_t process_num;
extern int32_t terminal_num;
pcb_t* current_pcb;

int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);

int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open(const uint8_t* filename);
int32_t close(int32_t fd);

int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);

/* Helper Functions */
void change_process(int32_t new_process_num, int32_t execute_halt_switch);
void terminal_switch(uint8_t terminal_id); /* Switch to the next terminal */
	
#endif
