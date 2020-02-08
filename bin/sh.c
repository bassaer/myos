# include <sh.h>

#include <console.h>
#include <color.h>
#include <echo.h>
#include <free.h>
#include <exit.h>
#include <queue.h>
#include <shutdown.h>
#include <util.h>

#define PROMPT        "myos> "

int exit_status = EXIT_SUCCESS;
int char_color = GRAY;

struct {
  char *buf;
  int size;
  int index;
} entry;

void init_shell(char *buf, int size) {
  entry.index = 0;
  entry.buf = buf;
  entry.size = size;

  char *os =
    "  __  __        ___  ____     \n"
    " |  \\/  |_   _ / _ \\/ ___|  \n"
    " | |\\/| | | | | | | \\___ \\ \n"
    " | |  | | |_| | |_| |___) |   \n"
    " |_|  |_|\\__, |\\___/|____/  \n"
    "         |___/              \n\n";

  put_str(os, GRAY);
  put_prompt();
}

void put_prompt() {
  if (exit_status == EXIT_SUCCESS) {
    put_str(PROMPT, GREEN);
  } else {
    put_str(PROMPT, RED);
  }
}

void clear_entry() {
  entry.index = 0;
  entry.buf[entry.index] = '\0';
}

void input_code(unsigned char code) {
  if (code == 0x0E && entry.index > 0) {
    entry.buf[--entry.index] = '\0';
    backspace();
    return;
  }
  char key;
  get_key(&key, code);
  if (key == '\0') {
    return;
  }
  if (key == '\n') {
    if (entry.index > 0) {
      newline();
      exec_cmd();
    }
    newline();
    clear_entry();
    put_prompt();
    return;
  }
  if (entry.size <= entry.index + 1) {
    // 入力サイズオーバー
    // 終端文字も含めるため+1
    return;
  }
  entry.buf[entry.index] = key;
  entry.index++;
  entry.buf[entry.index] = '\0';
  put_char(key, char_color);
}

void exec_cmd() {
  entry.buf[entry.index] = '\0';
  char *args[entry.size];
  // スペースで分割し、コマンドを取得
  int split_count = split(entry.buf, args, ' ');
  char *cmd = args[0];
  if (strcmp(cmd, "echo") == 0) {
    exit_status = echo(args, split_count);
  } else if (strcmp(cmd, "free") == 0) {
    exit_status = free();
  } else if (strcmp(cmd, "shutdown") == 0 || strcmp(cmd, "exit") == 0) {
    exit_status = shutdown(args, split_count);
  } else {
    put_str("command not found", char_color);
    exit_status = EXIT_FAILURE;
  }
}

void start_shell(struct Queue *queue) {
  unsigned char code;
  dequeue(queue, &code);
  input_code(code);
}
