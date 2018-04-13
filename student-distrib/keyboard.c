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

// const uint8_t shift_case[59] = 
//     {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 
//     0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
//     0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 
//     '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
//     0, 0, 0, ' ', 0};

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
    // char return_char;
    
    // switch (scancode){
    //     case 0x0B:    return_char = '0';    break;
    //     case 0x02:    return_char = '1';    break;
    //     case 0x03:    return_char = '2';    break;
    //     case 0x04:    return_char = '3';    break;
    //     case 0x05:    return_char = '4';    break;
    //     case 0x06:    return_char = '5';    break;
    //     case 0x07:    return_char = '6';    break;
    //     case 0x08:    return_char = '7';    break;
    //     case 0x09:    return_char = '8';    break;
    //     case 0x0a:    return_char = '9';    break;
    //     case 0x1E:    return_char = 'a';    break;
    //     case 0x30:    return_char = 'b';    break;
    //     case 0x2E:    return_char = 'c';    break;
    //     case 0x20:    return_char = 'd';    break;
    //     case 0x12:    return_char = 'e';    break;
    //     case 0x21:    return_char = 'f';    break;
    //     case 0x22:    return_char = 'g';    break;
    //     case 0x23:    return_char = 'h';    break;
    //     case 0x17:    return_char = 'i';    break;
    //     case 0x24:    return_char = 'j';    break;
    //     case 0x25:    return_char = 'k';    break;
    //     case 0x26:    return_char = 'l';    break;
    //     case 0x32:    return_char = 'm';    break;
    //     case 0x31:    return_char = 'n';    break;
    //     case 0x18:    return_char = 'o';    break;
    //     case 0x19:    return_char = 'p';    break;
    //     case 0x10:    return_char = 'q';    break;
    //     case 0x13:    return_char = 'r';    break;
    //     case 0x1F:    return_char = 's';    break;
    //     case 0x14:    return_char = 't';    break;
    //     case 0x16:    return_char = 'u';    break;
    //     case 0x2F:    return_char = 'v';    break;
    //     case 0x11:    return_char = 'w';    break;
    //     case 0x2D:    return_char = 'x';    break;
    //     case 0x15:    return_char = 'y';    break;
    //     case 0x2C:    return_char = 'z';    break;
    //     default:      return_char = 'x';    break;
    // }
    
    // return return_char;

    //print scancodes to check which set to use
    //handle unknown scancodes

    switch(scancode)
    {
        case 0x2A:  
            // left shift pressed
            flags.shift = 1;
            printf("left shift pressed | ");   
            break;

        case 0x36:
            // right shift pressed 
            flags.shift = 1;
            printf("right shift pressed | ");
            break;

        case 0xAA:
            // left shift released
            flags.shift = 0;
            printf("left shift released | ");
            //return 0;
            break;

        case 0xB6:
            // right shift released
            flags.shift = 0;
            printf("right shift released | ");
            break;

        case 0x3A:
            // caps
            flags.caps = ~flags.caps;
            printf("ALL CAPS | ");
            break;

        case 0x1D:
            // left or right ctrl pressed
            flags.ctrl = 1;
            printf("ctrl pressed | ");
            break;

        case 0x9D:
            // left or right ctrl released
            flags.ctrl = 0;
            printf("ctrl released | ");
            break;

        case 0x38:
            // left or right alt pressed
            flags.alt = 1;
            printf("alt pressed | ");
            break;

        case 0xB8:
            // left or right alt released
            flags.alt = 0;
            printf("alt released | ");
            break;
    }

    if(flags.ctrl == 1)
    {
        //check for ctrl-l
    }

    if(flags.shift == 0 && flags.caps == 0 && flags.alt == 0)
    {
        if(scancode < 0x3B)
            return lowercase[scancode];
    }

    else if(flags.shift == 1 && flags.caps == 0 && flags.alt == 0)
    {

    }

    else if(flags.shift == 0 && flags.caps == 1 && flags.alt == 0)
    {

    }

    else if(flags.shift == 1 && flags.caps == 1 && flags.alt == 0)
    {

    }
    
    else if(flags.alt == 1)
    {
        //not able to type
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
        //if(scancode > 0 && scancode < 0x3B)
        //{
            /* interpret the character */
            print_char = get_character(scancode);
            
            /* putting into keyboard buffer */
            if(buffer_index < BUFFER_SIZE-1)
                buffer[buffer_index++] = print_char;
            
            /* echo to the screen */
            if(print_char!=0)
               putc(print_char);
        //}
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
