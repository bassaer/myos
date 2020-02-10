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
#include <queue.h>
#include <sh.h>

#define KEYBUF_LIMIT    32

int main(void) {
  init_gdtidt();
  init_pic();

  io_sti();
  outb_p(PIC0_IMR, 0xf9);

  struct Queue queue;
  unsigned char keybuf[KEYBUF_LIMIT];

  init_queue(&queue, KEYBUF_LIMIT, keybuf);
  init_keyboard(&queue);

  init_shell();

  while(1) {
    io_cli(); // 割り込み無効化
    if (queue_status(&queue) == 0) {
      io_stihlt(); // 割り込み有効化 + HLT
    } else {
      start_shell(&queue);
      io_sti(); // 割り込み有効化
    }
  }

  return 0;
}
