#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "keyboard.h"

extern volatile unsigned long long GPT;
extern volatile unsigned long long RTC;

// Stub function used for unregistered ISRs
void interrupt_stub();

// ISR0 Handler
void pic_timer(void);

// Handler for easily calling ISR functions
void interrupt_handler(int interrupt_no);

// ISR1 Handler
void keyboard_interrupt(void);

// ISR8 Handler
void rtc_timer(void);

#endif