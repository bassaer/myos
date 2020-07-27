TARGET  = lib.o
#CC      = x86_64-linux-gnu-gcc
CC      = x86_64-w64-mingw32-gcc
CFLAGS  = -c -Wall -I../include -fno-builtin
OBJ     = string.o \
          queue.o

.PHONY: clean

all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $*.c

clean:
	rm -rf *.o
