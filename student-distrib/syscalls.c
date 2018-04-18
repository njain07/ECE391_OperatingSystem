#include "syscalls.h"

#define    SUCESS              0
#define    FAIL               -1
#define    FD_MAX_INDEX        7


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
int32_t open(const uint8_t* filename) {
    
    /* local variables for using as an index of FD array and dentry */
    int fd_index;
    dentry_t temp_dentry;
    
    
    /* PCB initialization */
    pcb_t* current_pcb = (pcb_t*)
    
    
    /* Sanity Check#1; filename check */
    if( (filename == NULL) || (*filename == NULL) ) return FAIL;
    
    
    /* Sanity Check#2; read the file from filesys */
    if( read_dentry_by_name( (uint8_t *) filename, &temp_dentry) ) return FAIL;
    
    
    /* Sanity Check#3-1; check for first file descriptor not in use */
    /* file descriptor should start from index 2 because of stdin and stdout */
    for (fd_index = 2; fd_index <= FD_MAX_INDEX; fd_index++){
        
        /* check all array datas, if it is in use, break out */
        if ( !current_pcb->file_descriptor_array[fd_index].flag )
            break;
    }
    
    /* Sanity Check#3-2; if every file descriptors are filled, we cant use; Sanity Check Done */
    if (file_descriptor > FD_MAX_INDEX) return FAIL;
    
    
    
    /* let file position and the flag of FD array to be set */
    current_pcb->file_descriptor_array[fd_index].file_position = 0;
    current_pcb->file_descriptor_array[fd_index].flag = 1;
    
    /* File Type Check */
    /* 0:RTC, 1:Directory, 2:Regular file(we only care inode here) */
    if (temp_dentry.file_type == 2){
        curr_pcb->file_descriptor_array[fd_index].inode = temp_dentry.inode_num;
        curr_pcb->file_descriptor_array[fd_index].file_operation_table_pointer = &file_op; //may need help
    }
    
    else{
        
        /* we dont care inodes anymore */
        current_pcb->file_descriptor_array[fd_index].inode = NULL;
        
        /* directory file */
        if (temp_dentry.file_type == 1)
            current_pcb->file_descriptor_array[fd_index].file_operation_table_pointer = &directory_op; //may need help
        
        /* rtc file */
        else
            current_pcb->file_descriptor_array[fd_index].file_operation_table_pointer = &rtc_op; //may need help, where can i help those operation?
    }
    
    (*current_pcb->file_descriptor_array[fd_index].file_operation_table_pointer->open)(); // may need help is it open or file_open?
    
    return fd_index;
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
 *      int8_t* fd - just like open's fd_index; index of file descriptor array
 *  OUTPUTS:
 *      none
 *  RETURN VALUE:
 *      success: 0   /   fail: -1
 */

int32_t close(int32_t fd){
    
    // need help on this init
    pcb_t* current_pcb = (pcb_t*)
    
    /* Sanity Check#1 index boundary */
    if(fd > FD_MAX_INDEX || fd < 2) return FAIL;
    
    /* check in-use flag */
    if (!current_pcb->file_descriptor_array[fd].flag) return FAIL;

    /* closing file */
    (*current_pcb->file_descriptor_array[fd].file_operation_table_pointer->close)();
    
    /* clearing flag */
    current_pcb->file_descriptor_array[fd].flag = 0;
    
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

