#include <event.h>

static EFI_BOOT_SERVICES *BootServices;

void init_event(EFI_BOOT_SERVICES *Services) {
  BootServices = Services;
  BootServices->SetWatchdogTimer(0, 0, 0, NULL);
}

void wait_event(UINTN NumberOfEvents, EFI_EVENT *Event, UINTN *Index) {
  BootServices->WaitForEvent(NumberOfEvents, Event, Index);
}
