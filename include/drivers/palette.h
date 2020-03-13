#ifndef MYOS_PALETTE_H
#define MYOS_PALETTE_H

#define PALETTE_BLACK         0
#define PALETTE_RED           1
#define PALETTE_GREEN         2
#define PALETTE_YELLOW        3
#define PALETTE_BLUE          4
#define PALETTE_PURPLE        5
#define PALETTE_LIGHT_BLUE    6
#define PALETTE_WHITE         7
#define PALETTE_LIGHT_GRAY    8
#define PALETTE_DARK_RED      9
#define PALETTE_DARK_GREEN    10
#define PALETTE_DARK_YELLOW   11
#define PALETTE_DARK_BLUE     12
#define PALETTE_DARK_PURPLE   13
#define PALETTE_BLUE_GRAY     14
#define PALETTE_DARK_GRAY     15

void init_palette(char *vrm);

void set_palette(int start, int end, unsigned char *rgb);

void fill_box(int size, unsigned int color, int start_x, int start_y, int end_x, int end_y);

#endif
