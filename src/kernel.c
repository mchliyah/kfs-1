
#include <stdint.h>

#define VGA_COLOR_ERROR 0x4F   // White-on-red
#define VGA_COLOR_OK    0x2F   // White-on-green
#define VGA_COLOR_INFO 0x01    // white-on-blue

void clear_screen(uint8_t color) {
    volatile uint16_t *vga = (volatile uint16_t*)0xB8000;
    for (int i = 0; i < 80*25; i++) {
        vga[i] = (color << 8) | ' ';
    }
}

__attribute__((section(".text"))) /* Make sure the entry point is properly aligned */
__attribute__((noreturn))
void _start() {
    // Initialize screen  default color reset
    clear_screen(0x07);

    // test colors green
    volatile uint16_t *vga = (volatile uint16_t*)0xB8000;
    vga[0] = (VGA_COLOR_OK << 8) | 'K';
    vga[1] = (VGA_COLOR_OK << 8) | 'F';
    vga[2] = (VGA_COLOR_INFO << 8) | 'S';
    
    // test color red
    vga[80] = (VGA_COLOR_ERROR << 8) | 'E';
    vga[81] = (VGA_COLOR_ERROR << 8) | 'R';
    vga[82] = (VGA_COLOR_ERROR << 8) | 'R';
    while(1) __asm__ volatile ("hlt");
}




