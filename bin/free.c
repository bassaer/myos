#include <free.h>

#include <console.h>
#include <exit.h>
#include <type.h>
#include <lib/string.h>

#define MB      (1024*1024)

int free() {
  /*
  CHAR16 *fmt =
    L"total : %dMB\r\n"
     "free  : %dMB\r\n"
     "used  : %dMB";

  mem_t mem = {
    0, 0, 0, 0, 0, 0, 0, 0
  };
  stats(&mem);

  printf(fmt, mem.total_bytes/MB, mem.used_bytes/MB, mem.free_bytes/MB);
  */
  return EXIT_SUCCESS;
}
