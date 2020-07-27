#ifndef MYOS_EVENT_H
#define MYOS_EVENT_H

#include <uefi.h>

void init_event(EFI_BOOT_SERVICES *Services);
void wait_event(UINTN NumberOfEvents, EFI_EVENT *Event, UINTN *Index);

#endif
