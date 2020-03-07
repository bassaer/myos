#ifndef MYOS_TIMER_H
#define MYOS_TIMER_H

#include <lib/queue.h>

#define TIMER_MAX_NUM       500

enum timer_status {
  STOPPED,
  READY,
  RUNNING
};

typedef struct {
  unsigned int timeout;
  enum timer_status status;
  queue_t *queue;
  unsigned char data;
} timer_t;

typedef struct {
  // システム起動時間
  unsigned int uptime;
  // 次回のタイマー
  unsigned int next;
  // 現在稼働中のタイマー
  unsigned int running_num;
  // ソート済みタイマーのリスト
  timer_t *sorted_timers[TIMER_MAX_NUM];
  // すべてのタイマーリスト
  timer_t all_timers[TIMER_MAX_NUM];
} timerctrl_t;

// カーネル内部の時間管理
timerctrl_t timerctrl;

void init_pit();

timer_t* new_timer();

void free_timer(timer_t *timer);

void init_timer(timer_t *timer, queue_t *queue, unsigned char data);

void handle_intr20(int *esp);

void set_timer(timer_t *timer, unsigned int timeout);

#endif
