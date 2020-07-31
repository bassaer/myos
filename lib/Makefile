TARGET  = lib.o
CC      = gcc
CFLAGS  = -c -Wall -I../include -fno-builtin -nostdlib -fPIE #-fno-pie
OBJ     = string.o \
          queue.o

.PHONY: clean

all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $*.c

clean:
	rm -rf *.o
