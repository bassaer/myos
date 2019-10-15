# include <sh.h>

#include <console.h>
#include <color.h>
#include <queue.h>


void init_shell() {
  char *os =
    "  __  __        ___  ____     \n"
    " |  \\/  |_   _ / _ \\/ ___|  \n"
    " | |\\/| | | | | | | \\___ \\ \n"
    " | |  | | |_| | |_| |___) |   \n"
    " |_|  |_|\\__, |\\___/|____/  \n"
    "         |___/              \n\n";

  put_str(os, GRAY);
  put_prompt();
}

void start_shell(struct Queue *queue) {
  unsigned char code;
  dequeue(queue, &code);
  input_code(code);
}
