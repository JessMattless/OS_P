#ifndef INTERRUPTS_H
#define INTERRUPTS_H

extern volatile unsigned long long GPT;

void interrupt_stub();

void pic_timer(void);

#endif