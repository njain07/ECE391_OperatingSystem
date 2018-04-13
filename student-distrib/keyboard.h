/* keyboard.h - Defines used in interactions with the keyboard devices and its interrupts
 * vim:ts=4 noexpandtab
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "types.h"
#include "lib.h"
#include "i8259.h"

#define KEYBOARD_IRQ    1
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_PORT   0x60
#define KEYBOARD_NUM    60
#define BUFFER_SIZE     128
#define KEY_NULL        '\0'

#include "lib.h"

typedef struct flag_struct
{
	uint8_t shift;
	uint8_t caps;
	uint8_t ctrl;
	uint8_t alt; 
} flag_struct;

uint8_t enter;
flag_struct flags;

char buffer[BUFFER_SIZE];
int buffer_index;

// extern char keyboard_buffer[BUFFER_SIZE];

/* initialize the keyboard */
void keyboard_init(void);

/* read the scancode */
char get_character(uint8_t scancode);

/* keyboard helper function */
void keyboard_interrupt_handler();

/* clear buffer */
void clear_buffer();

// backspace should use video memory
// cursor ???
// enter will call clear_buffer()

#endif
