#include "syscalls.h"

#define    SUCCESS             0
#define    FAIL               -1
#define    SIGNUM_MAX          4
#define    SIGNUM_MIN          0

int32_t halt(uint8_t* status)
{
    return 0;
}

int32_t execute(const uint8_t* command)
{
    // set stdin stdout with term_ops

	return 0;
}

int32_t read(int32_t fd, void* buf, int32_t nbytes)
{
    // keep track of file_pos in fd_array

	return 0;
}

int32_t write(int32_t fd, const void* buf, int32_t nbytes)
{
    /* fd cannot be 0 because we cannot write to stdin */
    if((fd < 1) || (fd > FD_MAX_INDEX))
        return FAIL;

    /* in case of the RTC, the syscall should always accept only a 4-byte integer specifying the interrupt rate in Hz */
    // not sure if this check is necessary since it's already in rtc_write
    if(current_pcb.file_array[fd].file_jmp_tbl == &rtc_ops)
    {
        if(sizeof(nbytes) != sizeof(int32_t))
            return FAIL;
    }

    /* writes to regular files should always return -1 to indicate failure since the filesystem is read-only */
    if(current_pcb.file_array[fd].file_jmp_tbl == &file_ops)
        return FAIL;

    /* cannot write to a file which is not in use */
    if(current_pcb.file_array[fd].flags == 0)
        return FAIL;

    int32_t bytes_written = (*current_pcb.file_array[fd].file_jmp_tbl->write)(fd, buf, nbytes);
	return bytes_written;
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

int32_t open(const uint8_t* filename) 
{    
    /* local variables for using as an index of FD array and dentry */
    uint8_t fd;
    dentry_t dentry;
    
    /* PCB initialization */
    // pcb_t* current_pcb = (pcb_t*) //??
    
    /* Sanity Check #1: filename check */
    if((filename == NULL) || (*filename == NULL)) //??
        return FAIL;
    
    /* Sanity Check #2: read the file from filesys */
    if(read_dentry_by_name((uint8_t*) filename, &dentry) == FAIL) 
        return FAIL;
    
    /* Sanity Check #3-1: check for first file descriptor not in use */
    /* file descriptor should start from index 2 because of stdin and stdout */
    for(fd=2; fd<=FD_MAX_INDEX; fd++)
    {   
        /* check all array datas, if it is not in use, break out */
        if(current_pcb.file_array[fd].flags == 0)
            break;
    }
    
    /* Sanity Check #3-2: if all file descriptors are in use, we can't open any more files */
    if(fd > FD_MAX_INDEX) 
        return FAIL;
    
    /* let file position and the flag of FD array to be set */
    current_pcb.file_array[fd].file_pos = 0;
    current_pcb.file_array[fd].flags = 1;
    

    /* File Type Check */
    /* 0: RTC, 1: Directory, 2: Regular file (we only care about the inode_num here) */
    if(dentry.file_type == 2)
    {
        current_pcb.file_array[fd].inode_num = dentry.inode_num;
        current_pcb.file_array[fd].file_jmp_tbl = &file_ops;
    }
    
    else
    {
        /* we dont care about inodes anymore */
        current_pcb.file_array[fd].inode_num = NULL;
        
        /* directory file */
        if(dentry.file_type == 1)
            current_pcb.file_array[fd].file_jmp_tbl = &dir_ops;
        
        /* rtc file */
        else
            current_pcb.file_array[fd].file_jmp_tbl = &rtc_ops;
    }
    
    (*current_pcb.file_array[fd].file_jmp_tbl->open)(filename);
    
    // printf("opened file\n");
    return fd;
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

int32_t close(int32_t fd)
{
    // need help on this init
    // pcb_t* current_pcb = (pcb_t*) //??
    
    /* Sanity Check #1 index boundary */
    if(fd > FD_MAX_INDEX || fd < 2) 
    	return FAIL;
    
    /* check in-use flag */
    if (current_pcb.file_array[fd].flags == 0) 
    	return FAIL;

    /* closing file */
    (*current_pcb.file_array[fd].file_jmp_tbl->close)(fd);
    
    /* setting the file to not in-use */
    current_pcb.file_array[fd].flags = 0;
    
    // printf("closed file\n");
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


/*  int32_t set_handler(int32_t signum, void* handler_address)
 *  DESCRIPTION:
 *      set the signal handler function depending on the parameter, signum.
 *  INPUTS:
 *             int32_t signum: signal number
 *             void *handler_address: pointer to the address of handler function
 *  OUTPUS:
 *      None
 *  RETURN VALUE:
 *      success: 0   /   fail: -1
 */
int32_t set_handler(int32_t signum, void* handler_address){
    
    /* local flag variable */
    int32_t flags;
    
    /* Sanity Check#1; is signum in boundary? */
    if( (signum < SIGNUM_MIN) || (signum > SIGNUM_MAX) ) return FAIL;
    
    /* critical section starts */
    cli_and_save(flags);
    
    /* if handler_addreess is NULL, default action should be taken */
    if (handler_address == NULL)
        current_pcb->signal_acttion[signum] = default_signal_handlers[signum]; // HD
    else
        current_pcb->signal_action[signum] = handler_address;
    
    cli_and_restore(flags);
    
    return SUCCESS;
}


/*  int32_t sigreturn(void)
 *  DESCRIPTION:
 *      copy the hardware context on the user-level stack back on to the processor
 *  INPUTS:
 *      None
 *  OUTPUS:
 *      None
 *  RETURN VALUE:
 *      success: 0   /   fail: -1
 */

int32_t sigreturn(void){
    /* to find the hardware context, you will need to know the user-level value of ESP(will be saved by your syscall handler) as well as exact setup of the user-level stack frame. */
    
    /* overwrite the kernel's copy of the process's hardware context that was saved on the kernel stack */
    
    
    
    
	return SUCCESS;
}

