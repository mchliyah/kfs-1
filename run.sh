#!/bin/bash
set -e

echo "=== Building bootloader ==="
nasm -f bin boot.asm -o boot.bin -l boot.lst
echo "Bootloader size: $(stat -c%s boot.bin) bytes"

echo "=== Building kernel ==="
gcc -m32 -ffreestanding -nostdlib -fno-pie -O1 \
    -Wall -Wextra -Werror \
    -fno-asynchronous-unwind-tables \
    -fno-stack-protector \
    -c kernel.c -o kernel.o
    
ld -m elf_i386 -T linker.ld -nostdlib \
    --orphan-handling=discard \
    -Map=kernel.map \
    -o kernel.bin kernel.o --oformat binary
echo "Kernel size: $(stat -c%s kernel.bin) bytes"

echo "=== Creating disk image ==="
dd if=/dev/zero of=disk.img bs=512 count=2880 status=progress
dd if=boot.bin of=disk.img conv=notrunc status=none
dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc status=none

echo "===cleaning files befor start==="
rm -f boot.bin boot.lst kernel.bin kernel.o kernel.map

echo "=== Starting QEMU ==="
qemu-system-x86_64 \
    -drive file=disk.img,format=raw,if=floppy \
    -d int,cpu_reset \
    -no-reboot \
    -serial stdio