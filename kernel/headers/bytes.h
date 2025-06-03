#ifndef BYTES_H
#define BYTES_H

#define CLI() asm ("cli")
#define STI() asm ("sti")

unsigned char inportb(unsigned short port);
void outportb(unsigned short port, unsigned char data);

unsigned short inportw(unsigned short port);
void outportw(unsigned short port, unsigned short data);

void wait(void);

// Returns 1 if interrupts are enabled, 0 otherwise
int check_interrupts();

unsigned long disableInterrupts();
void enableInterrupts(unsigned long flags);

#endif