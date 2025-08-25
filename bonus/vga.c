#include "vga.h"
#include "string.h"

static int cursor_x = 0;
static int cursor_y = 0;

void vga_clear_screen(uint8_t color) {
    volatile uint16_t *vga = (volatile uint16_t*)VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = VGA_ENTRY(' ', color);
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_clear_line(int line, uint8_t color) {
    if (line < 0 || line >= VGA_HEIGHT) return;
    
    volatile uint16_t *vga = (volatile uint16_t*)VGA_MEMORY;
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga[line * VGA_WIDTH + x] = VGA_ENTRY(' ', color);
    }
}

void vga_set_cursor_position(int x, int y) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        cursor_x = x;
        cursor_y = y;
    }
}

void vga_get_cursor_position(int* x, int* y) {
    if (x) *x = cursor_x;
    if (y) *y = cursor_y;
}

void vga_scroll_up(void) {
    volatile uint16_t *vga = (volatile uint16_t*)VGA_MEMORY;
    
    // Move all lines up by one
    for (int y = 0; y < VGA_HEIGHT - 1; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga[y * VGA_WIDTH + x] = vga[(y + 1) * VGA_WIDTH + x];
        }
    }
    
    // Clear the last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = VGA_ENTRY(' ', VGA_COLOR(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    }
    
    // Move cursor up if at bottom
    if (cursor_y > 0) {
        cursor_y--;
    }
}

void vga_putchar_at(char c, uint8_t color, int x, int y) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) return;
    
    volatile uint16_t *vga = (volatile uint16_t*)VGA_MEMORY;
    vga[y * VGA_WIDTH + x] = VGA_ENTRY(c, color);
}

void vga_putchar(char c, uint8_t color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 8) & ~7;  // Tab to next 8-character boundary
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            vga_putchar_at(' ', color, cursor_x, cursor_y);
        }
    } else {
        vga_putchar_at(c, color, cursor_x, cursor_y);
        cursor_x++;
    }
    
    // Handle line wrapping
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    // Handle scrolling
    if (cursor_y >= VGA_HEIGHT) {
        vga_scroll_up();
        cursor_y = VGA_HEIGHT - 1;
    }
}

void vga_print_string(const char* str, uint8_t color) {
    while (*str) {
        vga_putchar(*str++, color);
    }
}

void vga_print_string_at(const char* str, uint8_t color, int x, int y) {
    int old_x = cursor_x, old_y = cursor_y;
    vga_set_cursor_position(x, y);
    vga_print_string(str, color);
    vga_set_cursor_position(old_x, old_y);
}

void vga_print_number(uint32_t num, uint8_t base, uint8_t color) {
    if (base < 2 || base > 36) return;
    
    char buffer[32];
    char* ptr = buffer + sizeof(buffer) - 1;
    *ptr = '\0';
    
    if (num == 0) {
        *(--ptr) = '0';
    } else {
        while (num > 0) {
            int digit = num % base;
            *(--ptr) = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
            num /= base;
        }
    }
    
    vga_print_string(ptr, color);
}

void vga_print_hex(uint32_t num, uint8_t color) {
    vga_print_string("0x", color);
    vga_print_number(num, 16, color);
}

void vga_print_decimal(uint32_t num, uint8_t color) {
    vga_print_number(num, 10, color);
}
