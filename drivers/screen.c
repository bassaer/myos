#include <drivers/screen.h>

#include <drivers/font.h>
#include <drivers/palette.h>
#include <lib/string.h>

struct {
  unsigned int width;
  unsigned int height;
  char *vram;
} screen;


void init_screen(char *vram, unsigned int width, unsigned int height) {
  screen.width = width;
  screen.height = height;
  screen.vram = vram;

  init_palette(vram);

  fill_box(screen.width, PALETTE_BLUE_GRAY,  0,                 0,                  screen.width - 1,   screen.height - 29);
  fill_box(screen.width, PALETTE_LIGHT_GRAY, 0,                 screen.height - 28, screen.width - 1,   screen.height - 28);
  fill_box(screen.width, PALETTE_WHITE,      0,                 screen.height - 27, screen.width - 1,   screen.height - 27);
  fill_box(screen.width, PALETTE_LIGHT_GRAY, 0,                 screen.height - 26, screen.width - 1,   screen.height -  1);

  fill_box(screen.width, PALETTE_WHITE,      3,                 screen.height - 24, 59,                 screen.height - 24);
  fill_box(screen.width, PALETTE_WHITE,      2,                 screen.height - 24, 2,                  screen.height -  4);
  fill_box(screen.width, PALETTE_DARK_GRAY,  3,                 screen.height -  4, 59,                 screen.height -  4);
  fill_box(screen.width, PALETTE_DARK_GRAY,  59,                screen.height - 23, 59,                 screen.height -  5);
  fill_box(screen.width, PALETTE_BLACK,      2,                 screen.height -  3, 59,                 screen.height -  3);
  fill_box(screen.width, PALETTE_BLACK,      60,                screen.height - 24, 60,                 screen.height -  3);

  fill_box(screen.width, PALETTE_DARK_GRAY,  screen.width - 47, screen.height - 24, screen.width -  4,  screen.height - 24);
  fill_box(screen.width, PALETTE_DARK_GRAY,  screen.width - 47, screen.height - 23, screen.width - 47,  screen.height -  4);
  fill_box(screen.width, PALETTE_WHITE,      screen.width - 47, screen.height -  3, screen.width -  3,  screen.height -  3);
  fill_box(screen.width, PALETTE_WHITE,      screen.width - 3,  screen.height - 24, screen.width -  3,  screen.height -  3);
}

void put_c(int x, int y, char color, char c) {
  int i;
  char *font = FONT[(int)c];
  for (i = 0; i < 16; i++) {
    char *ptr = screen.vram + (y + i) * screen.width + x;
    char data = font[i];

    if ((data & 0x80) != 0) { ptr[0] = color; }
    if ((data & 0x40) != 0) { ptr[1] = color; }
    if ((data & 0x20) != 0) { ptr[2] = color; }
    if ((data & 0x10) != 0) { ptr[3] = color; }
    if ((data & 0x08) != 0) { ptr[4] = color; }
    if ((data & 0x04) != 0) { ptr[5] = color; }
    if ((data & 0x02) != 0) { ptr[6] = color; }
    if ((data & 0x01) != 0) { ptr[7] = color; }
  }
}

void put_s(int x, int y, char color, char *str) {
  while (*str != '\0') {
    put_c(x, y, color, *str);
    str++;
    x += 8;
  }
}
