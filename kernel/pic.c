#include "pic.h"
#include "bytes.h"

// TODO: Sort out Spurious IRQs https://wiki.osdev.org/8259_PIC#Spurious_IRQs

//PIC is Programmable Interrupt Controller

// https://wiki.osdev.org/8259_PIC

// End of Interrupt, used to tell the PIC that the
// calling Interrupt ReQuest (IRQ) has finished
void PIC_send_EOI(unsigned char irq) {
    if (irq >= 8) outportb(PIC2_COMMAND, PIC_EOI);
    outportb(PIC1_COMMAND, PIC_EOI);
}

// Some of the default IRQs conflict with CPU exceptions by default
// we remap the PIC to ensure this doesn't happen
// wait() is used here as the PIC might need time to run each command
void PIC_remap(int offset1, int offset2) {
    // Begin initializing the PIC
    outportb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    wait();
    outportb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    wait();

    // ICW2: Set new offsets
    outportb(PIC1_DATA, offset1);
    wait();
    outportb(PIC2_DATA, offset2);
    wait();

    // ICW3: Tell the master PIC that there is a slave present at IRQ2
    outportb(PIC1_DATA, 4);
    wait();
    
    // ICW3: Tell the slave PIC where the cascade from the master PIC will come from
    outportb(PIC2_DATA, 2);
    wait();

    // ICW4: have the PICs use 8086 mode
    outportb(PIC1_DATA, ICW4_8086);
    wait();
    outportb(PIC2_DATA, ICW4_8086);
    wait();

    // Unmask the PICs
    outportb(PIC1_DATA, 0);
    outportb(PIC2_DATA, 0);
}

void PIC_disable(void) {
    // Mask each interrupt to disable the PICs
    outportb(PIC1_DATA, 0xFF);
    outportb(PIC2_DATA, 0xFF);
}

void PIC_enable(void) {
    // Mask each interrupt to enable the PICs
    outportb(PIC1_DATA, 0x00);
    outportb(PIC2_DATA, 0x00);
}

// Mask specific IRQs, so the PIC will ignore them
void PIC_set_IRQ_mask(unsigned char IRQline) {
    unsigned short port;
    unsigned char value;

    if (IRQline < 8) port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = inportb(port) | (1 << IRQline);
    outportb(port, value);
}

// Clear the mask on specific IRQs, so the PIC will stop ignoring them
void PIC_clear_IRQ_mask(unsigned char IRQline) {
    unsigned short port;
    unsigned char value;

    if (IRQline < 8) port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = inportb(port) | ~(1 << IRQline);
    outportb(port, value);
}

// Get the register values from both PICs
// PIC1 is IRQs 0-7, PIC2 is IRQ2 8-15
// PIC2 IRQ 2 is the chain to PIC1
unsigned short _pic_get_irq_register(int ocw3) {
    outportb(PIC1_COMMAND, ocw3);
    outportb(PIC2_COMMAND, ocw3);

    return (inportb(PIC2_COMMAND) << 8) | inportb(PIC1_COMMAND);
}

// This function will show bit 2 0b0000000000000100 when any of PIC2's bits are set
unsigned short PIC_get_IRR(void) {
    return _pic_get_irq_register(PIC_READ_IRR);
}

// This function will show bit 2 0b0000000000000100 when any of PIC2's bits are set
unsigned short PIC_get_ISR(void) {
    return _pic_get_irq_register(PIC_READ_ISR);
}