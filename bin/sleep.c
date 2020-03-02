#include <sleep.h>

#include <console.h>
#include <exit.h>
#include <io.h>
#include <timer.h>
#include <lib/queue.h>
#include <lib/string.h>

void usage_sleep() {
  printf("usage : sleep <timeout>\n");
}


int sleep(char *args[], int size) {
  if (size != 2) {
    usage_sleep();
    return EXIT_FAILURE;
  }

  struct Queue queue;
  unsigned char buf[8];
  init_queue(&queue, 8, buf);
  int timeout = atoi(args[1]) * 100;

  set_timer(timeout, &queue, 1);

  while(1) {
    // 割り込み無効化
    io_cli();
    if (queue_status(&queue) != 0) {
      // 割り込み有効化 + HLT
      io_stihlt();
    } else {
      // 割り込み有効化
      io_sti();
      break;
    }
  }
  return EXIT_SUCCESS;
}