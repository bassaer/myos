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
  timerctrl.next_timeout = 0xffffffff;
  // タイマーの状態を初期化
  int i;
  for (i = 0; i < TIMER_MAX_NUM; i++) {
    timerctrl.all_timers[i].status = STOPPED;
  }
  timer_t *last_timer = new_timer();
  // 最大のタイムアウト時間を設定(約497日)
  last_timer->timeout = 0xffffffff;
  last_timer->status = RUNNING;
  // 次のタイマーはなし
  last_timer->next = 0;
  timerctrl.next_timer = last_timer;
  timerctrl.next_timeout = last_timer->timeout;
}

timer_t* new_timer() {
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

void free_timer(timer_t *timer) {
  timer->status = STOPPED;
}

void init_timer(timer_t *timer, queue_t *queue, unsigned char data) {
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
  if (timerctrl.next_timeout > timerctrl.uptime) {
    return;
  }
  // 次のタイマーを取得
  timer_t *timer = timerctrl.next_timer;
  while(1) {
    if (timer->timeout > timerctrl.uptime ){
      break;
    }
    // タイムアウト
    timer->status = READY;
    unsigned char buf[8];
    dequeue(timer->queue, buf);
    // 次のタイマーの番地をセット
    timer = timer->next;
  }
  timerctrl.next_timer = timer;

  // 最短のタイマーをセット
  timerctrl.next_timeout = timerctrl.next_timer->timeout;
}

void set_timer(timer_t *timer, unsigned int timeout) {
  // 現在時刻から指定時間経過後を終了点とする
  timer->timeout = timerctrl.uptime + timeout;
  timer->status = RUNNING;
  int eflags = io_load_eflags();
  // 割り込み禁止
  io_cli();

  enqueue(timer->queue, timer->data);

  timer_t *next_timer = timerctrl.next_timer;
  if (timer->timeout <= next_timer->timeout) {
    timerctrl.next_timer = timer;
    timer->next = next_timer;
    timerctrl.next_timeout = timer->timeout;
    io_store_eflags(eflags);
    return;
  }

  timer_t *prev_timer;
  while(1) {
    prev_timer = next_timer;
    next_timer = next_timer->next;
    if (timer->timeout <= next_timer->timeout) {
      // prev_timerとnext_timerの間に入れる場合
      prev_timer->next = timer;
      timer->next = next_timer;
      io_store_eflags(eflags);
      return;
    }
  }
}
