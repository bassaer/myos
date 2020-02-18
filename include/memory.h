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

/**
 * 指定したアドレス間で使用可能なメモリサイズを返す
 */
int calc_mem(unsigned int start, unsigned int end);

#endif
