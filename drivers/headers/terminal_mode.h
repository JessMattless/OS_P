#ifndef TERMINAL_MODE_H
#define TERMINAL_MODE_H

extern unsigned char current_directory[256];
extern unsigned char current_command[256];
extern unsigned int command_index;

// If command_mode is 1, the terminal will print the directory on a newline
extern unsigned char command_mode;

void enable_command_mode();
void disable_command_mode();

void terminal_init();

void set_directory(unsigned char* dir);

void print_directory();

void execute_command(unsigned char* command);

void execute_current_command();

void clear_command();

void qemu_shutdown();

#endif