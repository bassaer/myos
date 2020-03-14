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
#define FIRST_BYTE_MASK       0xc8
#define FIRST_BYTE_OK         0x08
#define CURSOR_BUTTON_MASK    0x07
#define CURSOR_X_MASK         0x10
#define CURSOR_Y_MASK         0x20

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
  char cursor[16*16];
  int display_width;
  int display_height;
  queue_t *queue;
  unsigned char buf[3];
  enum cursor_status status;
  int curr_x;
  int curr_y;
  int dx;
  int dy;
  int button;
} cursor_info;

void init_cursor(queue_t *queue, char bg_color, int width, int height) {
  cursor_info.queue = queue;
  cursor_info.status = WAITING;
  cursor_info.display_width = width;
  cursor_info.display_height = height;
  cursor_info.curr_x = width / 2;
  cursor_info.curr_y = height / 2;
  cursor_info.dx = 0;
  cursor_info.dy = 0;

  int i;
  for(i = 0; i < 3; i++) {
    cursor_info.buf[i] = 0;
  }
  int x, y;
  for (y = 0; y < WIDTH; y++) {
    for (x = 0; x < WIDTH; x++) {
      if (CURSOR[y][x] == '*') {
        cursor_info.cursor[y * WIDTH + x] = PALETTE_BLACK;
      }
      if (CURSOR[y][x] == 'O') {
        cursor_info.cursor[y * WIDTH + x] = PALETTE_WHITE;
      }
      if (CURSOR[y][x] == '.') {
        cursor_info.cursor[y * WIDTH + x] = bg_color;
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

void decode_cursor(unsigned char data) {
  switch(cursor_info.status) {
    case WAITING:
      if (data == CURSOR_ACK) {
        cursor_info.status = FIRST_BIT;
      }
      return;
    case FIRST_BIT:
      // 1バイト目のチェック
      if((data & FIRST_BYTE_MASK) == FIRST_BYTE_OK) {
        cursor_info.buf[0] = data;
        cursor_info.status = SECOND_BIT;
      }
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
  // ボタンの状態を下位3bitから取得
  cursor_info.button = cursor_info.buf[0] & CURSOR_BUTTON_MASK;
  // 基本的にx, yにはそのまま2, 3バイト目をいれる
  cursor_info.dx = cursor_info.buf[1];
  cursor_info.dy = cursor_info.buf[2];
  // １バイト目の移動に反応する桁の情報を使って
  // 8bit目以上を全部1 or 0にするか決める
  if ((cursor_info.buf[0] & CURSOR_X_MASK) != 0) {
    cursor_info.dx |= 0xffffff00;
  }
  if ((cursor_info.buf[0] & CURSOR_Y_MASK) != 0) {
    cursor_info.dy |= 0xffffff00;
  }
  // yの符号は画面の方向と逆
  cursor_info.dy *= -1;

  // debug
  char *btn = "-";
  if ((cursor_info.button & 0x01) != 0) {
    btn = "L";
  }
  if ((cursor_info.button & 0x02) != 0) {
    btn = "R";
  }
  if ((cursor_info.button & 0x04) != 0) {
    btn = "C";
  }
  fill_box(320, PALETTE_BLUE_GRAY, 0, 48, 320, 64);
  char str[256];
  sprintf(str, "btn: %s, (%d, %d)", btn, cursor_info.dx, cursor_info.dy);
  put_s(0, 48, PALETTE_WHITE, str);
}

void update_cursor() {
  fill_box(
      cursor_info.display_width, PALETTE_BLUE_GRAY,
      cursor_info.curr_x , cursor_info.curr_y,
      cursor_info.curr_x + WIDTH, cursor_info.curr_y + WIDTH
  );
  cursor_info.curr_x += cursor_info.dx;
  cursor_info.curr_y += cursor_info.dy;
  if (cursor_info.curr_x < 0) {
    cursor_info.curr_x = 0;
  }
  if (cursor_info.curr_y < 0) {
    cursor_info.curr_y = 0;
  }
  if (cursor_info.curr_x + WIDTH > cursor_info.display_width) {
    cursor_info.curr_x = cursor_info.display_width - WIDTH;
  }
  if (cursor_info.curr_y + WIDTH > cursor_info.display_height) {
    cursor_info.curr_y = cursor_info.display_height - WIDTH;
  }
  put_block(WIDTH, WIDTH, cursor_info.curr_x, cursor_info.curr_y, cursor_info.cursor, WIDTH);
}
