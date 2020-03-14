#include <keyboard.h>
#include <io.h>
#include <intr.h>

#include <console.h>
#include <color.h>

queue_t *keyboard_queue;

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

void init_keyboard(queue_t *queue) {
  keyboard_queue = queue;
  wait_keyboard();
  outb_p(PORT_KEYCMD, KEYCMD_WRITE);
  wait_keyboard();
  outb_p(PORT_KEYDAT, KBC_MODE);
}

/**
 * 割り込みハンドラ
 * 入力をバッファに, 空き状態をフラグで保持
 */
void handle_intr21(int *esp) {
  outb_p(PIC0_OCW2, 0x61); // IRQ-01受付完了をPICに通知
  unsigned char code = io_in(PORT_KEYDAT);
  enqueue(keyboard_queue, code);
}

void handle_intr27(int *esp) {
  outb_p(PIC0_OCW2, 0x67);
  return;
}
