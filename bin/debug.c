#include <debug.h>

#include <console.h>
#include <exit.h>
#include <util.h>

int debug() {
  printf("hello, %c%s", 'w', "orld!");
  return EXIT_SUCCESS;
}
