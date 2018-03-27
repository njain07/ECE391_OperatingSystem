#include "filesys.h"

/* filesys_init
 *   DESCRIPTION: initializes the filesystem
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
extern void filesys_init()
{
	// boot_block->num_dir = FILESYS_ADDR;
	// boot_block->num_inodes = FILESYS_ADDR + FOUR_B;
	// boot_block->num_dblocks = FILESYS_ADDR + EIGHT_B;
	// boot_block->reserved = FILESYS_ADDR + TWELVE_B;
	// boot_block->dentries = FILESYS_ADDR + SF_B;
}

/* filesys_open
 *   DESCRIPTION: helper function for opening the filesystem
 *   INPUTS: filename -- file to be opened
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t filesys_open(const uint8_t* filename)
{
	return 0;
}

/* filesys_read
 *   DESCRIPTION: helper function for reading the filesystem
 *   INPUTS: fd -- 
 *           buf -- buffer
 *           nbytes -- number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: fills buf with what has been read 
 */
int32_t filesys_read(int32_t fd, void* buf, int32_t nbytes)
{
	return 0;
}

/* filesys_write
 *   DESCRIPTION: helper function for writing to the filesystem
 *   INPUTS: fd -- 
 *           buf -- buffer
 * 			 nbytes -- number of bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t filesys_write(int32_t fd, const void* buf, int32_t nbytes)
{
	return 0;
}

/* filesys_close
 *   DESCRIPTION: helper function for closing the filesystem
 *   INPUTS: fd -- 
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t filesys_close(int32_t fd)
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
	// uint32_t index, num_dentries, retval;
	// retval = -1;
	// num_dentries = boot_block->num_dir;

	// for(index = 0; index < num_dentries; index++)
	// {
	// 	if(strncmp(fname, boot_block->dentries[index]->file_name, DENTRY_FILE_NAME_SIZE-1) == 0)
	// 	{
	// 		strncpy(dentry->file_name, boot_block->dentries[index]->file_name, DENTRY_FILE_NAME_SIZE-1);
	// 		dentry->file_name[DENTRY_FILE_NAME_SIZE] = '\0';
	// 		dentry->file_type = boot_block->dentries[index]->file_type;
	// 		dentry->inode_num = boot_block->dentries[index]->inode_num;
	// 		retval = 0;
	// 	}
	// }

	// return retval;
	return -1;
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
	// uint32_t num_dentries;
	// num_dentries = boot_block->num_dir;
	// if(index < 0 || index > (num_dentries-1) || dentry == NULL)
	// 	return -1;

	// strncpy(dentry->file_name, boot_block->dentries[index]->file_name, DENTRY_FILE_NAME_SIZE-1);
	// dentry->file_name[DENTRY_FILE_NAME_SIZE] = '\0';
	// dentry->file_type = boot_block->dentries[index]->file_type;
	// dentry->inode_num = boot_block->dentries[index]->inode_num;
	return 0;
}

/* int32_t rtc_init(const uint8_t* filename)
 *   DESCRIPTION: helper function for opening the RTC
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
