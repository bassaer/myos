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

void main(void) {
  init_gdtidt();
  move_cursor(0, 0);
  char str[] = "hello\nworld";
  put_str(str);
  while(1);
}
