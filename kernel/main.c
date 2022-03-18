/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/

#include <uefi.h>
#include <graphics/desktop.h>
#include <drivers/xhci.h>

void kernel_main(BootInfo *boot) {
  init_desktop(boot);
  init_xhci();
  while (1) __asm__ volatile("hlt");
}

