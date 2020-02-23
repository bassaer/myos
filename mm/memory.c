#include <memory.h>

#include <console.h>
#include <io.h>
#include <util.h>

#define EFLAGS_AC_BIT       0x00040000
#define CR0_CACHE_DISABLE   0x60000000

#define ATTEMPT_VALUE       0xaa55aa55
#define REVERSE_VALUE       0x55aa55aa

#define NULL                0x00000000

/**
 * 物理メモリの管理情報
 */
struct mem_info mem;

void init_mem_info() {
  mem.total_bytes = 0;
  mem.total_blocks = 0;
  mem.used_bytes = 0;
  mem.used_blocks = 0;
  mem.free_bytes = 0;
  mem.free_blocks = 0;

  stats(&mem);

  mem.total_blocks = mem.total_bytes / BLOCK_SIZE;
  mem.bitmap = (unsigned int *) KERN_MEM_START;
  // 全ブロックの状態をbitで表現するため、intの(32bit)で割る
  mem.bitmap_size = mem.total_blocks / 32;
}


/**
 * nビット目に1をセットする
 */
void set_bit(unsigned int bit) {
  mem.bitmap[bit / 32] |= (1 << (bit % 32));
}

/**
 * nビット目に0セットする
 */
void remove_bit(unsigned int bit) {
  mem.bitmap[bit / 32] &= ~(1 << (bit % 32));
}

/**
 * nビット目を取得する
 */
unsigned int get_bit(unsigned int bit) {
  return mem.bitmap[bit / 32] & (1 << (bit % 32));
}

/**
 * 割当可能なブロックを探索する
 * bitが0の最初のブロック番号をblcokに格納
 */
int find_free_block(unsigned int *block) {
  unsigned int i;
  for (i = 0; i < mem.bitmap_size; ++i) {
    if (mem.bitmap[i] == 0xffffffff) {
      continue;
    }
    unsigned int bit;
    for (bit = 0; bit < 32; ++bit) {
      *block = i * sizeof(unsigned int) * 8 + bit;
      return 1;
    }
  }
  return 0;
}

/**
 * 1ブロックの割当
 */
void* alloc_single_block() {
  if (mem.free_blocks <= 0) {
    return NULL;
  } 
  unsigned int block;
  if (!find_free_block(&block)) {
    return NULL;
  }
  set_bit(block);
  void *addr = (void *)(block * BLOCK_SIZE);
  mem.used_blocks++;
  mem.free_blocks--;
  return addr;
}

/**
 * 1ブロック開放
 */
void free_single_block(void *addr) {
  unsigned int block = (unsigned int) addr / BLOCK_SIZE;
  remove_bit(block);
  mem.used_blocks--;
  mem.free_blocks++;
}

/**
 * メモリの利用状況を取得する
 */
void stats(struct mem_info *mm) {
  // CPUが 386 or 486 以降かを判定
  char is486 = 0;
  unsigned int eflg = io_load_eflags();
  // 486以降はEFLAGS 18bit目がACフラグ
  // 書き込み可能であれば486判定
  eflg |= EFLAGS_AC_BIT;
  io_store_eflags(eflg);
  eflg = io_load_eflags();
  // 386の場合は自動で0に戻る
  if ((eflg & EFLAGS_AC_BIT) != 0) {
    is486 = 1;
  }
  // EFLAGSを戻す
  eflg &= ~EFLAGS_AC_BIT;
  io_store_eflags(eflg);

  unsigned int cr0;
  if (is486) {
    // 486以降のCPUにはキャッシュ機構があるため無効化
    cr0 = load_cr0();
    cr0 |= CR0_CACHE_DISABLE; // キャッシュ無効化
    store_cr0(cr0);
  }

  mm->total_bytes = scan_mem(0x00400000, 0xbfffffff) / (1024*1024);
  if (is486) {
    cr0 = load_cr0();
    cr0 &= ~CR0_CACHE_DISABLE; // キャッシュ有効化
    store_cr0(cr0);
  }
  mm->free_bytes = mm->total_bytes - mm->used_bytes;
}

/**
 * 使用可能なメモリサイズを返す
 * 判定方法はメモリを反転し、書き換え可能か確認
 *
 */
unsigned int scan_mem(unsigned int start, unsigned int end) {
  unsigned int index;
  // 4KB単位でチェック
  for (index = start; index <= end; index += 0x1000) {
    // 末尾4バイトをチェックするために +0xffc
    unsigned int *target = (unsigned int *)(index + 0xffc);
    // 確認前の状態を保存
    unsigned int tmp = *target;
    // 確認用の値を書き込む
    *target = ATTEMPT_VALUE;
    // 値を反転してみる
    *target ^= 0xffffffff;
    // 反転されてるか確認
    if (*target != REVERSE_VALUE) {
      // 値を戻す
      *target = tmp;
      break;
    }
    // 値を更に反転してみる
    *target ^= 0xffffffff;
    // 値が戻っているか確認
    if (*target != ATTEMPT_VALUE) {
      *target = tmp;
      break;
    }
    *target = tmp;
  }
  return index;
}
