#include <timer.h>

#include <intr.h>
#include <io.h>
#include <lib/queue.h>

#include <console.h>

#define PIT_CTRL      0x0043
#define PIT_CNT0      0x0040

/**
 * (PIT)Programmable Interval Timer
 * AT互換機ではPITに1.19318MHzのクロックが与えられている
 *
 * IRQ0の割り込み周期変更
 * AL = 0x34; OUT(0x43, AL);
 * AL = 割り込み周期の下位8bit; OUT(0x40, AL);
 * AL = 割り込み周期の上位8bit; OUT(0x40, AL);
 */
void init_pit() {
  /**
   * IRQ0の割り込み周期変更
   * 割り込み頻度は クロック/設定カウント値
   */
  outb_p(PIT_CTRL, 0x34);
  /**
   * 11932を設定することで100Hzになる
   * 11932=0x2e9c
   */
  outb_p(PIT_CNT0, 0x9c);
  outb_p(PIT_CNT0, 0x2e);
  timer.count++;
}

/**
 * IRQ0用割り込みハンドラ
 */
void handle_intr20(int *esp) {
  outb_p(PIC0_OCW2, 0x60); // IRQ-00受付完了をPICに通知
  timer.count++;
  // タイムアウトが設定されている場合
  if (timer.timeout > 0) {
    timer.timeout--;
    if (timer.timeout == 0 ){//&& queue_status(timer.queue) != 0) {
      unsigned char buf[8];
      dequeue(timer.queue, buf);
    }
  }
}

void set_timer(unsigned int timeout, struct Queue *queue, unsigned char data) {
  // EFLAGSは32bit拡張のレジスタ
  // キャリーフラグや割り込みフラグなどを保持する
  int eflags = io_load_eflags();
  // 設定中の割り込みを禁止する
  io_cli();
  timer.timeout = timeout;
  timer.queue = queue;
  timer.data = data;
  enqueue(timer.queue, timer.data);
  // 割り込み状態を元の状態に戻す
  io_store_eflags(eflags);
}
