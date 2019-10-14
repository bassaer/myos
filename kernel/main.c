/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/
#include <main.h>

#include <dsctbl.h>
#include <intr.h>
#include <io.h>
#include <sh.h>


int main(void) {
  init_gdtidt();
  init_pic();

  io_sti();
  outb_p(PIC0_IMR, 0xf9);

  start_shell();

  return 0;
}
