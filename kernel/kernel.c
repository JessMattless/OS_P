#include "screen.h"

void kernel_main(void) {
    screen_init();
    clear_screen();

    while (1) {} // Infinite loop to keep the kernel running
}