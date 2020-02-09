#include <debug.h>

#include <console.h>
#include <exit.h>
#include <util.h>

int debug() {
  printf("hello, %s", "world");
  return EXIT_SUCCESS;
}
