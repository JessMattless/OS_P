#include "screen.h"
#include "bytes.h"
#include "interrupts.h"
#include "keyboard.h"
#include "font.h"

unsigned char screen[CHARS_PER_LINE * MAX_LINES];
unsigned long screen_index = 0;
unsigned long screen_length = 0;

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

    // TODO: Text mode only
    // Clear the text mode screen
    for (int i = 0; i < CHARS_PER_LINE * MAX_LINES; i++) {
        screen[i] = ' ';
    }
}

void draw_screen() {
    for (int row = 0; row < MAX_LINES; row++) {
        for (int col = 0; col < CHARS_PER_LINE; col++) {
            put_char(
                screen[row * CHARS_PER_LINE + col],
                SCREEN_PADDING + (col * (FONT_SIZE + CHARACTER_PADDING)),
                SCREEN_PADDING + (row * (FONT_SIZE + LINE_PADDING)),
                0xFF, 0x00
            );
        }
    }
}

void put_pixel(int pos_x, int pos_y, unsigned char color) {    
    // Create a pointer to the start of video memory
    volatile char* vid_mem = (volatile char*) VIDEO_ADDRESS;

    // Get the pixel offset from the requested row/column
    unsigned long long memory_offset = (SCREEN_WIDTH * pos_y) + pos_x;

    vid_mem[memory_offset] = color;
}

void put_char(unsigned char ch, int x, int y, unsigned char fg, unsigned char bg) {
    unsigned long long glyph = font[ch];

    for (int cy = 0; cy < 8; cy++) {
        for (int cx = 0; cx < 8; cx++) {
            int index = (cy * 8) + cx;
            unsigned char draw_pixel = (glyph >> index) & 1;
            put_pixel(x + cx, y + cy, draw_pixel ? fg : bg);
        }
    }
}

void print_char(unsigned char ch) {
    switch (ch) {
        case 0x00:
            break;
        case '\b': // Backspace
            // TODO: Backspace within string should work properly, moving the string 
            if (screen_index > 0) {
                // If pressing backspace would send the cursor to the previous line
                if ((screen_index % CHARS_PER_LINE) == 0) {
                    int prev_line_end = screen_index - 1;
                    int prev_line_start = prev_line_end - (prev_line_end % CHARS_PER_LINE);

                    // If the last char of the previous line is not a space, just move back one
                    if (screen[prev_line_end] != ' ') {
                        screen_index = prev_line_end;
                    } else {
                        // Scan left for the closest non-space or newline
                        // If none are found, go to the start of the previous line
                        int target = prev_line_start;
                        for (int j = prev_line_end; j >= prev_line_start; j--) {
                            if (screen[j] == '\n') {
                                target = j;
                                break;
                            }
                            if (screen[j] != ' ') {
                                target = j + 1;
                                break;
                            }
                        }
                        screen_index = target;
                    }
                }
                else screen_index--;
                screen[screen_index] = ' ';
            }
            break;
        case 0x7F: // Delete
            break;
        case '\t': // Tab
            break;
        case '\n': // Enter
            screen[screen_index] = ch;
            set_cursor(0, (screen_index / CHARS_PER_LINE) + 1);
            
            // If a newline would put the cursor off screen, scroll the screen
            if (screen_index >= CHARS_PER_LINE * MAX_LINES) scroll_text(1);
            
            break;
        default:
            screen[screen_index] = ch;
            screen_index++;

            // If a new character put the cursor off screen, scroll the screen
            if (screen_index >= CHARS_PER_LINE * MAX_LINES) scroll_text(1);

            break;
    }

    if (screen_index > screen_length) screen_length = screen_index;

    draw_screen();
}

void scroll_text(unsigned char lines) {
    // Clamp lines
    if (lines <= 0 || lines >= MAX_LINES)
        lines = MAX_LINES;

    // Move lines up in the buffer
    for (int row = 0; row < MAX_LINES - lines; row++) {
        for (int col = 0; col < CHARS_PER_LINE; col++) {
            screen[row * CHARS_PER_LINE + col] = screen[(row + lines) * CHARS_PER_LINE + col];
        }
    }

    // Clear the bottom lines in the buffer
    for (int row = MAX_LINES - lines; row < MAX_LINES; row++) {
        for (int col = 0; col < CHARS_PER_LINE; col++) {
            screen[row * CHARS_PER_LINE + col] = ' ';
        }
    }

    screen_index = (MAX_LINES - lines) * CHARS_PER_LINE;

    // Set screen_length, used for detecting the end of the written string
    if (screen_length >= lines * CHARS_PER_LINE)
        screen_length -= lines * CHARS_PER_LINE;
    else
        screen_length = 0;
}

void set_cursor(unsigned short col, unsigned short row) {
    screen_index = (row * CHARS_PER_LINE) + col;
}

void draw_cursor() {
    // unsigned char ch;
    // if (((unsigned int)GPT / 9) % 2 == 0) ch = '_';
    // else ch = ' ';
    // put_char(ch, SCREEN_PADDING + ((screen_index % CHARS_PER_LINE) * (FONT_SIZE + CHARACTER_PADDING)), SCREEN_PADDING + ((screen_index / CHARS_PER_LINE) * (FONT_SIZE + LINE_PADDING)), 0xFF, 0x00);

    unsigned char ch = screen[screen_index];

    if (((unsigned int)GPT / 9) % 2 == 0) 
        put_char(ch, SCREEN_PADDING + ((screen_index % CHARS_PER_LINE) * (FONT_SIZE + CHARACTER_PADDING)), SCREEN_PADDING + ((screen_index / CHARS_PER_LINE) * (FONT_SIZE + LINE_PADDING)), 0x00, 0xFF);
    else
        put_char(ch, SCREEN_PADDING + ((screen_index % CHARS_PER_LINE) * (FONT_SIZE + CHARACTER_PADDING)), SCREEN_PADDING + ((screen_index / CHARS_PER_LINE) * (FONT_SIZE + LINE_PADDING)), 0xFF, 0x00);
}