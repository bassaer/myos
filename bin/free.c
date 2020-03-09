#include <free.h>

#include <console.h>
#include <exit.h>
#include <mm/memory.h>
#include <lib/string.h>

#define MB      (1024*1024)

int free() {
  char *fmt =
    "total : %dMB\n"
    "free  : %dMB\n"
    "used  : %dMB";

  mem_t mem = {
    0, 0, 0, 0, 0, 0
  };
  stats(&mem);

  printf(fmt, mem.total_bytes/MB, mem.used_bytes/MB, mem.free_bytes/MB);
  return EXIT_SUCCESS;
}
