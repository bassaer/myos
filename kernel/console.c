#include <console.h>
#include <color.h>
#include <io.h>

struct {
  unsigned int x;
  unsigned int y;
} cursor;

void move_cursor(unsigned int x, unsigned int y) {
    cursor.x = x;
    cursor.y = y;
    unsigned short pos = y * COLUMNS + x;
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos);
}

void put_char_pos(char c, unsigned char x, unsigned char y, unsigned short color) {
  unsigned char *pos;
  pos = (unsigned char *)(SCREEN_START + (((y * COLUMNS) + x) * 2));
  *(unsigned short *)pos = (unsigned short)((color << 8) | c);
}

void put_char(char c, unsigned short color) {
  switch(c) {
    case '\r':
      move_cursor(0, cursor.y);
      break;
    case '\n':
      move_cursor(0, cursor.y + 1);
      break;
    default:
      put_char_pos(c, cursor.x, cursor.y, color);
      if (cursor.x < COLUMNS - 1) {
        move_cursor(cursor.x + 1, cursor.y);
      } else {
        move_cursor(0, cursor.y + 1);
      }
      break;
  }
}

void put_str(char *str, unsigned short color) {
  while (*str != '\0') {
    put_char(*str, color);
    str++;
  }
}

void init_screen() {
  move_cursor(0, 0);
  char osname[6][34] = {
    "  __  ____   _____  ____       \n",
    " |  \\/  \\ \\ / / _ \\/ ___|  \n",
    " | |\\/| |\\ V / | | \\___ \\  \n",
    " | |  | | | || |_| |___) |     \n",
    " |_|  |_| |_| \\___/|____/   \n\n"
  };

  int i;
  for (i = 0; i < 6; ++i) {
    put_str(osname[i], GRAY);
  }
}
