#ifndef RTC_H
#define RTC_H

#define RTC_INDEX 0x70
#define RTC_DATA 0x71

#define RTC_REGISTER_A 0x0A
#define RTC_REGISTER_B 0x0B
#define RTC_REGISTER_C 0x0C
#define RTC_NMI_DISABLE 0x80
#define RTC_NMI_ENABLE 0x7F

#define RTC_ENABLE_SIGNAL 0x40

void RTC_init();

void nmi_enable();
void nmi_disable();

#endif