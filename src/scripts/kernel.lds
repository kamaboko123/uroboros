OUTPUT_FORMAT("binary");

SECTIONS {
    . = 0x00100000;
    .text : {
        bin/c/main.o(Main)
        *(.text)
    }
    
    .bss : {
        *(.bss)
    }
    
    .data : {
        *(.data)
    }
    
    .rodata : {
        *(.rodata)
    }
    
}

