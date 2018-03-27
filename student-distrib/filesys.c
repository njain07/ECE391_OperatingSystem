#include "filesys.h"

extern void filesys_init()
{

}

int32_t filesys_open(const uint8_t* filename)
{
	return 0;
}

int32_t filesys_read(int32_t fd, void* buf, int32_t nbytes)
{
	return 0;
}

int32_t filesys_write(int32_t fd, const void* buf, int32_t nbytes)
{
	return 0;
}

int32_t filesys_close(int32_t fd)
{
	return 0;
}

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry)
{
	return 0;
}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
{
	return 0;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
	return 0;
}
