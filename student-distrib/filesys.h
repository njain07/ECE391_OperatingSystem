#ifndef _FILESYS_H
#define _FILESYS_H

#include "types.h" 
#include "lib.h" 
#include "pcb.h"

#define FOUR_B 4
#define EIGHT_B 8
#define TWELVE_B 12
#define SF_B 64

#define DENTRY_FILE_NAME_SIZE 33
#define BOOT_BLOCK_RESERVED 13
#define NUM_DENTRIES 63

// typedef struct inode_t
// {

// } inode_t;

typedef struct dentry_t
{
	uint8_t file_name[DENTRY_FILE_NAME_SIZE];
	uint32_t file_type;		//0 for a file giving user-level access to the RTC, 1 for directory, 2 for a regular file
	uint32_t inode_num;		//only meaningful for regular files; ignored for RTC and directory
} dentry_t;

typedef struct boot_block_t
{
	uint32_t num_dir;
	uint32_t num_inodes;
	uint32_t num_dblocks;
	uint32_t reserved[BOOT_BLOCK_RESERVED];
	struct dentry_t dentries[NUM_DENTRIES];
} boot_block_t;

boot_block_t* boot_block;

extern void filesys_init();

int32_t filesys_open(const uint8_t* filename);
int32_t filesys_read(int32_t fd, void* buf, int32_t nbytes);
int32_t filesys_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t filesys_close(int32_t fd);

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

#endif
