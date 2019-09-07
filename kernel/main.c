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
#include <io.h>

void main(void) {
  init_gdtidt();
  init_screen();
  char prompt[] = ">";
  while(1) {
    put_str(prompt, GREEN);
    hlt();
  }
}
