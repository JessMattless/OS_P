#define VIDEO_ADDRESS 0xb8000 // The video address in memory
#define MAX_ROWS 25           // The max amount of rows of text on-screen
#define MAX_COLS 80           // The max amount of cols of text on-screen
#define WHITE_ON_BLACK 0x0f   // The attribute byte for text colour

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

int get_screen_offset(int col, int row);
void print_char(const char character, int col, int row, char attribute_byte);
int get_cursor();
void set_cursor(int offset);
void print_at(const char* string, int col, int row);
void print(const char* string);