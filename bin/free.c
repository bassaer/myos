#include <free.h>

#include <console.h>
#include <exit.h>
#include <memory.h>
#include <util.h>

int free() {
  char *fmt =
    "total : %dMB\n"
    "free  : %dMB\n"
    "used  : %dMB";

  struct MEM_INFO mem = {
    0, 0, 0
  };
  stats(&mem);

  printf(fmt, mem.total, mem.used, mem.free);
  return EXIT_SUCCESS;
}
