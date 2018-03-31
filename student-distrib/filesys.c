#include "filesys.h"

/*
 * Author: Nikita Jain and Vismayak Mohanarajan
 * Reference materials: MP3 Documentation
 * Filename: filesys.c
 * History:
 *    SL    1    Sun Mar 26 2018
 *        Wrote read_dentry_by_name and read_dentry_by_index
 *    SL    2    Thu Mar 29 2018
 *        Wrote filesys_init
 */


/* filesys_init
 *   DESCRIPTION: initializes the filesystem
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
extern void filesys_init()
{
	// boot_block = FILESYS_ADDR;

	boot_block->num_dir = *(FILESYS_ADDR);
	boot_block->num_inodes = *(FILESYS_ADDR + FOUR_B);
	boot_block->num_dblocks = *(FILESYS_ADDR + EIGHT_B);
	boot_block->reserved = *(FILESYS_ADDR + TWELVE_B);
	boot_block->dentries = *(FILESYS_ADDR + SF_B);
}

/* file_open
 *   DESCRIPTION: helper function for opening the filesystem
 *   INPUTS: filename -- file to be opened
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t file_open(const uint8_t* filename)
{
	read_dentry_by_name(filename)
	return 0;
}

/* file_read
 *   DESCRIPTION: helper function for reading the filesystem
 *   INPUTS: fd -- 
 *           buf -- buffer
 *           nbytes -- number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: fills buf with what has been read 
 */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes)
{
	//need to write
	return 0;
}

/* file_write
 *   DESCRIPTION: helper function for writing to the filesystem
 *   INPUTS: fd -- 
 *           buf -- buffer
 * 			 nbytes -- number of bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes)
{
	// return 0;
	return -1;
}

/* file_close
 *   DESCRIPTION: helper function for closing the filesystem
 *   INPUTS: fd -- 
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t file_close(int32_t fd)
{
	return 0;
}

/* dir_open
 *   DESCRIPTION: helper function for opening the directory
 *   INPUTS: filename -- file to be opened
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t dir_open(const uint8_t* filename)
{
	return 0;
}

/* dir_read
 *   DESCRIPTION: helper function for reading the directory
 *   INPUTS: fd -- 
 *           buf -- buffer
 *           nbytes -- number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: fills buf with what has been read 
 */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes)
{
	//need to write
	return 0;
}

/* dir_write
 *   DESCRIPTION: helper function for writing to the directory
 *   INPUTS: fd -- 
 *           buf -- buffer
 * 			 nbytes -- number of bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes)
{
	// return 0;
	return -1;
}

/* dir_closes
 *   DESCRIPTION: helper function for closing the directory
 *   INPUTS: fd -- 
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t dir_close(int32_t fd)
{
	return 0;
}

/* read_dentry_by_name
 *   DESCRIPTION: finds the corresponding dentry given the name
 *   INPUTS: fname -- 
 *           dentry --
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry)
{
	uint32_t index, num_dentries, retval;
	retval = -1;
	num_dentries = boot_block->num_dir;

	if(dentry != NULL)
	{
		for(index = 0; index < num_dentries; index++)
		{
			if(strncmp(fname, boot_block->dentries[index]->file_name, DENTRY_FILE_NAME_SIZE-1) == 0)
			{
				strncpy(dentry->file_name, boot_block->dentries[index]->file_name, DENTRY_FILE_NAME_SIZE-1);
				dentry->file_name[DENTRY_FILE_NAME_SIZE] = '\0';
				dentry->file_type = boot_block->dentries[index]->file_type;
				dentry->inode_num = boot_block->dentries[index]->inode_num;
				retval = 0;
			}
		}
	}

	return retval;
}

/* read_dentry_by_index
 *   DESCRIPTION: finds the corresponding dentry given the index
 *   INPUTS: index -- the index of the directory entry to be found
 *           dentry -- the directory entry we found
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success, -1 for failure
 *   SIDE EFFECTS: 
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
{
	uint32_t num_dentries;
	num_dentries = boot_block->num_dir;
	if(index < 0 || index > (num_dentries-1) || dentry == NULL)
		return -1;

	strncpy(dentry->file_name, boot_block->dentries[index]->file_name, DENTRY_FILE_NAME_SIZE-1);
	dentry->file_name[DENTRY_FILE_NAME_SIZE] = '\0';
	dentry->file_type = boot_block->dentries[index]->file_type;
	dentry->inode_num = boot_block->dentries[index]->inode_num;
	return 0;
}

/* read_data
 *   DESCRIPTION: 
 *   INPUTS: inode -- 
 *           offset -- 
 *           buf -- 
 *           length -- 
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
	return 0;
}
