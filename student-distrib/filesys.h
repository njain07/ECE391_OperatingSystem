#ifndef _FILESYS_H
#define _FILESYS_H

#include "types.h" 
#include "lib.h" 

typedef struct
{
	
} inode_t;

typedef struct
{
	uint8_t file_name[33];
	uint32_t file_type;
	uint32_t inode_num;
} dentry_t;

typedef struct
{
	uint32_t num_dir;
	uint32_t num_inodes;
	uint32_t num_dblocks;
} boot_t;

extern void filesys_init();
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

#endif
