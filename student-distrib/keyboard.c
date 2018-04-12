/* keyboard.c - Functions to interact with the keyboard devices and its interrupts
 * vim:ts=4 noexpandtab
 */

#include "keyboard.h"
#include "lib.h"

/* local variables */
char buffer[BUFFER_SIZE];
int buffer_index = 0;

/* Simple init helper for keyboard initialization */
void keyboard_init(void) {
    enable_irq(KEYBOARD_IRQ);
}

/* Read the actual character and return it */
char get_character(char scancode){
    char return_char;
    switch (scancode){
        case 0x0B:    return_char = '0';    break;
        case 0x02:    return_char = '1';    break;
        case 0x03:    return_char = '2';    break;
        case 0x04:    return_char = '3';    break;
        case 0x05:    return_char = '4';    break;
        case 0x06:    return_char = '5';    break;
        case 0x07:    return_char = '6';    break;
        case 0x08:    return_char = '7';    break;
        case 0x09:    return_char = '8';    break;
        case 0x0a:    return_char = '9';    break;
        case 0x1E:    return_char = 'a';    break;
        case 0x30:    return_char = 'b';    break;
        case 0x2E:    return_char = 'c';    break;
        case 0x20:    return_char = 'd';    break;
        case 0x12:    return_char = 'e';    break;
        case 0x21:    return_char = 'f';    break;
        case 0x22:    return_char = 'g';    break;
        case 0x23:    return_char = 'h';    break;
        case 0x17:    return_char = 'i';    break;
        case 0x24:    return_char = 'j';    break;
        case 0x25:    return_char = 'k';    break;
        case 0x26:    return_char = 'l';    break;
        case 0x32:    return_char = 'm';    break;
        case 0x31:    return_char = 'n';    break;
        case 0x18:    return_char = 'o';    break;
        case 0x19:    return_char = 'p';    break;
        case 0x10:    return_char = 'q';    break;
        case 0x13:    return_char = 'r';    break;
        case 0x1F:    return_char = 's';    break;
        case 0x14:    return_char = 't';    break;
        case 0x16:    return_char = 'u';    break;
        case 0x2F:    return_char = 'v';    break;
        case 0x11:    return_char = 'w';    break;
        case 0x2D:    return_char = 'x';    break;
        case 0x15:    return_char = 'y';    break;
        case 0x2C:    return_char = 'z';    break;
        default:      return_char = KEY_NULL;    break;
    }
    
    return return_char;

    //print scancodes to check which set to use
    //handle unknown scancodes

    // if(flags->shift == 0 && flags->caps == 0 && flags->ctrl == 0 && flags->alt == 0)
    // {

    // }

    // else if(flags->shift == 1 && flags->caps == 0 && flags->ctrl == 0 && flags->alt == 0)
    // {
        
    // }

    // else if(flags->shift == 0 && flags->caps == 1 && flags->ctrl == 0 && flags->alt == 0)
    // {
        
    // }

    // else if(flags->shift == 0 && flags->caps == 0 && flags->ctrl == 1 && flags->alt == 0)
    // {
        
    // }

    // else if(flags->shift == 0 && flags->caps == 0 && flags->ctrl == 0 && flags->alt == 1)
    // {
        
    // }

    // else if(flags->shift == 1 && flags->caps == 1 && flags->ctrl == 0 && flags->alt == 0)
    // {
        
    // }
}

void keyboard_interrupt_handler(void){
    
    char scancode = 0;
    char print_char = 0;
    
    /* critical section started */
    cli();
    send_eoi(KEYBOARD_IRQ);
    do{
        scancode=inb(KEYBOARD_PORT);
        putc(scancode);
    } while (scancode >= 0);

    /* interpret the character */
    print_char = get_character(scancode);
    
    /* putting into keyboard buffer */
    if(buffer_index < BUFFER_SIZE-1)
        buffer[buffer_index++] = print_char;
    
    /* echo to the screen */
    putc(print_char);
    
    /* INTERRUPT ended */
    
    
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
