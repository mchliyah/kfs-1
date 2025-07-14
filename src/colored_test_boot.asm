bits 16
org 0x7C00

%define ENDL 0x0D, 0x0A
%define COLOR_NORMAL 0x07  ; White-on-black
%define COLOR_ERROR 0x4F   ; White-on-red

; Move disk_error and print_string before they're referenced
disk_error:
    mov si, error_msg
    call print_string
    hlt

print_string:
    push bx
    mov bx, COLOR_ERROR
    cmp si, error_msg
    je .print_loop
    mov bx, COLOR_NORMAL
.print_loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .print_loop
.done:
    pop bx
    ret

error_msg db "Disk error!", ENDL, 0

start:
    ; Initialize segments and stack
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Reset disk system
    mov ah, 0x00
    mov dl, 0x00
    int 0x13
    jc disk_error

    ; Load kernel
    mov ax, 0x1000
    mov es, ax
    xor bx, bx
    mov ah, 0x02
    mov al, 2
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x00
    int 0x13
    jc disk_error
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
    jmp 0x10000

; GDT (unchanged)
gdt_start:
    dq 0x0
gdt_code:
    dw 0xFFFF, 0x0, 0x0, 10011010b, 11001111b, 0x0
gdt_data:
    dw 0xFFFF, 0x0, 0x0, 10010010b, 11001111b, 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

times 510-($-$$) db 0
dw 0xAA55