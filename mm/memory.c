#include <memory.h>

#include <console.h>
#include <io.h>
#include <util.h>

#define EFLAGS_AC_BIT       0x00040000
#define CR0_CACHE_DISABLE   0x60000000


/**
 * メモリの利用状況を取得する
 */
void stats(struct MEM_INFO *mm) {
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

  mm->total = calc_mem(0x00400000, 0xbfffffff);
  if (is486) {
    cr0 = load_cr0();
    cr0 &= ~CR0_CACHE_DISABLE; // キャッシュ有効化
    store_cr0(cr0);
  }
  mm->used = 0;
  mm->free = 0;
}

/**
 * 使用可能なメモリサイズを返す
 * 判定方法はメモリを反転し、書き換え可能か確認
 *
 */
int calc_mem(unsigned int start, unsigned int end) {
  return 12345;
}
