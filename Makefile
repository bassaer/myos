VER    = $(shell cat CHANGELOG.md | awk 'tolower($$0) ~/^\#\#\sversion\s.*/ {print $$3}')
IMG    = myos-$(VER).img
CFLAGS = -c -m32 -nostdlib -fno-builtin -fno-pie

.PHONY: img run clean

all: img

img: boot/ipl.bin kernel/init.bin
	mformat -f 1440 -C -B boot/ipl.bin -i $(IMG)
	mcopy kernel/init.bin -i $(IMG) ::

%.o: %.c
	gcc $(CFLAGS) -o $@ $*.c

%.bin: %.o
	ld $^ -T $*.ld -o $@

.s.o:
	as --32 -o $@ $<

boot/ipl.bin: boot/ipl.o
	ld $^ -T boot/ipl.ld -o $@

kernel/init.bin: kernel/init.o kernel/main.o
	ld $^ -T kernel/init.ld -o $@

run: img
	qemu-system-i386 -localtime -fda ./$(IMG)

clean:
	find . \( -name '*.img' -or -name '*.bin' -or -name '*.o' \) | xargs rm -f

# Makefile memo
# B=$(A:%.bin=%.o) -> A=aaa.binのとき B=aaa.o となる
# $< : 最初の依存するファイル
# $^ : すべての依存するファイル
# $* : サフィックスを除いたターゲット名
