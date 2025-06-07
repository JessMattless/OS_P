#include "screen.h"
#include "bytes.h"
#include "interrupts.h"
#include "keyboard.h"
#include "font.h"

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
    int memory_offset = (SCREEN_WIDTH * pos_y) + pos_x;

    vid_mem[memory_offset] = color;
}

void put_char(unsigned char ch, int x, int y, unsigned char fg, unsigned char bg) {
    unsigned long long glyph = font[ch];

    for (int cy = 0; cy < 8; cy++) {
        for (int cx = 0; cx < 8; cx++) {
            int index = (cy * 8) + cx;
            unsigned char draw_pixel = (glyph >> index) & 1;
            put_pixel(x + cx, y + cy, draw_pixel ? fg : bg);
        }
    }
}

void screen_test() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (test == 0) put_pixel(x, y, (y + x) + (GPT % 256));
            else if (test == 1) put_pixel(x, y, (y + x) - (GPT % 256));
        }
    }
}