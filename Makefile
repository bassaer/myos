VER     = $(shell cat CHANGELOG.md | awk 'tolower($$0) ~/^\#\#\sversion\s.*/ {print $$3}')
IMG     = myos-$(VER).img
CFLAGS  = -Iinclude -c -m32 -fno-pie -nostdlib -fno-builtin

.PHONY: img run clean

all: img

img: boot/ipl.bin boot/loader.bin kernel/init.bin
	mformat -f 1440 -C -B boot/ipl.bin -i $(IMG) ::
	mcopy boot/loader.bin -i $(IMG) ::
	mcopy kernel/init.bin -i $(IMG) ::

%.o: %.c
	gcc $(CFLAGS) -o $@ $*.c

%.bin: %.o
	ld $(LDFLAGS) $^ -T $*.ld -o $@

.s.o:
	as --32 -o $@ $<

boot/ipl.bin: boot/ipl.o
	ld $^ -T boot/ipl.ld -o $@

boot/loader.bin: boot/loader.o
	ld $^ -T boot/loader.ld -o $@

kernel/init.bin: kernel/main.o kernel/func.o kernel/dsctbl.o kernel/console.o
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
