CC        = gcc
INCLUDE   = -I../include -I../bin/include
CFLAGS    = -c -Wall -Wextra $(INCLUDE) -nostdlib -fno-builtin
KERN_OBJ  = kernel/main.o \
            kernel/console.o \

LIB_OBJ   = lib/queue.o \
            lib/string.o

MM_OBJ    = mm/memory.o \
            mm/pgtable.o

DRV_OBJ   = drivers/cursor.o \
            drivers/palette.o \
            drivers/screen.o \
            drivers/vram.o

KERN_C    = $(wildcard kernel/*.c)

LIB_C     = $(wildcard lib/*.c)

BIN_C     = $(wildcard bin/*.c)

ARCH_BOOT = arch/x86/boot

.PHONY: build clean

build: vmmyos

%.o: %.c
	${CC} $(CFLAGS) -o $@ $*.c

%.bin: %.o
	ld $(LDFLAGS) $^ -T $*.ld -o $@

vmmyos: main.o #$(KERN_OBJ) $(LIB_OBJ) $(BIN_OBJ) $(MM_OBJ) $(DRV_OBJ)
	#ld $^ -T kernel.ld -Map kernel.map -o $@
	ld $^ -Map kernel.map -o $@

clean:
	rm -rf *.img vmmyos *.o *.map

# Makefile memo
# B=$(A:%.bin=%.o) -> A=aaa.binのとき B=aaa.o となる
# $< : 最初の依存するファイル
# $^ : すべての依存するファイル
# $* : サフィックスを除いたターゲット名
