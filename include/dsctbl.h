#ifndef MYOS_DSCTBL_H
#define MYOS_DSCTBL_H

/**
 * IDTの属性設定
 * 割り込み設定が有効であることを示す
 */
#define AR_INTGATE32  0x008e

#define AR_DATA32_RW  0x4092
#define AR_CODE32_ER  0x409a

#define KRN_ADDR      0x00280000
#define KRN_LIMIT     0x0007ffff

#define GDT_ADDR      0x00270000
#define GDT_LIMIT     0x0000ffff
#define IDT_ADDR      0x0026f800
#define IDT_LIMIT     0x000007ff

struct SEGMENT_DESCRIPTOR {
  short limit_low;
  short base_low;
  char base_mid;
  char access_right;
  char limit_high;
  char base_high;
};

struct GATE_DESCRIPTOR {
  short offset_low;
  short selector;
  char dw_count;
  char access_right;
  short offset_high;
};

void init_gdtidt();
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

#endif
