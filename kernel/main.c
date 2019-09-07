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

void main(void) {
  struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
  init_gdtidt();
  init_pic();
  io_sti();
  outb_p(PIC0_IMR, 0xf9);
  init_console();
  char prompt[] = "> ";
  keybuf.flag = 0;
  while(1) {
    put_str(prompt, GREEN);
    //io_cli();
    //if (keybuf.flag == 0) {
    //  io_stihlt();
    //} else {
    //  //char in = keybuf.data;
    //  keybuf.flag = 0;
    //  io_sti();
    //  char in[] = "keydown";
    //  put_str(in, WHITE);
    //}
    io_hlt();
  }
}
