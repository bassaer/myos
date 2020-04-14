LIBS    = ../lib/string.o
CFLAGS  = -Wall -I../include -I../include/lib -Iinclude -fno-builtin -L$(LIBS)
OBJ     = ../lib/string.o \
          test_string.o

.PHONY: test clean

all: test

$(LIBS):
	make -C ../lib

%.o: %.c
	gcc -c $(CFLAGS) -o $@ $*.c

test_main.o: $(LIBS) test_main.c
	gcc -c $(CFLAGS) -L$< -o $@ test_main.c

test.bin: $(OBJ) test_main.o
	gcc $(CFLAGS) -o $@ $^

test: test.bin
	@./test.bin
	@printf "\033[%dm%s\033[0m\n" 32 "All tests passed."

clean:
	rm -rf *.o *.bin
