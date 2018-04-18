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


/* file descriptor structure */
typedef struct file_descriptor {
    uint32_t flag;
    uint32_t file_position;
    uint32_t inode;
    file_op_t* file_operation_table_pointer;
} fd_t;


/* process control block structure */
typedef struct process_control_block {
    uint32_t pid;
    uint32_t parent;
    uint32_t child;
    
    /* starting address of process and physical address of page */
    uint32_t p_start_addr;
    uint32_t page_addr;
    
    
    /* for syscall's halt */
    uint32_t p_ebp;
    uint32_t p_esp;
    uint32_t p_eip;
    
    /* file descriptor array, total 8, index 0,1 are already taken. */
    fd_t file_descritor_array[FD_SIZE];
    // maybe kernel stack
} pcb_t;




int32_t execute(const uint8_t* command);
int32_t halt(uint8_t* status);

/* syscall; file_open */
int32_t open(const uint8_t* filename);
int32_t read(int32_t fd, void* buf, int32_t nbytes);

/* syscall; file_write */
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t close(int32_t fd);
int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);

#endif
