/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/
#include <main.h>

#include <console.h>
#include <dsctbl.h>
#include <intr.h>
#include <io.h>
#include <keyboard.h>
#include <sched.h>
#include <timer.h>
#include <sh.h>
#include <mm/memory.h>
#include <lib/string.h>
#include <drivers/cursor.h>
#include <drivers/palette.h>
#include <drivers/screen.h>
#include <drivers/vram.h>

#define QUEUE_LIMIT    32
#define BOOT_INFO_ADDR  0x0ff0

int main() {
  boot_info_t *boot_info = (boot_info_t *)BOOT_INFO_ADDR;

  init_gdtidt();
  init_pic();

  io_sti();
  outb_p(PIC0_IMR, 0xf8); // PIT, PIC1, キーボードを許可(11111000)

  init_pit();

  unsigned char keyboard_buf[QUEUE_LIMIT];
  queue_t keyboard_q;
  init_queue(&keyboard_q, QUEUE_LIMIT, keyboard_buf);
  init_keyboard(&keyboard_q);

  init_mem_info();

  if (boot_info->gui == 1) {
    start_gui(boot_info, &keyboard_q);
  } else {
    start_cui(&keyboard_q);
  }
  return 0;
}

void start_cui(queue_t *keyboard_q) {
  init_shell();
  init_sched();

  while(1) {
    io_cli(); // 割り込み無効化
    if (queue_status(keyboard_q) == 0) {
      io_stihlt(); // 割り込み有効化 + HLT
    } else {
      start_shell(keyboard_q);
      io_sti(); // 割り込み有効化
    }
  }
}

void start_gui(boot_info_t *boot_info, queue_t *keyboard_q) {
  outb_p(PIC1_IMR, 0xef); // PIC1, カーソルを許可(11101111)

  init_screen(boot_info->vram, boot_info->width, boot_info->height);

  unsigned char cursor_buf[QUEUE_LIMIT];
  queue_t cursor_q;
  init_queue(&cursor_q, QUEUE_LIMIT, cursor_buf);
  init_cursor(&cursor_q ,PALETTE_BLUE_GRAY, boot_info->width, boot_info->height);
  enable_cursor();

  while(1) {
    io_cli(); // 割り込み無効化
    if (queue_status(keyboard_q) != 0) {
      fill_box(boot_info->width, PALETTE_BLUE_GRAY, 0, 16, 320, 32);

      int data = dequeue(keyboard_q);
      char buf[256];
      sprintf(buf, "kb -> %x", data);
      put_s(0, 16, PALETTE_WHITE, buf);

      io_sti(); // 割り込み有効化
    } else if (queue_status(&cursor_q) != 0) {
      int data = dequeue(&cursor_q);
      decode_cursor(data);
      update_cursor();
      io_sti();
    } else {
      io_stihlt(); // 割り込み有効化 + HLT
    }
  }
}
