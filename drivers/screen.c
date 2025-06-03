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

void clear_screen() {

    for (int y = 0; y < Y_RESOLUTION; y++) {
        for (int x = 0; x < X_RESOLUTION; x++) {
            put_pixel(x, y, x + y);
        }
    }
}

void put_pixel(int pos_x, int pos_y, unsigned char* color) {    
    // Create a pointer to the start of video memory
    volatile char* vid_mem = (volatile char*) VIDEO_ADDRESS;

    int memory_offset = (X_RESOLUTION * pos_y) + pos_x;

    vid_mem[memory_offset] = color;
}