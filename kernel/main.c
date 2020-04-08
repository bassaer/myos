/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/

#include<main.h>

void efi_main(void *ImageHandle __attribute__ ((unused)), struct EFI_SYSTEM_TABLE *SystemTable) {
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"start MyOS\r\n");

  struct EFI_INPUT_KEY key;
  unsigned short buf[3];
  while (1) {
    if (!SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key)) {
      if (key.UnicodeChar != L'\r') {
        buf[0] = key.UnicodeChar;
        buf[1] = L'\0';
      } else {
        buf[0] = L'\r';
        buf[1] = L'\n';
        buf[2] = L'\0';
      }
      SystemTable->ConOut->OutputString(SystemTable->ConOut, buf);
    }
  }
}
