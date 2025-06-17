#ifndef BYTES_H
#define BYTES_H

// Disable/Enable Interrupts shorthand
#define CLI() asm ("cli")
#define STI() asm ("sti")

// In Byte
unsigned char inportb(unsigned short port);
// Out Byte
void outportb(unsigned short port, unsigned char data);

// In Word
unsigned short inportw(unsigned short port);
// Out Word
void outportw(unsigned short port, unsigned short data);

// Send a dummy instruction to the CPU to create a small delay
void wait(void);

// Returns 1 if interrupts are enabled, 0 otherwise
int check_interrupts();

// Returns flags used to re-enable interrupts later
unsigned long disableInterrupts();
// Use flags saved from disabling interrupts to re-enable them with the same settings
void enableInterrupts(unsigned long flags);

#endif