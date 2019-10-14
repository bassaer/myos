# include <sh.h>

#include <console.h>
#include <color.h>
#include <io.h>
#include <keyboard.h>
#include <queue.h>

#define KEYBUF_LIMIT   32
#define CMD_LIMIT      256


void print_msg() {
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

void start_shell() {
  struct Queue queue;
  unsigned char keybuf[KEYBUF_LIMIT];

  init_queue(&queue, KEYBUF_LIMIT, keybuf);
  init_keyboard(&queue);

  char cmdline[CMD_LIMIT];
  init_console(cmdline, CMD_LIMIT);

  print_msg();

  while(1) {
    io_cli(); // 割り込み無効化
    if (queue_status(&queue) == 0) {
      io_stihlt(); // 割り込み有効化 + HLT
    } else {
      unsigned char code;
      dequeue(&queue, &code);
      io_sti(); // 割り込み有効化
      input_code(code);
    }
  }
}
