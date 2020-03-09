#include <sched.h>

#include <console.h>
#include <io.h>
#include <tss.h>
#include <dsctbl.h>
#include <mm/memory.h>

void init_sched() {
  /*
  tss_t tss1;
  tss1.ldtr = 0;
  tss1.iomap = 0x40000000;
  */


  //segment_descriptor_t *gdt = (segment_descriptor_t *) GDT_ADDR;

  //set_segmdesc(gdt + 3, 103, (int) &tss1, AR_TSS32);
  //set_segmdesc(gdt + 4, 103, (int) &tss2, AR_TSS32);

  //set_segmdesc(gdt + 3, 0xffffffff, 0x00000000, AR_TSS32);
  //set_segmdesc(gdt + 4, 0xffffffff, 0x00000000, AR_TSS32);

  //set_gdtr(GDT_LIMIT, GDT_ADDR);

  //set_tr(3 * 8);
}

void task2_main() {
  debug("start task2!!");
  /*
  while(1) {
    io_hlt();
  }
  */
}

/**
 * TRは現在実行中のタスクを記憶するためのレジスタ
 * ltrはTRを変更するための命令
 */
void set_tr(int tr) {
  __asm__ volatile(
      "ltr %%ax;"
      ::"a"(tr)
  );
  return; // returnが必要
}

/**
 * 指定のアドレスにロングjumpする
 */
void context_switch(int *addr) {
  __asm__ volatile(
      "ljmp *(%0);"
      ::"r"(&addr)
  );
}
