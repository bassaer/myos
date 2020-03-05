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
  // 最短のタイマーを設定するため最大値を初期値に設定
  timerctrl.next = 0xffffffff;
  timerctrl.running_num = 0;
  // タイマーの状態を初期化
  int i;
  for (i = 0; i < TIMER_MAX_NUM; i++) {
    timerctrl.all_timers[i].status = STOPPED;
  }
}

struct Timer* new_timer() {
  int i;
  for (i = 0; i < TIMER_MAX_NUM; i++) {
    if (timerctrl.all_timers[i].status == STOPPED) {
      timerctrl.all_timers[i].status = READY;
      return &timerctrl.all_timers[i];
    }
  }
  // 設定不可
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
  int i;
  for(i = 0; i < timerctrl.running_num; i++) {
    if (timerctrl.sorted_timers[i]->timeout > timerctrl.uptime ){
      break;
    }
    // タイムアウト
    timerctrl.sorted_timers[i]->status = READY;
    unsigned char buf[8];
    dequeue(timerctrl.sorted_timers[i]->queue, buf);
  }
  // i個タイムアウト
  timerctrl.running_num -= i;
  int j;
  for(j = 0; j < timerctrl.running_num; i++) {
    // 残りのタイマーをずらす
    timerctrl.sorted_timers[j] = timerctrl.sorted_timers[i + j];
  }
  if (timerctrl.running_num > 0) {
    // 最短のタイマーをセット
    timerctrl.next = timerctrl.sorted_timers[0]->timeout;
  } else {
    timerctrl.next = 0xffffffff;
  }
}

void set_timer(struct Timer *timer, unsigned int timeout) {
  // 現在時刻から指定時間経過後を終了点とする
  timer->timeout = timerctrl.uptime + timeout;
  timer->status = RUNNING;
  int eflags = io_load_eflags();
  // 割り込み禁止
  io_cli();

  // 格納する場所を探索
  int i;
  for (i = 0; i < timerctrl.running_num; i++) {
    if (timerctrl.sorted_timers[i]->timeout >= timer->timeout) {
      break;
    }
  }

  // 後ろにずらす
  int j;
  for (j = 0; j < timerctrl.running_num; j++) {
    timerctrl.sorted_timers[j] = timerctrl.sorted_timers[j - 1];
  }

  timerctrl.running_num++;
  // 時間順に格納する
  timerctrl.sorted_timers[i] = timer;
  enqueue(timer->queue, timer->data);
  // 最短のタイマーの場合は更新
  timerctrl.next = timerctrl.sorted_timers[0]->timeout;
  io_store_eflags(eflags);
}
