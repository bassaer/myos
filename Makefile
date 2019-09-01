VER    = $(shell cat CHANGELOG.md | awk 'tolower($$0) ~/^\#\#\sversion\s.*/ {print $$3}')
IMG    = myos-$(VER).img
CFLAGS = -c -nostdlib -fno-builtin

.PHONY: img run clean

all: img

img: boot/ipl.bin kernel/init.bin
	mformat -f 1440 -C -B boot/ipl.bin -i $(IMG)
	mcopy kernel/init.bin -i $(IMG) ::

%.o: %.c
	gcc $(CFLAGS) -o $@ $*.c

%.bin: %.o
	ld $^ -T $*.ld -o $@

kernel/init.bin: kernel/init.o kernel/main.o
	ld $^ -T kernel/init.ld -o $@

run: img
	qemu-system-i386 -localtime -fda ./$(IMG)

clean:
	find . \( -name '*.img' -or -name '*.bin' -or -name '*.o' \) | xargs rm -f

# Makefile memo
# B=$(A:%.bin=%.o) -> A=aaa.binのとき B=aaa.o となる
# $^ : すべての依存するファイル
# $* : サフィックスを除いたターゲット名
