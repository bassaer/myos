#ifndef MYOS_CURSOR_H
#define MYOS_CURSOR_H

#include<lib/queue.h>

void init_cursor(char *cursor, queue_t *q, char bg_color);

void handle_intr2c(int *esp);

void enable_cursor();

void update_cursor(unsigned char data);

#endif
