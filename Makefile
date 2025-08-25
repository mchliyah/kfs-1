# Directories
SRC_DIR := src
BONUS_DIR := bonus
BUILD_DIR := build
RUN_DIR := run

# Files
BOOT_SRC := $(SRC_DIR)/boot.asm
KERNEL_SRC := $(SRC_DIR)/kernel.c
LINKER_SCRIPT := $(SRC_DIR)/linker.ld

# Bonus files
BONUS_SRCS := $(BONUS_DIR)/bonus_kernel.c $(BONUS_DIR)/vga.c $(BONUS_DIR)/keyboard.c \
              $(BONUS_DIR)/memory.c $(BONUS_DIR)/shell.c $(BONUS_DIR)/string.c
BONUS_LINKER := $(BONUS_DIR)/bonus_linker.ld

BOOT_BIN := $(BUILD_DIR)/boot.bin
BOOT_LST := $(BUILD_DIR)/boot.lst
KERNEL_O := $(BUILD_DIR)/kernel.o
KERNEL_BIN := $(BUILD_DIR)/kernel.bin
KERNEL_MAP := $(BUILD_DIR)/kernel.map

# Bonus build files
BONUS_OBJS := $(BONUS_SRCS:$(BONUS_DIR)/%.c=$(BUILD_DIR)/bonus_%.o)
BONUS_BIN := $(BUILD_DIR)/bonus_kernel.bin
BONUS_MAP := $(BUILD_DIR)/bonus_kernel.map

DISK_IMG := $(RUN_DIR)/disk.img
BONUS_DISK_IMG := $(RUN_DIR)/bonus_disk.img

# Compiler and flags
ASM := nasm
ASMFLAGS := -f bin

CC := gcc
CFLAGS := -m32 -ffreestanding -nostdlib -fno-pie -O1 \
          -Wall -Wextra -Werror \
          -fno-asynchronous-unwind-tables \
          -fno-stack-protector

LD := ld
LDFLAGS := -m elf_i386 -T $(LINKER_SCRIPT) -nostdlib \
           --orphan-handling=discard

QEMU := qemu-system-x86_64
QEMUFLAGS := -drive file=$(DISK_IMG),format=raw,if=floppy \
             -d int,cpu_reset \
             -no-reboot \
             -serial stdio

.PHONY: all clean run bonus run-bonus bonus-test run-bonus-alt run-bonus-manual run-bonus-simple run-bonus-fallback run-bonus-clean run-bonus-gui

all: $(DISK_IMG)

bonus: $(BONUS_DISK_IMG)

bonus-test: $(BONUS_OBJS)
	@echo "=== Bonus kernel objects built successfully ==="
	@echo "Objects: $(BONUS_OBJS)"

# Bootloader build rule
$(BOOT_BIN) $(BOOT_LST): $(BOOT_SRC)
	@echo "=== Building bootloader ==="
	@mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) $< -o $(BOOT_BIN) -l $(BOOT_LST)
	@echo "Bootloader size: $$(stat -c%s $(BOOT_BIN)) bytes"

# Kernel object file
$(KERNEL_O): $(KERNEL_SRC)
	@echo "=== Building kernel ==="
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Kernel binary
$(KERNEL_BIN): $(KERNEL_O) $(LINKER_SCRIPT)
	$(LD) $(LDFLAGS) -Map=$(KERNEL_MAP) -o $@ $< --oformat binary
	@echo "Kernel size: $$(stat -c%s $(KERNEL_BIN)) bytes"

