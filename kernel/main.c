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
#include <drivers/palette.h>
#include <drivers/screen.h>
#include <drivers/vram.h>

#define KEYBUF_LIMIT    32
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

  init_screen(boot_info->vram, boot_info->width, boot_info->height);

  put_s(8, 8, PALETTE_WHITE, "hello world");

  while(1) {
    io_hlt();
  }
  return 0;
}
