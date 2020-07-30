TARGET  = lib.o
CC      = gcc
CFLAGS  = -c -Wall -I../include -fno-pie -fno-builtin -nostdlib
OBJ     = string.o \
          queue.o

.PHONY: clean

all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $*.c

clean:
	rm -rf *.o
