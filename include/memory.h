#ifndef MYOS_MEMORY_H
#define MYOS_MEMORY_H

#define BLOCK_SIZE        4096
#define KERN_MEM_START    0x00400000

struct mem_info {
  /** 総メモリサイズ*/
  unsigned int total_bytes;
  /** 総ブロック数*/
  unsigned int total_blocks;
  /** 使用中メモリサイズ*/
  unsigned int used_bytes;
  /** 使用中ブロック数*/
  unsigned int used_blocks;
  /** 未使用メモリサイズ*/
  unsigned int free_bytes;
  /** 未使用ブロック数*/
  unsigned int free_blocks;
  /** ビットマップ*/
  unsigned int *bitmap;
  /** ビットマップ長 */
  unsigned int bitmap_size;
};

/**
 * nビット目に1をセットする
 */
void set_bit(unsigned int bit);

/**
 * nビット目に0をセットする
 */
void remove_bit(unsigned int bit);

/**
 * nビット目を取得する
 */
unsigned int get_bit(unsigned int bit);

/**
 * 割当可能なブロックを探索する
 */
int find_free_block(unsigned int *block);

/**
 * 1ブロックの割当
 */
void* alloc_single_block();
/**
 * 1ブロックの開放
 */
void free_single_block(void *addr);

/**
 * 物理メモリ情報の初期化
 */
void init_mem_info();

/**
 * メモリ情報出力
 */
void stats();

/**
 * 指定したアドレス間で使用可能なメモリサイズを返す
 */
unsigned int scan_mem(unsigned int start, unsigned int end);

#endif
