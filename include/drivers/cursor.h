#ifndef MYOS_CURSOR_H
#define MYOS_CURSOR_H

#include<lib/queue.h>

void init_cursor(queue_t *q, char bg_color, int width, int height);

void handle_intr2c(int *esp);

void enable_cursor();

void decode_cursor(unsigned char data);

void update_cursor();

#endif
