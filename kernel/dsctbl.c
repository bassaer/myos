#include <dsctbl.h>
#include <io.h>
#include <console.h>

void init_gdtidt() {
  struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) GDT_ADDR;
  struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) IDT_ADDR;

  int i;
  for (i = 0; i <= GDT_LIMIT / 8; i++) {
    set_segmdesc(gdt + i, 0, 0 , 0);
  }
  // メモリ全体
  set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, AR_DATA32_RW);
  // カーネル
  //set_segmdesc(gdt + 2, KRN_LIMIT, KRN_ADDR, AR_CODE32_ER);
  set_segmdesc(gdt + 2, 0xffffffff, 0x00000000, AR_CODE32_ER);
  load_gdtr(GDT_LIMIT, GDT_ADDR);

  for (i = 0; i <= IDT_LIMIT / 8; i++) {
    set_gatedesc(idt + i, 0, 0, 0);
  }
  load_idtr(IDT_LIMIT, IDT_ADDR);

  // IDTの設定
  // 2*8 -> 2番目のセグメント
  // セグメント番号の下位3bitを0にするため8倍
  // キーボードはIRQ1なのでINT 0x21
  // 割り込み発生でasm_handle_intrをcall
  set_gatedesc(idt + 0x21, (int) asm_handle_intr, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x27, (int) asm_handle_intr27, 2 * 8, AR_INTGATE32);
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar) {
  if (limit > 0xfffff) {
    ar |= 0x8000;    // Gビットフラグを1すると
    limit /= 0x1000; // バイト単位 -> ページ単位に変更(4GBになる)
  }
  sd->limit_low = limit & 0xffff;
  sd->base_low = base & 0xffff;
  sd->base_mid = (base >> 16) & 0xff;
  sd->access_right = ar & 0xff;
  sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
  sd->base_high = (base >> 24) & 0x0ff;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar) {
  gd->offset_low = offset & 0xffff;
  gd->selector = selector;
  gd->dw_count = (ar >> 8) & 0xff;
  gd->access_right = ar & 0xff;
  gd->offset_high = (offset >> 16) & 0xffff;
}
