#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "lib.h"
#include "filesys.h"
#include "rtc.h"
#include "paging.h"
#include "pcb.h"

#define    NULL        0
#define    FD_SIZE     8
#define    KB8         0x2000

pcb_t current_pcb;

int32_t execute(const uint8_t* command);
int32_t halt(uint8_t* status);

int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open(const uint8_t* filename);
int32_t close(int32_t fd);

int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);

#endif