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

#define CHARACTER_PADDING 0x02
#define LINE_PADDING 0x02
#define SCREEN_PADDING 0x02

#define CHARS_PER_LINE 32
#define MAX_LINES 20

#define FONT_SIZE 0x08

struct RGB {
    unsigned char r, g, b;
};

extern unsigned char screen[CHARS_PER_LINE * MAX_LINES];
extern unsigned char screen_index;

void screen_init();
void clear_screen();

void put_pixel(int pos_x, int pos_y, unsigned char color);
void configure_palette(long index, struct RGB rgb);

// Puts a character at the specified position on screen
// The char is defined using the ASCII standard
void put_char(unsigned char ch, int x, int y, unsigned char fg, unsigned char bg);
void print_char(unsigned char ch);

void set_cursor(unsigned short col, unsigned short row);
void draw_cursor();

#endif