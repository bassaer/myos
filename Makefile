VER      = $(shell cat CHANGELOG.md | awk 'tolower($$0) ~/^\#\#\sversion\s.*/ {print $$3}')
IMG      = myos-$(VER).img
CFLAGS   = -c -m32 -Wall -Iinclude -fno-pie -fno-builtin -nostdlib
KERN_OBJ = kernel/main.o \
           kernel/func.o \
           kernel/dsctbl.o \
           kernel/console.o \
           kernel/intr.o \
           kernel/queue.o \
           kernel/keyboard.o \
           kernel/util.o

.PHONY: img run clean

all: img

img: boot/ipl.bin boot/loader.bin kernel/kernel.bin
	cat boot/loader.bin kernel/kernel.bin > sys.bin
	mformat -f 1440 -C -B boot/ipl.bin -i $(IMG)
	mcopy sys.bin -i $(IMG) ::

%.o: %.c
	gcc $(CFLAGS) -o $@ $*.c

%.bin: %.o
	ld $(LDFLAGS) $^ -T $*.ld -o $@

.s.o:
	as --32 -o $@ $<

boot/ipl.bin: boot/ipl.o
	ld $^ -T boot/ipl.ld -Map ipl.map -o $@

boot/loader.bin: boot/loader.o
	ld $^ -T boot/loader.ld -Map loader.map -o $@

kernel/kernel.bin: $(KERN_OBJ)
	ld $^ -T kernel/kernel.ld -Map kernel.map -o $@

run: img
	qemu-system-i386 -name myos -localtime -monitor stdio -fda ./$(IMG)

clean:
	find . \( -name '*.img' -or -name '*.bin' -or -name '*.o' -or -name '*.map' \) | xargs rm -f

# Makefile memo
# B=$(A:%.bin=%.o) -> A=aaa.binのとき B=aaa.o となる
# $< : 最初の依存するファイル
# $^ : すべての依存するファイル
# $* : サフィックスを除いたターゲット名
