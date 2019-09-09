#ifndef MYOS_QUEUE_H
#define MYOS_QUEUE_H

#define FLAGS_OVERRUN  0x0001

struct Queue {
  unsigned char *buf;  // 入力バッファ
  int next_w;          // 次回の書き込み先
  int next_r;          // 次回の読み込み先
  int size;            // バッファサイズ
  int free;            // バッファの空きサイズ
  int flags;           // 入力あふれフラグ
};

void init_queue(struct Queue *queue, int size, unsigned char *buf);

int queue_put(struct Queue *queue, unsigned char data);

int queue_get(struct Queue *queue);

int queue_status(struct Queue *queue);

#endif
