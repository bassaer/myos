IMG=myos.img

.PHONY: img run clean

all: img

img: boot/ipl.bin kernel/init.bin
	mformat -f 1440 -C -B boot/ipl.bin -i $(IMG)
	mcopy kernel/init.bin -i $(IMG) ::

%.bin: %.o
	ld $^ -T $*.ld -o $@

run: img
	qemu-system-i386 -localtime -fda ./$(IMG)

clean:
	find . \( -name *.img -or -name *.bin -or -name *.o \) | xargs rm -f

# Makefile memo
# B=$(A:%.bin=%.o) -> A=aaa.binのとき B=aaa.o となる
# $^ : すべての依存するファイル
# $* : サフィックスを除いたターゲット名
