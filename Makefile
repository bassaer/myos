IMG=myos.img
IPL=ipl.bin

.PHONY: ipl img run

all: ipl img run

img: $(IPL) boot.bin
	mformat -f 1440 -C -B $(IPL) -i $(IMG)
	mcopy boot.bin -i $(IMG) ::

ipl: $(IPL:%.bin=%.o)
	ld $^ -T ipl.ld -o $(IPL)

%.bin: %s %ld
	ld $^ -T $*.ld -o $@ $*.o

run: $(IMG)
	qemu-system-i386 -localtime -fda ./$(IMG)

clean:
	rm -f $(IMG) $(IPL) $(IPL:%.bin=%.o)
