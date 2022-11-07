#include "screen.h"

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);

// Print a character on screen at col, row or cursor position
void print_char(const char character, const int col, const int row, char attribute_byte) {
    // Create a byte (char) pointer to the start of video memory
    volatile char* vidmem = (volatile char*) VIDEO_ADDRESS;

    // If the attribute byte is 0x00, assume the default style
    if (!attribute_byte) {
        attribute_byte = WHITE_ON_BLACK;
    }

    // Get the offset of video memory for the screen location
    int offset;
    // If col and row are >= 0 use them for the offset
    if (col >= 0 && row >= 0) {
        offset = get_screen_offset(col, row);
    }
    // Else use the current cursor position
    else { 
        offset = get_cursor();
    }

    // If there is a newline character, set the offset to the end
    // of the current row, as to start a new line on the next row
    if (character == 0x0A) {
        // int rows = offset / (2*MAX_COLS);
        // offset = get_screen_offset(1, rows);

        int newRowOffset = (2*MAX_COLS) - offset;
        offset += newRowOffset;
    }
    // Else write the character and it's attributes to the current
    // location in video memory at the offset.
    else {
        // vidmem[offset] = character;
        // vidmem[offset+1] = attribute_byte;
        vidmem += offset;
        *vidmem = character;
        vidmem++;
        *vidmem = attribute_byte;
    }

    // Update the offset to the next character byte, two bytes ahead
    // of the current position
    offset += 2;
    // Make scrolling adjustment, for when we reach the bottom of the screen
    // offset = handle_scrolling(offset);
    // Update the cursor position on the screen
    set_cursor(offset);
}

// TODO, use the wiki the PDF is no longer viable
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

void print_at(const char *string, const int col, const int row) {
    // prints all characters in a string

    // If col and row aren't negative, place cursor at destination specified
    // Else continue normally
    if (col >= 0 && row >= 0) {
        set_cursor(get_screen_offset(col, row));
    }

    // Print the first character in the string at the specified col/row
    // xp /5cb 0x2180
    // Above is the command that should show in memory 
    const char* theChar = (char*)&string;
    // const char x = test[0];
    // char* theChar = (char*)0x2180;
    print_char(*theChar, col, row, WHITE_ON_BLACK);
    theChar++;

    // print_char(*string, col, row, WHITE_ON_BLACK);
    // string++;

    // Loop through the remaining characters in the string and place them at
    // the next location in video memory
    while(*theChar != 0x00) {
        print_char(*theChar, -1, -1, WHITE_ON_BLACK);
        
        theChar++;
    }
}

void print(const char *string) {
    // Shorthand method of calling print_at with a col and row of -1
    print_at(string, -1, -1);
}

void clear_screen() {
    int row = 0;
    int col = 0;

    for(row = 0; row<MAX_ROWS; row++) {
        for(col = 0; col<MAX_COLS;col++) {
            print_char(0x00, col, row, WHITE_ON_BLACK);
        }
    }
    set_cursor(get_screen_offset(1, 0));
}