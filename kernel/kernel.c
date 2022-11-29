#include "screen.h"

// void print_test();

void kernel_main() {
    // Create a pointer to a char, point it to the first
    // text cell of video memory (the top left of the screen)
    // char* video_memory = (char*) 0xb8000;
    // At the address being pointed to by video_memory
    // store the char 'X' (display 'X' at the top left of the screen)
    // *video_memory = 'X';
    // print_test();

    terminal_init();

    // clear_screen();

    // char x = 'e';
    // char* y = &x;

    // print_char('M');
    // print_char('P');

    // put_string("Hello", 0x0f, 13, 0);
    print("Hello");

    // print_char('H', -1, -1, WHITE_ON_BLACK);
    // print_char(x, -1, -1, WHITE_ON_BLACK);

    // print_at("Hello", 1, 0);
    // print_at("There", 0, 1);
    // print_at("asdasd", 0, 2);

    // for (;;) {}
}

// void print_test() {
//     // A function used to test character and string printing

//     clear_screen();

//     // print_char('H', -1, -1, WHITE_ON_BLACK);
//     // print_char('e', -1, -1, WHITE_ON_BLACK);
// 	// print_char('l', -1, -1, WHITE_ON_BLACK);
// 	// print_char('l', -1, -1, WHITE_ON_BLACK);
// 	// print_char('o', -1, -1, WHITE_ON_BLACK);
    
//     // const char* str = "Hello";


//     // print_char(str, -1, -1, WHITE_ON_BLACK);
//     // print_char(*str, -1, -1, WHITE_ON_BLACK);
//     // str++;

//     // print_at((char *)0x2180, -1, -1);
//     print_at("Hello", 1, 0);
//     print_at("There", 0, 1);
//     print_at("asdasd", 0, 2);


//     for (;;) {}
// }