/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */



/* Editor: Hyun Do Jung, 03.10.18
 * Reference materials: lecture 10 of ECE391sp18, OSDEV website, appendix
 * Filename: i8259.c
 * History:
 *    SL    1    Fri Mar 10 08:11:42 2018
 *        First editted for MP3.1
 *    SL    2    Thu Mar 15 16:02:00 2018
 *        Revision 1, adding more comments, making sure it works
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
 *   SIDE EFFECTS: MASTER and SLAVE PICs get initialized. 
 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
    

    /* OSDEV says we should have flag stored beforehand, but do we?
     mask all MASKS before even starting the initialization */
    master_mask = MASK_ALL;
    slave_mask = MASK_ALL;

    // CLEAR ALL IRQ LINES

    /* ICW1, with master port and slave port */
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW2_MASTER, MASTER_8259_PORT+1);
    outb(ICW3_MASTER, MASTER_8259_PORT+1);
    outb(ICW4, MASTER_8259_PORT+1);    
    
    outb(ICW1,  SLAVE_8259_PORT);
    /* ICW2, set starting points of MASTER and SLAVE, a.k.a IRQ remapping */
    /* If device raises an interrupt that is connected to IRQ0, IRQ number will be 0x20 */
    outb(ICW2_SLAVE,   SLAVE_8259_PORT+1);
    
    /* ICW3, differente MASTER and SLAVE PICs */
    /* MASTER IRQ2 is connected with SLAVE PIC */
    outb(ICW3_SLAVE,  SLAVE_8259_PORT+1);
    
    /* ICW4, Check if master does AUTO EOI or normal EOI */
    outb(ICW4, SLAVE_8259_PORT+1);
 
    /* initiatlize SLAVE PIC */
    enable_irq(IRQ2_FOR_SLAVE);
}


/*
 * enable_irq
 *   DESCRIPTION: Enables an interrupt requested depends on the 
 *                irq number(irq_num) that is passed in.
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: interrupts are enabled(unmasked) after.
 */

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return */
    if ( (irq_num < 0) || (irq_num > 15) )
        return;
    
    /* initiate mask = 1111 1110, active low */
    // uint8_t master_mask;
    uint8_t temp_mask = ENABLE_IRQ_MASK;
    int i = 0;
    
    /* If irq_num is in bound of MASTER PIC */
    if ( (irq_num >= 0) && (irq_num <= 7) ){
        
        /* left shift on the mask, 1 on each bit is moving to left, thus add 1 */
        // for (i = 0; i < irq_num; i++){
        //     temp_mask = (temp_mask << 1) + 1;
        // }
        // irq = 2
        // 1111 1011

        temp_mask = ~(0x1 << irq_num);
        
        master_mask = master_mask & temp_mask;
        outb(master_mask, MASTER_8259_PORT + 1);
        return;
    }
    
    /* If irq_num is in bound of SLAVE PIC */
    if ((irq_num >= 8) && (irq_num <= 15)){
        
        /* subtract irq_num by 8(range: 0 to 7) */
        irq_num -= 8;
        
        /* left shift on the mask, 1 on each bit is moving to left, thus add 1 */
        // for (i = 0; i < irq_num; i++) {
        //     temp_mask = (temp_mask << 1) + 1;
        // }
        
        temp_mask = ~(0x1 << irq_num);

        slave_mask = slave_mask & temp_mask;
        outb(slave_mask, SLAVE_8259_PORT + 1);
        return;
    }
}


/*
 * disable_irq
 *   DESCRIPTION: Disbles an interrupt requested depends on the 
 *                irq number(irq_num) that is passed in.
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: interrupts are disabled(masked) after.
 */

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return */
    if ((irq_num < 0) || (irq_num > 15))
        return;
    
    /* initiate mask = 0000 0001, undo enable_irq */
    uint8_t master_mask;
    uint8_t temp_mask = DISABLE_IRQ_MASK;
    int i = 0;
    
    /* If irq_num is in bound of MASTER PIC */
    if ((irq_num >= 0) && (irq_num <= 7)) {
        
        /* left shift on the mask, 0 on each bit is moving to left */
        for (i = 0; i < irq_num; i++){
            temp_mask = temp_mask << 1;
        }
        
        master_mask = master_mask | temp_mask;
        outb(master_mask, MASTER_8259_PORT + 1);
        return;
    }
    
    /* If irq_num is in bound of SLAVE PIC */
    if ((irq_num >= 8) && (irq_num <= 15)) {
        
        /* subtract irq_num by 8(range: 0 to 7) */
        irq_num -= 8;
        
        /* left shift on the mask, 0 on each bit is moving to left */
        for (i = 0; i < irq_num; i++) {
            temp_mask = temp_mask << 1; 
        }
        
        slave_mask = slave_mask | temp_mask;
        outb(slave_mask, SLAVE_8259_PORT + 1);
        return;
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
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return */
    if ( (irq_num < 0) || (irq_num > 15) )
        return;
    
    /* save current flag */
    uint32_t flag_backup;
    cli_and_save(flag_backup);
    
    /* MASTER PIC, EOI return */
    if ((irq_num >= 0) && (irq_num <= 7)) {
        //outb(master_mask, MASTER_8259_PORT + 1);
        outb(EOI | irq_num, MASTER_8259_PORT);
    }
    
    /* SLAVE PIC, EOI return */
    if ((irq_num >= 8) && (irq_num <= 15)) {
        irq_num -= 8;
        //outb(slave_mask, SLAVE_8259_PORT + 1);
        outb(EOI | irq_num,        SLAVE_8259_PORT);
        outb(EOI | IRQ2_FOR_SLAVE, MASTER_8259_PORT);
    }

    sti();
    restore_flags(flag_backup);
}
