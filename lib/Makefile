TARGET  = lib.o
CFLAGS  = -c -Wall -I../include -fno-builtin
OBJ     = string.o \
          queue.o

.PHONY: clean

all: $(OBJ)

%.o: %.c
	gcc $(CFLAGS) -o $@ $*.c

clean:
	rm -rf *.o
