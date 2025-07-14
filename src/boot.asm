bits 16
org 0x7C00

%define ENDL 0x0D, 0x0A

start:
    ; Initialize segments and stack
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00      ; Stack grows down from bootloader

    ; Reset disk system
    mov ah, 0x00
    mov dl, 0x00        ; Drive 0 (floppy)
    int 0x13
    jc disk_error

    ; Load kernel to 0x1000:0x0000 (physical 0x10000)
    mov ax, 0x1000
    mov es, ax
    xor bx, bx

    mov ah, 0x02        ; Read sectors
    mov al, 2           ; Read 2 sectors (kernel is small)
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Sector 2 (1 is boot sector)
    mov dh, 0           ; Head 0
    mov dl, 0x00        ; Drive 0 (floppy)
    int 0x13
    jc disk_error

    ; Verify we read enough sectors
    cmp al, 2
    jne disk_error

    ; Switch to protected mode
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm

bits 32
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Jump to kernel
    jmp 0x10000

disk_error:
    mov si, error_msg
    call print_string
    hlt

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

error_msg db "Disk error!", ENDL, 0

; GDT
gdt_start:
    dq 0x0
gdt_code:
    dw 0xFFFF       ; Limit low
    dw 0x0          ; Base low
    db 0x0          ; Base middle
    db 10011010b    ; Access
    db 11001111b    ; Granularity
    db 0x0          ; Base high
gdt_data:
    dw 0xFFFF       ; Limit low
    dw 0x0          ; Base low
    db 0x0          ; Base middle
    db 10010010b    ; Access
    db 11001111b    ; Granularity
    db 0x0          ; Base high
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

times 510-($-$$) db 0
dw 0xAA55