#ifndef TEXT_MODE_H
#define TEXT_MODE_H

// Text Mode Padding
#define CHARACTER_PADDING 0x02
#define LINE_PADDING 0x02
#define SCREEN_PADDING 0x02

// Text Mode screen size for characters
#define CHARS_PER_LINE 32
#define MAX_LINES 20

#define FONT_SIZE 0x08

extern unsigned char screen[CHARS_PER_LINE * MAX_LINES];
extern unsigned long screen_index;
extern unsigned long screen_length;

// Clear the screen character buffer
void clear_text();

// Draw any characters in the screen character buffer to the screen
void draw_text();

// NOTE: If wanting to put a character in a specific place, adjust the cursor and use print_char() instead
// Puts a character at the specified position on screen
// The char is defined using the ASCII standard
void put_char(unsigned char ch, int x, int y, unsigned char fg, unsigned char bg);

// Put a character at the current cursor position on the screen
void print_char(unsigned char ch);

void print(unsigned char* text);

// Scroll the screen the given number of lines
// Adjust the cursor accordingly
void scroll_text(unsigned char lines);

// Set the cursor on a specific col/row of the screen
void set_text_cursor(unsigned short col, unsigned short row);

// Draw the cursor, depending on the current GPT state
// The cursor flashes every ~.5 seconds
void draw_text_cursor();

void handle_text_input(unsigned short keycode);

void newline();

#endif