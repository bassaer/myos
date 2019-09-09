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
#include <queue.h>

extern struct Queue key_queue;

void main(void) {
  struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
  init_gdtidt();
  show_status("OK", "GDT");
  init_pic();
  show_status("OK", "PIC");
  io_sti();
  char *prompt = ">";
  outb_p(PIC0_IMR, 0xf9);
  show_status("OK", "Start MyOS");
  init_console();
  while(1) {
    put_str(prompt, GREEN);
    io_cli();
    if (queue_status(&key_queue) == 0) {
      io_stihlt();
    } else {
      int input = queue_get(&key_queue);
      io_sti();
      put_char((char)input, WHITE);
    }
    io_hlt();
  }
}
