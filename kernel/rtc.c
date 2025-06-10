#include "rtc.h"
#include "bytes.h"

// RTC is Real Time Clock

// https://wiki.osdev.org/RTC

// This must be done while interrupts are disabled
void RTC_init() {
    nmi_disable();

    unsigned char current_mask = inportb(RTC_INDEX);
    outportb(RTC_INDEX, current_mask | RTC_REGISTER_B);

    unsigned char data = inportb(RTC_DATA);
    outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REGISTER_B);
    outportb(RTC_DATA, data | RTC_ENABLE_SIGNAL);

    nmi_enable();
}

// Non-Maskable Interrupt
void nmi_enable() {
    unsigned char current_mask = inportb(RTC_INDEX);
    outportb(RTC_INDEX, current_mask | RTC_NMI_ENABLE);
    current_mask = inportb(RTC_DATA);
}

// Non-Maskable Interrupt
void nmi_disable() {
    unsigned char current_mask = inportb(RTC_INDEX);
    outportb(RTC_INDEX, current_mask | RTC_NMI_DISABLE);
    current_mask = inportb(RTC_DATA);
}