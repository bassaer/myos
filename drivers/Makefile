CC        = gcc
INCLUDE   = -I../include -I../include/drivers
CFLAGS    = -c -Wall -Wextra $(INCLUDE) -nostdlib -fno-builtin -fPIE

WIN_OBJ   = xhci.o

.PHONY: build clean

build: $(WIN_OBJ)

%.o: %.c
	${CC} $(CFLAGS) -o $@ $*.c

clean:
	rm -rf *.o
