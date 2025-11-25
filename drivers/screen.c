#include "screen.h"
#include "bytes.h"
#include "interrupts.h"
#include "keyboard.h"
#include "font.h"
#include "text_mode.h"
#include "terminal_mode.h"

unsigned char current_screen_mode;

void screen_init() {

    // Setup palette with 8-bit color using RRRGGGBB color
    for (unsigned char i = 0; i < 255; i++) {
        configure_palette(
            i,
            (struct RGB) {
                //Red
                ((i >> 5) & 0x7) * (256 / 3),
                //Green
                ((i >> 2) & 0x7) * (256 / 3),
                //Blue
                ((i >> 0) & 0x3) * (256 / 3)
            }
        );
    }
    // Set black and white specifically for the palette
    configure_palette(0, (struct RGB) {0, 0, 0});
    configure_palette(255, (struct RGB) {255, 255, 255});

    clear_screen();
}

// Set the color in the palette at a specific index to be the defined color
void configure_palette(long index, struct RGB rgb) {
    outportb(PALETTE_MASK, 0xFF);
    outportb(PALETTE_WRITE, index);
    outportb(PALETTE_DATA, rgb.r);
    outportb(PALETTE_DATA, rgb.g);
    outportb(PALETTE_DATA, rgb.b);
}

void clear_screen() {
    // Set every pixel on screen to be black
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            put_pixel(x, y, 0x00);
        }
    }
}

void put_pixel(int pos_x, int pos_y, unsigned char color) {    
    // Create a pointer to the start of video memory
    volatile char* vid_mem = (volatile char*) VIDEO_ADDRESS;

    // Get the pixel offset from the requested row/column
    unsigned long long memory_offset = (SCREEN_WIDTH * pos_y) + pos_x;

    vid_mem[memory_offset] = color;
}

void change_screen_mode(unsigned char mode) {
    current_screen_mode = mode;

    switch (current_screen_mode) {
        case TEXT_MODE:
            set_text_cursor(0, 0);
            break;
        case TERMINAL_MODE:
            set_text_cursor(0, 0);
            terminal_init();
            break;
        default: break;
    }
}

void screen_handler() {
    switch (current_screen_mode) {
        case TEXT_MODE: case TERMINAL_MODE:
            draw_text_cursor();
        default: break;
    }
}