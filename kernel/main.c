/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/

#include <dsctbl.h>
#include <console.h>
#include <color.h>

void io_hlt(void);

void main(void) {
  init_gdtidt();
  init_screen();
  char prompt[] = ">";
  while(1) {
    put_str(prompt, GRAY);
    io_hlt();
  }
}
