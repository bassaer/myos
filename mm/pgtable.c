#include <pgtable.h>

void set_pte_flag(page_table_entry *pte, unsigned int flags) {
  *pte |= flags;
}

void clear_pte_flag(page_table_entry *pte, unsigned int flags) {
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

void set_pde_flag(page_directory_entry *pde, unsigned int flags) {
  *pde |= flags;
}

void clear_pde_flag(page_directory_entry *pde, unsigned int flags) {
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
