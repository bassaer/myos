#include <free.h>

#include <console.h>
#include <exit.h>
#include <memory.h>
#include <string.h>

int free() {
  char *fmt =
    "total : %dMB\n"
    "free  : %dMB\n"
    "used  : %dMB";

  struct mem_info mem = {
    0, 0, 0, 0, 0, 0
  };
  stats(&mem);

  printf(fmt, mem.total_bytes, mem.used_bytes, mem.free_bytes);
  return EXIT_SUCCESS;
}
