#ifndef MYOS_DSCTBL_H
#define MYOS_DSCTBL_H

struct SEGMENT_DESCRIPTOR {
  short limit_low;
  short base_low;
  char base_mid;
  char base_high;
  char access_right;
  char limit_high;
};

struct GATE_DESCRIPTOR {
  short offset_low;
  short offset_high;
  short selector;
  char dw_count;
  char access_right;
};

void init_gdtidt();
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

#endif
