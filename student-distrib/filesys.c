/* Written by: Nikita
 * Debugged by: Nikita */

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
	boot_block = (boot_block_t*) FILESYS_ADDR;
	uint32_t* fa = (uint32_t*)FILESYS_ADDR;
	
	if(fa == NULL)
		return;
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
	int32_t retval = 0;
	file_descriptor_struct file = current_pcb->file_array[fd];

	uint32_t inode = file.inode_num;
	uint32_t offset = file.file_pos;

	retval = read_data(inode, offset, (uint8_t*) buf, nbytes);

	current_pcb->file_array[fd].file_pos+=retval;
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

/* file_open
 *   DESCRIPTION: helper function for opening the filesystem
 *   INPUTS: filename -- file to be opened
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t file_open(const uint8_t* filename)
{
	dentry_t dentry;
	read_dentry_by_name(filename, &dentry);
	return 0;
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
	// using boot block index , fill buf with only one filename
	dentry_t dentry = {{0}};
	uint32_t index;
	// boot_block_index = find_dentry_by_fd((uint32_t)fd);
	index = current_pcb->file_array[fd].file_pos;
	int32_t retval = read_dentry_by_index(index, &dentry);
	current_pcb->file_array[fd].file_pos++;
	if(retval == -1)
		return 0;
	else
	{
		uint32_t file_name_size;
		file_name_size = strlen((int8_t*)dentry.file_name);
		if(file_name_size > DENTRY_FILE_NAME_SIZE)
			file_name_size = DENTRY_FILE_NAME_SIZE;
		strncpy((int8_t*)buf, (int8_t*)dentry.file_name, file_name_size);
		return file_name_size;
	}
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

/* dir_open
 *   DESCRIPTION: helper function for opening the directory
 *   INPUTS: filename -- file to be opened
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t dir_open(const uint8_t* filename)
{
	dentry_t dentry;
	read_dentry_by_name(filename, &dentry);
	return 0;
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
 *   INPUTS: fname -- filename
 *           dentry -- the directory entry we found
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry)
{
	uint32_t index, num_dentries, retval;
	retval = -1;
	num_dentries = boot_block->num_dir;
	
	if(strlen((int8_t*)fname) > DENTRY_FILE_NAME_SIZE)
		return retval;

	if(dentry != NULL)
	{
		for(index = 0; index < num_dentries; index++)
		{
			if(strncmp((int8_t*)fname, (int8_t*)boot_block->dentries[index].file_name, DENTRY_FILE_NAME_SIZE-1) == 0)
			{
				strncpy((int8_t*)dentry->file_name, (int8_t*)boot_block->dentries[index].file_name, DENTRY_FILE_NAME_SIZE);
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
 *   INPUTS: index -- the boot block index of the directory entry to be found
 *           dentry -- the directory entry we found
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success, -1 for failure
 *   SIDE EFFECTS: 
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
{
	uint32_t num_dentries;
	num_dentries = boot_block->num_dir;
	if(index < 0 || index > (num_dentries-1) || dentry == NULL || dentry == 0)
		return -1;

	strncpy((int8_t*)dentry->file_name, (int8_t*)boot_block->dentries[index].file_name, DENTRY_FILE_NAME_SIZE);
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
	uint32_t orig_length;
	orig_length = length;

	// check for valid inode
	if(inode<0 || inode>=(boot_block->num_inodes))
		return -1;

	// find pointer to inode
	inode_t* inode_ptr;
	uint8_t* fa = (uint8_t*)FILESYS_ADDR;
	inode_ptr = (inode_t*) (fa + ((inode+1)*BLOCK_SIZE));

	// check for valid offset
	if(offset<0 || offset>(inode_ptr->length))
		return -1;

	// check for valid length
	if(length < 0)
		return -1;

	if((offset+length) >= (inode_ptr->length))
		length = (inode_ptr->length) - offset;

	// loop through data blocks
	uint32_t data_block, data_block_num, block_offset, read_from_block, length_left, bytes_read_successfully;
	uint8_t* data_block_ptr, * buf_ptr;
	data_block = offset / BLOCK_SIZE;
	block_offset = offset % BLOCK_SIZE;

	length_left = length;
	buf_ptr = buf;
	bytes_read_successfully = 0;
	while(length_left > 0)
	{
		// determine how much to read from the current block
		read_from_block = ((block_offset + length_left) > BLOCK_SIZE)? (BLOCK_SIZE - block_offset) : length_left;

		// calculating the pointer to the correct data block to read from
		data_block_num = inode_ptr->data_blocks[data_block];
		data_block_ptr = (uint8_t*) (fa + ((boot_block->num_inodes+1)*BLOCK_SIZE) + (data_block_num*BLOCK_SIZE));
		
		// copying data from the filesystem to the buffer
		if(data_block_ptr != NULL)
		{
			memcpy(buf_ptr, (data_block_ptr + block_offset), read_from_block);
			bytes_read_successfully += read_from_block;
		}
		else
		{
			// if data_block_ptr is NULL then we return -1 if we haven't read anything;
			// if we have read some number of bytes successfully before reaching a NULL data_block_ptr, 
			// then we return that number of bytes
			bytes_read_successfully = (bytes_read_successfully==0)? -1 : bytes_read_successfully;
			return bytes_read_successfully;
		}
			
		buf_ptr += read_from_block;
		length_left = length_left - read_from_block;
		block_offset = 0;
		if(length_left > 0){
			data_block++;
		}
	}

	if(bytes_read_successfully < orig_length)
		buf[bytes_read_successfully] = '\0';
	return bytes_read_successfully;
}

/* Helper functions */
uint32_t find_dentry_by_fd(uint32_t fd)
{
	file_descriptor_struct file = current_pcb->file_array[fd];
	uint32_t inode_index = file.inode_num;
	
	int32_t retval;
	uint32_t dentry_index = 0;
	dentry_t dentry;
	dentry.inode_num = -1;
	
	while(dentry.inode_num != inode_index)
	{
		retval = read_dentry_by_index(dentry_index, &dentry);
		dentry_index++;

		if((dentry_index >= NUM_DENTRIES) || (retval == -1))
			return -1;
	}

	return dentry_index;
}
