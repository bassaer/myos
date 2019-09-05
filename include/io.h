#define outb(value, port)        \
__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))
