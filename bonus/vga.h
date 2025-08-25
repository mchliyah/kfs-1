#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>

// VGA constants
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// VGA colors
#define VGA_COLOR_BLACK         0x0
#define VGA_COLOR_BLUE          0x1
#define VGA_COLOR_GREEN         0x2
#define VGA_COLOR_CYAN          0x3
#define VGA_COLOR_RED           0x4
#define VGA_COLOR_MAGENTA       0x5
#define VGA_COLOR_BROWN         0x6
#define VGA_COLOR_LIGHT_GREY    0x7
#define VGA_COLOR_DARK_GREY     0x8
#define VGA_COLOR_LIGHT_BLUE    0x9
#define VGA_COLOR_LIGHT_GREEN   0xA
#define VGA_COLOR_LIGHT_CYAN    0xB
#define VGA_COLOR_LIGHT_RED     0xC
#define VGA_COLOR_LIGHT_MAGENTA 0xD
#define VGA_COLOR_YELLOW        0xE
#define VGA_COLOR_WHITE         0xF

// VGA helper macros
#define VGA_ENTRY(ch, color) ((uint16_t)(ch) | ((uint16_t)(color) << 8))
#define VGA_COLOR(fg, bg) ((bg << 4) | fg)

// VGA functions
void vga_clear_screen(uint8_t color);
void vga_clear_line(int line, uint8_t color);
void vga_set_cursor_position(int x, int y);
void vga_get_cursor_position(int* x, int* y);
void vga_scroll_up(void);
void vga_putchar_at(char c, uint8_t color, int x, int y);
void vga_putchar(char c, uint8_t color);
void vga_print_string(const char* str, uint8_t color);
void vga_print_string_at(const char* str, uint8_t color, int x, int y);
void vga_print_number(uint32_t num, uint8_t base, uint8_t color);
void vga_print_hex(uint32_t num, uint8_t color);
void vga_print_decimal(uint32_t num, uint8_t color);

#endif // VGA_H
