#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "asmlib.h"

#define VRAM_ADDR 0x000A0000
#define SCREEN_XSIZE 320
#define SCREEN_YSIZE 200

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void init_screen(unsigned int color);
void print_asc(int pos_x, int pos_y, unsigned char color, char *s);
void put_font_asc(int pos_x, int pos_y, unsigned char color, char c);

#endif
