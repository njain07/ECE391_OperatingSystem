#include "keyboard.h"

#define KEYBOARD_SIZE 59

int32_t terminal_num;

/* character arrays */
/* Scan Code Set 1 from OSDEV */

/* 0, esc, backspace
 * tab
 * enter, left ctrl
 * left shift
 * right shift, keypad*, left alt, caps
 */
const uint8_t lowercase[KEYBOARD_SIZE] = 
    {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
    0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 
    '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
    0, 0, 0, ' ', 0};

const uint8_t shift_case[KEYBOARD_SIZE] = 
    {0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 
    0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', 
    '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 
    0, 0, 0, ' ', 0};

const uint8_t caps_case[KEYBOARD_SIZE] = 
    {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 
    0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 
    '`', 0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 
    0, 0, 0, ' ', 0};

const uint8_t shift_caps_case[KEYBOARD_SIZE] = 
    {0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', 
    0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '\"', 
    '~', 0, '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 
    0, 0, 0, ' ', 0};

/* Simple init helper for keyboard initialization */
void keyboard_init(void) {
    enable_irq(KEYBOARD_IRQ);
    buffer_index1 = 0;
    buffer_index2 = 0;
    buffer_index3 = 0;
    flags.shift = 0;
    flags.caps = 0;
    flags.ctrl = 0;
    flags.alt = 0;
    enter = 0;
}

/*
 * get_character
 *   DESCRIPTION: read the actual character and return it
 *   INPUTS: scancode
 *   OUTPUTS: none
 *   RETURN VALUE: returns actual character being pressed
 *   SIDE EFFECTS: none
 */
char get_character(uint8_t scancode){
    // handles flags
    switch(scancode)
    {
        case 0x2A:  
            // left shift pressed
            flags.shift = 1;
            // printf("left shift pressed | ");   
            break;

        case 0x36:
            // right shift pressed 
            flags.shift = 1;
            // printf("right shift pressed | ");
            break;

        case 0xAA:
            // left shift released
            flags.shift = 0;
            // printf("left shift released | ");
            break;

        case 0xB6:
            // right shift released
            flags.shift = 0;
            // printf("right shift released | ");
            break;

        case 0x3A:
            // caps
            flags.caps = !(flags.caps);
            // printf("ALL CAPS | ");
            break;

        case 0x1D:
            // left or right ctrl pressed
            flags.ctrl = 1;
            // printf("ctrl pressed | ");
            break;

        case 0x9D:
            // left or right ctrl released
            flags.ctrl = 0;
            // printf("ctrl released | ");
            break;

        case 0x38:
            // left or right alt pressed
            flags.alt = 1;
            // printf("alt pressed | ");
            break;

        case 0xB8:
            // left or right alt released
            flags.alt = 0;
            // printf("alt released | ");
            break;
    }

    // backspace
    if(scancode == 0x0E)
    {
        if(terminal_num == 1)
        {
            if(buffer_index1>0)
            {
                buffer1[buffer_index1-1] = KEY_NULL;
                buffer_index1--;
                backspace();
            }
        }
        if(terminal_num == 2)
        {
            if(buffer_index2>0)
            {
                buffer2[buffer_index2-1] = KEY_NULL;
                buffer_index2--;
                backspace();
            }
        }
        if(terminal_num == 3)
        {
            if(buffer_index3>0)
            {
                buffer3[buffer_index3-1] = KEY_NULL;
                buffer_index3--;
                backspace();
            }
        }
    }

    char return_char = 0;

    // enter
    if(scancode == 0x1C)
    {
        enter = 1;
        enter_func();
        return_char = '\n';
    }

    if(flags.ctrl == 1)
    {
        // check for ctrl-l
        if(scancode == 0x26 || scancode == 0xA6)
        {
            clear();
            return 0;
        }
    }

    if(flags.alt == 1)
    {
        // check for alt+F1 for terminal change
        if(scancode == 0x3B)
        {
            terminal_switch(1);
        }
        // check for alt+F2 for terminal change
        if(scancode == 0x3C)
        {
            terminal_switch(2);     // switch to terminal 2
        }
        // check for alt+F3 for terminal change
        if(scancode == 0x3D)
        {
            terminal_switch(3);     // switch to terminal 3
        }
    }

    /* return corresponding character */
    
    if(scancode != 0x0E)
    {
        if(flags.shift == 0 && flags.caps == 0)
        {
            if(scancode < 0x3B)
                return_char = lowercase[scancode];
        }

        else if(flags.shift == 1 && flags.caps == 0)
        {
            if(scancode < 0x3B)
                return_char = shift_case[scancode];
        }

        else if(flags.shift == 0 && flags.caps == 1)
        {
            if(scancode < 0x3B)
                return_char = caps_case[scancode];
        }

        else if(flags.shift == 1 && flags.caps == 1)
        {
            if(scancode < 0x3B)
                return_char = shift_caps_case[scancode];
        }
    }

    return return_char;

}

/*
 * keyboard_interrupt_handler
 *   DESCRIPTION: initializes the keyboard port
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void keyboard_interrupt_handler(void){
    
    uint8_t scancode = 0;
    char print_char = 0;
    
    /* critical section started */
    cli();
    send_eoi(KEYBOARD_IRQ);

    if(inb(KEYBOARD_STATUS_PORT))
    {
        scancode = inb(KEYBOARD_PORT);
        /* interpret the character */
        print_char = get_character(scancode);
        
        /* putting into keyboard buffer 1*/
        if(terminal_num == 1)
        {
            if((buffer_index1 < BUFFER_SIZE-1) && (print_char != 0))
            {
                buffer1[buffer_index1] = print_char;
                buffer_index1++;
                /* echo to the screen */
                putc(print_char);
            }
        }

        /* putting into keyboard buffer 2*/
        if(terminal_num == 2)
        {
            if((buffer_index2 < BUFFER_SIZE-1) && (print_char != 0))
            {
                buffer2[buffer_index2] = print_char;
                buffer_index2++;
                /* echo to the screen */
                putc(print_char);
            }
        }

        /* putting into keyboard buffer 3*/
        if(terminal_num == 3)
        {
            if((buffer_index3 < BUFFER_SIZE-1) && (print_char != 0))
            {
                buffer3[buffer_index3] = print_char;
                buffer_index3++;
                /* echo to the screen */
                putc(print_char);
            }
        }
    }

    /* critical section ended */
    sti();
}


/*
 * clear_buffer
 *   DESCRIPTION: clears the terminal buffer
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void clear_buffer(void){
    int i;

    for(i=0; i < BUFFER_SIZE; i++)
    {
        if(terminal_num == 1)
            buffer1[i] = KEY_NULL;
        if(terminal_num == 2)
            buffer2[i] = KEY_NULL;
        if(terminal_num == 3)
            buffer3[i] = KEY_NULL;
    }

    if(terminal_num == 1)
        buffer_index1 = 0;
    if(terminal_num == 2)
        buffer_index2 = 0;
    if(terminal_num == 3)
        buffer_index3 = 0;
}
