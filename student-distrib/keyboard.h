/* keyboard.h - Defines used in interactions with the keyboard devices and its interrupts
 * vim:ts=4 noexpandtab
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "types.h"
#include "lib.h"
#include "i8259.h"

#define KEYBOARD_IRQ    1
#define KEYBOARD_PORT   0x60
#define KEYBOARD_NUM    60
#define BUFFER_SIZE     128
#define KEY_NULL        '\0'


/* initialize the keyboard */
void keyboard_init(void);

/* read the scancode */
char get_scancode(char scancode);

/* keyboard helper function */
void keyboard_interrupt_handler();

/* clear buffer */
void clear_buffer();

#endif
