VER       = $(shell ./scripts/changelog.sh -v)
IMG       = myos-$(VER).img
CFLAGS    = -c -m32 -Wall -Iinclude -Ibin/include -fno-pie -fno-builtin -nostdlib
KERN_OBJ  = kernel/main.o \
            kernel/func.o \
            kernel/dsctbl.o \
            kernel/console.o \
            kernel/intr.o \
            kernel/keyboard.o \
            kernel/sched.o \
            kernel/timer.o

LIB_OBJ   = lib/queue.o \
            lib/string.o

MM_OBJ    = mm/memory.o \
            mm/pgtable.o

DRV_OBJ   = drivers/cursor.o \
            drivers/palette.o \
            drivers/screen.o \
            drivers/vram.o

BIN_OBJ   = bin/echo.o \
            bin/free.o \
            bin/ls.o \
            bin/sh.o \
            bin/shutdown.o \
            bin/sleep.o

ARCH_BOOT = arch/x86/boot

ifeq ($(UI), CUI)
	ARCH_HEAD = arch/x86/cui
else
	ARCH_HEAD = arch/x86/gui
endif

.PHONY: install img run package clean

all: package

install:
	sudo apt install -y gcc mtools qemu-system-i386

img: $(ARCH_BOOT)/ipl.bin $(ARCH_HEAD)/head.bin kernel/kernel.bin
	cat $(ARCH_HEAD)/head.bin kernel/kernel.bin > sys.bin
	mformat -f 1440 -C -B $(ARCH_BOOT)/ipl.bin -i $(IMG)
	mcopy sys.bin -i $(IMG) ::

%.o: %.c
	gcc $(CFLAGS) -o $@ $*.c

%.bin: %.o
	ld $(LDFLAGS) $^ -T $*.ld -o $@

.s.o:
	as --32 -o $@ $<

font:
	python scripts/font.py -f scripts/font.txt

$(ARCH_BOOT)/ipl.bin: $(ARCH_BOOT)/ipl.o
	ld $^ -T $(ARCH_BOOT)/ipl.ld -Map ipl.map -o $@

$(ARCH_HEAD)/head.bin: $(ARCH_HEAD)/head.o
	ld $^ -T $(ARCH_HEAD)/head.ld -Map head.map -o $@

# TODO : ひとまずkernelにすべてリンクするが、あとでユーザ空間を分ける
kernel/kernel.bin: $(KERN_OBJ) $(LIB_OBJ) $(BIN_OBJ) $(MM_OBJ) $(DRV_OBJ)
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
