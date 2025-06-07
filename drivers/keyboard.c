#include "keyboard.h"
#include "bytes.h"
#include "interrupts.h"

// Queue for keyboard commands
// TODO: Make a better way of making this queue
struct keyboard_command keyboard_queue[256];
unsigned char keyboard_queue_pos = 0;

// Keyboard modes
enum {
    KB_DEFAULT,
    KB_WAITING,
} keyboard_state = KB_DEFAULT;

char keyboard_timeout = 0;
char keyboard_retries = 0;

// TODO: Test code, remove this when keyboard is working fully
char test = 0;

// These variable names are long, might change them later
unsigned char keyboard_expected_response_count = 0;
unsigned char keyboard_current_response_count = 0;
char keyboard_response_bytes[2];

void keyboard_init() {
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
    if ((data == 0xFA && keyboard_state != KB_WAITING) || (current_command.command == 0xFE && data != 0xFE) || keyboard_retries >= KEYBOARD_MAX_RETRIES) {
        keyboard_retries = 0;

        keyboard_timeout = GPT + 10;
        switch (current_command.command) {
            // 0xF2, Identify keyboard, should output 0-2 bytes
            case 0xF2:
                keyboard_expected_response_count = 2;
                keyboard_state = KB_WAITING;
                break;
            default: 
                break;
        }

        // If current command acknowledged, move onto next command
        keyboard_queue[keyboard_queue_pos].command = 0x00;
        keyboard_queue[keyboard_queue_pos].data = 0x00;

        if (keyboard_queue_pos != 0)
        keyboard_queue_pos--;

        send_next_keyboard_command();
    }
    // If 0xFE, resend the last command
    else if (data == 0xFE) {
        keyboard_retries++;
        send_next_keyboard_command();
    }

    else {
        switch (keyboard_state) {
            case KB_WAITING:
                keyboard_response_bytes[keyboard_current_response_count++] = data;
                if (keyboard_current_response_count == keyboard_expected_response_count || GPT > keyboard_timeout) {
                    keyboard_state = KB_DEFAULT;
                    send_next_keyboard_command();
                }
                break;
            default: case KB_DEFAULT:
                // TODO: Test code, remove this
                // Escape Key
                if (data == 0x01) test = 1;
                // 1 Key
                if (data == 0x02) test = 0;
                break;
        }
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