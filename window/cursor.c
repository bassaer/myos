#include <window/cursor.h>
#include <window/desktop.h>

#define CURSOR_WIDTH 16

struct {
  unsigned int x;
  unsigned int y;
  int frame[CURSOR_WIDTH * CURSOR_WIDTH];
  int bg_color;
} cursor;


char CURSOR[CURSOR_WIDTH][CURSOR_WIDTH] = {
  "...**...........",
  "...*O*..........",
  "...*OO*.........",
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

void init_cursor(unsigned int x, unsigned int y, int bg_color) {
  cursor.x = x;
  cursor.y = y;
  cursor.bg_color = bg_color;
  unsigned int i, j;
  for (i = 0; i < CURSOR_WIDTH; i++) {
    for (j = 0; j < CURSOR_WIDTH; j++) {
      switch(CURSOR[j][i]) {
        case '*':
          cursor.frame[j * CURSOR_WIDTH + i] = BLACK;
          break;
        case 'O':
          cursor.frame[j * CURSOR_WIDTH + i] = WHITE;
          break;
        default:
          cursor.frame[j * CURSOR_WIDTH + i] = bg_color;
      }
    }
  }
}

void update_cursor(unsigned int x, unsigned int y) {
  fill_box(cursor.bg_color, cursor.x, cursor.y, cursor.x + CURSOR_WIDTH, cursor.y + CURSOR_WIDTH);
  cursor.x = x;
  cursor.y = y;
  put_block(cursor.x, cursor.y, CURSOR_WIDTH, CURSOR_WIDTH, cursor.frame);
}
