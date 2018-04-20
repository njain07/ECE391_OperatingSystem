#include "syscalls.h"

#define SUCCESS   0
#define FAIL      -1

#define DEL 0x7F
#define E   0x45
#define L   0x4C
#define F   0x46
#define ELF_BYTES   4

int32_t process_num = -1;
// current_pcb = NULL;

<<<<<<< HEAD
uint8_t

int32_t halt(uint8_t* status)
=======
int32_t halt(uint8_t status)
>>>>>>> fabc8878fee7f51c022549e64d057f5fcc900d45
{
    if(current_pcb->parent == -1)
    {
        process_num = -1;
        execute((uint8_t*)"shell");
    }

    int32_t old_process_num = process_num;
    int32_t esp_value = current_pcb->p_esp;
    int32_t ebp_value = current_pcb->p_ebp;

    /* STEP 1: restore parent data in PCB */
    /* STEP 2: restore parent paging */
    if(current_pcb->parent != -1)
        change_process(current_pcb->parent, 2);

    /* STEP 3: close relevant fds */
    pcb_t* parent_pcb;
    parent_pcb = (pcb_t*)(MB_8 - (KB_8*(old_process_num+1)));
    int32_t i;
    for(i=0; i<FD_SIZE; i++)
        parent_pcb->file_array[i].flags = 0;

    /* STEP 4: jump to execute return */
    asm volatile 
    (
        "movl %0, %%esp;"
        "movl %1, %%ebp;"
        "jmp halt_return;"
        : 
        : "r" (esp_value), "r" (ebp_value)
    );
    return -1;
}

int32_t execute(const uint8_t* command)
{
    /* STEP 1: parse arguments */
    uint8_t program[32];
    uint8_t arguments[96];
    uint32_t i = 0;

    while(command[i] != ' ' && command[i] != '\0')
    {
        program[i] = command[i];
        i++;
    }
    program[i] = '\0';
    i++;    // skips the ' ' between the program and arguments

    while(i<(strlen((int8_t*)command)))
    {
        arguments[i] = command[i];
        i++;
        
    }
    arguments[i] = '\0'; 

    /* STEP 2: check file validity (DEL ELF at beginning of executable files) */
    int32_t retval;
    dentry_t exec_file_dentry;
    retval = read_dentry_by_name(program, &exec_file_dentry);
    if(retval == -1) {
        // printf("fail 1\n");
        return FAIL;
    }

    uint8_t elf_buf[ELF_BYTES];
    retval = read_data(exec_file_dentry.inode_num, 0, elf_buf, ELF_BYTES);
    if(retval != ELF_BYTES) {
        // printf("fail 2\n");
        return FAIL;
    }

    if((elf_buf[0]!=DEL) || (elf_buf[1]!=E) || (elf_buf[2]!=L) || (elf_buf[3]!=F)) {
        // printf("fail 3\n");
        return FAIL;
    }

    /* STEP 3: set up paging */ 
    /* STEP 5: create PCB */
    /* STEP 6: prepare for context switch */
    change_process((process_num+1), 1);

    /* STEP 4: load file into memory */
    // uint8_t* buf = (uint8_t*)(MB_8 + (MB_4*process_num));
    uint8_t* buf = (uint8_t*)0x08048000;
    uint8_t* fa = (uint8_t*)FILESYS_ADDR;
    inode_t* inode_ptr = (inode_t*) (fa + ((exec_file_dentry.inode_num+1)*BLOCK_SIZE_ADDR));
    read_data(exec_file_dentry.inode_num, 0, buf, inode_ptr->length);

    /* STEP 5: open fd's */
    current_pcb->file_array[0].file_jmp_tbl = &terminal_ops;
    current_pcb->file_array[1].file_jmp_tbl = &terminal_ops;
    
    current_pcb->file_array[0].flags = 1;
    current_pcb->file_array[1].flags = 1;

    for(i=2; i<FD_SIZE; i++)
        current_pcb->file_array[i].flags = 0;

    // save esp and ebp inside current_pcb
    asm volatile 
    (
        "movl %%esp, %0;"
        "movl %%ebp, %1;" 
        : "=g" (current_pcb->p_esp), "=g" (current_pcb->p_ebp)
    );

    /* STEP 7: fake IRET */
    //do memcpy for bytes 24-27; little-endian format (shift manually)
    // uint8_t* eip_value, eip_value24, eip_value25, eip_value26, eip_value27;
    // memcpy((uint8_t*)eip_value24, (uint8_t*)buf[24-1], 1);
    // memcpy((uint8_t*)eip_value25, (uint8_t*)buf[25-1], 1);
    // memcpy((uint8_t*)eip_value26, (uint8_t*)buf[26-1], 1);
    // memcpy((uint8_t*)eip_value27, (uint8_t*)buf[27-1], 1);
    // eip_value[0] = eip_value27;
    // eip_value[1] = eip_value26;
    // eip_value[2] = eip_value25;
    // eip_value[3] = eip_value24;
    // int32_t esp_value = MB_8 + (MB_4*(process_num+1)) - 4;
    uint32_t* entry = (uint32_t*) (buf + 24);
    int32_t esp_value = (int32_t) (MB_8*16) + MB_4;

    asm volatile 
    (
        "pushl %3;"
        "pushl %2;"
        "pushfl;"
        "pushl %1;"
        "pushl %0;"
        "iret;"
        "halt_return:"
        :
        : "r" (/*eip_value*/ *entry), "r" (USER_CS), "r" (esp_value), "r" (USER_DS)
    );

	return 0;
}

