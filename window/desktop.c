#include <window/desktop.h>

void init_desktop(BootInfo *boot) {
  UINTN bar_y = boot->height * 0.92;
  UINTN bar_height = boot->height - bar_y;
  UINTN box_width =  bar_height * 0.7;
  UINTN margin = (bar_height - box_width) / 2;

  fill_box(boot, BLUE_GRAY_800, 0, 0, boot->width, boot->height);
  fill_box(boot, BLUE_GRAY_500, 0, bar_y, boot->width, boot->height);
  fill_box(boot, BLUE_GRAY_400, margin, bar_y + margin, margin + box_width * 2, bar_y + margin + box_width);
}

void fill_box(BootInfo *boot, int color, UINTN start_x, UINTN start_y, UINTN end_x, UINTN end_y) {
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pixel;
  UINTN x, y;
  for (y = start_y; y < end_y; y++) {
    for (x = start_x; x < end_x; x++) {
      pixel = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)boot->framebuffer + (boot->width * y) + x;
      pixel->Red   = (UINTN)(color >> 16) & 0xFF;
      pixel->Green = (UINTN)(color >>  8) & 0xFF;
      pixel->Blue  = (UINTN)(color      ) & 0xFF;
    }
  }
}

