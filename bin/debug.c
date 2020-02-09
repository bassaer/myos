#include <debug.h>

#include <console.h>
#include <exit.h>
#include <util.h>

int debug() {
  char str[128];
  sprintf(str, "hello, %s", "world");
  put_str(str, CHAR_COLOR);
  return EXIT_SUCCESS;
}
