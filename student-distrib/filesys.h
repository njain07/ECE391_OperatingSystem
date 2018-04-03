#ifndef _FILESYS_H
#define _FILESYS_H

#include "types.h" 
#include "lib.h" 
#include "pcb.h"

#define FOUR_B 1
#define EIGHT_B 2
#define TWELVE_B 3
#define SF_B 16

#define NUM_DATA_BLOCKS 1023
#define DENTRY_FILE_NAME_SIZE 33
#define DENTRY_PADDING 6
#define BOOT_BLOCK_RESERVED 13
#define NUM_DENTRIES 63

#define FILESYS_ADDR 0x0040F000
#define BLOCK_SIZE_ADDR 4096*8
#define BLOCK_SIZE 4096

typedef struct inode_t
{
	uint32_t length;
	uint32_t data_blocks[NUM_DATA_BLOCKS];
} inode_t;

typedef struct dentry_t
{
	uint8_t file_name[DENTRY_FILE_NAME_SIZE];
	uint32_t file_type;		//0 for a file giving user-level access to the RTC, 1 for directory, 2 for a regular file
	uint32_t inode_num;		//only meaningful for regular files; ignored for RTC and directory
	uint32_t padding[DENTRY_PADDING];
} dentry_t;

typedef struct boot_block_t
{
	uint32_t num_dir;
	uint32_t num_inodes;
	uint32_t num_dblocks;
	uint32_t* reserved;
	dentry_t* dentries;
} boot_block_t;

boot_block_t* boot_block;

extern void filesys_init();

int32_t file_open(const uint8_t* filename);
int32_t file_read(int32_t fd, void* buf, int32_t nbytes);
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t file_close(int32_t fd);

int32_t dir_open(const uint8_t* filename);
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes);
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t dir_close(int32_t fd);

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

#endif
