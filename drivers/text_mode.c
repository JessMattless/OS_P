#include "text_mode.h"
#include "font.h"
#include "screen.h"
#include "interrupts.h"
#include "terminal_mode.h"

unsigned char screen[CHARS_PER_LINE * MAX_LINES];
unsigned long screen_index = 0;
unsigned long screen_length = 0;

void clear_text() {
    // Clear the text mode screen
    for (int i = 0; i < CHARS_PER_LINE * MAX_LINES; i++) {
        screen[i] = ' ';
    }
}

void draw_text() {
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
            //TODO: Backspace should not be able to delete past the prompt in terminal mode
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

                if (command_index > 0) {
                    command_index--;
                    current_command[command_index] = 0;
                }
            }
            break;
        case 0x7F: // Delete
            break;
        case '\t': // Tab
            break;
        case '\n': // Enter
            screen[screen_index] = ch;
            set_text_cursor(0, (screen_index / CHARS_PER_LINE) + 1);
            
            // If a newline would put the cursor off screen, scroll the screen
            if (screen_index >= CHARS_PER_LINE * MAX_LINES) scroll_text(1);

            if (current_screen_mode == TERMINAL_MODE) {
                current_command[command_index] = 0;
                execute_current_command();
                print_directory();
                command_index = 0;
            }
            
            break;
        default:
            screen[screen_index] = ch;
            screen_index++;

            if (command_index < 255) {
                current_command[command_index] = ch;
                command_index++;
            }

            // If a new character put the cursor off screen, scroll the screen
            if (screen_index >= CHARS_PER_LINE * MAX_LINES) scroll_text(1);

            break;
    }

    if (screen_index > screen_length) screen_length = screen_index;

    draw_text();
}

void print(unsigned char* text) {
    while (*text != 0x00) {
        print_char(*text);
        text++;
    }
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

void set_text_cursor(unsigned short col, unsigned short row) {
    screen_index = (row * CHARS_PER_LINE) + col;
}

void draw_text_cursor() {
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

void handle_text_input(unsigned short keycode) {
    enum keyboard_layout layout_key = scancode_map[keycode];

    // Check for key press, discard if released
    if (keycode < 0x180) {
        // Directional key input for text mode
        switch (layout_key) {
            case KEY_LEFT:
                if (screen_index > 0) {
                    // If pressing left would put the cursor on the previous line
                    if ((screen_index % CHARS_PER_LINE) == 0) {
                        int prev_line_end = screen_index - 1;
                        int prev_line_start = prev_line_end - (prev_line_end % CHARS_PER_LINE);

                        // If the last char of the previous line is not a space, just move back one
                        if (screen[prev_line_end] != ' ') {
                            screen_index--;
                        }
                        else {
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
                    } else screen_index--;
                    draw_text();
                    draw_text_cursor();
                }
                return;
            case KEY_RIGHT:
                // TODO: Get this working with reducing screen_length when backspacing
                if (screen_index < (MAX_LINES * CHARS_PER_LINE) - 1 && screen_index < screen_length) {
                    if (screen[screen_index] == '\n') screen_index += CHARS_PER_LINE - (screen_index % CHARS_PER_LINE);
                    else screen_index++;
                    draw_text();
                    draw_text_cursor();
                }
                return;
            case KEY_UP:
                // TODO: Make this work with newlines properly
                if (screen_index > CHARS_PER_LINE - 1) {
                    screen_index -= CHARS_PER_LINE;
                    draw_text();
                    draw_text_cursor();
                }
                return;
            case KEY_DOWN:
                // TODO: Make this work with newlines properly
                if (screen_index < (MAX_LINES * CHARS_PER_LINE) - CHARS_PER_LINE - 1 && screen_index + CHARS_PER_LINE < screen_length) {
                    screen_index += CHARS_PER_LINE;
                    draw_text();
                    draw_text_cursor();
                }
                return;
            default: break;
        }
    }

    // If keycode is a mapping code
    if (keycode < 0x80 || (keycode >= 0x100 && (keycode & 0x7F) < 0x80)) {
        kb.keys[layout_key] = 1;

        // Set key flags
        if (layout_key == KEY_LSHIFT || layout_key == KEY_RSHIFT) kb.key_flags |= 0b00001000;
        else if (layout_key == KEY_LCTRL || layout_key == KEY_RCTRL) kb.key_flags |= 0b00000100;
        else if (layout_key == KEY_LALT || layout_key == KEY_RALT) kb.key_flags |= 0b00000010;
        else if (layout_key == KEY_LGUI || layout_key == KEY_RGUI) kb.key_flags |= 0b00000001;

        if (!get_keyboard_key_held(layout_key)) print_char(get_ascii(layout_key));
    }
    // Else it's a breaking code
    else {
        kb.keys[layout_key] = 0;

        // Break key flags
        if (layout_key == KEY_LSHIFT || layout_key == KEY_RSHIFT) kb.key_flags &= 0b11110111;
        else if (layout_key == KEY_LCTRL || layout_key == KEY_RCTRL) kb.key_flags &= 0b11111011;
        else if (layout_key == KEY_LALT || layout_key == KEY_RALT) kb.key_flags &= 0b11111101;
        else if (layout_key == KEY_LGUI || layout_key == KEY_RGUI) kb.key_flags &= 0b11111110;
    }
}

void newline() {
    print_char('\n');
}