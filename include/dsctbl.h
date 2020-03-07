#ifndef MYOS_DSCTBL_H
#define MYOS_DSCTBL_H

/**
 * IDTの属性設定
 * 割り込み設定が有効であることを示す
 */
#define AR_INTGATE32  0x008e

#define AR_DATA32_RW  0x4092
#define AR_CODE32_ER  0x409a

#define KERN_ADDR     0x00000000
#define KERN_LIMIT    0xffffffff

#define GDT_ADDR      0x00270000
#define GDT_LIMIT     0x0000ffff
#define IDT_ADDR      0x0026f800
#define IDT_LIMIT     0x000007ff

typedef struct {
  short limit_low;
  short base_low;
  char base_mid;
  char access_right;
  char limit_high;
  char base_high;
} segment_descriptor_t;

typedef struct {
  short offset_low;
  short selector;
  char dw_count;
  char access_right;
  short offset_high;
} gate_descriptor_t;

void init_gdtidt();
void set_segmdesc(segment_descriptor_t *sd, unsigned int limit, int base, int ar);
void set_gatedesc(gate_descriptor_t *gd, int offset, int selector, int ar);

#endif
