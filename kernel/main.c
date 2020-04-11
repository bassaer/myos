/*---------------------------*
|  __  __        ___  ____   |
| |  \/  |_   _ / _ \/ ___|  |
| | |\/| | | | | | | \___ \  |
| | |  | | |_| | |_| |___) | |
| |_|  |_|\__, |\___/|____/  |
|         |___/              |
*---------------------------*/

#include <uefi.h>
#include <color.h>
#include <console.h>
#include <event.h>

#include <sh.h>

#define PROMPT  L"myos> "

void efi_main(void *ImageHandle __attribute__ ((unused)), EFI_SYSTEM_TABLE *SystemTable) {
  init_event(SystemTable->BootServices);
  init_console(SystemTable->ConIn, SystemTable->ConOut);
  init_shell();
  start_shell();
}
