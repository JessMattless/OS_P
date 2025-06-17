#include "interrupts.h"
#include "pic.h"
#include "bytes.h"

// General Program Timer
volatile unsigned long long GPT = 0;
// Real Time Clock
volatile unsigned long long RTC = 0;

void interrupt_stub(void) {
    return;
}

void interrupt_handler(int interrupt_no) {
    // Calls the correct defined function for specified interrupts
    switch (interrupt_no) {
        case 0x20: pic_timer();
        case 0x21: keyboard_interrupt();
        case 0x28: rtc_timer();
        default: 
            PIC_send_EOI(0);
            return;
    }
}

void pic_timer(void) {
    // Increment the General Program Timer
    GPT++;
}

void keyboard_interrupt(void) {
    // Send any keyboard inputs to the keyboard handler
    keyboard_handler(inportb(KEYBOARD_DATA));
}

void rtc_timer(void) {
    // Increment the Real Time Clock
    RTC++;
}