#ifndef MYOS_MEMORY_H
#define MYOS_MEMORY_H

#define BLOCK_SIZE        4096

#define PMEM_KERN_START   0x00280000
#define PMEM_FREE_START   0x00400000

#define VMEM_USER_START   0x00000000
#define VMEM_USER_END     0xbfffffff
#define VMEM_KERN_START   0xc0000000
#define VMEM_KERN_END     0xffffffff

#define MEM_SUCCESS       0
#define MEM_ERROR         (-1)

extern unsigned int _KERN_END;

typedef struct {
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
} mem_t;

unsigned int get_kernel_size();

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
int init_mem_info();

int init_pg_table();

int map_page(unsigned long paddr, unsigned long vaddr);
void set_mem(unsigned long *mem, unsigned long value, unsigned int size);

/**
 * メモリ情報出力
 */
void stats();

/**
 * 指定したアドレス間で使用可能なメモリサイズを返す
 */
unsigned int scan_mem(unsigned int start, unsigned int end);

/**
 * ページフォールハンドラ
 */
void handle_intr14(int *esp);

#endif
