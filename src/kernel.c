/* Simple, direct kernel entry point */
__attribute__((noreturn)) void _start(void) {
    /* 1. Set up video memory */
    volatile char *vga = (volatile char*)0xB8000;
    
    /* 2. Write "KFS" in white-on-black */
    vga[0] = 'K'; vga[1] = 0x07;
    vga[2] = 'F'; vga[3] = 0x07;
    vga[4] = 'S'; vga[5] = 0x07;
    
    /* 3. Infinite halt loop */
    while(1) {
        __asm__("hlt");  // More efficient than empty while(1)
    }
}