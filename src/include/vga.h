#ifndef KFS_VGA_H
#define KFS_VGA_H

#include "string.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_COLOR_ERROR 0x4F
#define VGA_COLOR_OK    0x02
#define VGA_COLOR_INFO  0x01
#define VGA_COLOR_DEBUG 0x04
#define VGA_COLOR_DEFAULT 0x0F

/* Global VGA state */
static volatile unsigned short *_vga_buffer = (volatile unsigned short*)0xB8000;
static int _cursor_x = 0;
static int _cursor_y = 0;
static unsigned char _current_color = VGA_COLOR_DEFAULT;

#define VGA_INIT() do { \
    _vga_buffer = (volatile unsigned short*)0xB8000; \
    _cursor_x = 0; \
    _cursor_y = 0; \
    _current_color = VGA_COLOR_DEFAULT; \
} while(0)

#define VGA_SET_COLOR(color) do { \
    _current_color = (color); \
} while(0)

#define VGA_GET_COLOR() (_current_color)

#define VGA_CLEAR_SCREEN() do { \
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) { \
        _vga_buffer[i] = (0x00 << 8) | ' '; \
    } \
    _cursor_x = 0; \
    _cursor_y = 0; \
} while(0)

#define VGA_SCROLL_IF_NEEDED() do { \
    if (_cursor_y >= VGA_HEIGHT) { \
        for (int y = 1; y < VGA_HEIGHT; y++) { \
            for (int x = 0; x < VGA_WIDTH; x++) { \
                _vga_buffer[(y - 1) * VGA_WIDTH + x] = _vga_buffer[y * VGA_WIDTH + x]; \
            } \
        } \
        for (int x = 0; x < VGA_WIDTH; x++) { \
            _vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (0x00 << 8) | ' '; \
        } \
        _cursor_y = VGA_HEIGHT - 1; \
        _cursor_x = 0; \
    } \
} while(0)

#define VGA_PUTCHAR_COLOR(c, color) do { \
    if ((c) == '\n') { \
        _cursor_x = 0; \
        _cursor_y++; \
    } else if ((c) == '\r') { \
        _cursor_x = 0; \
    } else { \
        int pos = _cursor_y * VGA_WIDTH + _cursor_x; \
        _vga_buffer[pos] = ((color) << 8) | (unsigned char)(c); \
        _cursor_x++; \
        if (_cursor_x >= VGA_WIDTH) { \
            _cursor_x = 0; \
            _cursor_y++; \
        } \
    } \
    VGA_SCROLL_IF_NEEDED(); \
} while(0)

#define VGA_PUTCHAR(c) VGA_PUTCHAR_COLOR((c), _current_color)

#define VGA_WRITE_COLOR(str, color) do { \
    const char *s = (str); \
    for (size_t i = 0; s[i] != '\0'; i++) { \
        VGA_PUTCHAR_COLOR(s[i], (color)); \
    } \
} while(0)

#define VGA_WRITE(str) VGA_WRITE_COLOR((str), _current_color)

#define VGA_SET_CURSOR(x, y) do { \
    if ((x) >= 0 && (x) < VGA_WIDTH && (y) >= 0 && (y) < VGA_HEIGHT) { \
        _cursor_x = (x); \
        _cursor_y = (y); \
    } \
} while(0)

#define VGA_PUTCHAR_AT(c, x, y, color) do { \
    if ((x) >= 0 && (x) < VGA_WIDTH && (y) >= 0 && (y) < VGA_HEIGHT) { \
        int pos = (y) * VGA_WIDTH + (x); \
        _vga_buffer[pos] = ((color) << 8) | (unsigned char)(c); \
    } \
} while(0)

#define VGA_WRITE_AT(str, x, y, color) do { \
    VGA_SET_CURSOR((x), (y)); \
    VGA_WRITE_COLOR((str), (color)); \
} while(0)

#endif /* KFS_VGA_H */
