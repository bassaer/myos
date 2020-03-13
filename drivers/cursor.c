#include <drivers/cursor.h>

#include <drivers/palette.h>

#define WIDTH   16

char CURSOR2[16][16] = {
  "**************..",
  "*OOOOOOOOOOO*...",
  "*OOOOOOOOOO*....",
  "*OOOOOOOOO*.....",
  "*OOOOOOOO*......",
  "*OOOOOOO*.......",
  "*OOOOOOO*.......",
  "*OOOOOOOO*......",
  "*OOOO**OOO*.....",
  "*OOO*..*OOO*....",
  "*OO*....*OOO*...",
  "*O*......*OOO*..",
  "**........*OOO*.",
  "*..........*OOO*",
  "............*OO*",
  ".............***"
};

char CURSOR[16][16] = {
  "...**...........",
  "...*O*..........",
  "...*OO*........",
  "...*OOO*........",
  "...*OOOO*.......",
  "...*OOOOO*......",
  "...*OOOOOO*.....",
  "...*OOOOOOO*....",
  "...*OOOOOOOO*...",
  "...*OOOOOOOOO*..",
  "...*OOOOOOOOOO*.",
  "...*OOOO*******.",
  "...*OOO*........",
  "...*OO*.........",
  "...*O*..........",
  "...**..........."
};


void init_cursor(char *cursor, char bg_color) {
  int x, y;
  for (y = 0; y < WIDTH; y++) {
    for (x = 0; x < WIDTH; x++) {
      if (CURSOR[y][x] == '*') {
        cursor[y * WIDTH + x] = PALETTE_BLACK;
      }
      if (CURSOR[y][x] == 'O') {
        cursor[y * WIDTH + x] = PALETTE_WHITE;
      }
      if (CURSOR[y][x] == '.') {
        cursor[y * WIDTH + x] = bg_color;
      }
    }
  }
}
