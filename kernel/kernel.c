#include "screen.h"

void print_test();

void main() {
    // Create a pointer to a char, point it to the first
    // text cell of video memory (the top left of the screen)
    // char* video_memory = (char*) 0xb8000;
    // At the address being pointed to by video_memory
    // store the char 'X' (display 'X' at the top left of the screen)
    // *video_memory = 'X';
    print_test();
}

void print_test() {
    // A function used to test character and string printing

    clear_screen();

    // print_char('H', -1, -1, WHITE_ON_BLACK);
    // print_char('e', -1, -1, WHITE_ON_BLACK);
	// print_char('l', -1, -1, WHITE_ON_BLACK);
	// print_char('l', -1, -1, WHITE_ON_BLACK);
	// print_char('o', -1, -1, WHITE_ON_BLACK);

    print_at("Hello", -1, -1);

    for (;;) {}
}

/*----------------------------- LOW LEVEL REGISTER MANIPULATION -----------------------------*/
unsigned char port_byte_in(unsigned short port) {
    // A C wrapper function that will read a byte from the specified port
    // "=a" (result): put AL register in variable RESULT when complete
    // "d" (port): load EDX register with port
    // __asm__("in %%dx, %%ax"  assembly template
    //  : "=a" (result)         outputs
    //  : "d" (port)            inputs
    //  );
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}
void port_byte_out(unsigned short port, unsigned char data) {
    // "a" (data): load EAX register with data
    // "d" (data): load EDX register with port
    __asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}
unsigned char port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}
void port_word_out(unsigned short port, unsigned short data) {
    // "a" (data): load EAX register with data
    // "d" (data): load EDX register with port
    __asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
}
/*-------------------------- END OF LOW LEVEL REGISTER MANIPULATION --------------------------*/