int32_t read(int32_t fd, void* buf, int32_t nbytes)
{
    // keep track of file_pos in fd_array !!! NOT IMPLEMENTED YET

    /* fd cannot be 1 because we cannot read from stdout */
    if((fd < 0) || (fd == 1) || (fd > FD_MAX_INDEX))
        return FAIL;

    if(buf == NULL)
        return FAIL;

    /* cannot read from a file which is not in use */
    if(current_pcb->file_array[fd].flags == 0)
        return FAIL;

    int32_t bytes_read = (*current_pcb->file_array[fd].file_jmp_tbl->read)(fd, buf, nbytes);
	return bytes_read;
}

int32_t write(int32_t fd, const void* buf, int32_t nbytes)
{
    /* fd cannot be 0 because we cannot write to stdin */
    if((fd < 1) || (fd > FD_MAX_INDEX))
        return FAIL;

    /* in case of the RTC, the syscall should always accept only a 4-byte integer specifying the interrupt rate in Hz */
    // not sure if this check is necessary since it's already in rtc_write
    if(current_pcb->file_array[fd].file_jmp_tbl == &rtc_ops)
    {
        if(sizeof(nbytes) != sizeof(int32_t))
            return FAIL;
    }

    /* writes to regular files should always return -1 to indicate failure since the filesystem is read-only */
    if(current_pcb->file_array[fd].file_jmp_tbl == &file_ops)
        return FAIL;

    /* cannot write to a file which is not in use */
    if(current_pcb->file_array[fd].flags == 0)
        return FAIL;

    int32_t bytes_written = (*current_pcb->file_array[fd].file_jmp_tbl->write)(fd, buf, nbytes);
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
        if(current_pcb->file_array[fd].flags == 0)
            break;
    }
    
    /* Sanity Check #3-2: if all file descriptors are in use, we can't open any more files */
    if(fd > FD_MAX_INDEX) 
        return FAIL;
    
    /* let file position and the flag of FD array to be set */
    current_pcb->file_array[fd].file_pos = 0;
    current_pcb->file_array[fd].flags = 1;
    

    /* File Type Check */
    /* 0: RTC, 1: Directory, 2: Regular file (we only care about the inode_num here) */
    if(dentry.file_type == 2)
    {
        current_pcb->file_array[fd].inode_num = dentry.inode_num;
        current_pcb->file_array[fd].file_jmp_tbl = &file_ops;
    }
    
    else
    {
        /* we dont care about inodes anymore */
        current_pcb->file_array[fd].inode_num = NULL;
        
        /* directory file */
        if(dentry.file_type == 1)
            current_pcb->file_array[fd].file_jmp_tbl = &dir_ops;
        
        /* rtc file */
        else
            current_pcb->file_array[fd].file_jmp_tbl = &rtc_ops;
    }
    
    (*current_pcb->file_array[fd].file_jmp_tbl->open)(filename);
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
    /* Sanity Check #1 index boundary */
    if(fd > FD_MAX_INDEX || fd < 2) 
    	return FAIL;
    
    /* check in-use flag */
    if (current_pcb->file_array[fd].flags == 0) 
    	return FAIL;

    /* closing file */
    (*current_pcb->file_array[fd].file_jmp_tbl->close)(fd);
    
    /* setting the file to not in-use */
    current_pcb->file_array[fd].flags = 0;

    return SUCCESS;
}



int32_t getargs(uint8_t* buf, int32_t nbytes)
{
    /* parse individual arguments ??? */

	return 0;
}

int32_t vidmap(uint8_t** screen_start)
{
	return 0;
}

int32_t set_handler(int32_t signum, void* handler_address)
{
	return -1;
}

int32_t sigreturn(void)
{
	return -1;
}

/* Helper Function */
void change_process(int32_t new_process_num, int32_t execute_halt_switch)
{
    cli();

    current_pcb = (pcb_t*)(MB_8 - (KB_8*(new_process_num+1)));
    switch(execute_halt_switch)
    {
        case 1:
            current_pcb->parent = process_num;
            break;
        case 2:
            current_pcb->parent = -1;
            break;
        case 3:
            break;
    }
    current_pcb->pid = new_process_num;
    process_num = new_process_num;
    // printf("process_num: %d\n", process_num);
    process_page(process_num);
    tss.esp0 = MB_8 - (KB_8*process_num) - 4;

    sti();
}
