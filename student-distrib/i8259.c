/* Written by: Michael
 * Debugged by: Nikita */

#include "i8259.h"
#include "lib.h"

#define MIN_IRQ     0
#define MIN_S_IRQ   8
#define MAX_IRQ     15

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
void i8259_init(void) {
    master_mask = MASK_ALL;
    slave_mask = MASK_ALL;

    /* If device raises an interrupt that is connected to IRQ0, IRQ number will be 0x20 */
    /* MASTER IRQ2 is connected with SLAVE PIC */
    /* ICW1, with master port and slave port */
    /* ICW2, set starting points of MASTER and SLAVE, a.k.a IRQ remapping */
    /* ICW3, differente MASTER and SLAVE PICs */
    /* ICW4, Check if master does AUTO EOI or normal EOI */
    
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW2_MASTER, MASTER_8259_PORT+1);
    outb(ICW3_MASTER, MASTER_8259_PORT+1);
    outb(ICW4, MASTER_8259_PORT+1); 

    outb(ICW1,  SLAVE_8259_PORT);
    outb(ICW2_SLAVE,   SLAVE_8259_PORT+1);
    outb(ICW3_SLAVE,  SLAVE_8259_PORT+1);
    outb(ICW4, SLAVE_8259_PORT+1);
 
    /* initialize SLAVE PIC */
    enable_irq(IRQ2_FOR_SLAVE);
}

/*
 * enable_irq
 *   DESCRIPTION: Enables (unmasks) an interrupt requested depends on the 
 *                irq number(irq_num) that is passed in.
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: interrupts are enabled(unmasked) after.
 */
void enable_irq(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return */
    if ( (irq_num < MIN_IRQ) || (irq_num > MAX_IRQ) )
        return;
    
    /* initiate mask = 1111 1111, active low */
    uint8_t temp_mask = ENABLE_IRQ_MASK;
    
    /* If irq_num is in bound of MASTER PIC */
    if ( (irq_num >= MIN_IRQ) && (irq_num < MIN_S_IRQ) ){
        temp_mask = ~(0x1 << irq_num);
        
        master_mask = master_mask & temp_mask;
        outb(master_mask, MASTER_8259_PORT + 1);
        return;
    }
    
    /* If irq_num is in bound of SLAVE PIC */
    if ((irq_num >= MIN_S_IRQ) && (irq_num <= MAX_IRQ)){
        /* subtract irq_num by 8(range: 0 to 7) */
        irq_num -= MIN_S_IRQ;
        
        temp_mask = ~(0x1 << irq_num);

        slave_mask = slave_mask & temp_mask;
        outb(slave_mask, SLAVE_8259_PORT + 1);
        return;
    }
}

/*
 * disable_irq
 *   DESCRIPTION: Disbles (masks) an interrupt requested depends on the 
 *                irq number(irq_num) that is passed in.
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: interrupts are disabled(masked) after.
 */
void disable_irq(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return */
    if ((irq_num < MIN_IRQ) || (irq_num > MAX_IRQ))
        return;
    
    /* initiate mask = 0000 0000, undo enable_irq */
    uint8_t temp_mask = DISABLE_IRQ_MASK;
    
    /* If irq_num is in bound of MASTER PIC */
    if ((irq_num >= MIN_IRQ) && (irq_num < MIN_S_IRQ)) {
        temp_mask = 0x1 << irq_num;

        master_mask = master_mask | temp_mask;
        outb(master_mask, MASTER_8259_PORT + 1);
        return;
    }
    
    /* If irq_num is in bound of SLAVE PIC */
    if ((irq_num >= MIN_S_IRQ) && (irq_num <= MAX_IRQ)) {
        /* subtract irq_num by 8(range: 0 to 7) */
        irq_num -= MIN_S_IRQ;
        
        temp_mask = 0x1 << irq_num;

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
void send_eoi(uint32_t irq_num) {
    
    /* if IRQ_NUM is out of bound, simply do nothing, but return */
    if ( (irq_num < MIN_IRQ) || (irq_num > MAX_IRQ) )
        return;
    
    /* save current flag */
    uint32_t flag_backup;
    cli_and_save(flag_backup);
    
    /* MASTER PIC, EOI return */
    if ((irq_num >= MIN_IRQ) && (irq_num < MIN_S_IRQ)) {
        outb(EOI | irq_num, MASTER_8259_PORT);
    }
    
    /* SLAVE PIC, EOI return */
    if ((irq_num >= MIN_S_IRQ) && (irq_num <= MAX_IRQ)) {
        irq_num -= MIN_S_IRQ;
        outb(EOI | irq_num,        SLAVE_8259_PORT);
        outb(EOI | IRQ2_FOR_SLAVE, MASTER_8259_PORT);
    }

    sti();
    restore_flags(flag_backup);
}
