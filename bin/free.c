#include <free.h>

#include <console.h>
#include <exit.h>
#include <memory.h>
#include <util.h>

int free() {
  char msg[128];
  char *fmt =
    "      total  used  free\n"
    "Mem:  %d      %d     %d\n";

  struct MEM_INFO mem = {
    0, 0, 0
  };
  stats(&mem);

  sprintf(msg, fmt, mem.total, mem.used, mem.free);
  put_str(msg, CHAR_COLOR);
  return EXIT_SUCCESS;
}
