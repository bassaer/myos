/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/
#include <console.h>
#include <dsctbl.h>
#include <intr.h>
#include <io.h>
#include <keyboard.h>
#include <sched.h>
#include <timer.h>
#include <sh.h>
#include <mm/memory.h>
#include <lib/queue.h>
#include <lib/string.h>
#include <drivers/cursor.h>
#include <drivers/palette.h>
#include <drivers/screen.h>
#include <drivers/vram.h>

#define QUEUE_LIMIT    32
#define BOOT_INFO_ADDR  0x0ff0

typedef struct {
  char cyls;
  char leds;
  char vmode;
  char reserve;
  short width;
  short height;
  char  *vram;
} boot_info_t;

int main(void) {
  boot_info_t *boot_info = (boot_info_t *)BOOT_INFO_ADDR;

  init_gdtidt();
  init_pic();

  io_sti();
  outb_p(PIC0_IMR, 0xf8); // PIT, PIC1, キーボードを許可(11111000)
  outb_p(PIC1_IMR, 0xef); // PIT, PIC1, カーソルを許可(11101111)

  init_screen(boot_info->vram, boot_info->width, boot_info->height);

  unsigned char keyboard_buf[QUEUE_LIMIT];
  queue_t keyboard_q;
  init_queue(&keyboard_q, QUEUE_LIMIT, keyboard_buf);
  init_keyboard(&keyboard_q);

  unsigned char cursor_buf[QUEUE_LIMIT];
  queue_t cursor_q;
  init_queue(&cursor_q, QUEUE_LIMIT, cursor_buf);
  char cursor[16*16];
  init_cursor(cursor, &cursor_q ,PALETTE_BLUE_GRAY);
  enable_cursor();

  put_block(16, 16, boot_info->width / 2, (boot_info->height - 50) / 2, cursor, 16);

  while(1) {
    io_cli(); // 割り込み無効化
    if (queue_status(&keyboard_q) != 0) {
      fill_box(boot_info->width, PALETTE_BLUE_GRAY, 0, 16, 320, 32);

      int data = dequeue(&keyboard_q);
      char buf[256];
      sprintf(buf, "kb -> %x", data);
      put_s(0, 16, PALETTE_WHITE, buf);

      io_sti(); // 割り込み有効化
    } else if (queue_status(&cursor_q) != 0) {
      int data = dequeue(&cursor_q);
      update_cursor(data);
      io_sti();
    } else {
      io_stihlt(); // 割り込み有効化 + HLT
    }
  }
  return 0;
}
