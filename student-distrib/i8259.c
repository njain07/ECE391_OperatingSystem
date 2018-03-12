/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */


/*
 * Editor: Hyun Do Jung, 03.10.18
 * Reference materials: lecture 10 of ECE391sp18, OSDEV website, appendix
 * Filename: i8259.c
 * History:
 *    SL    1    Fri Mar 10 08:11:42 2018
 *        First editted for MP3.1
 */


#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */


/*
 * i8259_init
 *   DESCRIPTION: Uses 4 Initialization Control Words(ICW) to initialize 8259 PIC controller
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: MASTER and SLAVE PICs get initialized
 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
    
    /* set masks */
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
    /* MASTER PIC */
    if (auto_eoi)
        outb_p(AEOI, MASTER_8259_PORT+1);
    else
        outb_p(ICW4, MASTER_8259_PORT+1);
    
    /* SLAVE PIC */
    outb_p(ICW4, SLAVE_8259_PORT);
 
    /* initiatlize SLAVE PIC */
    enable_irq(IRQ2);
}


/*
 * enable_irq
 *   DESCRIPTION: Enables an interrupt requested depends on the irq number that is passed in.
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: interrupts are enabled(unmasked) after.
 */

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return*/
    if ( (irq_num < 0) || (irq_num > 15) )
        return;
    
    /* initiate mask = 1111 1110, active low */
    uint8_t temp_mask = ENABLE_IRQ_MASK;
    
    /* If irq_num is in bound ofMASTER */
    if ( (irq_num >= 0) && (irq_num <= 7) ){
        
        /* left shift on the mask, 1 on each bit is moving to left, thus add 1 */
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
        
        /* left shift on the mask, 1 on each bit is moving to left, thus add 1 */
        int i = 0;
        for (i = 0; b < irq_num; i++){
            mask = (mask << 1) + 1;
        }
        
        slave_mask = slave_mask & mask;
        outb(slave_mask, SLAVE_8259_PORT + 1);
    }
}


/*
 * disable_irq
 *   DESCRIPTION: disables an interrupt requested depends on the irq number that is passed in.
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: interrupts are disabled(masked) after.
 */

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return*/
    if ( (irq_num < 0) || (irq_num > 15) )
        return;
    
    /* initiate mask = 0000 0001, undo enable_irq */
    uint8_t temp_mask = DISABLE_IRQ_MASK;
    
    /* If irq_num is in bound of MASTER PIC */
    if ( (irq_num >= 0) && (irq_num <= 7) ){
        
        /* left shift on the mask, 1 on each bit is moving to left, thus add 1 */
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
        
        /* left shift on the mask, 1 on each bit is moving to left, thus add 1 */
        int i = 0;
        for (i = 0; b < irq_num; i++){
            mask = (mask << 1) + 1;
        }
        
        slave_mask = slave_mask ^ mask;
        outb(slave_mask, SLAVE_8259_PORT + 1);
    }
    
}


/*
 * send_eoi
 *   DESCRIPTION: after interrupt calling is done, we have to send EOI(end of interrupt)
 *                signal for the specified IRQ
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Send EOI signal to specified IRQ which ends the interrupt calling.
 */

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return*/
    if ( (irq_num < 0) || (irq_num > 15) )
        return;
    
    /* save current flag */
    uint32_t flag_backup;
    cli_and_save(flag_backup);
    
    /* MASTER PIC, EOI return */
    if ( (irq_num >= 0) && (irq_num <= 7) ){
        outb(master_mask, MASTER_8259_PORT + 1);
        outb(EOI | irq_num, MASTER_8259_PORT);
    }
    
    /* SLAVE PIC, EOI return */
    if ( (irq_num >= 0) && (irq_num <= 7) ){
        irq_num -= 8;
        outb(slave_mask, SLAVE_8259_PORT + 1);
        outb(EOI | irq_num, MASTER_8259_PORT);
        outb(EOI + IRQ2,    MASTER_8259_PORT);
    }
}
