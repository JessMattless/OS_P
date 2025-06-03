#include "screen.h"
#include "bytes.h"

// enum vga_color {
//     COLOR_BLACK = 0,
// 	COLOR_BLUE = 1,
// 	COLOR_GREEN = 2,
// 	COLOR_CYAN = 3,
// 	COLOR_RED = 4,
// 	COLOR_MAGENTA = 5,
// 	COLOR_BROWN = 6,
// 	COLOR_LIGHT_GREY = 7,
// 	COLOR_DARK_GREY = 8,
// 	COLOR_LIGHT_BLUE = 9,
// 	COLOR_LIGHT_GREEN = 10,
// 	COLOR_LIGHT_CYAN = 11,
// 	COLOR_LIGHT_RED = 12,
// 	COLOR_LIGHT_MAGENTA = 13,
// 	COLOR_LIGHT_BROWN = 14,
// 	COLOR_WHITE = 15
// };

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