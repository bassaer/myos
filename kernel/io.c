#include <io.h>

void outb(unsigned short port, unsigned char value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}
