#include <window/desktop.h>
#include <window/font.h>

#include <stdarg.h>
#include <lib/string.h>

struct {
  unsigned int width;
  unsigned int height;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *framebuffer;
} screen;

void init_desktop(BootInfo *boot) {
  screen.width = boot->width;
  screen.height = boot->height;
  screen.framebuffer = boot->framebuffer;

  unsigned int bar_y = screen.height * 0.92;
  unsigned int bar_height = screen.height - bar_y;
  unsigned int box_width =  bar_height * 0.7;
  unsigned int margin = (bar_height - box_width) / 2;

  fill_box(BLUE_GRAY_800, 0, 0, screen.width, screen.height);
  fill_box(BLUE_GRAY_500, 0, bar_y, screen.width, screen.height);
  fill_box(BLUE_GRAY_400, margin, bar_y + margin, margin + box_width * 2, bar_y + margin + box_width);
  printf(margin * 3 , bar_y + margin * 2.5, "MyOS");
}

void set_color(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pixel, int color) {
  pixel->Red   = (color >> 16) & 0xFF;
  pixel->Green = (color >>  8) & 0xFF;
  pixel->Blue  = (color      ) & 0xFF;
}

void fill_box(int color, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y) {
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pixel;
  unsigned int x, y;
  for (y = start_y; y < end_y; y++) {
    for (x = start_x; x < end_x; x++) {
      pixel = screen.framebuffer + (screen.width * y) + x;
      set_color(pixel, color);
    }
  }
}

void put_c(int x, int y, int color, char c) {
  char *font = FONT[(int)c];
  int i;
  for (i = 0; i < 16; i++) {
    // 8x16 font
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pixel = screen.framebuffer + (y + i) * screen.width + x;
    char data = font[i];

    if ((data & 0x80) != 0) { set_color(&pixel[0], color); }
    if ((data & 0x40) != 0) { set_color(&pixel[1], color); }
    if ((data & 0x20) != 0) { set_color(&pixel[2], color); }
    if ((data & 0x10) != 0) { set_color(&pixel[3], color); }
    if ((data & 0x08) != 0) { set_color(&pixel[4], color); }
    if ((data & 0x04) != 0) { set_color(&pixel[5], color); }
    if ((data & 0x02) != 0) { set_color(&pixel[6], color); }
    if ((data & 0x01) != 0) { set_color(&pixel[7], color); }
  }
}

void put_s(int x, int y, int color, char *str) {
  int next_x = x;
  int next_y = y;
  while (*str != '\0') {
    if (*str == '\n') {
      next_x = x;
      next_y += FONT_HEIGHT;
    } else {
      put_c(next_x, next_y, color, *str);
      next_x += FONT_WIDTH;
    }
    str++;
  }
}

void printf(int x, int y, char *format, ...) {
  char buf[256];
  va_list list;
  va_start(list, format);
  format_str(buf, format, &list);
  put_s(x, y, WHITE, buf);
  va_end(list);
}
