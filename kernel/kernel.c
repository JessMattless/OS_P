#include "screen.h"
#include "bytes.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"

void kernel_main(void) {
    PIC_remap(0x20, 0x28);

    screen_init();
    clear_screen();

    init_IDT();

    // Set an interrupt mask so the PIC will only see IRQ0
    // IRQ0 = Programmable Interrupt Timer Interrupt
    // https://wiki.osdev.org/Interrupts
    PIC_set_IRQ_mask(0b11111100);

    // Enable interrupts for the system
    STI();

    while (1) {
        screen_test();

        wait();
    } // Infinite loop to keep the kernel running
}