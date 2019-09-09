#include <queue.h>

void init_queue(struct Queue *queue, int size, unsigned char *buf) {
  queue->size = size;
  queue->buf = buf;
  queue->free = size;
  queue->flags = 0;
  queue->next_w = 0;
  queue->next_r = 0;
}

int queue_put(struct Queue *queue, unsigned char data) {
  if (queue->free == 0) {
    queue->flags |= FLAGS_OVERRUN;
    return -1;
  }
  queue->buf[queue->next_w] = data;
  queue->next_w++;
  if (queue->next_w == queue->size) {
    queue->next_w = 0;
  }
  queue->free--;
  return 0;
}

int queue_get(struct Queue *queue) {
  if (queue->free == queue->size) {
    // バッファが空
    return -1;
  }
  int data = queue->buf[queue->next_r];
  queue->next_r;
  if (queue->next_r == queue->next_r) {
    queue->next_r = 0;
  }
  queue->free++;
  return data;
}

int queue_status(struct Queue *queue) {
  return queue->size - queue->free;
}
