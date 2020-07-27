CC        = gcc
INCLUDE   = -I../include -I../window/include
CFLAGS    = -c -Wall -Wextra $(INCLUDE) -nostdlib -fno-builtin
KERN_OBJ  = main.o
WIN_OBJ   = ../window/desktop.o

.PHONY: build clean

build: vmmyos

%.o: %.c
	${CC} $(CFLAGS) -o $@ $*.c

vmmyos: $(KERN_OBJ) $(WIN_OBJ)
	#ld $^ -T kernel.ld -Map kernel.map -o $@
	ld $^ -Map kernel.map -o $@

clean:
	rm -rf *.img vmmyos *.o *.map

# Makefile memo
# B=$(A:%.bin=%.o) -> A=aaa.binのとき B=aaa.o となる
# $< : 最初の依存するファイル
# $^ : すべての依存するファイル
# $* : サフィックスを除いたターゲット名
