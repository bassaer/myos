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
  timerctrl.uptime = 0;

  // タイマーの状態を初期化
  int i;
  for (i = 0; i < TIMER_MAX_NUM; i++) {
    timerctrl.timers[i].status = STOPPED;
  }
}

struct Timer* new_timer() {
  int i;
  for (i = 0; i < TIMER_MAX_NUM; i++) {
    if (timerctrl.timers[i].status == STOPPED) {
      timerctrl.timers[i].status = READY;
      return &timerctrl.timers[i];
    }
  }
  return 0;
}


void free_timer(struct Timer *timer) {
  timer->status = STOPPED;
}

void init_timer(struct Timer *timer, struct Queue *queue, unsigned char data) {
  timer->queue = queue;
  timer->data = data;
}

/**
 * IRQ0用割り込みハンドラ
 */
void handle_intr20(int *esp) {
  outb_p(PIC0_OCW2, 0x60); // IRQ-00受付完了をPICに通知
  // uptimeを更新
  timerctrl.uptime++;
  // 次のタイマーの時刻を確認
  if (timerctrl.next > timerctrl.uptime) {
    return;
  }
  // 最短のタイマーを設定するため最大値を初期値に設定
  timerctrl.next = 0xffffffff;
  int i;
  for(i = 0; i < TIMER_MAX_NUM; i++) {
    if (timerctrl.timers[i].status == RUNNING) {
      // タイムアウトが設定されている場合
      if (timerctrl.timers[i].timeout <= timerctrl.uptime ){
        timerctrl.timers[i].status = READY;
        unsigned char buf[8];
        dequeue(timerctrl.timers[i].queue, buf);
      } else if (timerctrl.next > timerctrl.timers[i].timeout) {
        // 最短のタイマーをセット
        timerctrl.next = timerctrl.timers[i].timeout;
      }
    }
  }
}

void set_timer(struct Timer *timer, unsigned int timeout) {
  // 現在時刻から指定時間経過後を終了点とする
  timer->timeout = timerctrl.uptime + timeout;
  timer->status = RUNNING;
  enqueue(timer->queue, timer->data);
  // 最短のタイマーの場合は更新
  if (timerctrl.next > timer->timeout) {
    timerctrl.next = timer->timeout;
  }
}
