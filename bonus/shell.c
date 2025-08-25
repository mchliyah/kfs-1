#include "shell.h"
#include "vga.h"
#include "string.h"
#include "memory.h"
#include "keyboard.h"

static char input_buffer[SHELL_BUFFER_SIZE];
static int buffer_index = 0;

// Available commands
static shell_command_t commands[] = {
    {"help", "Show available commands", cmd_help},
    {"clear", "Clear the screen", cmd_clear},
    {"echo", "Echo text to screen", cmd_echo},
    {"memory", "Show memory information", cmd_memory},
    {"version", "Show kernel version", cmd_version},
    {"reboot", "Reboot the system", cmd_reboot},
    {"test", "Run system tests", cmd_test},
    {NULL, NULL, NULL} // End marker
};

void shell_init(void) {
    buffer_index = 0;
    memset(input_buffer, 0, SHELL_BUFFER_SIZE);
    
    uint8_t color = VGA_COLOR(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_print_string("KFS Bonus Shell v1.0\n", color);
    vga_print_string("Type 'help' for available commands.\n\n", color);
    vga_print_string(SHELL_PROMPT, VGA_COLOR(VGA_COLOR_CYAN, VGA_COLOR_BLACK));
}

void shell_run(void) {
    while (1) {
        if (keyboard_has_data()) {
            uint8_t scancode = keyboard_get_scancode();
            char c = scancode_to_ascii(scancode);
            
            if (c != 0) {
                shell_process_input(c);
            }
        }
    }
}

void shell_process_input(char c) {
    uint8_t color = VGA_COLOR(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    switch (c) {
        case '\n': // Enter
            vga_putchar('\n', color);
            if (buffer_index > 0) {
                input_buffer[buffer_index] = '\0';
                shell_execute_command(input_buffer);
                buffer_index = 0;
                memset(input_buffer, 0, SHELL_BUFFER_SIZE);
            }
            vga_print_string(SHELL_PROMPT, VGA_COLOR(VGA_COLOR_CYAN, VGA_COLOR_BLACK));
            break;
            
        case '\b': // Backspace
            if (buffer_index > 0) {
                buffer_index--;
                input_buffer[buffer_index] = '\0';
                vga_putchar('\b', color);
            }
            break;
            
        default:
            if (c >= 32 && c <= 126 && buffer_index < SHELL_BUFFER_SIZE - 1) {
                input_buffer[buffer_index++] = c;
                vga_putchar(c, color);
            }
            break;
    }
}

int shell_parse_command(const char* input, char* argv[]) {
    static char cmd_buffer[SHELL_BUFFER_SIZE];
    strcpy(cmd_buffer, input);
    
    int argc = 0;
    char* token = cmd_buffer;
    char* end = cmd_buffer + strlen(cmd_buffer);
    
    while (token < end && argc < SHELL_MAX_ARGS - 1) {
        // Skip whitespace
        while (*token == ' ' || *token == '\t') token++;
        if (*token == '\0') break;
        
        argv[argc++] = token;
        
        // Find end of token
        while (*token != ' ' && *token != '\t' && *token != '\0') token++;
        if (*token != '\0') {
            *token = '\0';
            token++;
        }
    }
    
    argv[argc] = NULL;
    return argc;
}

void shell_execute_command(const char* input) {
    char* argv[SHELL_MAX_ARGS];
    int argc = shell_parse_command(input, argv);
    
    if (argc == 0) return;
    
    // Find and execute command
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].function(argc, argv);
            return;
        }
    }
    
    // Command not found
    uint8_t error_color = VGA_COLOR(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    vga_print_string("Command not found: ", error_color);
    vga_print_string(argv[0], error_color);
    vga_print_string("\nType 'help' for available commands.\n", error_color);
}

// Built-in commands implementation

void cmd_help(int argc, char* argv[]) {
    (void)argc; (void)argv; // Suppress unused parameter warnings
    
    uint8_t color = VGA_COLOR(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    vga_print_string("Available commands:\n", color);
    
    for (int i = 0; commands[i].name != NULL; i++) {
        vga_print_string("  ", color);
        vga_print_string(commands[i].name, VGA_COLOR(VGA_COLOR_YELLOW, VGA_COLOR_BLACK));
        vga_print_string(" - ", color);
        vga_print_string(commands[i].description, color);
        vga_print_string("\n", color);
    }
}

void cmd_clear(int argc, char* argv[]) {
    (void)argc; (void)argv;
    vga_clear_screen(VGA_COLOR(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
}

void cmd_echo(int argc, char* argv[]) {
    uint8_t color = VGA_COLOR(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    for (int i = 1; i < argc; i++) {
        vga_print_string(argv[i], color);
        if (i < argc - 1) {
            vga_print_string(" ", color);
        }
    }
    vga_print_string("\n", color);
}

void cmd_memory(int argc, char* argv[]) {
    (void)argc; (void)argv;
    
    uint8_t color = VGA_COLOR(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    size_t free_mem = memory_get_free();
    size_t used_mem = memory_get_used();
    size_t total_mem = free_mem + used_mem;
    
    vga_print_string("Memory Information:\n", color);
    vga_print_string("  Total: ", color);
    vga_print_decimal(total_mem, color);
    vga_print_string(" bytes\n", color);
    vga_print_string("  Used:  ", color);
    vga_print_decimal(used_mem, color);
    vga_print_string(" bytes\n", color);
    vga_print_string("  Free:  ", color);
    vga_print_decimal(free_mem, color);
    vga_print_string(" bytes\n", color);
}

void cmd_version(int argc, char* argv[]) {
    (void)argc; (void)argv;
    
    uint8_t color = VGA_COLOR(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    vga_print_string("KFS (Kernel From Scratch) Bonus Version\n", color);
    vga_print_string("Built with C-only bonus features\n", color);
    vga_print_string("Features: VGA, Keyboard, Memory, Shell\n", color);
}

void cmd_reboot(int argc, char* argv[]) {
    (void)argc; (void)argv;
    
    uint8_t color = VGA_COLOR(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    vga_print_string("Rebooting system...\n", color);
    
    // Wait a moment
    for (volatile int i = 0; i < 1000000; i++);
    
    // Trigger a triple fault to reboot
    __asm__ volatile ("cli");
    __asm__ volatile ("lidt %%cs:0" ::: "memory");
    __asm__ volatile ("int $0x03");
}

void cmd_test(int argc, char* argv[]) {
    (void)argc; (void)argv;
    
    uint8_t color = VGA_COLOR(VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    vga_print_string("Running system tests...\n", color);
    
    // Test memory allocation
    vga_print_string("Testing memory allocation... ", color);
    void* ptr1 = kmalloc(100);
    void* ptr2 = kmalloc(200);
    void* ptr3 = kmalloc(50);
    
    if (ptr1 && ptr2 && ptr3) {
        vga_print_string("OK\n", VGA_COLOR(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
        kfree(ptr1);
        kfree(ptr2);
        kfree(ptr3);
    } else {
        vga_print_string("FAILED\n", VGA_COLOR(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    }
    
    // Test string functions
    vga_print_string("Testing string functions... ", color);
    char test_str[] = "Hello";
    if (strlen(test_str) == 5 && strcmp(test_str, "Hello") == 0) {
        vga_print_string("OK\n", VGA_COLOR(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    } else {
        vga_print_string("FAILED\n", VGA_COLOR(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    }
    
    vga_print_string("All tests completed!\n", color);
}
