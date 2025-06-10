#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA 0x60
#define KEYBOARD_COMMAND 0x64

#define KEYBOARD_MAX_RETRIES 0x03

#define KEYBOARD_MAX_RESPONSES 0x06

// #define KEYBOARD_SHIFT_FLAG 0b00001000
// #define KEYBOARD_CTRL_FLAG 0b00000100
// #define KEYBOARD_ALT_FLAG 0b00000010
// #define KEYBOARD_GUI_FLAG 0b00000001

// Note: This is the ISO105 Layout of a UK keyboard.
enum keyboard_layout {
    KEY_NULL,
    KEY_ESCAPE,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_MINUS,
    KEY_EQUALS,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_OPEN_BRACKET, //Open Square Bracket
    KEY_CLOSE_BRACKET, //Close Square Bracket
    KEY_ENTER,
    KEY_LCTRL,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_GRAVE, // to the left of 1
    KEY_LSHIFT,
    KEY_HASHTAG,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_COMMA,
    KEY_DOT,
    KEY_SLASH,
    KEY_RSHIFT,
    KEY_NUM_MULTIPLY,
    KEY_LALT,
    KEY_SPACE,
    KEY_CAPSLOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_NUM_LOCK,
    KEY_SCROLL_LOCK,
    KEY_NUM_7,
    KEY_NUM_8,
    KEY_NUM_9,
    KEY_NUM_MINUS,
    KEY_NUM_4,
    KEY_NUM_5,
    KEY_NUM_6,
    KEY_NUM_PLUS,
    KEY_NUM_1,
    KEY_NUM_2,
    KEY_NUM_3,
    KEY_NUM_0,
    KEY_NUM_DOT,
    KEY_BACKSLASH,
    KEY_F11,
    KEY_F12,
    
    // Extended Keys
    KEY_NUM_ENTER,
    KEY_RCTRL,
    KEY_PRINT_SCREEN,
    KEY_NUM_DIVIDE,
    KEY_RALT,
    KEY_HOME,
    KEY_UP,
    KEY_PGUP,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_END,
    KEY_DOWN,
    KEY_PGDOWN,
    KEY_INSERT,
    KEY_DELETE,
    KEY_LGUI,
    KEY_RGUI,
    KEY_APPS,
    
    // Special Keys
    KEY_PAUSE, 
};

// {command, data}
struct keyboard_command {
    unsigned char command;
    unsigned char data;
};

struct keyboard {
    // Each key on the keyboard, 1 if pressed
    unsigned char keys[0x201];
    unsigned char prev_keys[0x201];

    // |0000|x|x|x|x|
    // |0000| Shift | Ctrl | Alt | Cmd |
    unsigned char key_flags;

    // TODO: Sort toggle flags
    // Various "Locks" for the keyboard
    // |00000|x|x|x|
    // |00000| Caps | Scroll | Num |
    unsigned char toggle_flags;
};

extern struct keyboard_command keyboard_queue[256];
extern unsigned char keyboard_queue_pos;

extern struct keyboard kb;

extern const unsigned short scancode_map[0x201];

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

// Reset the response bytes, for use when we begin searching for more bytes
void reset_keyboard_response_bytes();

// Takes a scancode of 1 or more bytes and translates them to a new scancode
unsigned short translate_scancode(unsigned char scancode[KEYBOARD_MAX_RESPONSES]);

// Take a keycode and map it to a keyboard key
void handle_keycode(unsigned short keycode);

// Set previous keyboard state, for used to check if a key is being held
void set_keyboard_key_transition_states();

// Compare the current state of a key to it's previous state, to check if held
// 1 if held
unsigned char get_keyboard_key_held(enum keyboard_layout key);

unsigned char get_ascii(enum keyboard_layout key);

#endif