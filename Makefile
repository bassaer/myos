IMG=myos.img
IPL=ipl.bin

.PHONY: ipl img run

all: ipl img run

img: $(IPL)
	mformat -f 1440 -C -B $(IPL) -i $(IMG) ::

ipl: $(IPL:%.bin=%.o)
	ld $^ -T ipl.ld -o $(IPL)

run: $(IMG)
	qemu-system-i386 -localtime -hda ./$(IMG)

clean:
	rm -f $(IMG) $(IPL) $(IPL:%.bin=%.o)
