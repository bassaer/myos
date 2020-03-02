#ifndef MYOS_TIMER_H
#define MYOS_TIMER_H

#include <lib/queue.h>

struct timer_ctrl {
  unsigned int count;
  unsigned int timeout;
  struct Queue *queue;
  unsigned char data;
} timer;

void init_pit();

void handle_intr20(int *esp);

void set_timer(unsigned int timeout, struct Queue *queue, unsigned char data);

#endif
