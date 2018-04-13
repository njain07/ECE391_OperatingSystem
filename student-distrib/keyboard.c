/* keyboard.c - Functions to interact with the keyboard devices and its interrupts
 * vim:ts=4 noexpandtab
 */

#include "keyboard.h"
#include "lib.h"

/* local variables */
char buffer[BUFFER_SIZE];
int buffer_index = 0;

/* character arrays */
/* Scan Code Set 1 from OSDEV */

/* 0, esc, backspace
 * tab
 * enter, left ctrl
 * left shift
 * right shift, keypad*, left alt, caps
 */
const uint8_t lowercase[59] = 
    {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
    0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 
    '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
    0, 0, 0, ' ', 0};

const uint8_t shift_case[59] = 
    {0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 
    0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', 
    '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 
    0, 0, 0, ' ', 0};

const uint8_t caps_case[59] = 
    {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 
    0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 
    '`', 0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 
    0, 0, 0, ' ', 0};

const uint8_t shift_caps_case[59] = 
    {0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', 
    0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '\"', 
    '~', 0, '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 
    0, 0, 0, ' ', 0};

/* Simple init helper for keyboard initialization */
void keyboard_init(void) {
    enable_irq(KEYBOARD_IRQ);
    flags.shift = 0;
    flags.caps = 0;
    flags.ctrl = 0;
    flags.alt = 0;
}

/* Read the actual character and return it */
char get_character(uint8_t scancode){
    //print scancodes to check which set to use
    //handle unknown scancodes

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
            //return 0;
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

        case 0x0E:
            // backspace pressed
            break;

        case 0x8E:
            // backspace released
            break;
    }

    if(scancode == 0x0E)
        backspace();

    if(flags.ctrl == 1)
    {
        //check for ctrl-l
        if(scancode == 0x26 || scancode == 0xA6)
        {
            clear();
            return 0;
        }
    }

    if(flags.alt == 1)
        return 0;

    if(flags.shift == 0 && flags.caps == 0)
    {
        if(scancode < 0x3B)
            return lowercase[scancode];
    }

    else if(flags.shift == 1 && flags.caps == 0)
    {
        if(scancode < 0x3B)
            return shift_case[scancode];
    }

    else if(flags.shift == 0 && flags.caps == 1)
    {
        if(scancode < 0x3B)
            return caps_case[scancode];
    }

    else if(flags.shift == 1 && flags.caps == 1)
    {
        if(scancode < 0x3B)
            return shift_caps_case[scancode];
    }

    return 0;

}

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
        
        /* putting into keyboard buffer */
        if(buffer_index < BUFFER_SIZE-1)
            buffer[buffer_index++] = print_char;
        
        /* echo to the screen */
        if(print_char!=0)
           putc(print_char);
    }

    /* critical section ended */
    sti();
}


/* clear the whole buffer  */
void clear_buffer(void){
    int i;

    for(i=0; i < BUFFER_SIZE; i++)
        buffer[i] = KEY_NULL;

    buffer_index = 0;
}
