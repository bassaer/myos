VER       = $(shell ./scripts/changelog.sh -v)
IMG       = myos-$(VER).img
.PHONY: install img lib run package test clean

all: package

prepare:
	sudo apt install -y mtools qemu gcc-mingw-w64-x86-64 ovmf qemu-system-x86

kernel/vmmyos: graphics
	@$(MAKE) build -C kernel

arch/x86/BOOTX64.EFI: lib
	@$(MAKE) build -C arch/x86

img: arch/x86/BOOTX64.EFI kernel/vmmyos
	dd if=/dev/zero of=$(IMG) bs=1k count=1440
	mformat -i $(IMG) -f 1440 ::
	mmd -i $(IMG) ::/EFI
	mmd -i $(IMG) ::/EFI/BOOT
	mcopy -i $(IMG) $< ::/EFI/BOOT
	mcopy -i $(IMG) kernel/vmmyos ::/

font:
	python scripts/font.py -f scripts/font.txt

lib:
	@$(MAKE) -C lib

graphics:
	@$(MAKE) -c graphics

run: img
	qemu-system-x86_64 -name myos \
                     -monitor stdio \
                     -bios /usr/share/ovmf/OVMF.fd \
                     -net none \
                     -drive format=raw,file=$(IMG) \
                     -device nec-usb-xhci,id=xhci \
                     -device usb-mouse \
                     -device usb-kbd \
                     -d guest_errors \
                     || true

usb: arch/x86/BOOTX64.EFI kernel/vmmyos
	sudo mount /dev/sda /mnt
	sudo rm -rf /mnt/EFI
	sudo rm -rf /mnt/vmmyos
	mkdir -p /mnt/EFI/BOOT
	sudo cp arch/x86/BOOTX64.EFI /mnt/EFI/BOOT
	sudo cp kernel/vmmyos /mnt/
	sudo umount /mnt

package: img
	rm -rf ./release
	mkdir release
	tar zcvf release/myos-$(VER).tag.gz ${IMG}

clean:
	@$(MAKE) clean -C arch/x86
	@$(MAKE) clean -C kernel
	@$(MAKE) clean -C lib
	@$(MAKE) clean -C graphics
