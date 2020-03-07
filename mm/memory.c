#include <mm/memory.h>

#include <console.h>
#include <io.h>
#include <mm/pgtable.h>

#define EFLAGS_AC_BIT       0x00040000
#define CR0_CACHE_DISABLE   0x60000000

#define ATTEMPT_VALUE       0xaa55aa55
#define REVERSE_VALUE       0x55aa55aa

/**
 * 物理メモリの管理情報
 */
mem_t mem;

int init_mem_info() {
  mem.total_bytes = 0;
  mem.total_blocks = 0;
  mem.used_bytes = 0;
  mem.used_blocks = 0;
  mem.free_bytes = 0;
  mem.free_blocks = 0;

  stats(&mem);

  mem.total_blocks = mem.total_bytes / BLOCK_SIZE;
  mem.free_blocks = mem.total_blocks;
  mem.bitmap = (unsigned int *)(PMEM_FREE_START - get_kernel_size());
  // 全ブロックの状態をbitで表現するため、intの(32bit)で割る
  mem.bitmap_size = mem.total_blocks / (sizeof(unsigned int) * 8);
  // すべての領域でビットマップ0とし、開放する
  set_mem((void *)mem.bitmap, 0x00, mem.bitmap_size);

  return init_pg_table();
}

unsigned int get_kernel_size() {
  return (unsigned int)(&_KERN_END - PMEM_KERN_START);
}

int init_pg_table() {
  // 0x00400000 - 0x00401000 を割り当て
  page_table_entry *user_table = (page_table_entry *)alloc_single_block();
  // 0x00401000 - 0x00402000 を割り当て
  page_table_entry *kernel_table = (page_table_entry *)alloc_single_block();
  if (user_table == NULL || kernel_table == NULL) {
    return MEM_ERROR;
  }
  // 使用するブロックを初期化
  set_mem(user_table, 0x00, BLOCK_SIZE);
  set_mem(kernel_table, 0x00, BLOCK_SIZE);

  unsigned int i;
  unsigned long paddr;
  unsigned long vaddr;
  // ユーザ空間割当
  for (i = 0, paddr = PMEM_FREE_START, vaddr = VMEM_USER_START; i < PTE_NUM; ++i, paddr += BLOCK_SIZE, vaddr += BLOCK_SIZE) {
    page_table_entry *pte = get_pte(user_table, vaddr);
    set_pte_flags(pte, PDE_FLAG_PRESENT | PDE_FLAG_RW);
    set_pte_paddr(pte, paddr);
  }
  // カーネル空間割当
  // TODO : 仮想アドレス開始をVMEM_KERN_START(0xc0000000)に変更する
  //        動作確認のためストレートマップさせている
  for (i = 0, paddr = PMEM_KERN_START, vaddr = 0x28000000; i < PTE_NUM; ++i, paddr += BLOCK_SIZE, vaddr += BLOCK_SIZE) {
    page_table_entry *pte = get_pte(kernel_table, vaddr);
    set_pte_flags(pte, PDE_FLAG_PRESENT | PDE_FLAG_RW);
    set_pte_paddr(pte, paddr);
  }

  page_directory_entry *pd_table = (page_directory_entry *)alloc_single_block();
  if (pd_table == NULL) {
    return MEM_ERROR;
  }

  // ユーザ空間ページディレクトリ設定
  page_directory_entry *pde = get_pde(pd_table, VMEM_USER_START);
  set_pde_flags(pde, PDE_FLAG_PRESENT | PDE_FLAG_RW);
  set_pde_paddr(pde, (unsigned long)user_table);

  // カーネル空間ぺージディレクトリ設定
  // TODO : 仮想アドレス開始をVMEM_KERN_START(0xc0000000)に変更する
  pde = get_pde(pd_table, 0x28000000);
  set_pde_flags(pde, PDE_FLAG_PRESENT | PDE_FLAG_RW);
  set_pde_paddr(pde, (unsigned long)user_table);

  // ページディレクトリを設定
  set_pd(pde);
  // ページングを有効化
  enable_paging();

  return MEM_SUCCESS;
}

int map_page(unsigned long paddr, unsigned long vaddr) {
  page_directory_entry *curr_pd = get_curr_pd();
  if (curr_pd == NULL) {
    debug("error at 94");
    return MEM_ERROR;
  }
  page_directory_entry *pde = get_pde(curr_pd, vaddr);
  page_table_entry *pg_table;
  if (is_pde_present(pde)) {
    pg_table = (page_table_entry *)get_pde_ptaddr(pde);
  } else {
    pg_table = (page_table_entry *)alloc_single_block();
    if (pg_table == NULL) {
      debug("error at 102");
      return MEM_ERROR;
    }
    //.ブロック初期化
    set_mem(pg_table, 0x00, BLOCK_SIZE);
    set_pde_flags(pde, PDE_FLAG_PRESENT | PDE_FLAG_RW);
    set_pde_paddr(pde, *pg_table);
  }

  page_table_entry *pte = get_pte(pg_table, vaddr);
  set_pte_flags(pte, PDE_FLAG_PRESENT | PDE_FLAG_RW);
  set_pte_paddr(pte, paddr);
  return MEM_SUCCESS;
}

void set_mem(unsigned long *mem, unsigned long value, unsigned int size) {
  unsigned int i;
  for(i = 0; i < size; ++i) {
    mem[i] = value;
  }
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
      if (!get_bit(bit)) {
        *block = i * sizeof(unsigned int) * 8 + bit;
        return 1;
      }
    }
  }
  return 0;
}

/**
 * 1ブロックの割当
 */
void* alloc_single_block() {
  if (mem.free_blocks <= 0) {
    // 割当可能なメモリがない場合
    return NULL;
  } 
  unsigned int block;
  if (!find_free_block(&block)) {
    // 割当可能なメモリが見つからかなった
    return NULL;
  }
  set_bit(block);
  // block番目の物理アドレスを取得
  // 開始を使っていない物理メモリ領域から割り当てる
  void *paddr = (void *)(PMEM_FREE_START + block * BLOCK_SIZE);
  mem.used_blocks++;
  mem.free_blocks--;
  return paddr;
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
void stats(mem_t *mm) {
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

  mm->total_bytes = scan_mem(0x00400000, 0xbfffffff);
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
