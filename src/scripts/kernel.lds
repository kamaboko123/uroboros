OUTPUT_FORMAT("binary");

SECTIONS {
    . = 0x00100000;
    .text : {
        bin/main.o (.text)
    }
}

