#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "asmlib.h"
#include "kernel.h"

void init_palette(void);
void set_palette(int32_t start, int32_t end, uint8_t *rgb);
void init_screen(uint32_t color);
void print_asc(int32_t pos_x, int32_t pos_y, uint8_t color, char *s);
void put_font_asc(int32_t pos_x, int32_t pos_y, uint8_t color, uint8_t *font);

#endif
