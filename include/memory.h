#ifndef MYOS_MEMORY_H
#define MYOS_MEMORY_H

struct MEM_INFO {
  int total;
  int used;
  int free;
};

/**
 * メモリ情報出力
 */
void stats(struct MEM_INFO *mem);

#endif
