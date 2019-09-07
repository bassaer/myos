/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/

#include <color.h>
#include <console.h>
#include <dsctbl.h>
#include <io.h>
#include <pic.h>

void main(void) {
  init_gdtidt();
  init_pic();
  init_console();
  char prompt[] = ">";
  while(1) {
    put_str(prompt, GREEN);
    hlt();
  }
}
