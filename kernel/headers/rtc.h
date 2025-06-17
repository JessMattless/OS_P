#ifndef RTC_H
#define RTC_H

// RTC Access Registers
#define RTC_INDEX 0x70
#define RTC_DATA 0x71

#define RTC_REGISTER_A 0x0A
#define RTC_REGISTER_B 0x0B
#define RTC_REGISTER_C 0x0C
#define RTC_NMI_DISABLE 0x80
#define RTC_NMI_ENABLE 0x7F

#define RTC_ENABLE_SIGNAL 0x40

// Initialise the RTC
// This must be done while interrupts are disabled
void RTC_init();

// Enable The Non-Maskable Interrupt
void nmi_enable();
// Disable The Non-Maskable Interrupt
void nmi_disable();

#endif