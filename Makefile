VER       = $(shell ./scripts/changelog.sh -v)
IMG       = myos-$(VER).img
CC        = x86_64-w64-mingw32-gcc
CFLAGS    = -Wall -Wextra -Iinclude -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10
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
	sudo apt install -y mtools qemu-system-x86_64 gcc-mingw-w64-x86-64 ovmf

BOOTX64.EFI: kernel/main.c
	${CC} ${CFLAGS} -e efi_main -o $@ $<

img: BOOTX64.EFI
	dd if=/dev/zero of=$(IMG) bs=1k count=1440
	mformat -i $(IMG) -f 1440 ::
	mmd -i $(IMG) ::/EFI
	mmd -i $(IMG) ::/EFI/BOOT
	mcopy -i $(IMG) $< ::/EFI/BOOT

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
	qemu-system-x86_64 -name myos \
                     -localtime \
                     -monitor stdio \
                     -bios /usr/share/ovmf/OVMF.fd \
                     -net none \
                     -usbdevice disk::$(IMG) \
                     || true

package: img
	rm -rf ./release
	mkdir release
	tar zcvf release/myos-$(VER).tag.gz ${IMG}

clean:
	find . \( -name '*.img' -or -name '*.bin' -or -name '*.o' -or -name '*.map' \) | xargs rm -f
	rm -rf ./release BOOTX64.EFI

# Makefile memo
# B=$(A:%.bin=%.o) -> A=aaa.binのとき B=aaa.o となる
# $< : 最初の依存するファイル
# $^ : すべての依存するファイル
# $* : サフィックスを除いたターゲット名
