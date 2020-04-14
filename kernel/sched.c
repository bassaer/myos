#include <sched.h>

#include <console.h>
#include <io.h>
#include <tss.h>
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
  /*
  while(1) {
    io_hlt();
  }
  */
}

