#include <console.h>

#include <io.h>
#include <util.h>

#include <stdarg.h>

#define KEY_TABLE_SIZE    0x54

struct {
  unsigned int x;
  unsigned int y;
} cursor;

/**
 *  各KeyのScanCodeがindexになるように配置
 *  https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1
 */
static unsigned char keytable[KEY_TABLE_SIZE] = {
  0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,  0,
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '@', '[', '\n', 0, 'a', 's',
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0,   0,   ']', 'z', 'x', 'c', 'v',
  'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
  '2', '3', '0', '.'
};

void get_key(char *key, unsigned char code) {
  if (code > KEY_TABLE_SIZE) {
    *key = 0;
    return;
  }
  *key = keytable[code];
}

void move_cursor(unsigned int x, unsigned int y) {
    cursor.x = x;
    cursor.y = y;
    unsigned short pos = y * COLUMNS + x + CONSOLE_OFFSET;
    outb_p(0x3D4, 14);
    outb_p(0x3D5, pos >> 8);
    outb_p(0x3D4, 15);
    outb_p(0x3D5, pos);
}

void put_char_pos(char c, unsigned char x, unsigned char y, unsigned short color) {
  unsigned char *pos;
  pos = (unsigned char *)(SCREEN_START + (((y * COLUMNS) + x + CONSOLE_OFFSET) * 2));
  *(unsigned short *)pos = (unsigned short)((color << 8) | c);
}

void backspace() {
  put_char_pos(' ', cursor.x - 1, cursor.y, 0);
  move_cursor(cursor.x - 1, cursor.y);
}

void newline() {
  move_cursor(0, cursor.y + 1);
}

void clear() {
  while(cursor.x > 0) {
    backspace();
  }
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

int itoa(int src, char *dst, int base) {
  int len = 0;
  int buf[10];

  while(1) {
    buf[len++] = src % base;
    if (src < base) {
      break;
    }
    src /= base;
  }

  // 桁
  int digit = len;

  while(len) {
    --len;
    *(dst++) = buf[len] < 10 ? buf[len] + 0x30 : buf[len] - 9 + 0x60;
  }

  return digit;
}

void printf(char *format, ...) {
  char buf[256];
  char *str = buf;
  va_list list;
  char *s_arg;
  va_start(list, format);
  while(*format) {
    if (*format == '%') {
      ++format;
      switch(*format) {
        case 'd':
          str += itoa(va_arg(list, int), str, 10);
          break;
        case 'x':
          *(str++) = '0';
          *(str++) = 'x';
          str += itoa(va_arg(list, int), str, 16);
          break;
        case 'c':
          *(str++) = va_arg(list, int);
          break;
        case 's':
          s_arg = va_arg(list, char*);
          while(*s_arg) {
            *(str++) = *(s_arg++);
          }
          break;
      }
      format++;
    } else {
      *(str++) = *(format++);
    }
  }
  // NULL終端
  *str = 0x00;
  put_str(buf, CHAR_COLOR);
  va_end(list);
}

