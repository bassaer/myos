#include <uefi.h>
#include <console.h>
#include <event.h>
#include <lib/string.h>

#define BUF_SIZE 1024

void halt() {
  while (1) __asm__ volatile("hlt");
}

void ls(EFI_SYSTEM_TABLE *SystemTable) {
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
  EFI_GUID guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  SystemTable->BootServices->LocateProtocol(&guid, NULL, (void **)&fs);
  EFI_FILE_PROTOCOL *root;
  EFI_STATUS status = fs->OpenVolume(fs, &root);
  if (status != EFI_SUCCESS) {
    return;
  }

  UINTN index = 0;
  EFI_FILE_INFO *file = NULL;
  while(1) {
    INTN buf_size = BUF_SIZE;
    status = root->Read(root, &buf_size, file);
    if (status != EFI_SUCCESS) {
      SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Read error!\r\n");
      break;
    }
    if (!buf_size) {
      break;
    }
    printf(L"index: %d, bufsize: %d\r\n", index, buf_size);
    printf(L"name: %s\r\n", file->FileName);
    //printf(L"size: %d\r\n", file->Size);
    //printf(L"physicalSize: %d\r\n", file->PhysicalSize);
    index++;
  }
  if (index == 0) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Not Found\r\n");
  } else {
    printf(L"found %e files", index);
  }
  root->Close(root);
}

//EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
EFI_STATUS efi_main(void *ImageHandle __attribute__ ((unused)), EFI_SYSTEM_TABLE *SystemTable) {
  init_event(SystemTable->BootServices);
  init_console(SystemTable->ConIn, SystemTable->ConOut);
  printf( L"loading kernel...\r\n");
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
    } else if (status != EFI_SUCCESS) {
    }
  } while(status != EFI_SUCCESS);

  if (status != EFI_SUCCESS) {
    halt();
  }

  //printf(L"[info] MapKey: %d, MemoryMapSize: %d, vadd: %d, padd: %d\r\n", MapKey, MemoryMapSize, MemoryMap.VirtualStart, MemoryMap.PhysicalStart);

  //__asm__ volatile("jmp *%0"::"r"((UINTN)0x0000000000001000));

  //status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

  ls(SystemTable);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"jump to kernel...\r\n");
  halt();

  //printf(L"Start Shell\r\n");

  //start_shell();

  return EFI_SUCCESS;
}
