#ifndef MYOS_TIMER_H
#define MYOS_TIMER_H

void init_pit();

void handle_intr20(int *esp);

#endif
