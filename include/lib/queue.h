#ifndef MYOS_QUEUE_H
#define MYOS_QUEUE_H

#define FLAGS_OVERRUN  0x0001

typedef struct {
  unsigned char *buf;  // 入力バッファ
  int next_w;          // 次回の書き込み先
  int next_r;          // 次回の読み込み先
  int size;            // バッファサイズ
  int free;            // バッファの空きサイズ
  int flags;           // 入力あふれフラグ
} queue_t;

void init_queue(queue_t *queue, int size, unsigned char *buf);

int enqueue(queue_t *queue, unsigned char data);

int dequeue(queue_t *queue, unsigned char *data);

int queue_status(queue_t *queue);

#endif
