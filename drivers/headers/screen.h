#ifndef SCREEN_H
#define SCREEN_H


#define VIDEO_ADDRESS 0xA0000       // The video address in memory
#define SCREEN_WIDTH 320            // The width of the screen in pixels
#define SCREEN_HEIGHT 200           // The height of the screen in pixels

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

// VGA Palette Registers
#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

// Text Mode Padding
#define CHARACTER_PADDING 0x02
#define LINE_PADDING 0x02
#define SCREEN_PADDING 0x02

// Text Mode screen size for characters
#define CHARS_PER_LINE 32
#define MAX_LINES 20

#define FONT_SIZE 0x08

struct RGB {
    unsigned char r, g, b;
};

extern unsigned char screen[CHARS_PER_LINE * MAX_LINES];
extern unsigned long screen_index;
extern unsigned long screen_length;

void screen_init();

// Set all pixels on screen to black
// (Text Mode Only) Clear the screen character buffer
void clear_screen();

// (Text Mode)
// Draw any characters in the screen character buffer to the screen
void draw_screen();

// Put a pixel of the specified color at the specified position
void put_pixel(int pos_x, int pos_y, unsigned char color);

// Configure the VGA mode palette
void configure_palette(long index, struct RGB rgb);

// (Text Mode)
// NOTE: If wanting to put a character in a specific place, adjust the cursor and use print_char() instead
// Puts a character at the specified position on screen
// The char is defined using the ASCII standard
void put_char(unsigned char ch, int x, int y, unsigned char fg, unsigned char bg);

// (Text Mode)
// Put a character at the current cursor position on the screen
void print_char(unsigned char ch);

// (Text Mode)
// Scroll the screen the given number of lines
// Adjust the cursor accordingly
void scroll_text(unsigned char lines);

// (Text Mode)
// Set the cursor on a specific col/row of the screen
void set_cursor(unsigned short col, unsigned short row);

// (Text Mode)
// Draw the cursor, depending on the current GPT state
// The cursor flashes every ~.5 seconds
void draw_cursor();

#endif