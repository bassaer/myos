# include <sh.h>

#include <console.h>
#include <color.h>
#include <echo.h>
#include <exit.h>
#include <free.h>
#include <ls.h>
#include <shutdown.h>
#include <sleep.h>
#include <lib/string.h>

#include<uefi.h>

#define PROMPT        L"myos> "

/**
 * 入力情報
 */
typedef struct {
  /**
   * 入力文字列
   */
  CHAR16 buf[CMD_LIMIT];
  /**
   * 現在の入力位置
   */
  int index;
} entry_t;

static entry_t entries[SCREEN_HEIGHT];

/**
 * 入力履歴一時保存用
 */
static entry_t cache_entry;

static int exit_status = EXIT_SUCCESS;
static int cur_line = 0;
static int selected_line = 0;
static int max_width = 0;

void init_shell() {
  cur_line = 0;
  selected_line = 0;
  int i;
  for (i = 0; i < SCREEN_HEIGHT; ++i) {
    entries[i].index = 0;
    entries[i].buf[0] = L'\0';
  }

  max_width = 256;

  CHAR16 *os =
    L"  __  __        ___  ____     \r\n"
     " |  \\/  |_   _ / _ \\/ ___|  \r\n"
     " | |\\/| | | | | | | \\___ \\ \r\n"
     " | |  | | |_| | |_| |___) |   \r\n"
     " |_|  |_|\\__, |\\___/|____/  \r\n"
     "         |___/              \r\n\n";
  put_str(os, WHITE);
}

void put_prompt() {
  if (exit_status == EXIT_SUCCESS) {
    put_str(PROMPT, GREEN);
  } else {
    put_str(PROMPT, RED);
  }
  set_color(GRAY);
}

void init_entry() {
  cur_line++;
  selected_line = cur_line;
}

void copy_entry(entry_t *src, entry_t *dst) {
    dst->index = src->index;
    strcpy(src->buf, dst->buf);
}

void put_text(CHAR16 *text) {
  put_str(text, GRAY);
}

void newline() {
  put_text(L"\r\n");
}

void eval_key(EFI_INPUT_KEY *key) {
  switch(key->ScanCode) {
    // 入力履歴補完
    case CURSOR_UP:
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
        put_str(entries[cur_line].buf, GRAY);
      }
      return;
    case CURSOR_DOWN:
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
        put_text(entries[cur_line].buf);
      }
      return;
  }
  if (key->UnicodeChar == L'\0') {
    return;
  }
  if (key->UnicodeChar == L'\r') {
    if (entries[cur_line].index > 0) {
      newline();
      exec_cmd();
      init_entry();
    }
    newline();
    put_prompt();
    return;
  }
  if (max_width <= entries[cur_line].index + 1) {
    // 入力サイズオーバー
    // 終端文字も含めるため+1
    return;
  }
  entries[cur_line].buf[entries[cur_line].index++] = key->UnicodeChar;
  entries[cur_line].buf[entries[cur_line].index] = L'\0';
  put_char(key->UnicodeChar, GRAY);
}

void exec_cmd() {
  printf(L"OK");
  /*
  entries[cur_line].buf[entries[cur_line].index] = '\0';
  CHAR16 *args[max_width];

  // 元の文字列を操作するため、一時配列を用意
  CHAR16 tmp[strlen(entries[cur_line].buf)];
  //strcpy(entries[cur_line].buf, tmp);
  // スペースで分割し、コマンドを取得
  int split_count = split(tmp, args, L' ');
  CHAR16 *cmd = args[0];
  if (strcmp(cmd, L"echo") == 0) {
    exit_status = echo(args, split_count);
  } else if (strcmp(cmd, L"free") == 0) {
    exit_status = free();
  } else if (strcmp(cmd, L"shutdown") == 0 || strcmp(cmd, L"exit") == 0) {
    exit_status = shutdown(args, split_count);
  } else if(strcmp(cmd, L"sleep") == 0) {
    exit_status = sleep(args, split_count);
  } else if (strcmp(cmd, L"ls") == 0) {
    exit_status = ls();
  } else {
    put_text(L"command not found");
    exit_status = EXIT_FAILURE;
  }
  */
}

void start_shell() {
  EFI_INPUT_KEY key;
  put_prompt();
  while (1) {
    read_key(&key);
    eval_key(&key);
  }
}
