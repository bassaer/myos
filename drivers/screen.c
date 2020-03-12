#include <drivers/screen.h>

#include <drivers/palette.h>

#define WIDTH    320
#define HEIGHT   200

void init_screen() {
  init_palette();
  fill_box(WIDTH, PALETTE_BLUE_GRAY,  0,          0,            WIDTH - 1,  HEIGHT - 29);
  fill_box(WIDTH, PALETTE_LIGHT_GRAY, 0,          HEIGHT - 28,  WIDTH - 1,  HEIGHT - 28);
  fill_box(WIDTH, PALETTE_WHITE,      0,          HEIGHT - 27,  WIDTH - 1,  HEIGHT - 27);
  fill_box(WIDTH, PALETTE_LIGHT_GRAY, 0,          HEIGHT - 26,  WIDTH - 1,  HEIGHT -  1);

  fill_box(WIDTH, PALETTE_WHITE,      3,          HEIGHT - 24,         59,  HEIGHT - 24);
  fill_box(WIDTH, PALETTE_WHITE,      2,          HEIGHT - 24,          2,  HEIGHT -  4);
  fill_box(WIDTH, PALETTE_DARK_GRAY,  3,          HEIGHT -  4,         59,  HEIGHT -  4);
  fill_box(WIDTH, PALETTE_DARK_GRAY,  59,         HEIGHT - 23,         59,  HEIGHT -  5);
  fill_box(WIDTH, PALETTE_BLACK,      2,          HEIGHT -  3,         59,  HEIGHT -  3);
  fill_box(WIDTH, PALETTE_BLACK,      60,         HEIGHT - 24,         60,  HEIGHT -  3);

  fill_box(WIDTH, PALETTE_DARK_GRAY,  WIDTH - 47, HEIGHT - 24,  WIDTH -  4, HEIGHT - 24);
  fill_box(WIDTH, PALETTE_DARK_GRAY,  WIDTH - 47, HEIGHT - 23,  WIDTH - 47, HEIGHT -  4);
  fill_box(WIDTH, PALETTE_WHITE,      WIDTH - 47, HEIGHT -  3,  WIDTH -  3, HEIGHT -  3);
  fill_box(WIDTH, PALETTE_WHITE,      WIDTH - 3,  HEIGHT - 24,  WIDTH -  3, HEIGHT -  3);
}

