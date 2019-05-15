#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "asmlib.h"
#include "kernel.h"

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void init_screen(unsigned int color);
void print_asc(int pos_x, int pos_y, unsigned char color, char *s);
void put_font_asc(int pos_x, int pos_y, unsigned char color, unsigned char *font);

#endif
