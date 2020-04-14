#include <ls.h>

#include <console.h>
#include <exit.h>

int ls() {
  printf(L"hello, %c%s", L'w', L"orld!");
  return EXIT_SUCCESS;
}
