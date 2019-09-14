#ifndef MYOS_KEYBOARD_H
#define MYOS_KEYBOARD_H

#define PORT_KEYDAT       0x0060
#define PORT_KEYSTA       0x0064
#define PORT_KEYCMD       0x0064
#define KEYSTA_NOTREADY   0x02
#define KEYCMD_WRITE      0x60
#define KBC_MODE          0x47

#include <queue.h>

struct KEYBUF {
  char data;
  unsigned char flag;
} keybuf;

void get_key(char *key, unsigned char code);

void wait_keyboard();
void init_keyboard(struct Queue *q);

void handle_intr(int *esp);
void handle_intr27(int *esp);

#endif