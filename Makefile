VER      = $(shell ./scripts/changelog.sh -v)
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

MM_OBJ   = mm/memory.o

BIN_OBJ  = bin/debug.o \
           bin/echo.o \
           bin/free.o \
           bin/sh.o \
           bin/shutdown.o

.PHONY: install img run package clean


all: package

install:
	sudo apt install -y gcc mtools qemu-system-i386

img: init/ipl.bin init/head.bin kernel/kernel.bin
	cat init/head.bin kernel/kernel.bin > sys.bin
	mformat -f 1440 -C -B init/ipl.bin -i $(IMG)
	mcopy sys.bin -i $(IMG) ::

%.o: %.c
	gcc $(CFLAGS) -o $@ $*.c

%.bin: %.o
	ld $(LDFLAGS) $^ -T $*.ld -o $@

.s.o:
	as --32 -o $@ $<

init/ipl.bin: init/ipl.o
	ld $^ -T init/ipl.ld -Map ipl.map -o $@

init/head.bin: init/head.o
	ld $^ -T init/head.ld -Map head.map -o $@

# TODO : ひとまずkernelにすべてリンクするが、あとでユーザ空間を分ける
kernel/kernel.bin: $(KERN_OBJ) $(BIN_OBJ) $(MM_OBJ)
	ld $^ -T kernel/kernel.ld -Map kernel.map -o $@

run: img
	qemu-system-i386 -name myos -localtime -monitor stdio -device isa-debug-exit -fda ./$(IMG) || true

package: img
	rm -rf ./release
	mkdir release
	tar zcvf release/myos-$(VER).tag.gz ${IMG}

clean:
	find . \( -name '*.img' -or -name '*.bin' -or -name '*.o' -or -name '*.map' \) | xargs rm -f
	rm -rf ./release

# Makefile memo
# B=$(A:%.bin=%.o) -> A=aaa.binのとき B=aaa.o となる
# $< : 最初の依存するファイル
# $^ : すべての依存するファイル
# $* : サフィックスを除いたターゲット名
