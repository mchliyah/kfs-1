#include <stdint.h>
#include "vga.h"
#include "keyboard.h"
#include "memory.h"
#include "shell.h"
#include "string.h"

// Entry point for bonus kernel
__attribute__((section(".text")))
__attribute__((noreturn))
void bonus_main() {
    // Initialize VGA with a clean screen
    vga_clear_screen(VGA_COLOR(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    
    // Display boot message
    uint8_t boot_color = VGA_COLOR(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    vga_print_string("KFS Bonus Kernel Loading...\n", boot_color);
    
    // Initialize subsystems
    vga_print_string("Initializing memory manager... ", boot_color);
    memory_init();
    vga_print_string("OK\n", VGA_COLOR(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    
    vga_print_string("Initializing keyboard... ", boot_color);
    keyboard_init();
    vga_print_string("OK\n", VGA_COLOR(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    
    vga_print_string("Starting shell... ", boot_color);
    vga_print_string("OK\n\n", VGA_COLOR(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    
    // Start the shell
    shell_init();
    shell_run();
    
    // Should never reach here
    while(1) __asm__ volatile ("hlt");
}
