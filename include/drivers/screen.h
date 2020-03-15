#ifndef MYOS_SCREEN_H
#define MYOS_SCREEN_H

void init_screen(char *vram, unsigned int width, unsigned height);

void put_c(int x, int y, char color, char c);

void put_s(int x, int y, char color, char *str);

void put_block(int width, int height, int start_x, int start_y, char *buf, int px);

#endif
