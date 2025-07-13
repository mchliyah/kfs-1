ENTRY(_start)

SECTIONS {
    . = 0x10000;
    
    .text : {
        *(.text)
    }
    
    .data : {
        *(.data)
        *(.rodata*)
    }
    
    .bss : {
        *(.bss)
        *(COMMON)
    }
    
    /* Discard unwanted sections */
    /DISCARD/ : {
        *(.comment)
        *(.note.GNU-stack)
        *(.eh_frame)
    }
}