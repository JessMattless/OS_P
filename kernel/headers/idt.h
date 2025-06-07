#ifndef IDT_H
#define IDT_H

#define IDT_LOCATION 0x0
#define IDT_SIZE     0x03FF
#define IDT_SEGMENT_SIZE 0x40

#define ISR_EXTERN(n) extern void isr##n(void);

// https://wiki.osdev.org/Interrupt_Descriptor_Table

// The IDT should have 256 entries
// Not all have to be filled
// Any more than 256 is ignored

// Offset           (bits 0-15)
// Segment Selector (bits 16-31)
// Reserved         (bits 32-39)
// Gate Type        (bits 40-43)
// 0                (bit 44)
// DPL              (bits 45-46)
// P                (bit 47)
// Offset           (bits 48-63)


struct IDT {
    unsigned short limit;
    unsigned long base;
}__attribute__((packed));

struct IDT_Segment {
    // Stores bits 0-15 of the IDT segment
    // |xxxxxxxxxxxxxxxx|
    unsigned short offset_low;
    
    // Stores bits 16-31
    // |xxxxxxxxxxxxxxxx|
    unsigned short segment_selector;
    
    unsigned char zero; // Required 0
    
    // Bits 32-39 are reserved
    
    // Stores bits 40-47
    // |x|xx|0|xxxx|
    // View attached wiki page
    unsigned char type_attributes;

    // Stores bits 48-63
    // |xxxxxxxxxxxxxxxx|
    unsigned short offset_high;
}__attribute__((packed));

extern struct IDT_Segment idt[256];

void create_IDT_entry(unsigned int interrupt_no, void (*handler)());

// Initialise the IDT with defined functions and stubs
void init_IDT();

#endif