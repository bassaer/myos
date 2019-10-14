#include <console.h>
#include <color.h>
#include <io.h>
#include <util.h>

#define PROMPT        "myos> "

#define EXIT_SUCCESS  0
#define EXIT_FAILURE  1

struct {
  unsigned int x;
  unsigned int y;
} cursor;

struct {
  char *buf;
  int size;
  int index;
} entry;

int exit_status = EXIT_SUCCESS;
int char_color = GRAY;

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

void show_status(char *status, char *msg) {
  put_str("[", WHITE);
  put_str(status, GREEN);
  put_str("] ", WHITE);
  put_str(msg, WHITE);
  put_str("\n", WHITE);
}

void input_code(unsigned char code) {
  char key;
  switch(code) {
  case 0x0E:
    backspace();
    break;
  default:
    get_key(&key, code);
    input_key(key);
  }
}

void input_key(char key) {
  if (key == '\0') {
    return;
  }
  if (key == '\n') {
    if (entry.index > 0) {
      exec_cmd();
    }
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
  put_char(key, char_color);
}

void backspace() {
  if (entry.index == 0) {
    return;
  }
  put_char_pos(' ', cursor.x - 1, cursor.y, char_color);
  move_cursor(cursor.x - 1, cursor.y);
  entry.buf[--entry.index] = '\0';
}

void newline() {
  move_cursor(0, cursor.y + 1);
  put_prompt();
  entry.index = 0;
  entry.buf[entry.index] = '\0';
}

void put_prompt() {
  if (exit_status == EXIT_SUCCESS) {
    put_str(PROMPT, GREEN);
  } else {
    put_str(PROMPT, RED);
  }
}

void init_console(char *buf, int size) {
  entry.index = 0;
  entry.buf = buf;
  entry.size = size;
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

void exec_cmd() {
  move_cursor(0, cursor.y + 1);
  entry.buf[entry.index] = '\0';
  char *args[entry.size];
  // スペースで分割し、コマンドを取得
  int split_count = split(entry.buf, args, ' ');
  char *cmd = args[0];
  if (strcmp(cmd, "echo") == 0) {
    if (split_count != 2) {
      put_str("command not found", char_color);
      exit_status = EXIT_FAILURE;
      return;
    }
    char *arg = args[1];
    put_str(arg, char_color);
    exit_status = EXIT_SUCCESS;
  } else if (strcmp(cmd, "shutdown") == 0 || strcmp(cmd, "exit") == 0) {
    shutdown();
  } else {
    put_str("command not found", char_color);
    exit_status = EXIT_FAILURE;
  }
}
