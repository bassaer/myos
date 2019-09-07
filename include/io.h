#ifndef MYOS_IO_H
#define MYOS_IO_H

/**
 * kernel/func.s
 */
void hlt(void);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void outb_p(int port, int data);

/**
 * kernel/io.c
 */
void outb(unsigned short port, unsigned char value);

#endif
