#ifndef SCREEN_H
#define SCREEN_H


#define VIDEO_ADDRESS 0xA0000 // The video address in memory
#define X_RESOLUTION 320           // The width of the screen in pixels
#define Y_RESOLUTION 200           // The height of the screen in pixels

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void clear_screen();

void put_pixel(int pos_x, int pos_y, unsigned char* color);

#endif