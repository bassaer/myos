#include <echo.h>

#include <console.h>
#include <exit.h>

void usage_echo() {
  put_str("usage : echo <message>", CHAR_COLOR);
}

int echo(char *args[], int size) {
  if (size != 2) {
    usage_echo();
    return EXIT_FAILURE;
  }
  char *arg = args[1];
  put_str(arg, CHAR_COLOR);
  return EXIT_SUCCESS;
}
