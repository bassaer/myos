#ifndef MYOS_PGTABLE_H
#define MYOS_PGTABLE_H

typedef unsigned long page_table_entry;
typedef unsigned long page_directory_entry;

void set_pte_flag(page_table_entry *pte, unsigned int flags);

void clear_pte_flag(page_table_entry *pte, unsigned int flags);

void set_pte_paddr(page_table_entry *pte, unsigned long paddr);

void* get_pte_paddr(page_table_entry *pte);

void set_pde_flag(page_directory_entry *pde, unsigned int flags);

void clear_pde_flag(page_directory_entry *pde, unsigned int flags);

void set_pde_paddr(page_directory_entry *pde, unsigned long paddr);

void* get_pde_paddr(page_directory_entry *pde);

#endif
