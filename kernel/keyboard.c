#include <keyboard.h>
#include <io.h>
#include <intr.h>
#include <queue.h>

#include <console.h>
#include <color.h>

struct Queue *queue;

static char keytable[0x54] = {
  0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '@', '[', '\n',   0,   'a', 's',
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0,   0,   ']', 'z', 'x', 'c', 'v',
  'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
  '2', '3', '0', '.'
};

void get_key(char *key, unsigned char code) {
  *key = keytable[code];
}

/**
 * キーボードコントローラがデータ送信可能になるまで待つ
 */
void wait_keyboard() {
  while(1) {
    if ((io_in(PORT_KEYSTA) & KEYSTA_NOTREADY) == 0) {
      break;
    }
  }
}


void init_keyboard(struct Queue *q) {
  queue = q;
  wait_keyboard();
  outb_p(PORT_KEYCMD, KEYCMD_WRITE);
  wait_keyboard();
  outb_p(PORT_KEYDAT, KBC_MODE);
}

/**
 * 割り込みハンドラ
 * 入力をバッファに, 空き状態をフラグで保持
 */
void handle_keyboard(int *esp) {
  outb_p(PIC0_OCW2, 0x61); // IRQ-01受付完了をPICに通知
  unsigned char code = io_in(PORT_KEYDAT);
  char key;
  get_key(&key, code);
  enqueue(queue, key);
}

void handle_intr27(int *esp) {
  outb_p(PIC0_OCW2, 0x67);
  return;
}
