# include <sh.h>

#include <console.h>
#include <color.h>
#include <echo.h>
#include <exit.h>
#include <free.h>
#include <keyboard.h>
#include <ls.h>
#include <shutdown.h>
#include <sleep.h>
#include <lib/queue.h>
#include <lib/string.h>

#define PROMPT        "myos> "

int exit_status = EXIT_SUCCESS;
int char_color = GRAY;
int cur_line = 0;
int selected_line = 0;
int max_width = 0;

/**
 * 入力情報
 */
typedef struct {
  /**
   * 入力文字列
   */
  char buf[CMD_LIMIT];
  /**
   * 現在の入力位置
   */
  int index;
} entry_t;

entry_t entries[SCREEN_HEIGHT];

/**
 * 入力履歴一時保存用
 */
entry_t cache_entry;

void init_shell() {
  init_console();
  cur_line = 0;
  selected_line = 0;
  int i;
  for (i = 0; i < SCREEN_HEIGHT; ++i) {
    entries[i].index = 0;
    entries[i].buf[0] = '\0';
  }

  max_width = 256;

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

void init_entry() {
  cur_line++;
  selected_line = cur_line;
}

void copy_entry(entry_t *src, entry_t *dst) {
    dst->index = src->index;
    strcpy(src->buf, dst->buf);
}

void input_code(int code) {
  if (code == BACKSPACE && entries[cur_line].index > 0) {
    entries[cur_line].buf[--(entries[cur_line].index)] = '\0';
    backspace();
    return;
  }
  switch(code) {
    // 入力履歴補完
    case UP:
      if(selected_line > 0) {
        if (cur_line == selected_line) {
          // 未実行の入力を一時保存
          copy_entry(&entries[cur_line], &cache_entry);
        }
        clear_line();
        put_prompt();
        selected_line--;
        entries[cur_line].index = entries[selected_line].index;
        strcpy(entries[selected_line].buf, entries[cur_line].buf);
        put_str(entries[cur_line].buf, CHAR_COLOR);
      }
      return;
    case DOWN:
      if (cur_line > selected_line ) {
        clear_line();
        put_prompt();
        selected_line++;
        if (cur_line == selected_line) {
          copy_entry(&cache_entry, &entries[cur_line]);
        } else {
          entries[cur_line].index = entries[selected_line].index;
          strcpy(entries[selected_line].buf, entries[cur_line].buf);
        }
        put_str(entries[cur_line].buf, CHAR_COLOR);
      }
      return;
  }
  char key;
  get_key(&key, code);
  if (key == '\0' || key == 0) {
    return;
  }
  if (key == '\n') {
    if (entries[cur_line].index > 0) {
      newline();
      exec_cmd();
      init_entry();
    }
    newline();

    int diff = get_y() + 1 - ROWS;
    while(diff > 0) {
      scroll();
      diff--;
    }

    put_prompt();

    return;
  }
  if (max_width <= entries[cur_line].index + 1) {
    // 入力サイズオーバー
    // 終端文字も含めるため+1
    return;
  }
  entries[cur_line].buf[entries[cur_line].index++] = key;
  entries[cur_line].buf[entries[cur_line].index] = '\0';
  put_char(key, char_color);
}

void exec_cmd() {
  entries[cur_line].buf[entries[cur_line].index] = '\0';
  char *args[max_width];

  // 元の文字列を操作するため、一時配列を用意
  char tmp[strlen(entries[cur_line].buf)];
  strcpy(entries[cur_line].buf, tmp);

  // スペースで分割し、コマンドを取得
  int split_count = split(tmp, args, ' ');
  char *cmd = args[0];
  if (strcmp(cmd, "echo") == 0) {
    exit_status = echo(args, split_count);
  } else if (strcmp(cmd, "free") == 0) {
    exit_status = free();
  } else if (strcmp(cmd, "shutdown") == 0 || strcmp(cmd, "exit") == 0) {
    exit_status = shutdown(args, split_count);
  } else if(strcmp(cmd, "sleep") == 0) {
    exit_status = sleep(args, split_count);
  } else if (strcmp(cmd, "ls") == 0) {
    exit_status = ls();
  } else {
    put_str("command not found", char_color);
    exit_status = EXIT_FAILURE;
  }
}

void start_shell(queue_t *queue) {
  int code = dequeue(queue);
  input_code(code);
}
