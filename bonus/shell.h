#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include <stddef.h>

// Shell constants
#define SHELL_BUFFER_SIZE 256
#define SHELL_MAX_ARGS 16
#define SHELL_PROMPT "kfs> "

// Shell command structure
typedef struct {
    const char* name;
    const char* description;
    void (*function)(int argc, char* argv[]);
} shell_command_t;

// Shell functions
void shell_init(void);
void shell_run(void);
void shell_process_input(char c);
void shell_execute_command(const char* input);
int shell_parse_command(const char* input, char* argv[]);

// Built-in commands
void cmd_help(int argc, char* argv[]);
void cmd_clear(int argc, char* argv[]);
void cmd_echo(int argc, char* argv[]);
void cmd_memory(int argc, char* argv[]);
void cmd_version(int argc, char* argv[]);
void cmd_reboot(int argc, char* argv[]);
void cmd_test(int argc, char* argv[]);

#endif // SHELL_H
