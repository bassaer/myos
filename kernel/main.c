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
#include <drivers/vram.h>
#include <drivers/screen.h>

#define KEYBUF_LIMIT    32

int main(void) {
  init_screen();
  while(1) {
    io_hlt();
  }
  return 0;
}
