#include "graphic.h"
unsigned char *vram = (unsigned char *)VRAM_ADDR;

void init_palette(void){
    static unsigned char table_rgb[16 * 3] = {
            0x00, 0x00, 0x00,
            0xff, 0x00, 0x00,
            0x00, 0xff, 0x00,
            0xff, 0xff, 0x00,
            0x00, 0x00, 0xff,
            0xff, 0x00, 0xff,
            0x00, 0xff, 0xff,
            0xff, 0xff, 0xff,
            0xc6, 0xc6, 0xc6,
            0x84, 0x00, 0x00,
            0x00, 0x84, 0x00,
            0x84, 0x84, 0x00,
            0x00, 0x00, 0x84,
            0x84, 0x00, 0x84,
            0x00, 0x84, 0x84,
            0x84, 0x84, 0x84
        };
    
    set_palette(0, 15, table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char *rgb){
    int i;
    int eflags;
    
    eflags = io_load_eflags(); //割り込み許可フラグの値を記録
    io_cli(); //割り込み禁止(許可フラグを0にする)
    io_out8(0x03c8, start);
    
    for(i = start; i <= end; i++){
            io_out8(0x03c9, rgb[0] / 4);
            io_out8(0x03c9, rgb[1] / 4);
            io_out8(0x03c9, rgb[2] / 4);
            rgb += 3;
        }
    
    io_store_eflags(eflags); //割り込み許可フラグを戻す
    return;
}

void init_screen(unsigned int color){
    unsigned char *vram = (unsigned char *)VRAM_ADDR;
    for(int y = 0; y < SCREEN_YSIZE; y++){
        for(int x = 0; x < SCREEN_XSIZE; x++){
            vram[y * SCREEN_XSIZE + x] = 0; //black
        }
    }
}

void print_asc(int pos_x, int pos_y, unsigned char color, char *s){
    extern unsigned char hankaku[4096];
    for(int i = 0; s[i] != '\0'; i++){
        put_font_asc(pos_x + i * 8, pos_y, color, (hankaku + s[i] * 16));
    }
}

void put_font_asc(int pos_x, int pos_y, unsigned char color, unsigned char *font){
    for(int i = 0; i < 16; i++){
        unsigned char *p = vram + (pos_y + i) * SCREEN_XSIZE + pos_x;
        unsigned char d = font[i];
        if((d & 0x80) != 0) p[0] = color;
        if((d & 0x40) != 0) p[1] = color;
        if((d & 0x20) != 0) p[2] = color;
        if((d & 0x10) != 0) p[3] = color;
        if((d & 0x08) != 0) p[4] = color;
        if((d & 0x04) != 0) p[5] = color;
        if((d & 0x02) != 0) p[6] = color;
        if((d & 0x01) != 0) p[7] = color;
    }
}
