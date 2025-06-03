#ifndef SCREEN_H
#define SCREEN_H


#define VIDEO_ADDRESS 0xA0000       // The video address in memory
#define SCREEN_WIDTH 320            // The width of the screen in pixels
#define SCREEN_HEIGHT 200           // The height of the screen in pixels

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

struct RGB {
    unsigned char r, g, b;
};

void screen_init();
void clear_screen();

void put_pixel(int pos_x, int pos_y, unsigned char color);
void configure_palette(long index, struct RGB rgb);

#endif