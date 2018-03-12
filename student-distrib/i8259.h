/* i8259.h - Defines used in interactions with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */


/*
 * Editor: Hyun Do Jung, 03.10.18
 * Reference materials: lecture 10 of ECE391sp18, OSDEV website, appendix
 * Filename: i8259.c
 * History:
 *    SL    1    Fri Sep 10 08:11:42 2018
 *        First editted for MP3.1
 */

#ifndef _I8259_H
#define _I8259_H

#include "types.h"

/* Ports that each PIC sits on */
#define MASTER_8259_PORT    0x20
#define SLAVE_8259_PORT     0xA0

/* Initialization control words to init each PIC.
 * See the Intel manuals for details on the meaning
 * of each word */
#define IRQ2				0x02
#define ICW1                0x11
#define ICW2_MASTER         0x20
#define ICW2_SLAVE          0x28
#define ICW3_MASTER         0x04
#define ICW3_SLAVE          0x02
#define ICW4                0x01
#define AEOI                0x03

/* End-of-interrupt byte.  This gets OR'd with
 * the interrupt number and sent out to the PIC
 * to declare the interrupt finished */
#define EOI                 0x60
#define ENABLE_IRQ_MASK     0xFE
#define DISABLE_IRQ_MASK    0x01


/* Externally-visible functions */
/* Initialize both PICs */
void i8259_init(void);

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num);

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num);

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num);

#endif /* _I8259_H */
