#include <drivers/cursor.h>

#include <io.h>
#include <intr.h>
#include <keyboard.h>
#include <drivers/palette.h>
#include <drivers/screen.h>
#include <lib/queue.h>
#include <lib/string.h>

#define WIDTH                 16
#define KEYCMD_SENDTO_CURSOR  0xd4
#define CURSORCMD_ENABLE      0xf4

char CURSOR[16][16] = {
  "...**...........",
  "...*O*..........",
  "...*OO*........",
  "...*OOO*........",
  "...*OOOO*.......",
  "...*OOOOO*......",
  "...*OOOOOO*.....",
  "...*OOOOOOO*....",
  "...*OOOOOOOO*...",
  "...*OOOOOOOOO*..",
  "...*OOOOOOOOOO*.",
  "...*OOOO*******.",
  "...*OOO*........",
  "...*OO*.........",
  "...*O*..........",
  "...**..........."
};

queue_t *cursor_queue;

void init_cursor(char *cursor, queue_t *queue, char bg_color) {
  cursor_queue = queue;
  int x, y;
  for (y = 0; y < WIDTH; y++) {
    for (x = 0; x < WIDTH; x++) {
      if (CURSOR[y][x] == '*') {
        cursor[y * WIDTH + x] = PALETTE_BLACK;
      }
      if (CURSOR[y][x] == 'O') {
        cursor[y * WIDTH + x] = PALETTE_WHITE;
      }
      if (CURSOR[y][x] == '.') {
        cursor[y * WIDTH + x] = bg_color;
      }
    }
  }
}

void enable_cursor() {
  wait_keyboard();
  outb_p(PORT_KEYCMD, KEYCMD_SENDTO_CURSOR);
  wait_keyboard();
  outb_p(PORT_KEYDAT ,CURSORCMD_ENABLE);
  return;
}

void handle_intr2c(int *esp) {
  outb_p(PIC0_OCW2, 0x64); // IRQ-01の受付完了をPIC1に通知
  outb_p(PIC0_OCW2, 0x62); // IRQ-01の受付完了をPIC0に通知

  unsigned char data = io_in(PORT_KEYDAT);
  enqueue(cursor_queue, data);
  return;
}
