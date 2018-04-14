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
	uint32_t* fa = (uint32_t*)FILESYS_ADDR;

	boot_block->num_dir = *(fa);
	boot_block->num_inodes = *(fa + FOUR_B);
	boot_block->num_dblocks = *(fa + EIGHT_B);
	boot_block->reserved = (uint32_t*)(fa + TWELVE_B);
	boot_block->dentries = (dentry_t*)(fa + SF_B);
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
	dentry_t* dentry;
	read_dentry_by_name(filename, dentry);
	return 0;
}

/* file_read
 *   DESCRIPTION: helper function for reading the filesystem
 *   INPUTS: fd -- file descriptor	
 *           buf -- buffer
 *           nbytes -- number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: number of bytes read
 *   SIDE EFFECTS: fills buf with what has been read 
 */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes)
{
	int32_t retval;
	file_descriptor_struct file = file_array[fd];

	uint32_t inode = file.inode;
	uint32_t offset = file.file_pos;
	uint8_t* temp_buf = (uint8_t*) buf;
	retval = read_data(inode, offset, temp_buf, nbytes);
	return retval;
}

/* file_write
 *   DESCRIPTION: helper function for writing to the filesystem
 *   INPUTS: fd -- file descriptor
 *           buf -- buffer
 * 			 nbytes -- number of bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes)
{
	return -1;
}

/* file_close
 *   DESCRIPTION: helper function for closing the filesystem
 *   INPUTS: fd -- file descriptor
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
	dentry_t* dentry;
	read_dentry_by_name(filename, dentry);
	return 0;
}

/* dir_read
 *   DESCRIPTION: helper function for reading the directory
 *   INPUTS: fd -- file descriptor
 *           buf -- buffer
 *           nbytes -- number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: fills buf with what has been read 
 */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes)
{
	dentry_t* dentry;
	int32_t inode_index = dentry->inode_num;



	// uint32_t dir_index = find_dentry_by_fd(fd);
	// retval = read_dentry_by_index(dir_index, dentry);

	// if(retval == -1)
	// 	return -1;
	// else

	return 0;
}

/* dir_write
 *   DESCRIPTION: helper function for writing to the directory
 *   INPUTS: fd -- file descriptor
 *           buf -- buffer
 * 			 nbytes -- number of bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes)
{
	return -1;
}

/* dir_closes
 *   DESCRIPTION: helper function for closing the directory
 *   INPUTS: fd -- file descriptor
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
			if(strncmp((int8_t*)fname, (int8_t*)boot_block->dentries[index].file_name, DENTRY_FILE_NAME_SIZE-1) == 0)
			{
				strncpy((int8_t*)dentry->file_name, (int8_t*)boot_block->dentries[index].file_name, DENTRY_FILE_NAME_SIZE-1);
				dentry->file_name[DENTRY_FILE_NAME_SIZE] = '\0';
				dentry->file_type = boot_block->dentries[index].file_type;
				dentry->inode_num = boot_block->dentries[index].inode_num;
				retval = 0;
				break;
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

	strncpy((int8_t*)dentry->file_name, (int8_t*)boot_block->dentries[index].file_name, DENTRY_FILE_NAME_SIZE-1);
	dentry->file_name[DENTRY_FILE_NAME_SIZE] = '\0';
	dentry->file_type = boot_block->dentries[index].file_type;
	dentry->inode_num = boot_block->dentries[index].inode_num;
	return 0;
}

/* read_data
 *   DESCRIPTION: reads data from the file corresponding to the inode
 *   INPUTS: inode -- the inode of the file to read data from
 *           offset -- number of bytes from the beginning of the filename
 * 					   which gives the location to start reading
 *           buf -- the buffer to be filled with the data being read
 *           length -- number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: number of bytes read for success, -1 for failure
 *   SIDE EFFECTS: 
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
	// check for valid inode
	if(inode<0 || inode>=(boot_block->num_inodes))
		return -1;

	// find pointer to inode
	inode_t* inode_ptr;
	uint32_t* fa = (uint32_t*)FILESYS_ADDR;
	inode_ptr = (inode_t*) *(fa + ((inode+1)*BLOCK_SIZE_ADDR));

	// check for valid offset
	if(offset<0 || offset>=(inode_ptr->length))
		return -1;

	// check for valid length
	if(length<0)
		return -1;
	if((offset+length)>(inode_ptr->length))
		length = (inode_ptr->length) - offset;

	// loop through data blocks
	uint32_t data_block, data_block_num, initial_offset, read_from_block, length_left, bytes_read_successfully;
	uint8_t* data_block_ptr, * buf_ptr;
	data_block = offset / BLOCK_SIZE;
	initial_offset = offset % BLOCK_SIZE;

	length_left = length;
	buf_ptr = buf;
	bytes_read_successfully = 0;
	while(length_left > 0)
	{
		read_from_block = (length_left > BLOCK_SIZE)? BLOCK_SIZE : length_left;
		data_block_num = inode_ptr->data_blocks[data_block];
		data_block_ptr = (uint8_t*) *(fa + ((boot_block->num_inodes)*BLOCK_SIZE_ADDR) + (data_block_num*BLOCK_SIZE_ADDR));
		if(data_block_ptr != NULL)
		{
			memcpy(buf_ptr, data_block_ptr, read_from_block);
			bytes_read_successfully += read_from_block;
		}
		else
		{
			bytes_read_successfully = (bytes_read_successfully==0)? -1 : bytes_read_successfully;
			return bytes_read_successfully;
		}
			

		buf_ptr += read_from_block;
		length_left = length_left - read_from_block;
		if(length_left > 0)
			data_block++;
	}

	return bytes_read_successfully;
}

/* Helper functions */
// uint32_t find_dentry_by_fd(uint32_t fd)
// {
// 	file_descriptor_struct file = file_array[fd];
// 	uint32_t inode_index = file.inode;
	
// 	int32_t retval;
// 	uint32_t dentry_index = 0;
// 	dentry_t* dentry;
// 	dentry->inode_num = -1;
	
// 	while(dentry->inode_num != inode_index)
// 	{
// 		retval = read_dentry_by_index(dentry_index, dentry);
// 		dentry_index++;

// 		if((dentry_index > 62) || (retval == -1))
// 			return -1;
// 	}

// 	return dentry_index;
// }
