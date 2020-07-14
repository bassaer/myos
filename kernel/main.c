/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/

#include<uefi.h>

void kernel_main(BootInfo *boot) {
  UINTN x, y;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pixel;
  BOOLEAN reverse = false;
  for (y = 0; y < boot->height; y++) {
    for (x = 0; x < boot->width; x++) {
      pixel = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)boot->framebuffer + (boot->width * y) + x;
      if (x % 100 == 0) {
        reverse = !reverse;
      }
      if (reverse) {
        pixel->Red = 0;
        pixel->Green = 229;
        pixel->Blue = 255;
      } else {
        pixel->Red = 0;
        pixel->Green = 184;
        pixel->Blue = 212;
      }
    }
  }
  while (1) __asm__ volatile("hlt");
}
