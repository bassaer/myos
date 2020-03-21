#ifndef MYOS_PGTABLE_H
#define MYOS_PGTABLE_H

#define PTE_FLAG_PRESENT        0x00000001
#define PTE_FLAG_RW             0x00000002

#define PDE_FLAG_PRESENT        0x00000001
#define PDE_FLAG_RW             0x00000002

#define PTE_MASK                0x000003ff
#define PTE_SHIFT               12
#define PTE_NUM                 1024

#define PDE_MASK                0x000003ff
#define PDE_SHIFT               22
#define PDE_PT_ADDR             0xfffff000
#define PDE_NUM                 1024

typedef unsigned long page_table_entry;
typedef unsigned long page_directory_entry;

page_directory_entry* get_curr_pd();

void set_pte_flags(page_table_entry *pte, unsigned int flags);

void clear_pte_flags(page_table_entry *pte, unsigned int flags);

void set_pte_paddr(page_table_entry *pte, unsigned long paddr);

void* get_pte_paddr(page_table_entry *pte);

int is_pte_present(page_table_entry *pte);

int is_pte_writeable(page_table_entry *pte);

int get_pte_index(unsigned long vaddr);

page_table_entry* get_pte(page_table_entry *table, unsigned int vaddr);

void set_pde_flags(page_directory_entry *pde, unsigned int flags);

void clear_pde_flags(page_directory_entry *pde, unsigned int flags);

void set_pde_paddr(page_directory_entry *pde, unsigned long paddr);

void* get_pde_paddr(page_directory_entry *pde);

int is_pde_present(page_directory_entry *pde);

int is_pde_writeable(page_directory_entry *pde);

unsigned long get_pde_index(unsigned long vaddr);

page_directory_entry* get_pde(page_directory_entry *table, unsigned long vaddr);

page_table_entry get_pde_ptaddr(page_directory_entry *pd);

void set_pd(page_directory_entry *pde);

void enable_paging();

#endif
