/* Make sure the entry point is properly aligned */
__attribute__((section(".text")))
__attribute__((noreturn)) 
void _start(void) {
    /* Video memory starts at 0xB8000 */
    volatile unsigned short *vga_buffer = (volatile unsigned short*)0xB8000;
    
    /* Clear screen */
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = (0x07 << 8) | ' ';
    }
    
    /* First line */
    // vga_buffer[0] = (0x07 << 8) | 'H';
    // vga_buffer[1] = (0x07 << 8) | 'i';
    // vga_buffer[2] = (0x07 << 8) | '!';
    
    /* Second line (80 characters per line in VGA text mode) */
    vga_buffer[80] = (0x07 << 8) | '4';
    vga_buffer[81] = (0x07 << 8) | '2';
    
    /* Infinite loop */
    while(1) {
        __asm__ volatile ("hlt");
    }
}