#include "terminal_mode.h"
#include "text_mode.h"
#include "bytes.h"

unsigned char current_command[256];
unsigned char current_directory[256];
unsigned int command_index = 0;
unsigned char command_mode = 1;

// Helper function to compare strings
int compare_string(unsigned char* str1, unsigned char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return 0;
        i++;
    }
    return str1[i] == str2[i];
}

void enable_command_mode() {
    command_mode = 1;
}

void disable_command_mode() {
    command_mode = 0;
}

void terminal_init() {
    for (int i = 0; i < 256; i++) {
        current_command[i] = '\0';
        current_directory[i] = '\0';
    }
    enable_command_mode();
    set_directory((unsigned char*)"/");
    print_directory();
    execute_command("");
}

void set_directory(unsigned char* dir) {
    int i = 0;
    while (dir[i] != '\0' && i < 255) {
        current_directory[i] = dir[i];
        i++;
    }
    current_directory[i] = '\0';
}

void print_directory() {
    if (command_mode) {
        print(current_directory);
        print((unsigned char*)">");
    }
}

void execute_command(unsigned char* command) {
    //TODO: Move to command parser
    if (compare_string(command, (unsigned char*)"help")) {
        disable_command_mode();
        print("no");

        newline();
        enable_command_mode();
    }
    else if (compare_string(command, (unsigned char*)"shutdown")) {
        //TODO: ACPI shutdown
        disable_command_mode();
        print("Shutting down...");

        newline();
        qemu_shutdown();
    }
    // else {
    //     disable_command_mode();
    //     print("Unknown command: ");
    //     print(command);

    //     newline();
    // }


    clear_command();
}

void execute_current_command() {
    unsigned char clean_command[256];
    int i = 0;
    
    // Copy only the valid command characters (up to command_index)
    while (i < command_index && i < 255) {
        clean_command[i] = current_command[i];
        i++;
    }
    clean_command[i] = '\0';
    
    execute_command(clean_command);
}

void clear_command() {
    int i = 0;
    while (i < 256) {
        current_command[i] = '\0';
        i++;
    }
    command_index = 0;
}

//TODO: Move to command file
void qemu_shutdown() {
    outportw(0x604, 0x2000);
}