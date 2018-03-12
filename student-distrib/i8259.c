/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */


/*
 * edited by HD, 03.10.18
 * Reference materials: lecture 10 of ECE391sp18.
 * How: we are going to init PIC(8259) step by step, walking through ICW
 *      per MASTER and SLAVE chips. All ICWs are orignally defined and given in i8259.h
 *
 */



/* Initialize the 8259 PIC */
void i8259_init(void) {
    
    master_mask = 0xFF;
    slave_mask = 0xFF;
    
    /* ICW1, with master port and slave port */
    outb_p(ICW1, MASTER_8259_PORT);
    outb_p(ICW1,  SLAVE_8259_PORT);
    
    /* ICW2, set starting points of MASTER and SLAVE, a.k.a IRQ remapping */
    /* If device raises an interrupt that is connected to IRQ0, IRQ number will be 0x20 */
    outb_p(ICW2_MASTER, MASTER_8259_PORT+1);
    outb_p(ICW2_SLAVE,   SLAVE_8259_PORT+1);
    
    /* ICW3, differente MASTER and SLAVE PICs */
    /* MASTER IRQ2 is connected with SLAVE PIC */
    outb_pICW3_MASTER, MASTER_8259_PORT+1);
    outb_p(ICW3_SLAVE,  SLAVE_8259_PORT+1);
    
    /* ICW4, Check if master does AUTO EOI or normal EOI */
    if (auto_eoi)
        outb_p(AEOI, MASTER_8259_PORT+1);
    else
        outb_p(ICW4, MASTER_8259_PORT+1);
    
    outb_p(ICW4, SLAVE_8259_PORT);
 
    /* initiatlize SLAVE PIC */
    enable_irq(IRQ2);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return*/
    if ( (irq_num < 0) || (irq_num > 15) )
        return;
    
    /* initiate mask = 1111 1110, active low */
    uint8_t temp_mask = 0xFE;
    
    /* If irq_num is in bound ofMASTER */
    if ( (irq_num >= 0) && (irq_num <= 7) ){
        
        /* left shift of mask */
        int i = 0;
        for (i = 0; b < irq_num; i++){
            mask = (mask << 1) + 1;
        }
        
        master_mask = master_mask & mask;
        outb(master_mask, MASTER_8259_PORT + 1);
    }
    
    /* If irq_num is in bound of SLAVE PIC */
    if ( (irq_num >= 8) && (irq_num <= 15) ){
        
        /* subtract irq_num by 8(range: 0 to 7) */
        irq_num -= 8;
        
        /* left shift of mask */
        int i = 0;
        for (i = 0; b < irq_num; i++){
            mask = (mask << 1) + 1;
        }
        
        slave_mask = slave_mask & mask;
        outb(slave_mask, SLAVE_8259_PORT + 1);
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return*/
    if ( (irq_num < 0) || (irq_num > 15) )
        return;
    
    /* initiate mask = 0000 0001, undo enable_irq */
    uint8_t temp_mask = 0x01;
    
    /* If irq_num is in bound of MASTER PIC */
    if ( (irq_num >= 0) && (irq_num <= 7) ){
        
        /* left shift of mask */
        int i = 0;
        for (i = 0; b < irq_num; i++){
            mask = (mask << 1) + 1;
        }
        
        master_mask = master_mask ^ mask;
        outb(master_mask, MASTER_8259_PORT + 1);
    }
    
    /* If irq_num is in bound of SLAVE PIC */
    if ( (irq_num >= 8) && (irq_num <= 15) ){
        
        /* subtract irq_num by 8(range: 0 to 7) */
        irq_num -= 8;
        
        /* left shift of mask */
        int i = 0;
        for (i = 0; b < irq_num; i++){
            mask = (mask << 1) + 1;
        }
        
        slave_mask = slave_mask ^ mask;
        outb(slave_mask, SLAVE_8259_PORT + 1);
    }
    
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
}

