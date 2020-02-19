#include <memory.h>

#include <console.h>
#include <io.h>
#include <util.h>

#define EFLAGS_AC_BIT       0x00040000
#define CR0_CACHE_DISABLE   0x60000000

#define ATTEMPT_VALUE       0xaa55aa55
#define REVERSE_VALUE       0x55aa55aa

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

  mm->total = scan_mem(0x00400000, 0xbfffffff) / (1024*1024);
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
