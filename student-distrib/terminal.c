/* terminal.c - Defines used in interactions with the terminals including basic file syste
 *              functions
 * vim:ts=4 noexpandtab
 */

/*
 * Editor: Hyun Do Jung, 03.19.18
 * Reference materials: OSDEV website, appendix B, MP3 document
 * Filename: terminal.c
 * History:
 *    SL    1    Thu Sep 10 22:08:20 2018
 *        First made for MP3.3
*/
 
#include "terminal.h"

/*
 * terminal_init
 *   DESCRIPTION: initialize terminals; values set
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: return 0 on success
 *   SIDE EFFECTS: none
 */
void terminal_init(void)
{
    keyboard_init();
    b_index = 0;
    read_index = 0;
    read_flag = 0;
}


/*
 * terminal_open
 *   DESCRIPTION: opens a terminal
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
int32_t terminal_open(const uint8_t* filename)
{
    return 0;
}

/*
 * terminal_close
 *   DESCRIPTION: clear all screen and the terminal.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: return 0
 *   SIDE EFFECTS: clear the screen
 */
int32_t terminal_close(int32_t fd)
{
    clear_buffer();
    clear();
    return 0;
}

/*
 * terminal_read
 *   DESCRIPTION: Reads the data from one line which wil be indicated by an ending with ENTER or
 *                the buffer gets full.
 *   INPUTS: Pointer to the buffer and bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: the nubmer of bytes read in the buffer.
 *   SIDE EFFECTS: fills the buffer with data.
 */

int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes)
{
    
    int i = 0;
    
    /* Boundary check */
    if( (nbytes < 0) || (buf == NULL) )
        return FAIL;
    
    /* NO BYTES, */
    if (nbytes == 0)
        return 0;
    
    /* spin until flag is up */
    while(enter != 1); // and while buffer_index < BUFFER SIZE

    /* Changing name with buffer for given parameter buf*/
    // for(i=0; i<BUFFER_SIZE; i++)
    // {
    //     buf[i] = buffer[i];
    //     nbytes++;
    // }
    char* buffer = (char*)buf;
    
    /* Fill buffer with data from keyboard buffer */
    for(i = 0; i < read_index; i++){
        // buffer[i] = keyboard_buffer[i];
        
        /* if enter was pressed, we stop reading the buffer, change the flag */
        if(buffer[read_index] == '\0'){
            read_flag = 0;
            break;
        }
    }
    
    read_flag = 0;
    
    return nbytes;
}

/*
 * terminal_write
 *   DESCRIPTION: Based on the buffer we read off data and writes it on the screen.
 *   INPUTS: bascially none. (just prototype)
 *   OUTPUTS: none
 *   RETURN VALUE: the nubmer of bytes written, OR -1 for failure
 *   SIDE EFFECTS: fills the buffer with data.
 */
int32_t term_write(int32_t fd, const void* buf, int32_t nbytes)
{
    int i = 0;
    
    /* Boundary check */
    if( (nbytes < 0) || (buf == NULL) )
        return FAIL;
    
    /* NO BYTES */
    if (b_index == BUFFER_SIZE)
        return 0;
    
    char* buffer = (char*)buf;
    
    /* critical section begins */
    cli();
    
    /* implementing putc */
    for(i = 0; i < nbytes; i++){
        if(buffer[i])
            putc((char) buffer[i]);
    }
    
    /* critical section ends */
    sti();
    
    return nbytes;
}

