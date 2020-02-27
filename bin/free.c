#include <free.h>

#include <console.h>
#include <exit.h>
#include <mm/memory.h>
#include <lib/string.h>

int free() {
  char *fmt =
    "total : %dMB\n"
    "free  : %dMB\n"
    "used  : %dMB";

  struct mem_info mem = {
    0, 0, 0, 0, 0, 0
  };
  stats(&mem);

  int mb = 1024*1024;
  printf(fmt, mem.total_bytes/mb, mem.used_bytes/mb, mem.free_bytes/mb);
  return EXIT_SUCCESS;
}
