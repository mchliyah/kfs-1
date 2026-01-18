#include "vga.h"
#include "keyboard.h"

/* Entry point: keep in a dedicated section placed first by the linker script. */
__attribute__((section(".text.start")))
__attribute__((noreturn))
void _start(void) {
    VGA_INIT();
    VGA_CLEAR_SCREEN();
    VGA_WRITE_AT("Type here: ", 0, 0, VGA_COLOR_OK);
    VGA_SET_CURSOR(0, 1);

    while (1) {
        int ch = KEYBOARD_POLL_CHAR();
        if (ch != -1) {
            if (ch == '\r') {
                VGA_WRITE("\n");
            } else {
                VGA_PUTCHAR((char)ch);
            }
        }
        __asm__ volatile ("hlt");
    }
}
