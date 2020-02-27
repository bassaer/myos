#include <mm/pgtable.h>

#include <mm/memory.h>

page_directory_entry *curr_pd;

page_directory_entry* get_curr_pd() {
  return curr_pd;
}

void set_pte_flags(page_table_entry *pte, unsigned int flags) {
  *pte |= flags;
}

void clear_pte_flags(page_table_entry *pte, unsigned int flags) {
  *pte &= ~flags;
}

void set_pte_paddr(page_table_entry *pte, unsigned long paddr) {
  // 下位12bitをマスク
  paddr &= 0xfffff000;
  *pte |= paddr;
}

void* get_pte_paddr(page_table_entry *pte) {
  // 下位12bitをマスク
  return (void *)(*pte & 0xfffff000);
}

int is_pte_present(page_table_entry *pte) {
  return (unsigned int)pte & (unsigned int)PTE_FLAG_PRESENT;
}

int is_pte_writeable(page_table_entry *pte) {
  return (unsigned int)pte & (unsigned int)PTE_FLAG_RW;
}

/**
 * 仮想アドレスを右に12bitシフトとマスクで
 * 12から21bitの10bitを取得
 * これがページテーブルのインデックスになる
 */
unsigned long get_pte_index(unsigned long vaddr) {
  return (vaddr >> PTE_SHIFT) & PTE_MASK;
}

page_table_entry* get_pte(page_table_entry *table, unsigned int vaddr) {
  if (table == NULL) {
    return (page_table_entry *)NULL;
  }
  return (page_table_entry *)(table[get_pte_index(vaddr)]);
}


void set_pde_flags(page_directory_entry *pde, unsigned int flags) {
  *pde |= flags;
}

void clear_pde_flags(page_directory_entry *pde, unsigned int flags) {
  *pde &= ~flags;
}

void set_pde_paddr(page_directory_entry *pde, unsigned long paddr) {
  // 下位12bitをマスク
  paddr &= 0xfffff000;
  *pde |= paddr;
}

void* get_pde_paddr(page_directory_entry *pde) {
  // 下位12bitをマスク
  return (void *)(*pde & 0xfffff000);
}


int is_pde_present(page_directory_entry *pde) {
  return (unsigned int)pde & (unsigned int)PDE_FLAG_PRESENT;
}

int is_pde_writeable(page_directory_entry *pde) {
  return (unsigned int)pde & (unsigned int)PDE_FLAG_RW;
}

unsigned long get_pde_index(unsigned long vaddr) {
  return (vaddr >> PDE_SHIFT) & PDE_MASK;
}

page_directory_entry* get_pde(page_directory_entry *table, unsigned long vaddr) {
  if (table == NULL) {
    return (page_directory_entry *)NULL;
  }
  return &table[get_pde_index(vaddr)];
}

page_table_entry get_pde_ptaddr(page_directory_entry *pd) {
  return *pd & PDE_PT_ADDR;
}

void set_pd(page_directory_entry *pde) {
  __asm__("movl %%cr3, %0"::"r"(pde));
}

void enable_paging() {
  __asm__(
      "pushl %eax;"
      "movl %cr0, %eax;"
      "OR 0x80000000, %eax;"
      "movl %eax, %cr0;"
      "pop %eax;"
  );
}
