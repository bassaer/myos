CC        = gcc
INCLUDE   = -I../include -I../include/window
CFLAGS    = -c -Wall -Wextra $(INCLUDE) -nostdlib -fno-builtin -fPIE

WIN_OBJ   = cursor.o \
            screen.o

.PHONY: build clean

build: $(WIN_OBJ)

%.o: %.c
	${CC} $(CFLAGS) -o $@ $*.c

clean:
	rm -rf *.o
