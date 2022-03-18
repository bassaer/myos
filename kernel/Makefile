CC        = gcc
INCLUDE   = -I../include -I../graphics/include
CFLAGS    = -c -Wall -Wextra $(INCLUDE) -nostdlib -fno-builtin -fPIE
KERN_OBJ  = main.o
WIN_OBJ   = ../graphics/desktop.o \
            ../graphics/cursor.o

LIB_OBJ   = ../lib/string.o

.PHONY: build clean

build: vmmyos

%.o: %.c
	${CC} $(CFLAGS) -o $@ $*.c

vmmyos: $(KERN_OBJ) $(WIN_OBJ) $(LIB_OBJ)
	#ld $^ -T kernel.ld -Map kernel.map -o $@
	ld $^ -Map kernel.map -pie -o $@

clean:
	rm -rf *.img vmmyos *.o *.map

# Makefile memo
# B=$(A:%.bin=%.o) -> A=aaa.binのとき B=aaa.o となる
# $< : 最初の依存するファイル
# $^ : すべての依存するファイル
# $* : サフィックスを除いたターゲット名
