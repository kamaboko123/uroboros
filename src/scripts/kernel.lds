MEMORY
{
  RAM (rwx) : ORIGIN = 0x00100000, LENGTH = 1M
}

SECTIONS {
    .text : {
        bin/c/main.o(Main)
        *(.text .text.*)
    } > RAM
    
    .bss : {
        *(.bss .bss.*)
    } > RAM
    
    .data : {
        *(.data .data.*)
    } > RAM
    
    .rodata : {
        *(.rodata .rodata.*)
    } > RAM
}

