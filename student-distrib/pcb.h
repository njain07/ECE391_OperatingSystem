#ifndef _PCB_H
#define _PCB_H

#include "filesys.h"

typedef struct file_ops
{
	int32_t (*open) (const uint8_t* filename);
	int32_t (*read) (int32_t fd, void* buf, int32_t nbytes);
	int32_t (*write) (int32_t fd, const void* buf, int32_t nbytes);
	int32_t (*close) (int32_t fd);
} file_ops;

typedef struct file_descriptor_struct
{
	file_ops* file_jmp_tbl;	//file operations jump table
	int32_t inode_num;		//only valid for data files; should be 0 for directories and RTC device file
	int32_t file_pos;		//keeps track of where the user is currently reading from the file
	int32_t flags;			//marks file descriptor as "in-use" etc.
} file_descriptor_struct;

file_descriptor_struct file_array[8];

typedef struct pcb 
{
	// file_descriptor_struct file_array[8];
	
} pcb;

#endif
