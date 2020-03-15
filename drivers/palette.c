#include <drivers/palette.h>

#include <io.h>

#define PALETTE_NUM_PORT    0x03c8
#define PALETTE_COLOR_PORT  0x03c9
#define PALETTE_ADDR        0xa0000

char *vram = (char *)PALETTE_ADDR;

void init_palette(char *_vram) {
  vram = _vram;

  static unsigned char rgb_table[16 * 3] = {
    0x00, 0x00, 0x00, //  0: black
    0xff, 0x00, 0x00, //  1: red
    0x00, 0xff, 0x00, //  2: green
    0xff, 0xff, 0x00, //  3: yellow
    0x00, 0x00, 0xff, //  4: blue
    0xff, 0x00, 0xff, //  5: purple
    0x00, 0xff, 0xff, //  6: light blue
    0xff, 0xff, 0xff, //  7: white
    0xc6, 0xc6, 0xc6, //  8: litght gray
    0x84, 0x00, 0x00, //  9: dark red
    0x00, 0x84, 0x00, // 10: dark green
    0x84, 0x84, 0x00, // 11: dark yellow
    0x00, 0x00, 0x84, // 12: dark blue
    0x84, 0x00, 0x84, // 13: dark purple
    0x00, 0x84, 0x84, // 14: blue gray
    0x84, 0x84, 0x84, // 15: dark gray
  };
  set_palette(0, 15, rgb_table);
}

void set_palette(int start, int end, unsigned char *rgb) {
  // eflagsを保持
  int eflags = io_load_eflags();
  // 割り込み禁止
  io_cli();

  // パレットの色を設定
  outb_p(PALETTE_NUM_PORT, start);
  int i;
  for(i = start; i <= end; i++) {
    outb_p(PALETTE_COLOR_PORT, rgb[0] / 4);
    outb_p(PALETTE_COLOR_PORT, rgb[1] / 4);
    outb_p(PALETTE_COLOR_PORT, rgb[2] / 4);
    rgb += 3;
  }

  // eflagsを戻す
  io_store_eflags(eflags);
}

/**
 * 四角形描画
 * width: 画面幅
 */
void fill_box(int width, unsigned int color, int start_x, int start_y, int end_x, int end_y) {
  int x, y;
  for (y = start_y; y <= end_y; y++) {
    for (x = start_x; x <= end_x; x++) {
      vram[y * width + x] = color;
    }
  }
}
