#ifndef MYOS_TIMER_H
#define MYOS_TIMER_H

#include <lib/queue.h>

#define TIMER_MAX_NUM       500

enum TimerStatus {
  STOPPED,
  READY,
  RUNNING
};

struct Timer {
  unsigned int timeout;
  enum TimerStatus status;
  struct Queue *queue;
  unsigned char data;
};

struct TimerCtrl {
  // システム起動時間
  unsigned int uptime;
  // 次回のタイマー
  unsigned int next;
  // 現在稼働中のタイマー
  unsigned int running_num;
  // ソート済みタイマーのリスト
  struct Timer *sorted_timers[TIMER_MAX_NUM];
  // すべてのタイマーリスト
  struct Timer all_timers[TIMER_MAX_NUM];
} timerctrl;

void init_pit();

struct Timer* new_timer();

void free_timer(struct Timer *timer);

void init_timer(struct Timer *timer, struct Queue *queue, unsigned char data);

void handle_intr20(int *esp);

void set_timer(struct Timer *timer, unsigned int timeout);

#endif
