#include <sleep.h>

#include <console.h>
#include <exit.h>
#include <io.h>
#include <sched.h>
#include <tss.h>
#include <lib/queue.h>
#include <lib/string.h>

void usage_sleep() {
  printf(L"usage : sleep <timeout>\n");
}


int sleep(CHAR16 *args[], int size) {
  if (size != 1) {
    usage_sleep();
  }
  printf(L"sleep %d\r\n", args[1]);
  return EXIT_SUCCESS;
}