# Disk image
$(DISK_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "=== Creating disk image ==="
	@mkdir -p $(@D)
	dd if=/dev/zero of=$@ bs=512 count=2880 status=progress
	dd if=$(BOOT_BIN) of=$@ conv=notrunc status=none
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc status=none

# Run in QEMU
run: $(DISK_IMG)
	@echo "=== Starting QEMU ==="
	$(QEMU) $(QEMUFLAGS)

# Clean up
clean:
	@echo "=== Cleaning build files ==="
	rm -rf $(BUILD_DIR) $(RUN_DIR)

# rebuild
re: clean all

# Print file sizes after build
print-sizes: $(BOOT_BIN) $(KERNEL_BIN)
	@echo "Bootloader size: $$(stat -c%s $(BOOT_BIN)) bytes"
	@echo "Kernel size: $$(stat -c%s $(KERNEL_BIN)) bytes"

# Bonus kernel object files
$(BUILD_DIR)/bonus_%.o: $(BONUS_DIR)/%.c
	@echo "=== Building bonus: $< ==="
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(BONUS_DIR) -c $< -o $@

# Bonus kernel binary
$(BONUS_BIN): $(BONUS_OBJS) $(BONUS_LINKER)
	@echo "=== Linking bonus kernel ==="
	$(LD) $(LDFLAGS) -T $(BONUS_LINKER) -Map=$(BONUS_MAP) -o $@ $(BONUS_OBJS) --oformat binary
	@echo "Bonus kernel size: $$(stat -c%s $(BONUS_BIN)) bytes"

# Bonus disk image
$(BONUS_DISK_IMG): $(BOOT_BIN) $(BONUS_BIN)
	@echo "=== Creating bonus disk image ==="
	@mkdir -p $(@D)
	dd if=/dev/zero of=$@ bs=512 count=2880 status=progress
	dd if=$(BOOT_BIN) of=$@ conv=notrunc status=none
	dd if=$(BONUS_BIN) of=$@ bs=512 seek=1 conv=notrunc status=none

# Run bonus in QEMU (alternative)
run-bonus-alt: $(BONUS_DISK_IMG)
	@echo "=== Starting QEMU (alternative) with bonus kernel ==="
	/usr/bin/qemu-system-i386 -fda $(BONUS_DISK_IMG) -serial stdio -no-reboot

# Manual run commands for testing
run-bonus-manual: $(BONUS_DISK_IMG)
	@echo "=== Manual QEMU commands ==="
	@echo "Try one of these commands:"
	@echo "/usr/bin/qemu-system-i386 -fda $(BONUS_DISK_IMG)"
	@echo "/usr/bin/qemu-system-x86_64 -fda $(BONUS_DISK_IMG) -nographic"
	@echo "qemu-system-i386 -fda $(BONUS_DISK_IMG)"

# Simple test run without debug flags
run-bonus-simple: $(BONUS_DISK_IMG)
	@echo "=== Starting QEMU (simple) ==="
	/usr/bin/qemu-system-i386 -fda $(BONUS_DISK_IMG)

# Fixed run-bonus target
run-bonus: $(BONUS_DISK_IMG)
	@echo "=== Starting QEMU with bonus kernel ==="
	/usr/bin/qemu-system-i386 -fda $(BONUS_DISK_IMG) -serial stdio

# Fallback if system qemu doesn't work
run-bonus-fallback: $(BONUS_DISK_IMG)
	@echo "=== Trying fallback QEMU ==="
	@echo "If this fails, install qemu with: sudo apt install qemu-system-x86"
	qemu-system-i386 -fda $(BONUS_DISK_IMG) -nographic

# Clean environment run (fixes snap conflicts)
run-bonus-clean: $(BONUS_DISK_IMG)
	@echo "=== Starting QEMU with clean environment (text mode) ==="
	env -i PATH=/usr/bin:/bin /usr/bin/qemu-system-i386 -drive file=$(BONUS_DISK_IMG),format=raw,if=floppy -nographic

# Graphical version (requires X11/desktop)
run-bonus-gui: $(BONUS_DISK_IMG)
	@echo "=== Starting QEMU with GUI ==="
	env -i PATH=/usr/bin:/bin DISPLAY=$$DISPLAY /usr/bin/qemu-system-i386 -drive file=$(BONUS_DISK_IMG),format=raw,if=floppy