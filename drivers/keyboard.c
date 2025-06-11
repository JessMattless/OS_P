#include "keyboard.h"
#include "bytes.h"
#include "interrupts.h"
#include "screen.h"

// https://wiki.osdev.org/PS/2_Keyboard

// Queue for keyboard commands
// TODO: Make a better way of making this queue
struct keyboard_command keyboard_queue[256];
unsigned char keyboard_queue_pos = 0;

struct keyboard kb;

// Keyboard modes
enum {
    KB_DEFAULT,
    KB_WAITINGCMD,
    KB_WAITINGSCAN
} keyboard_state = KB_DEFAULT;

char keyboard_timeout = 0;
char keyboard_retries = 0;

// These variable names are long, might change them later
unsigned char keyboard_expected_response_count = 0;
unsigned char keyboard_current_response_count = 0;
unsigned char keyboard_response_bytes[KEYBOARD_MAX_RESPONSES];

void keyboard_init() {
    // Fill out the command table
    for (unsigned char i = 0; i < 256; i++) {
        struct keyboard_command stub_command = {0x00, 0x00};
        keyboard_queue[i] = stub_command;
    }

    // Disable sending scancodes
    queue_keyboard_command((struct keyboard_command){0xF5, 0});
    // Identify keyboard
    queue_keyboard_command((struct keyboard_command){0xF2, 0});
    // Re-enable scanning
    queue_keyboard_command((struct keyboard_command){0xF4, 0});
}

void keyboard_handler(unsigned char data) {
    struct keyboard_command current_command = keyboard_queue[keyboard_queue_pos];
    // If sent 0xFA (acknowledged last command), move to the next command in the queue
    // Unless the keyboard is currently waiting for a specific reply
    // Special case for command 0xFE as it doesn't return 0xFA on success
    // If a command fails, retry a maximum of 3 times before giving up
    if ((data == 0xFA && keyboard_state == KB_DEFAULT) || (current_command.command == 0xFE && data != 0xFE) || keyboard_retries >= KEYBOARD_MAX_RETRIES) {
        keyboard_retries = 0;

        keyboard_timeout = GPT + 10;
        switch (current_command.command) {
            // 0xF2, Identify keyboard, should output 0-2 bytes
            case 0xF2:
                keyboard_expected_response_count = 2;
                keyboard_state = KB_WAITINGCMD;
                reset_keyboard_response_bytes();
                break;
            default:
                break;
        }
            
            // If current command acknowledged, move onto next command
            keyboard_queue[keyboard_queue_pos].command = 0x00;
            keyboard_queue[keyboard_queue_pos].data = 0x00;
            
            // If not at the start of the queue, move to the next command
            if (keyboard_queue_pos != 0) keyboard_queue_pos--;
            
            send_next_keyboard_command();
    }
    // If 0xFE, resend the last command
    else if (data == 0xFE) {
        keyboard_retries++;
        send_next_keyboard_command();
    }
    // Else we have received a scancode
    else {
        switch (keyboard_state) {
            case KB_WAITINGCMD: case KB_WAITINGSCAN:
                // If we are waiting for 1 or more bytes of a multi-byte scancode
                keyboard_response_bytes[keyboard_current_response_count++] = data;
                if (keyboard_current_response_count == keyboard_expected_response_count || GPT > keyboard_timeout) {
                    // If the scancode is complete, send it off to be handled by the driver, and start the next command
                    keyboard_state = KB_DEFAULT;
                    keyboard_current_response_count = 0;

                    if (keyboard_state == KB_WAITINGSCAN) handle_keycode(translate_scancode(keyboard_response_bytes));

                    send_next_keyboard_command();
                }
                break;
            default: case KB_DEFAULT:
                switch (data) {
                    // Extended Byte
                    case 0xE0:
                        // Up to 4 responses, normally 2 except for print screen
                        keyboard_expected_response_count = 4;
                        keyboard_state = KB_WAITINGSCAN;
                        // Explicitly set the first response byte to 0xE0
                        keyboard_response_bytes[0] = 0xE0;
                        keyboard_current_response_count = 1;
                        break;
                    // Special Byte
                    case 0xE1:
                        // Up to 6 responses, looking for "pause" button
                        keyboard_expected_response_count = 6;
                        keyboard_state = KB_WAITINGSCAN;
                        // Explicitly set the first response byte to 0xE1
                        keyboard_response_bytes[0] = 0xE1;
                        keyboard_current_response_count = 1;
                        break;
                    // Single-Byte scancodes
                    default:
                        handle_keycode(data);
                        break;
                }
                break;
            }
            
        set_keyboard_key_transition_states();
        send_next_keyboard_command();
    }
}

void queue_keyboard_command(struct keyboard_command command) {
    struct keyboard_command *current_command = &keyboard_queue[keyboard_queue_pos];

    if (keyboard_queue_pos != 0 || current_command->command != 0)
        keyboard_queue_pos++;
    *current_command = command;
}

void send_keyboard_command(struct keyboard_command command) {
    outportb(KEYBOARD_COMMAND, command.command);
    wait();
    // Only send data bytes if the command is expecting them
    if (command.command == 0xED || command.command == 0xF0 || command.command == 0xF3 || command.command == 0xFB || command.command == 0xFC || command.command == 0xFD)
        outportb(KEYBOARD_DATA, command.data);
}

