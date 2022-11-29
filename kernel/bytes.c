#include "bytes.h"

unsigned char port_byte_in(unsigned short port) {
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

void port_byte_out(unsigned short port, unsigned char data) {
    // "a" (data): load EAX register with data
    // "d" (data): load EDX register with port
    __asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}

unsigned char port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data) {
    // "a" (data): load EAX register with data
    // "d" (data): load EDX register with port
    __asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
}