MEMORY
{
  RAM (rwx) : ORIGIN = 0x00300000, LENGTH = 10K
}

SECTIONS {
    .text : {
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

    /DISCARD/ : { *(.eh_frame) }
}
