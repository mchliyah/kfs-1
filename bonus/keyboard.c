#include "keyboard.h"

// US QWERTY keyboard layout
static const char scancode_to_ascii_table[128] = {
    0,   27, '1', '2', '3', '4', '5', '6',  // 0x00-0x07
    '7', '8', '9', '0', '-', '=',  8,   9,  // 0x08-0x0F (8=backspace, 9=tab)
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', // 0x10-0x17
    'o', 'p', '[', ']',  13,   0, 'a', 's', // 0x18-0x1F (13=enter, 0=ctrl)
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', // 0x20-0x27
   '\'', '`',   0,'\\', 'z', 'x', 'c', 'v', // 0x28-0x2F (0=shift)
    'b', 'n', 'm', ',', '.', '/',   0, '*', // 0x30-0x37 (0=shift, *=keypad)
      0, ' ',   0,   0,   0,   0,   0,   0, // 0x38-0x3F (0=alt, space, caps, f1-f5)
      0,   0,   0,   0,   0,   0,   0,   0, // 0x40-0x47 (f6-f10, num, scroll, keypad)
      0,   0,   0,   0,   0,   0,   0,   0, // 0x48-0x4F (keypad)
      0,   0,   0,   0,   0,   0,   0,   0, // 0x50-0x57
      0,   0,   0,   0,   0,   0,   0,   0, // 0x58-0x5F
      0,   0,   0,   0,   0,   0,   0,   0, // 0x60-0x67
      0,   0,   0,   0,   0,   0,   0,   0, // 0x68-0x6F
      0,   0,   0,   0,   0,   0,   0,   0, // 0x70-0x77
      0,   0,   0,   0,   0,   0,   0,   0  // 0x78-0x7F
};

// Shifted characters
static const char scancode_to_ascii_shifted[128] = {
    0,   27, '!', '@', '#', '$', '%', '^',  // 0x00-0x07
    '&', '*', '(', ')', '_', '+',  8,   9,  // 0x08-0x0F
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', // 0x10-0x17
    'O', 'P', '{', '}',  13,   0, 'A', 'S', // 0x18-0x1F
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', // 0x20-0x27
    '"', '~',   0, '|', 'Z', 'X', 'C', 'V', // 0x28-0x2F
    'B', 'N', 'M', '<', '>', '?',   0, '*', // 0x30-0x37
      0, ' ',   0,   0,   0,   0,   0,   0, // 0x38-0x3F
      0,   0,   0,   0,   0,   0,   0,   0, // 0x40-0x47
      0,   0,   0,   0,   0,   0,   0,   0, // 0x48-0x4F
      0,   0,   0,   0,   0,   0,   0,   0, // 0x50-0x57
      0,   0,   0,   0,   0,   0,   0,   0, // 0x58-0x5F
      0,   0,   0,   0,   0,   0,   0,   0, // 0x60-0x67
      0,   0,   0,   0,   0,   0,   0,   0, // 0x68-0x6F
      0,   0,   0,   0,   0,   0,   0,   0, // 0x70-0x77
      0,   0,   0,   0,   0,   0,   0,   0  // 0x78-0x7F
};

static int shift_pressed = 0;
static int caps_lock = 0;

void keyboard_init(void) {
    shift_pressed = 0;
    caps_lock = 0;
}

int keyboard_has_data(void) {
    return (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OUTPUT_FULL) != 0;
}

uint8_t keyboard_get_scancode(void) {
    while (!keyboard_has_data()) {
        // Wait for keyboard data
    }
    return inb(KEYBOARD_DATA_PORT);
}

char scancode_to_ascii(uint8_t scancode) {
    // Handle key releases
    if (scancode & KEY_RELEASED) {
        uint8_t key = scancode & ~KEY_RELEASED;
        if (key == KEY_SHIFT_LEFT || key == KEY_SHIFT_RIGHT) {
            shift_pressed = 0;
        }
        return 0; // Don't generate character for key releases
    }
    
    // Handle key presses
    switch (scancode) {
        case KEY_SHIFT_LEFT:
        case KEY_SHIFT_RIGHT:
            shift_pressed = 1;
            return 0;
            
        case KEY_CAPS_LOCK:
            caps_lock = !caps_lock;
            return 0;
            
        case KEY_CTRL:
        case KEY_ALT:
            return 0; // Don't generate characters for modifier keys
            
        default:
            if (scancode >= 128) return 0; // Invalid scancode
            
            char c;
            if (shift_pressed) {
                c = scancode_to_ascii_shifted[scancode];
            } else {
                c = scancode_to_ascii_table[scancode];
            }
            
            // Apply caps lock to letters
            if (caps_lock && c >= 'a' && c <= 'z') {
                c = c - 'a' + 'A';
            } else if (caps_lock && c >= 'A' && c <= 'Z' && !shift_pressed) {
                c = c - 'A' + 'a';
            }
            
            return c;
    }
}