void send_next_keyboard_command() {
    send_keyboard_command(keyboard_queue[keyboard_queue_pos]);
}

void reset_keyboard_response_bytes() {
    for (int i = 0; i < KEYBOARD_MAX_RESPONSES; i++) {
        keyboard_response_bytes[i] = 0;
    }
}

unsigned short translate_scancode(unsigned char scancode[KEYBOARD_MAX_RESPONSES]) {
    // Unique key for Pause/Break
    if (scancode[0] == 0xE1) return 0x200;
    // Extended Keys
    else if (scancode[0] == 0xE0) return 0x100 + scancode[1];
    // Single-byte scancodes
    else return scancode[0];
}

void handle_keycode(unsigned short keycode) {
    if (keycode > 0x200) return; // Out of Range
    
    enum keyboard_layout layout_key = scancode_map[keycode];
    if (layout_key == KEY_NULL) return; // Not mapped

    // If keycode is a mapping code
    if (keycode < 0x80 || (keycode >= 0x100 && (keycode & 0x7F) < 0x80)) {
        kb.keys[layout_key] = 1;


        // TODO: Sort out key flags
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

void set_keyboard_key_transition_states() {
    for (int k = 0; k < sizeof(kb.keys); k++) {
        kb.prev_keys[k] = kb.keys[k];
    }
}

unsigned char get_keyboard_key_held(enum keyboard_layout key) {
    if (kb.keys[key] && kb.prev_keys[key]) return 1;
    return 0;
}

unsigned char get_ascii(enum keyboard_layout key) {
    unsigned char isShift = (kb.key_flags & KEYBOARD_SHIFT_FLAG) == 0x08;
    unsigned char isAlt = (kb.key_flags & KEYBOARD_ALT_FLAG) == 0x02;
    switch (key) {
        case KEY_A: return (isShift) ? 'A' : 'a';
        case KEY_B: return (isShift) ? 'B' : 'b';
        case KEY_C: return (isShift) ? 'C' : 'c';
        case KEY_D: return (isShift) ? 'D' : 'd';
        case KEY_E: return (isShift) ? 'E' : 'e';
        case KEY_F: return (isShift) ? 'F' : 'f';
        case KEY_G: return (isShift) ? 'G' : 'g';
        case KEY_H: return (isShift) ? 'H' : 'h';
        case KEY_I: return (isShift) ? 'I' : 'i';
        case KEY_J: return (isShift) ? 'J' : 'j';
        case KEY_K: return (isShift) ? 'K' : 'k';
        case KEY_L: return (isShift) ? 'L' : 'l';
        case KEY_M: return (isShift) ? 'M' : 'm';
        case KEY_N: return (isShift) ? 'N' : 'n';
        case KEY_O: return (isShift) ? 'O' : 'o';
        case KEY_P: return (isShift) ? 'P' : 'p';
        case KEY_Q: return (isShift) ? 'Q' : 'q';
        case KEY_R: return (isShift) ? 'R' : 'r';
        case KEY_S: return (isShift) ? 'S' : 's';
        case KEY_T: return (isShift) ? 'T' : 't';
        case KEY_U: return (isShift) ? 'U' : 'u';
        case KEY_V: return (isShift) ? 'V' : 'v';
        case KEY_W: return (isShift) ? 'W' : 'w';
        case KEY_X: return (isShift) ? 'X' : 'x';
        case KEY_Y: return (isShift) ? 'Y' : 'y';
        case KEY_Z: return (isShift) ? 'Z' : 'z';
        case KEY_1: return (isShift) ? '!' : '1';
        case KEY_2: return (isShift) ? '"' : '2';
        case KEY_3: return (isShift) ? 0x9C : '3';
        case KEY_4: return (isAlt) ? 0x9B : (isShift) ? '$' : '4';
        case KEY_5: return (isShift) ? '%' : '5';
        case KEY_6: return (isShift) ? '^' : '6';
        case KEY_7: return (isShift) ? '&' : '7';
        case KEY_8: return (isShift) ? '*' : '8';
        case KEY_9: return (isShift) ? '(' : '9';
        case KEY_0: return (isShift) ? ')' : '0';
        case KEY_SPACE: return ' ';
        case KEY_ENTER: return '\n';
        case KEY_TAB: return '\t';
        case KEY_BACKSPACE: return '\b';
        case KEY_MINUS: return (isShift) ? '_' : '-';
        case KEY_EQUALS: return (isShift) ? '+' : '=';
        case KEY_SEMICOLON: return (isShift) ? ':' : ';';
        case KEY_APOSTROPHE: return (isShift) ? '@' : '\'';
        case KEY_HASHTAG: return (isShift) ? '~' : '#';
        case KEY_GRAVE: return (isAlt) ? 0x7F : (isShift) ? 0xAA : '`';
        case KEY_COMMA: return (isShift) ? '<' : ',';
        case KEY_DOT: return (isShift) ? '>' : '.';
        case KEY_SLASH: return (isShift) ? '?' : '/';
        case KEY_BACKSLASH: return (isShift) ? '|' : '\\';
        case KEY_OPEN_BRACKET: return (isShift) ? '{' : '[';
        case KEY_CLOSE_BRACKET: return (isShift) ? '}' : ']';

        default: return 0;
    }
}