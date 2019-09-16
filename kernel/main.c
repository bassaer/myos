/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/
#include <main.h>

#include <color.h>
#include <console.h>
#include <dsctbl.h>
#include <intr.h>
#include <io.h>
#include <keyboard.h>
#include <queue.h>

#define KEYBUF_LIMIT   32
#define CMD_LIMIT      256


int main(void) {
  init_gdtidt();
  show_status("OK", "GDT");

  init_pic();
  show_status("OK", "PIC");

  io_sti();
  outb_p(PIC0_IMR, 0xf9);

  struct Queue queue;
  char keybuf[KEYBUF_LIMIT];
  init_queue(&queue, KEYBUF_LIMIT, keybuf);
  init_keyboard(&queue);
  show_status("OK", "Keyboad");

  char cmdline[CMD_LIMIT];
  init_console(&cmdline, CMD_LIMIT);

  while(1) {
    io_cli(); // 割り込み無効化
    if (queue_status(&queue) == 0) {
      io_stihlt(); // 割り込み有効化 + HLT
    } else {
      char key;
      dequeue(&queue, &key);
      io_sti(); // 割り込み有効化
      input_key(key);
    }
  }
  return 0;
}
