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
#define CURSOR_ACK            0xfa

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



enum cursor_status {
  WAITING,
  FIRST_BIT,
  SECOND_BIT,
  THIRD_BIT,
};

struct {
  queue_t *queue;
  unsigned char buf[3];
  enum cursor_status status;
} cursor_info;

void init_cursor(char *cursor, queue_t *queue, char bg_color) {
  cursor_info.queue = queue;
  cursor_info.status = WAITING;
  int i;
  for(i = 0; i < 3; i++) {
    cursor_info.buf[i] = 0;
  }

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
  cursor_info.status = WAITING;
  return;
}

void handle_intr2c(int *esp) {
  outb_p(PIC1_OCW2, 0x64); // IRQ-01の受付完了をPIC1に通知
  outb_p(PIC0_OCW2, 0x62); // IRQ-01の受付完了をPIC0に通知

  unsigned char data = io_in(PORT_KEYDAT);
  enqueue(cursor_info.queue, data);
  return;
}

void update_cursor(unsigned char data) {
  switch(cursor_info.status) {
    case WAITING:
      if (data == CURSOR_ACK) {
        cursor_info.status = FIRST_BIT;
      }
      return;
    case FIRST_BIT:
      cursor_info.buf[0] = data;
      cursor_info.status = SECOND_BIT;
      return;
    case SECOND_BIT:
      cursor_info.buf[1] = data;
      cursor_info.status = THIRD_BIT;
      return;
    case THIRD_BIT:
      cursor_info.status = FIRST_BIT;
      cursor_info.buf[2] = data;
      break;
    default:
      return;
  }
  fill_box(320, PALETTE_BLUE_GRAY, 0, 48, 320, 64);
  char str[256];
  sprintf(str, "status: %d, data: %x, %x, %x, %x", cursor_info.status, data, cursor_info.buf[0], cursor_info.buf[1], cursor_info.buf[2]);
  put_s(0, 48, PALETTE_WHITE, str);
}
