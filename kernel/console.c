#include <console.h>

#include <io.h>

#include <stdarg.h>

#define KEY_TABLE_SIZE    0x54

struct {
  unsigned int x;
  unsigned int y;
} cursor;

/**
 * 画面情報配列
 * スクロール分を考慮し画面の2倍を保持する
 */
unsigned short screen_buffer[ROWS*2][COLUMNS*2];

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

void init_vram() {
  unsigned short white_space = (unsigned short)((CHAR_COLOR << 8) | ' ');
  unsigned char x, y;
  for(y = 0; y < ROWS*2; ++y) {
    for(x = 0; x < COLUMNS*2; ++x) {
      write_vram(white_space, x, y);
    }
  }
}

void init_console() {
  unsigned short x, y;
  for(y = 0; y < ROWS*2; ++y) {
    for(x = 0; x < COLUMNS*2; ++x) {
      screen_buffer[y][x] = '\0';
    }
  }
  init_vram();
}

void get_key(char *key, unsigned char code) {
  if (code > KEY_TABLE_SIZE) {
    *key = 0;
    return;
  }
  *key = keytable[code];
}

unsigned int get_y() {
  return cursor.y;
}

/**
 * BIOSを使用しないカーソル移動
 * https://wiki.osdev.org/Text_Mode_Cursor#Enabling_the_Cursor_2
 *
 * (y * 最大幅, x + 左端スペース分) で位置表現する
 *
 *    0   1   2  ...   79
 *   80  81  82  ...  159
 *  160  ................
 *  ................ 2000
 *
 */
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
  unsigned short colored_char = (unsigned short)((color << 8) | c);
  // スクロール用に保存
  screen_buffer[y][x] = colored_char;
  if (x + 1 < COLUMNS * 2) {
    screen_buffer[y][x + 1] = '\0';
  }
  write_vram(colored_char, x, y);
}

/**
 * テキストモードでVRAMの0x8B000(=SCREEN_START)を基準に
 * 文字、色を書き込むことで画面にテキストを出力する
 * 画面サイズは 80x25. 左に余白を入れるので実際は79x25
 * https://wiki.osdev.org/Printing_To_Screen
 */
void write_vram(unsigned short c, unsigned char x, unsigned char y) {
  unsigned char *pos = (unsigned char *)(SCREEN_START + (((y * COLUMNS) + x + CONSOLE_OFFSET) * 2));
  *(unsigned short *)pos = c;
}

void backspace() {
  put_char_pos(' ', cursor.x - 1, cursor.y, WHITE);
  move_cursor(cursor.x - 1, cursor.y);
}

void newline() {
  move_cursor(0, cursor.y + 1);
}

void clear_line() {
  while(cursor.x > 0) {
    backspace();
  }
}

/**
 * スクロール処理
 * 一度画面をクリアし、screen_bufferから１行ずつ移動させる
 */
void scroll() {
  init_vram();
  unsigned char x, y;
  for (y = 1; y < ROWS*2; ++y) {
    for (x = 0; x < COLUMNS*2; ++x) {
      // 上書き
      write_vram(screen_buffer[y][x], x, y - 1);
      // 1つ上にスライド
      screen_buffer[y - 1][x] = screen_buffer[y][x];
      if (screen_buffer[y][x] == '\0') {
        write_vram(' ', x, y - 1);
        break;
      }
    }
  }
  move_cursor(0, cursor.y - 1);
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

char* _sprintf(char *str, char *format, va_list *arg) {
  va_list list;
  va_copy(list, *arg);
  char *s_arg;
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
  va_end(list);
  return str;
}

char* sprintf(char *str, char *format, ...) {
  va_list list;
  va_start(list, format);
  _sprintf(str, format, &list);
  va_end(list);
  return str;
}

void printf(char *format, ...) {
  char buf[256];
  va_list list;
  va_start(list, format);
  _sprintf(buf, format, &list);
  put_str(buf, CHAR_COLOR);
  va_end(list);
}

/**
 * 画面右上に40字以内のデバッグ情報を出力する
 */
void debug(char *format, ...) {
  va_list list;
  va_start(list, format);

  int len = COLUMNS - 1;
  char buf[len];
  _sprintf(buf, format, &list);

  unsigned short color = (BLUE << 4) | WHITE; //  white-on-blue
  int done = 0;
  int x;
  for (x = 0; x < len; ++x) {
    if (buf[x] == '\0') {
      done = 1;
    }
    unsigned short c;
    if (done == 0) {
      c = (unsigned short)((color << 8) | buf[x]);
    } else {
      // Null終端以降は空文字で埋める
      c = (unsigned short)((color << 8) | ' ');
    }
    write_vram(c, x, 0);
  }

  va_end(list);
}

