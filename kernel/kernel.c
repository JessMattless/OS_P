#include "screen.h"

// void print_test();

void kernel_main(void) {
    // Create a pointer to a char, point it to the first
    // text cell of video memory (the top left of the screen)
    // char* video_memory = (char*) 0xb8000;
    // At the address being pointed to by video_memory
    // store the char 'X' (display 'X' at the top left of the screen)
    // *video_memory = 'X';
    // print_test();

    terminal_init();
    set_cursor(0);


    // print_char(0x08);
    print("Hello there");
    print_char(0x09);
    print_char('X');
    print_char(0x0A);
    print("Hello there_");
    print_char(0x09);
    print_char('X');
    print_char(0x0A);
    print("Hello there");
    print_char(0x09);
    print_char('X');
    print_char(0x0A);
    print("Hello there_");
    print_char(0x09);
    print_char('X');
    print_char(0x0A);
    print("Hello there");
    print_char(0x09);
    print_char('X');

    scroll_screen(1);

    // print_char('X');

    while (1) {} // Infinite loop to keep the kernel running
}