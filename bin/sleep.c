#include <sleep.h>

#include <console.h>
#include <exit.h>
#include <io.h>
#include <sched.h>
#include <tss.h>
#include <timer.h>
#include <lib/queue.h>
#include <lib/string.h>
#include <mm/memory.h>

void usage_sleep() {
  printf("usage : sleep <timeout>\n");
}


int sleep(char *args[], int size) {
  if (size != 2) {
    usage_sleep();
    return EXIT_FAILURE;
  }

  timer_t *timer = new_timer();
  if(timer == 0) {
    printf("failed to make timer\n");
    return EXIT_FAILURE;
  }

  queue_t queue;
  unsigned char buf[8];
  init_queue(&queue, 8, buf);

  init_timer(timer, &queue, 1);

  int timeout = atoi(args[1]) * 100;
  set_timer(timer, timeout);

  tss_t tss2;
  tss2.ldtr = 0;
  tss2.iomap = 0x40000000;
  tss2.eip = (int) &task2_main;
  tss2.eax = 0;
  tss2.ecx = 0;
  tss2.edx = 0;
  tss2.ebx = 0;
  tss2.esp = (int)alloc_single_block();
  tss2.ebp = 0;
  tss2.esi = 0;
  tss2.edi = 0;
  tss2.es =  1 * 8;
  tss2.cs =  2 * 8;
  tss2.ss =  1 * 8;
  tss2.ds =  1 * 8;
  tss2.fs =  1 * 8;
  tss2.gs =  1 * 8;

  debug("tss2 = %x", (int *)tss2.eip);

  while(1) {
    // 割り込み無効化
    io_cli();
    if (queue_status(&queue) != 0) {
      // 割り込み有効化 + HLT
      io_stihlt();
    } else {
      context_switch(tss2.eip);
      // 割り込み有効化
      io_sti();
      break;
    }
  }
  return EXIT_SUCCESS;
}
