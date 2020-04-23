#include <uefi.h>

void halt() {
  while (1) __asm__ volatile("hlt");
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  UINTN MemoryMapSize = 4800;
  EFI_MEMORY_DESCRIPTOR MemoryMap[4800];
  UINTN MapKey;
  UINTN DescriptorSize;
  UINT32 DescriptorVersion;

  EFI_STATUS status = EFI_SUCCESS;
  do {
    status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, (EFI_MEMORY_DESCRIPTOR *)MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (status == EFI_BUFFER_TOO_SMALL) {
      EFI_STATUS allocStatus = SystemTable->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, (void **)&MemoryMap);
      if (allocStatus != EFI_SUCCESS) {
      }
    }
  } while(status != EFI_SUCCESS);

  if (status != EFI_SUCCESS) {
    halt();
  }

  //printf(L"[info] MapKey: %d, MemoryMapSize: %d, vadd: %d, padd: %d\r\n", MapKey, MemoryMapSize, MemoryMap.VirtualStart, MemoryMap.PhysicalStart);
  //__asm__ volatile("jmp *%0"::"r"(0x00000000000b0000));
  //status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"booting...\r\n");
  halt();
  //printf(L"Start Shell\r\n");

  //start_shell();

  return EFI_SUCCESS;
}
