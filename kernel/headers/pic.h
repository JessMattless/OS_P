#ifndef PIC_H
#define PIC_H

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

// Interrupt Status Registers

// Interrupt Request Register
// OCW3 IRQ ready next command
#define PIC_READ_IRR 0x0A

// In-Service Register
// OCW3 IRQ service next command
#define PIC_READ_ISR 0x0B


// End of Interrupt
#define PIC_EOI 0x20

#define ICW1_ICW4	0x01		// Indicates that ICW4 will be present
#define ICW1_SINGLE	0x02		// Single (cascade) mode
#define ICW1_INTERVAL4	0x04	// Call address interval 4 (8)
#define ICW1_LEVEL	0x08		// Level triggered (edge) mode
#define ICW1_INIT	0x10		// Initialization - required

#define ICW4_8086	0x01		// 8086/88 (MCS-80/85) mode
#define ICW4_AUTO	0x02		// Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08	// Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C	// Buffered mode/master
#define ICW4_SFNM	0x10		// Special fully nested (not)

void PIC_send_EOI(unsigned char irq);

void PIC_remap(int offset1, int offset2);

void PIC_disable(void);
void PIC_enable(void);

void PIC_set_IRQ_mask(unsigned char IRQ);
void PIC_clear_IRQ_mask(unsigned char IRQ);

unsigned short _pic_get_irq_register(int ocw3);
unsigned short PIC_get_IRR(void);
unsigned short PIC_get_ISR(void);

#endif