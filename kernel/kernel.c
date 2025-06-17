#include "screen.h"
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
    clear_screen();

    init_IDT();

    // Set an interrupt mask so the PIC will only see certain IRQs
    // IRQ0 = Programmable Interrupt Timer Interrupt
    // IRQ1 = Keyboard Interrupt
    // IRQ8 = Real Time Clock Interrupt
    // https://wiki.osdev.org/Interrupts
    PIC_set_IRQ_mask(0b01111100);

    // Enable interrupts for the system
    STI();

    set_cursor(0, 0);

    while (1) {        
        draw_cursor();
        wait();
    } // Infinite loop to keep the kernel running
}