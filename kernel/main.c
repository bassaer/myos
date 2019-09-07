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

void io_hlt(void);
void init_console();

void main(void) {
  init_gdtidt();
  init_screen();
  char prompt[] = ">";
  put_str(prompt);
  while(1);
}
