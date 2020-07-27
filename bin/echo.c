#include <echo.h>

#include <console.h>
#include <exit.h>

void usage_echo() {
  put_str(L"usage : echo <message>", CHAR_COLOR);
}

int echo(CHAR16 *args[], int size) {
  if (size != 2) {
    usage_echo();
    return EXIT_FAILURE;
  }
  CHAR16 *arg = args[1];
  put_str(arg, CHAR_COLOR);
  return EXIT_SUCCESS;
}
