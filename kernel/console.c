#include <console.h>
#include <color.h>
#include <io.h>
#include<util.h>

#define PROMPT "> "

struct {
  unsigned int x;
  unsigned int y;
} cursor;

struct {
  char *buf;
  int size;
  int index;
} entry;

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

void init_console(char *buf, int size) {
  entry.buf = buf;
  entry.size = size;
  entry.index = 0;

  move_cursor(0, 3);
  char *os =
    "  __  __        ___  ____     \n"
    " |  \\/  |_   _ / _ \\/ ___|  \n"
    " | |\\/| | | | | | | \\___ \\ \n"
    " | |  | | |_| | |_| |___) |   \n"
    " |_|  |_|\\__, |\\___/|____/  \n"
    "         |___/              \n\n";

  put_str(os, GRAY);

  put_str(PROMPT, GREEN);
}

void show_status(char *status, char *msg) {
  put_str("[", WHITE);
  put_str(status, GREEN);
  put_str("] ", WHITE);
  put_str(msg, WHITE);
  put_str("\n", WHITE);
}

void input_key(char key) {
  if (key == '\0') {
    return;
  }
  if (key == '\n') {
    exec_cmd();
    newline();
    return;
  }
  if (entry.size <= entry.index + 1) {
    // 入力サイズオーバー
    // 終端文字も含めるため+1
    return;
  }
  entry.buf[entry.index] = key;
  entry.index++;
  put_char(key, GRAY);
}

void newline() {
  move_cursor(0, cursor.y + 1);
  put_str(PROMPT, GREEN);
}

void exec_cmd() {
  move_cursor(0, cursor.y + 1);
  entry.buf[entry.index] = '\0';
  if (strcmp(entry.buf, "echo") == 0) {
    put_str(entry.buf, GRAY);
  } else {
    put_str("command not found", GRAY);
  }
  entry.index = 0;
}
