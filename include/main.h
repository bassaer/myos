#ifndef MYOS_MAIN_H
#define MYOS_MAIN_H

#define ADR_BOOTINFO 0x00000ff0

struct BOOTINFO {
  char cyls;
  char leds;
};

#endif
