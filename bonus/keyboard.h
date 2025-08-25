#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// Keyboard ports
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Keyboard status flags
#define KEYBOARD_STATUS_OUTPUT_FULL 0x01
#define KEYBOARD_STATUS_INPUT_FULL 0x02

// Special keys
#define KEY_ESCAPE      0x01
#define KEY_BACKSPACE   0x0E
#define KEY_TAB         0x0F
#define KEY_ENTER       0x1C
#define KEY_CTRL        0x1D
#define KEY_SHIFT_LEFT  0x2A
#define KEY_SHIFT_RIGHT 0x36
#define KEY_ALT         0x38
#define KEY_CAPS_LOCK   0x3A
#define KEY_F1          0x3B
#define KEY_F2          0x3C
#define KEY_F3          0x3D
#define KEY_F4          0x3E
#define KEY_F5          0x3F
#define KEY_F6          0x40
#define KEY_F7          0x41
#define KEY_F8          0x42
#define KEY_F9          0x43
#define KEY_F10         0x44
#define KEY_NUM_LOCK    0x45
#define KEY_SCROLL_LOCK 0x46
#define KEY_F11         0x57
#define KEY_F12         0x58

// Key state flags
#define KEY_RELEASED 0x80

// Keyboard functions
void keyboard_init(void);
uint8_t keyboard_get_scancode(void);
char scancode_to_ascii(uint8_t scancode);
int keyboard_has_data(void);

// Port I/O functions (implemented in C using inline assembly)
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

#endif // KEYBOARD_H
