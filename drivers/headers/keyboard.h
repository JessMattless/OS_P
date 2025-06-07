#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA 0x60
#define KEYBOARD_COMMAND 0x64

#define KEYBOARD_MAX_RETRIES 0x03

struct keyboard_command {
    unsigned char command;
    unsigned char data;
};

extern struct keyboard_command keyboard_queue[256];
extern unsigned char keyboard_queue_pos;

// TODO: Test code, remove this when keyboard is working fully
extern char test;

// https://wiki.osdev.org/PS/2_Keyboard

void keyboard_init();

// Handle inputs given to the CPU from the keyboard
void keyboard_handler(unsigned char data);

// Queue an input to the keyboard
// Data is ignored if command doesn't support it
// See https://wiki.osdev.org/PS/2_Keyboard for a list of commands
void queue_keyboard_command(struct keyboard_command command);
void send_keyboard_command(struct keyboard_command command);

// Send the next queued keyboard input to they keyboard
void send_next_keyboard_command();

#endif