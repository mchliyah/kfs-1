
  // White on red
#define VGA_COLOR_ERROR 0x4F

  // Green on black
#define VGA_COLOR_OK  0x02
   // White on blue
#define VGA_COLOR_INFO 0x01
 // Red on black
#define VGA_COLOR_DEBUG 0x04


/* Make sure the entry point is properly aligned */
__attribute__((section(".text")))
__attribute__((noreturn)) 
void _start(void) {
    /* Video memory starts at 0xB8000 */
    volatile unsigned short *vga_buffer = (volatile unsigned short*)0xB8000;
    
    /* Clear screen */
    for (int i = 0; i < 80 * 25; i++) {
        // set screen to color black
        vga_buffer[i] = (0x00 << 8) | ' ';
    }
    
    /* First line */
    // vga_buffer[0] = (VGA_COLOR_DEBUG << 8) | 'H';
    // vga_buffer[1] = (VGA_COLOR_DEBUG << 8) | 'i';
    // vga_buffer[2] = (VGA_COLOR_DEBUG << 8) | '!';

    /* Second line (80 characters per line in VGA text mode) */
    vga_buffer[80] = (VGA_COLOR_OK << 8) | '4';
    vga_buffer[81] = (VGA_COLOR_OK << 8) | '2';
    

    // third line saying good by 

    // vga_buffer[160] = (VGA_COLOR_INFO << 8) | 'B';
    // vga_buffer[161] = (VGA_COLOR_INFO << 8) | 'y';
    // vga_buffer[162] = (VGA_COLOR_INFO << 8) | 'e';
    /* Infinite loop */
    while(1) {
        __asm__ volatile ("hlt");
    }
}


