#ifndef MYOS_MAIN_H
#define MYOS_MAIN_H

#include <lib/queue.h>

typedef struct {
  char cyls;
  char leds;
  char vmode;
  char reserve;
  short width;
  short height;
  char  *vram;
  short gui;
} boot_info_t;

void start_cui(queue_t *keyboard_q);
void start_gui(boot_info_t *boot_info, queue_t *keyboard_q);

#endif
