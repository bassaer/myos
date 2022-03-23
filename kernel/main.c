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
  device_context_t* device_context[MAX_DEVICE_SLOTS + 1];
  init_xhci(&device_context);
  while (1) __asm__ volatile("hlt");
}

