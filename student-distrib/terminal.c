#include "terminal.h"

terminal_t terminal1 = {0, {-1, -1, -1, -1, -1, -1}, 0, 0};
terminal_t terminal2 = {0, {-1, -1, -1, -1, -1, -1}, 0, 0};
terminal_t terminal3 = {0, {-1, -1, -1, -1, -1, -1}, 0, 0};

// terminal_t terminal_array[] = {terminal1, terminal2, terminal3};

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
    terminal_array[0] = terminal1;
    terminal_array[1] = terminal2;
    terminal_array[2] = terminal3;
}

/*
 * terminal_read
 *   DESCRIPTION: Reads the data from one line which wil be indicated by an ending with ENTER or
 *                the buffer gets full.
 *   INPUTS: Pointer to the buffer and bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: the number of bytes read in the buffer.
 *   SIDE EFFECTS: fills the buffer with data.
 */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes)
{
    int i = 0;

    /* spin until flag is up */
    while(enter != 1);

    char* temp_buf = (char*) buf;

    /* Changing name with buffer for given parameter buf*/
    int buffer_index;
    char buffer[BUFFER_SIZE];

    if(terminal_num == 1)
    {
        buffer_index = buffer_index1;
        strcpy(buffer, buffer1);
    }
    else if(terminal_num == 2)
    {
        buffer_index = buffer_index2;
        strcpy(buffer, buffer2);
    }
    else if(terminal_num == 3)
    {
        buffer_index = buffer_index3;
        strcpy(buffer, buffer3);
    }

    for(i=0; i < buffer_index; i++)
    {
        temp_buf[i] = buffer[i];
    }
    temp_buf[i] = '\n';
    i++;
    clear_buffer();
    enter = 0;
    
    return i;

}

/*
 * terminal_write
 *   DESCRIPTION: Based on the buffer we read off data and writes it on the screen.
 *   INPUTS: bascially none. (just prototype)
 *   OUTPUTS: none
 *   RETURN VALUE: the number of bytes written, OR -1 for failure
 *   SIDE EFFECTS: fills the buffer with data.
 */
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes)
{
    int i = 0;
    
    char* temp_buf = (char*) buf;

    /* critical section begins */
    cli();
    
    /* implementing putc */
    for(i=0; i < nbytes; i++)
    {
        if(temp_buf[i] != '\0')
            putc((char) temp_buf[i]);
    }
    
    /* critical section ends */
    sti();
    
    return nbytes;
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
