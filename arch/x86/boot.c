#include <uefi.h>
#include <console.h>
#include <event.h>
#include <lib/string.h>

#define BUF_SIZE 1024

//#define KERN_ADDR (UINTN)0x0000000000001000
#define KERN_ADDR (UINTN)0x0000000000002000
#define STACK_BASE (UINTN)0x0000000000010000
#define KERN_PATH L"kernel.bin"

EFI_SYSTEM_TABLE *ST;

void halt() {
  while (1) __asm__ volatile("hlt");
}

UINTN get_kernel_size(EFI_FILE_PROTOCOL *root) {
  EFI_FILE_INFO *file = NULL;
  while(1) {
    INTN buf_size = BUF_SIZE;
    EFI_STATUS status = root->Read(root, &buf_size, file);
    if (status != EFI_SUCCESS) {
      break;
    }
    if (!buf_size) {
      break;
    }
    if (strcmp(KERN_PATH, file->FileName) == 0) {
      return file->FileSize;
    }
  }
  return 0;
}

EFI_STATUS load_kernel(EFI_FILE_PROTOCOL *root, void *addr, INTN file_size) {
  UINTN buf_size = 0;
  UINTN remain = file_size;
  EFI_STATUS status;

  while(remain > 0) {
    buf_size = remain > BUF_SIZE ? BUF_SIZE : remain;
    status = root->Read(root, &buf_size, addr);

    if (status != EFI_SUCCESS) {
      return status;
    }

    if (remain > BUF_SIZE) {
      addr += buf_size;
    }
    remain -= buf_size;
  }
  return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  ST = SystemTable;
  // init_event(SystemTable->BootServices);
  init_console(ST->ConIn, ST->ConOut);
  printf( L"loading kernel...\r\n");

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
  EFI_SIMPLE_POINTER_PROTOCOL *pointer;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *input;
  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *dptt;
  EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *dpft;
  EFI_DEVICE_PATH_UTILITIES_PROTOCOL *dpu;

  EFI_GUID fs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GUID pointer_guid = EFI_SIMPLE_POINTER_PROTOCOL_GUID;
  EFI_GUID input_guid = EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID;
  EFI_GUID dptt_guid = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
  EFI_GUID dpft_guid = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;
  EFI_GUID dpu_guid = EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;

  EFI_STATUS status = ST->BootServices->LocateProtocol(&fs_guid, NULL, (void **)&fs);
  if (status != EFI_SUCCESS) {
    return status;
  }
  status = ST->BootServices->LocateProtocol(&gop_guid, NULL, (void **)&gop);
  if (status != EFI_SUCCESS) {
    return status;
  }
  if (!gop) {
    printf(L"failed locate graphic\r\n");
  } else {
    printf(L"with: %d, height: %d\r\n", gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution);
  }
  status += ST->BootServices->LocateProtocol(&pointer_guid, NULL, (void **)&pointer);
  status += ST->BootServices->LocateProtocol(&input_guid, NULL, (void **)&input);
  status += ST->BootServices->LocateProtocol(&dptt_guid, NULL, (void **)&dptt);
  status += ST->BootServices->LocateProtocol(&dpft_guid, NULL, (void **)&dpft);
  status += ST->BootServices->LocateProtocol(&dpu_guid, NULL, (void **)&dpu);

  if (status != EFI_SUCCESS) {
    printf(L"failed locate protocol\r\n");
  }

  ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);

  EFI_FILE_PROTOCOL *root;
  status = fs->OpenVolume(fs, &root);
  if (status != EFI_SUCCESS) {
    return status;
  }

  INTN kernel_size = get_kernel_size(root);
  if (kernel_size == 0) {
    printf( L"Not found kernel\r\n");
    halt();
  }

  EFI_FILE_PROTOCOL *kernel;
  status = root->Open(root, &kernel, KERN_PATH, EFI_FILE_MODE_READ, 0);
  if (status != EFI_SUCCESS) {
    printf( L"failed to open kernel\r\n");
    halt();
    return status;
  }

  struct _header {
    void *bss_start;
    UINTN bss_size;
  } header;

  printf( L"start load head: %d\r\n", sizeof(header));
  UINTN header_size = sizeof(header);

  status = load_kernel(kernel, (void *)&header, header_size);
  if (status != EFI_SUCCESS) {
    printf(L"failed to load kernel head: %d\r\n", status);
    return status;
  }

  printf( L"done load head, addr: %x, size: %d\r\n", header.bss_start, header_size);

  status = load_kernel(kernel, (void *)KERN_ADDR, kernel_size - header_size);
  if (status != EFI_SUCCESS) {
    printf(L"failed to load kernel: %d\r\n", status);
    return status;
  }
  printf( L"done load form kern addr\r\n");

  kernel->Close(kernel);
  //root->Close(root);

  ST->BootServices->SetMem(header.bss_start, header.bss_size, 0);

  printf(L"done load kernel\r\n");

  /*
  UINTN MemoryMapSize = 0;
  EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
  UINTN MapKey;
  UINTN DescriptorSize;
  UINT32 DescriptorVersion;

  status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, (EFI_MEMORY_DESCRIPTOR *)MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (status != EFI_BUFFER_TOO_SMALL) {
    return status;
  }
  MemoryMapSize += 1024 * DescriptorSize;
  status = SystemTable->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, (void **)&MemoryMap);
  if (status != EFI_SUCCESS) {
    return status;
  }
  status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, (EFI_MEMORY_DESCRIPTOR *)MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (status != EFI_SUCCESS) {
    return status;
  }
  status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
  */

  EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
  UINTN MemoryMapSize = 0;
  UINTN MapKey;
  UINTN DescriptorSize;
  UINT32 DescriptorVersion;

  do {
    status =  ST->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    while (status == EFI_BUFFER_TOO_SMALL) {
      if (MemoryMap) {
        ST->BootServices->FreePool(MemoryMap);
      }
      ST->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, (void **)&MemoryMap);
      status =  ST->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    }
  } while(status != EFI_SUCCESS);

  return ST->BootServices->ExitBootServices(ImageHandle, MapKey);

  if (status != EFI_SUCCESS) {
    printf(L"failed to exit boot services\r\n");
    return status;
  }

  __asm__ volatile("mov %0, %%rsp\n"
                   "jmp *%1\n"::"r"(STACK_BASE), "r"(KERN_ADDR));

  return EFI_SUCCESS;
}
