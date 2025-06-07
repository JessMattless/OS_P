#include "interrupts.h"
#include "pic.h"
#include "bytes.h"

// General Program Timer
volatile unsigned long long GPT = 0;

// Stub function used for unregistered ISRs
void interrupt_stub(void) {
    return;
}

// Handler for easily calling ISR functions
void interrupt_handler(int interrupt_no) {
    switch (interrupt_no) {
        case 0x20: pic_timer();
        case 0x21: keyboard_interrupt();
        default: 
            PIC_send_EOI(0);
            return;
    }
}

// Increment the General Program Timer
void pic_timer(void) {
    GPT++;
}

// Send any keyboard inputs to the keyboard handler
void keyboard_interrupt(void) {
    keyboard_handler(inportb(KEYBOARD_DATA));
}