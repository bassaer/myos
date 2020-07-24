#ifndef MYOS_DESKTOP_H
#define MYOS_DESKTOP_H

#include<uefi.h>

void init_desktop(BootInfo *boot);

void fill_box(BootInfo *boot, int color, UINTN start_x, UINTN start_y, UINTN end_x, UINTN end_y);

#endif
