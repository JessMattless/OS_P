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

// For some reason this doesn't work
// char* video_memory = (char*) VIDEO_ADDRESS;

static inline char get_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

void terminal_init() {
    clear_screen();
    // print_logo();
}

void clear_screen() {
    char color = get_color(COLOR_WHITE, COLOR_BLACK);
    for(int row = 0; row < MAX_ROWS; row++) {
        for(int col = 0; col < MAX_COLS; col++) {
            put_char(0x00, color, col, row);
        }
    }
    set_cursor(get_screen_offset(0, 0));
}

void print_logo() {
    // char str[] = {'I', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    // char str[] = "Hello";
    // char color = get_color(COLOR_WHITE, COLOR_BLACK);
    // for (int i = 0; i < 15; i++) {
    //     for (int j = 0; j < 45; j++) {
            // print_char(str[0]);
        // }
    // }
}

void put_char(char ch, char color, int col, int row) {
    // Create a pointer to the start of video memory
    volatile char* video_memory = (volatile char*) VIDEO_ADDRESS;

    // The offset of video memory for the screen location
    int offset;
    // If col and row are >= 0 use them for the offset
    if (col >= 0 && row >= 0) offset = get_screen_offset(col, row);
    // Else use the current cursor position
    else offset = get_cursor();

    video_memory += offset;
    *video_memory = ch;
    video_memory++;
    *video_memory = color;

    // Update the offset to the next character byte, two bytes ahead
    // of the current position
    offset += 2;
    // Make scrolling adjustment, for when we reach the bottom of the screen
    // offset = handle_scrolling(offset);
    // Update the cursor position on the screen
    set_cursor(offset);
}

void print_char(char ch) {
    put_char(ch, get_color(COLOR_WHITE, COLOR_BLACK), -1, -1);
}

void put_string(char* string, char color, int col, int row) {
    // prints all characters in a string

    // If col and row aren't negative, place cursor at destination specified
    // Else continue normally
    if (col >= 0 && row >= 0) {
        set_cursor(get_screen_offset(col, row));
    }

    // Print the first character in the string at the specified col/row
    // put_char(*string, color, col, row);
    // string++;

    // char* str = string;

    // char str[] = "There";
    char* point;
    point = string;
    point++;

    // Loop through the remaining characters in the string and place them at
    // the next location in video memory
    while(*point != 0x00) {
        put_char(*point, color, -1, -1);
        point++;
    }
    // while(*string != 0x00) {
    //     put_char(*string, color, -1, -1);
    //     string++;
    // }
}

void print(char* string) {
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

    port_byte_out(REG_SCREEN_CTRL, 15);
    int offset = port_byte_in(REG_SCREEN_DATA);
    port_byte_out(REG_SCREEN_CTRL, 14);
    offset += port_byte_in(REG_SCREEN_DATA) << 8;

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
    port_byte_out(REG_SCREEN_DATA, offset);
}

// void print_at(const char *string, const int col, const int row) {
//     // prints all characters in a string

//     // If col and row aren't negative, place cursor at destination specified
//     // Else continue normally
//     if (col >= 0 && row >= 0) {
//         set_cursor(get_screen_offset(col, row));
//     }

//     // Print the first character in the string at the specified col/row

//     // xp /5cb 0x2180
//     // Above is the command that should show in memory 

//     // const char* theChar = string;

//     // string += 0x11a4; // 0x11c8 0x11ca

//     // if (string < (char*)0x2180) {
//     //     print_char('<', 1, 0, WHITE_ON_BLACK);
//     // }
//     // else if (string > (char*)0x2180) {
//     //     print_char('>', 1, 0, WHITE_ON_BLACK);
//     // }
//     // else if (string == (char*)0x2180) {
//     //     print_char('=', 1, 0, WHITE_ON_BLACK);
//     // }

//     // string += 0x11a3;
//     string += 0x11a3 + 25 + 25;

//     // string += 0x11b9;
//     // string += 0x11b9 + 25;
//     // string += 0x11d2; // 0x11cc + 6

//     // string += 6;

//     print_char(*string, col, row, WHITE_ON_BLACK);
//     string++;

//     // Loop through the remaining characters in the string and place them at
//     // the next location in video memory
//     while(*string != 0x00) {
//         print_char(*string, -1, -1, WHITE_ON_BLACK);

//         string++;
//     }
// }

// void print(const char *string) {
//     // Shorthand method of calling print_at with a col and row of -1
//     print_at(string, -1, -1);
// }