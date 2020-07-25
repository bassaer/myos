#ifndef MYOS_DESKTOP_H
#define MYOS_DESKTOP_H

#include<uefi.h>

void init_desktop(BootInfo *boot);

void set_color(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pixel, int color);

void fill_box(int color, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y);

void put_c(int x, int y, int color, char c);

void put_s(int x, int y, int color, char *str);

#endif
