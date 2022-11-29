#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VIDEO_ADDRESS 0xb8002 // The video address in memory
#define MAX_ROWS 25           // The max amount of rows of text on-screen
#define MAX_COLS 80           // The max amount of cols of text on-screen
// #define WHITE_ON_BLACK 0x0f   // The attribute byte for text colour

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void terminal_init();
void clear_screen();
void print_logo();

void put_char(char ch, char color, int col, int row);
void print_char(char ch);

void put_string(char* string, char color, int col, int row);
void print(char* string);

int get_screen_offset(int col, int row);
int get_cursor();
void set_cursor(int offset);

#endif