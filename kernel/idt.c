#include "idt.h"
#include "interrupts.h"

struct IDT_Segment idt[256];

extern void _isr_stub();

ISR_EXTERN(32);

// Create entries pointing to defined functions to fill the IDT
void create_IDT_entry(unsigned int interrupt_no, void (*handler)()) {
    unsigned long handler_address = (unsigned long)handler;

    idt[interrupt_no].offset_low = handler_address & 0xFFFF;
    idt[interrupt_no].segment_selector = 0x08; //Code segment
    idt[interrupt_no].zero = 0;
    idt[interrupt_no].type_attributes = 0b10001110; // 
    idt[interrupt_no].offset_high = (handler_address >> 16) & 0xFFFF;
}

void init_IDT() {
    // fill the idt with references to a stub pointer, for any unused entries that we don't define manually
    for (int i = 0; i < 256; i++) create_IDT_entry(i, _isr_stub);

    /* Begin ISR definitions*/

    create_IDT_entry(0x20, isr32);

    /* End of ISR Definitions */

    struct IDT idt_pointer;
    idt_pointer.limit = sizeof(idt) - 1;
    idt_pointer.base = (unsigned long) &idt;

    __asm__ volatile ("lidt %0" : : "m"(idt_pointer));
}