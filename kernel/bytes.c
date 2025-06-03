#include "bytes.h"

unsigned char inportb(unsigned short port) {
    // A C wrapper function that will read a byte from the specified port
    // "=a" (result): put AL register in variable RESULT when complete
    // "d" (port): load EDX register with port
    // __asm__("in %%dx, %%ax"  assembly template
    //  : "=a" (result)         outputs
    //  : "d" (port)            inputs
    //  );
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void outportb(unsigned short port, unsigned char data) {
    // "a" (data): load EAX register with data
    // "d" (data): load EDX register with port
    __asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}

unsigned short inportw(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void outportw(unsigned short port, unsigned short data) {
    // "a" (data): load EAX register with data
    // "d" (data): load EDX register with port
    __asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
}

void wait(void) {
    // Send a piece of data to an unused register, used to get a very small delay
    outportb(0x80, 0);
}

int check_interrupts() {
    // Check if any interrupt flags are enabled for the CPU
    unsigned long flags;
    __asm__("pushf\n\t" 
            "pop %0" 
            : "=g" (flags));
    return flags & (1 << 9);
}

unsigned long disableInterrupts() {
    unsigned long flags;
    __asm__("pushf\n\t"
            "cli\n\t"
            "pop %0"
            : "=r" (flags)
            : : "memory");
    return flags;
}

// Use flags saved from disabling interrupts to re-enable them with the same settings
void enableInterrupts(unsigned long flags) {
    __asm__("push %0\n\t"
            "popf\n\t"
            : : "rm" (flags)
            : "memory", "cc");
}
