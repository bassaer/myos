CC        = x86_64-w64-mingw32-gcc
INCLUDE   = -I../../include
CFLAGS    = -Wall -Wextra $(INCLUDE) -nostdlib -fno-builtin -Wl,--subsystem,10

MAKE      = make --no-print-directory

.PHONY: build clean

build: BOOTX64.EFI

BOOTX64.EFI: boot.c #$(KERN_C) $(LIB_C) $(BIN_C)
	$(CC) $(CFLAGS) -e efi_main -o $@ $^ -lgcc

clean:
	rm -rf *.o BOOTX64.EFI
