#ifndef _PCB_H
#define _PCB_H

#include "filesys.h"
#include "rtc.h"
#include "terminal.h"

#define MAX_FD_STRUCT_SIZE 8
#define FD_MAX_INDEX       7

/* Edited by HD for CP4, set_handler in syscalls.c */
#define SIGNUM_MAX_INDEX   5

/* Lumetta course notes, page 42 */
typedef struct file_ops_t
{
	int32_t (*read) (int32_t fd, void* buf, int32_t nbytes);
	int32_t (*write) (int32_t fd, const void* buf, int32_t nbytes);
	int32_t (*open) (const uint8_t* filename);
	int32_t (*close) (int32_t fd);
} file_ops_t;

extern file_ops_t rtc_ops;
extern file_ops_t dir_ops;
extern file_ops_t file_ops;
extern file_ops_t terminal_ops;

typedef struct file_descriptor_struct
{
	file_ops_t* file_jmp_tbl;	//file operations jump table
	int32_t inode_num;		//only valid for data files; should be 0 for directories and RTC device file
	int32_t file_pos;		//keeps track of where the user is currently reading from the file
	int32_t flags;			//marks file descriptor as "in-use" (=1) etc.
} file_descriptor_struct;	//fd_t

typedef struct pcb_t
{
	file_descriptor_struct file_array[MAX_FD_STRUCT_SIZE];
	uint8_t* program;
	uint8_t* arguments;

	int32_t pid;			// process id
	int32_t parent;			// parent process id
	int32_t child;			// child process id

	uint32_t p_start_addr;	// process starting address
	uint32_t page_addr;		// physical address of page

	/* for halt syscall */
	uint32_t p_ebp;			// parent_ebp
	uint32_t p_esp;			// parent_esp
	uint32_t p_eip;			// parent_eip
	
	//maybe add kernel stack?
    
    /* EDITED by HD */
    /* MP3 Document: signal handling info should go in the PCB, pending signals, masked signals, handler actions / addresses for each signal */
    
    /* SIGNAL PART for cp4 */
    /* array for signal action, total 5 we deal with */
    // int32_t* signal_action[SIGNUM_MAX_INDEX];
    
    // int32_t signal_mask;
    // int32_t signal_pending;
    
} pcb_t;

extern pcb_t* current_pcb;

#endif
