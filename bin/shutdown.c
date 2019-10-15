#include <shutdown.h>

#include <console.h>
#include <exit.h>
#include <io.h>

void usage_shutdown(char *cmd) {
  put_str("usage : ", CHAR_COLOR);
  put_str(cmd, CHAR_COLOR);
  put_str(" command does not need options.", CHAR_COLOR);
}

int shutdown(char *args[], int size) {
  if (size != 1) {
    usage_shutdown(args[0]);
    return EXIT_FAILURE;
  }
  outb_p(0x8900, 'S');
  outb_p(0x8900, 'h');
  outb_p(0x8900, 'u');
  outb_p(0x8900, 't');
  outb_p(0x8900, 'd');
  outb_p(0x8900, 'o');
  outb_p(0x8900, 'w');
  outb_p(0x8900, 'n');
  outb_p(0x501, 0x31);

  return EXIT_SUCCESS;
}
