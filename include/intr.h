#ifndef MYOS_INTR_H
#define MYOS_INTR_H

#define PIC0_ICW1     0x0020
#define PIC0_OCW2     0x0020
#define PIC0_IMR      0x0021
#define PIC0_ICW2     0x0021
#define PIC0_ICW3     0x0021
#define PIC0_ICW4     0x0021
#define PIC1_ICW1     0x00a0
#define PIC1_OCW2     0x00a0
#define PIC1_IMR      0x00a1
#define PIC1_ICW2     0x00a1
#define PIC1_ICW3     0x00a1
#define PIC1_ICW4     0x00a1

#define PORT_KEYDAT   0x0060

struct KEYBUF {
  char data;
  unsigned char flag;
} keybuf;

void init_pic();
void handle_intr(int *esp);
void handle_intr27(int *esp);

#endif
