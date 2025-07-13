# Directories
SRC_DIR := src
BUILD_DIR := build
RUN_DIR := run

# Files
BOOT_SRC := $(SRC_DIR)/boot.asm
KERNEL_SRC := $(SRC_DIR)/kernel.c
LINKER_SCRIPT := $(SRC_DIR)/linker.ld

BOOT_BIN := $(BUILD_DIR)/boot.bin
BOOT_LST := $(BUILD_DIR)/boot.lst
KERNEL_O := $(BUILD_DIR)/kernel.o
KERNEL_BIN := $(BUILD_DIR)/kernel.bin
KERNEL_MAP := $(BUILD_DIR)/kernel.map

DISK_IMG := $(RUN_DIR)/disk.img

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

.PHONY: all clean run

all: $(DISK_IMG)

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

# Print file sizes after build
print-sizes: $(BOOT_BIN) $(KERNEL_BIN)
	@echo "Bootloader size: $$(stat -c%s $(BOOT_BIN)) bytes"
	@echo "Kernel size: $$(stat -c%s $(KERNEL_BIN)) bytes"