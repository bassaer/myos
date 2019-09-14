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


int main(void) {
  init_gdtidt();
  show_status("OK", "GDT");

  init_pic();
  show_status("OK", "PIC");

  io_sti();
  outb_p(PIC0_IMR, 0xf9);

  struct Queue queue;
  unsigned char keybuf[32];
  init_queue(&queue, 32, keybuf);
  init_keyboard(&queue);
  show_status("OK", "Keyboad");

  init_console();
  char *prompt = ">";
  put_str(prompt, GREEN);

  while(1) {
    io_cli(); // 割り込み無効化
    if (queue_status(&queue) == 0) {
      io_stihlt(); // 割り込み有効化 + HLT
    } else {
      int input = dequeue(&queue);
      io_sti(); // 割り込み有効化
      if (input == 0x1e) {
        put_str("A\n", GRAY);
      } else {
        put_str("NOT A\n", GRAY);
      }
    }
  }
  return 0;
}
