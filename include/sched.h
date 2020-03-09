#ifndef MYOS_SCHED_H
#define MYOS_SCHED_H

void init_sched();

void set_tr(int tr);

void context_switch(int *addr);

void task2_main();

#endif
