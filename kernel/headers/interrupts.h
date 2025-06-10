#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "keyboard.h"

extern volatile unsigned long long GPT;
extern volatile unsigned long long RTC;

void interrupt_stub();

void pic_timer(void);
void keyboard_interrupt(void);
void rtc_timer(void);

#endif