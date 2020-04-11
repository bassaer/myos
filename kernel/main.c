/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/

#include<uefi.h>
#include<color.h>

#define PROMPT  L"myos> "

void efi_main(void *ImageHandle __attribute__ ((unused)), EFI_SYSTEM_TABLE *SystemTable) {
  SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  SystemTable->ConOut->EnableCursor(SystemTable->ConOut, true);
  unsigned short *os =
    L"  __  __        ___  ____     \r\n"
     " |  \\/  |_   _ / _ \\/ ___|  \r\n"
     " | |\\/| | | | | | | \\___ \\ \r\n"
     " | |  | | |_| | |_| |___) |   \r\n"
     " |_|  |_|\\__, |\\___/|____/  \r\n"
     "         |___/              \r\n\n";
  SystemTable->ConOut->OutputString(SystemTable->ConOut, os);
  SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_GREEN);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, PROMPT);
  SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY);

  EFI_INPUT_KEY key;
  unsigned short buf[3];
  while (1) {
    if (!SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key)) {
      if (key.UnicodeChar != L'\r') {
        buf[0] = key.UnicodeChar;
        buf[1] = L'\0';
        SystemTable->ConOut->OutputString(SystemTable->ConOut, buf);
      } else {
        buf[0] = L'\r';
        buf[1] = L'\n';
        buf[2] = L'\0';
        SystemTable->ConOut->OutputString(SystemTable->ConOut, buf);
        SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_GREEN);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, PROMPT);
      }
      SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY);
    }
  }
}
