#include <uefi.h>
#include <console.h>
#include <event.h>
#include <lib/string.h>

#define BUF_SIZE 1024

void halt() {
  while (1) __asm__ volatile("hlt");
}

void ls(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs) {
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
      break;
    }
    if (!buf_size) {
      break;
    }
    printf(L"index: %d, bufsize: %d\r\n", index, buf_size);
    printf(L"name: %s\r\n", file->FileName);
    index++;
  }
  root->Close(root);
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  init_event(SystemTable->BootServices);
  init_console(SystemTable->ConIn, SystemTable->ConOut);
  printf( L"loading kernel...\r\n");

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
  EFI_GUID guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  SystemTable->BootServices->LocateProtocol(&guid, NULL, (void **)&fs);
  ls(fs);

  UINTN MemoryMapSize = 0;
  EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
  UINTN MapKey;
  UINTN DescriptorSize;
  UINT32 DescriptorVersion;

  EFI_STATUS status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, (EFI_MEMORY_DESCRIPTOR *)MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (status != EFI_BUFFER_TOO_SMALL) {
    return status;
  }
  MemoryMapSize += 2 * DescriptorSize;
  status = SystemTable->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, (void **)&MemoryMap);
  if (status != EFI_SUCCESS) {
    return status;
  }
  status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, (EFI_MEMORY_DESCRIPTOR *)MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (status != EFI_SUCCESS) {
    return status;
  }

  status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
  if (status != EFI_SUCCESS) {
    return status;
  }
  //__asm__ volatile("jmp *%0"::"r"((UINTN)0x0000000000001000));

  halt();

  return EFI_SUCCESS;
}
