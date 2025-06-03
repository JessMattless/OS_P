#include "screen.h"
#include "bytes.h"

enum vga_color {
    COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15
};

static inline char get_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

void terminal_init() {
    clear_screen();
    
    //TODO: Print things like logo and info to screen.
}

void clear_screen() {
    char color = get_color(COLOR_WHITE, COLOR_BLACK);
    for(int row = 0; row < MAX_ROWS; row++) {
        for(int col = 0; col < MAX_COLS; col++) {
            put_char(' ', color, col, row);
        }
    }
}

void put_char(char ch, char color, int col, int row) {    
    if (ch == 0x00) ch = ' '; // If the character is null, replace it with a space

    // Create a pointer to the start of video memory
    volatile char* video_memory = (volatile char*) VIDEO_ADDRESS;

    // The offset of video memory for the screen location
    int offset;
    // If col and row are >= 0 use them for the offset
    if (col >= 0 && row >= 0) offset = get_screen_offset(col, row);
    // Else use the current cursor position
    else offset = get_cursor();
    
    switch (ch) {
        case 0x00: // Null
            ch = ' ';
        case 0x08: // Backspace
            offset -= 2;
            video_memory[offset] = ' ';
            break;
        case 0x09: // Tab
            int currentCol = (offset / 2) % MAX_COLS;
            int tabOffset = (currentCol % 8);
            for (int i = 0; i < 8 - tabOffset; i++) {
                video_memory[offset] = ' ';
                video_memory[offset + 1] = color;
                offset += 2;
            }
            break;
        case 0x0A: // Line Feed
            int adjustedOffset = offset / 2;
            adjustedOffset += MAX_COLS - (adjustedOffset % MAX_COLS);
            offset = adjustedOffset * 2;
            break;
        case 0x0D: // Carriage Return
        default: // Most characters
            video_memory[offset] = ch;
            video_memory[offset + 1] = color;
    
            offset += 2;
            break;
    }


    // Update the cursor position on the screen
    set_cursor(offset);
}

void print_char(char ch) {
    put_char(ch, get_color(COLOR_WHITE, COLOR_BLACK), -1, -1);
}

void put_string(const char* string, char color, int col, int row) {
    // prints all characters in a string

    // If col and row aren't negative, place cursor at destination specified
    // Else continue normally
    if (col >= 0 && row >= 0) {
        set_cursor(get_screen_offset(col, row));
    }

    while(*string != 0x00) {
        put_char(*string, color, -1, -1);
        string++;
    }
}

void print(const char* string) {
    put_string(string, get_color(COLOR_WHITE, COLOR_BLACK), -1, -1);
}

int get_screen_offset(int col, int row) {
    // Calculate the screen offset for the cursor using the column and row given
    // multiply the result by 2 because each character takes up 2 bytes
    // (one for the ASCII character and another for the details of the character
    // E.g. foreground/background colour)

    // unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    int screen_offset = ((row * MAX_COLS) + col) * 2;
    return screen_offset;
}

int get_cursor() {
    // The cursor device uses its control register as an index
    // to select its own internal registers, the ones we need for this are:
    // reg 14: The high byte of the cursor's offset
    // reg 15: The low byte of the cursor's offset
    // Once the register has been selected, it can be read from or written to.

    port_byte_out(REG_SCREEN_CTRL, 14);
    int high = port_byte_in(REG_SCREEN_DATA);
    port_byte_out(REG_SCREEN_CTRL, 15);
    int low = port_byte_in(REG_SCREEN_DATA);

    int offset = (high << 8) | low;

    //Because the cursor offset reported by VGA is the number of characters
    // the number must be multiplied by 2 to account for the character cell offset
    return offset * 2;
}

void set_cursor(int offset) {
    offset /= 2;
    // Because the character cell offset is two bytes, it must be divided
    // by 2 to get the character offset.

    // Similar to get_cursor, except we write to the internal device
    // registers instead of reading from them.

    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xFF));
}

void scroll_screen(int lines) {
    // For now, this just discards the data in the top row of the terminal, and moves everything up by 1 line
    // TODO: Allow scrolling in both directions, will require saving the information scrolled so you can display it again

    int offset = get_cursor() / 2; // Get cursor in terminal space
    volatile char* video_memory = (volatile char*) VIDEO_ADDRESS;

    if (lines < 1) lines = 1; // Added for now to prevent scrolling
    
    if (offset + (MAX_COLS * 2) > MAX_COLS * MAX_ROWS) {
        // if the cursor is on the bottom 2 rows of the terminal, set the minimum scroll to 2 lines, to give a buffer at the bottom of the screen.
        if (lines < 2) lines = 2;
    }

    int scrollOffset = 0;

    // For each character in the terminal excluding the lines to be sent offscreen at the top
    for (int i = MAX_COLS * lines; i < (MAX_COLS * MAX_ROWS); i++) {
        unsigned short chData = get_char_data(i*2);
        // put_char(chData & 0xFF, chData >> 8, -1, -1);

        video_memory[scrollOffset] = chData & 0xFF;
        video_memory[scrollOffset + 1] = chData >> 8;

        scrollOffset += 2;
    }

    offset -= MAX_COLS * lines;
    set_cursor(offset * 2);
}

// To get data back: 
// char: chData & 0xFF
// color: chData >> 8
unsigned short get_char_data(int offset) {
    volatile char* video_memory = (volatile char*) VIDEO_ADDRESS;

    // take the specified character and corresponding color, and add it to a short to be returned.
    unsigned short chData = ((video_memory[offset + 1] << 8) & 0xFF00) | (video_memory[offset] & 0xFF);

    return chData;
}