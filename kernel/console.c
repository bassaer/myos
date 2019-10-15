#include <console.h>
#include <io.h>

struct {
  unsigned int x;
  unsigned int y;
} cursor;

static unsigned char keytable[0x54] = {
  0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '@', '[', '\n',   0,   'a', 's',
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0,   0,   ']', 'z', 'x', 'c', 'v',
  'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
  '2', '3', '0', '.'
};

void get_key(char *key, unsigned char code) {
  *key = keytable[code];
}

void move_cursor(unsigned int x, unsigned int y) {
    cursor.x = x;
    cursor.y = y;
    unsigned short pos = y * COLUMNS + x;
    outb_p(0x3D4, 14);
    outb_p(0x3D5, pos >> 8);
    outb_p(0x3D4, 15);
    outb_p(0x3D5, pos);
}

void put_char_pos(char c, unsigned char x, unsigned char y, unsigned short color) {
  unsigned char *pos;
  pos = (unsigned char *)(SCREEN_START + (((y * COLUMNS) + x) * 2));
  *(unsigned short *)pos = (unsigned short)((color << 8) | c);
}

void backspace() {
  put_char_pos(' ', cursor.x - 1, cursor.y, 0);
  move_cursor(cursor.x - 1, cursor.y);
}

void newline() {
  move_cursor(0, cursor.y + 1);
}

void put_char(char c, unsigned short color) {
  switch(c) {
    case '\0':
      break;
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

void shutdown() {
  outb_p(0x8900, 'S');
  outb_p(0x8900, 'h');
  outb_p(0x8900, 'u');
  outb_p(0x8900, 't');
  outb_p(0x8900, 'd');
  outb_p(0x8900, 'o');
  outb_p(0x8900, 'w');
  outb_p(0x8900, 'n');
  outb_p(0x501, 0x31);
}
