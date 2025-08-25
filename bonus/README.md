# KFS Bonus Features

This project implements bonus features for the KFS (Kernel From Scratch) project using **only C language** (no assembly except for minimal inline assembly for I/O).

## Bonus Features Implemented

### 1. **Enhanced VGA Text Mode** (`vga.h` / `vga.c`)
- ✅ Full-featured VGA text mode driver
- ✅ 16 color support (foreground/background)
- ✅ Cursor management and positioning
- ✅ Screen scrolling
- ✅ String printing with colors
- ✅ Number printing (decimal, hexadecimal, any base)
- ✅ Character-by-character output with formatting

### 2. **String Library** (`string.h` / `string.c`)
- ✅ Complete string manipulation functions:
  - `strlen`, `strcmp`, `strncmp`
  - `strcpy`, `strncpy`, `strcat`
  - `memset`, `memcpy`, `memcmp`
- ✅ All implemented in pure C

### 3. **Keyboard Input Handler** (`keyboard.h` / `keyboard.c`)
- ✅ PS/2 keyboard driver
- ✅ Scancode to ASCII conversion
- ✅ Support for shift keys and caps lock
- ✅ US QWERTY layout mapping
- ✅ Special key handling (backspace, enter, tab, etc.)

### 4. **Memory Management** (`memory.h` / `memory.c`)
- ✅ Simple heap allocator
- ✅ `kmalloc()` and `kfree()` functions
- ✅ `krealloc()` for memory reallocation
- ✅ Memory usage tracking
- ✅ Block merging and splitting for efficiency

### 5. **Interactive Shell** (`shell.h` / `shell.c`)
- ✅ Command-line interface
- ✅ Built-in commands:
  - `help` - Show available commands
  - `clear` - Clear screen
  - `echo` - Echo text
  - `memory` - Show memory statistics
  - `version` - Show kernel version
  - `reboot` - Restart system
  - `test` - Run system tests
- ✅ Command parsing and execution
- ✅ Interactive input with backspace support

### 6. **Main Bonus Kernel** (`bonus_kernel.c`)
- ✅ Initialization sequence for all subsystems
- ✅ Integration of all bonus features
- ✅ Boot messages and status reporting

## Building

### Build Bonus Features Only (C compilation test)
```bash
make bonus-test
```

### Build Complete Bonus Kernel (requires bootloader)
```bash
make bonus
```

### Run Bonus Kernel in QEMU
```bash
make run-bonus
```

## File Structure

```
bonus/
├── bonus_kernel.c      # Main bonus kernel entry point
├── bonus_linker.ld     # Linker script for bonus kernel
├── vga.h / vga.c       # VGA text mode driver
├── keyboard.h / keyboard.c # Keyboard input handler
├── memory.h / memory.c # Memory management
├── shell.h / shell.c   # Interactive shell
└── string.h / string.c # String manipulation library
```

## Features Demonstrated

1. **Pure C Implementation** - No assembly code except minimal inline assembly for I/O ports
2. **Modular Design** - Each feature is in separate files with clean interfaces
3. **Interactive System** - Fully functional shell that responds to user input
4. **Memory Management** - Dynamic allocation and deallocation
5. **Rich Display** - Colorful output with proper formatting
6. **Real Hardware Support** - Works with actual PS/2 keyboards and VGA displays

## Usage

Once booted, the bonus kernel will display a shell prompt:
```
kfs> 
```

Try these commands:
- `help` - See all available commands
- `memory` - Check memory usage
- `test` - Run system tests
- `echo Hello World` - Display text
- `clear` - Clear the screen

## Technical Notes

- Memory heap starts at 1MB (0x100000) with 1MB size
- VGA buffer at 0xB8000 for text mode
- Keyboard uses port 0x60 for data, 0x64 for status
- All I/O operations use inline assembly for `inb`/`outb`
- Compiled with `-ffreestanding -nostdlib` for bare metal execution
