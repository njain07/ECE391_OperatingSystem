#include "syscalls.h"

#define    SUCESS              0
#define    FAIL               -1

int32_t execute(const uint8_t* command)
{
	return 0;
}

int32_t halt(uint8_t* status)
{
	return 0;
}

/*  int32_t open(const uint8_t *filename)
 *  DESCRIPTION:
 *      open the file; actually gives access to the file system
 *  INPUTS:
 *      const uint8_t* filename: pointer to the filename
 *  OUTPUTS:
 *      none
 *  RETURN VALUE:
 *      success: 0   /   fail: -1
 */

/* okay, so IDK why it should be int32_t :( explain#1 */
int32_t open(const uint8_t* filename) 
{    
    /* local variables for using as an index of FD array and dentry */
    uint8_t fd;
    dentry_t* dentry;
    
    /* PCB initialization */
    pcb_t* current_pcb = (pcb_t*) //??
    
    /* Sanity Check #1: filename check */
    if((filename == NULL) || (*filename == NULL)) //??
    	return FAIL;
    
    /* Sanity Check #2: read the file from filesys */
    if(read_dentry_by_name((uint8_t*) filename, dentry) == FAIL) 
    	return FAIL;
    
    /* Sanity Check #3-1: check for first file descriptor not in use */
    /* file descriptor should start from index 2 because of stdin and stdout */
    for(fd=2; fd<=FD_MAX_INDEX; fd++)
    {   
        /* check all array datas, if it is not in use, break out */
        if(current_pcb->file_array[fd].flags == 0)
            break;
    }
    
    /* Sanity Check #3-2: if all file descriptors are in use, we can't open any more files */
    if(file_descriptor > FD_MAX_INDEX) 
    	return FAIL;
    
    /* let file position and the flag of FD array to be set */
    current_pcb->file_array[fd].file_pos = 0;
    current_pcb->file_array[fd].flags = 1;
    

    /* File Type Check */
    /* 0: RTC, 1: Directory, 2: Regular file(we only care about the inode_num here) */
    if(dentry->file_type == 2)
    {
        current_pcb->file_array[fd].inode_num = dentry->inode_num;
        current_pcb->file_array[fd].file_jmp_tbl = &file_ops; // not sure if initialized correctly??
    }
    
    else
    {
        /* we dont care inodes anymore */
        current_pcb->file_array[fd].inode_num = NULL; //?? not sure if we can set the inode_num to 0
        
        /* directory file */
        if(dentry->file_type == 1)
            current_pcb->file_array[fd].file_jmp_tbl = &dir_ops; // not sure if initialized correctly??
        
        /* rtc file */
        else
            current_pcb->file_array[fd].file_operation_table_pointer = &rtc_ops; // not sure if initialized correctly??

        // what about term_ops or keyboard_ops?? -- not sure if we'll need since they're not a type of file
    }
    
    (*current_pcb->file_array[fd].file_operation_table_pointer->open)(); // may need help is it open or file_open? //?? not sure if syntax is correct
    
    return fd;
}

int32_t read(int32_t fd, void* buf, int32_t nbytes)
{
	return 0;
}

int32_t write(int32_t fd, const void* buf, int32_t nbytes)
{
	return 0;
}

/*  int32_t close(int32_t fd)
 *  DESCRIPTION:
 *      close the specific file descriptor
 *  INPUTS:
 *      int8_t* fd - just like open's fd; index of file descriptor array
 *  OUTPUTS:
 *      none
 *  RETURN VALUE:
 *      success: 0   /   fail: -1
 */

int32_t close(int32_t fd){
    
    // need help on this init
    pcb_t* current_pcb = (pcb_t*) //??
    
    /* Sanity Check #1 index boundary */
    if(fd > FD_MAX_INDEX || fd < 2) 
    	return FAIL;
    
    /* check in-use flag */
    if (current_pcb->file_array[fd].flags == 0) 
    	return FAIL;

    /* closing file */
    (*current_pcb->file_array[fd].file_jmp_tbl->close)(); //?? not sure if syntax is correct
    
    /* setting the file to not in-use */
    current_pcb->file_array[fd].flags = 0;
    
    return SUCCESS;
}

int32_t getargs(uint8_t* buf, int32_t nbytes)
{
	return 0;
}

int32_t vidmap(uint8_t** screen_start)
{
	return 0;
}

int32_t set_handler(int32_t signum, void* handler_address)
{
	return 0;
}

int32_t sigreturn(void)
{
	return 0;
}

