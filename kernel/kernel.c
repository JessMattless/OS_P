#include "screen.h"
#include "text_mode.h"
#include "terminal_mode.h"
#include "bytes.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"
#include "rtc.h"
#include "interrupts.h"

void kernel_main(void) {
    PIC_remap(0x20, 0x28);
    RTC_init();

    screen_init();

    init_IDT();

    // Set an interrupt mask so the PIC will only see certain IRQs
    // IRQ0 = Programmable Interrupt Timer Interrupt
    // IRQ1 = Keyboard Interrupt
    // IRQ8 = Real Time Clock Interrupt
    // https://wiki.osdev.org/Interrupts
    PIC_set_IRQ_mask(0b01111100);

    // Enable interrupts for the system
    STI();

    change_screen_mode(TERMINAL_MODE);

    while (1) {
        screen_handler();

        wait();
    } // Infinite loop to keep the kernel running
}