#include <uefi.h>
#include <console.h>
#include <event.h>
#include <lib/string.h>

#define BUF_SIZE 4*1024
#define PAGE_SIZE 4*1024

#define KERN_ADDR 0x0000000000002000
#define STACK_BASE 0x0000000000010000
#define KERN_PATH L"\\vmmyos"

#define UINT64_MAX  0xffffffffffffffff

EFI_SYSTEM_TABLE *gST;
EFI_BOOT_SERVICES *gBS;
EFI_RUNTIME_SERVICES *gRT;

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

void halt() {
  while (1) __asm__ volatile("hlt");
}

void start_kernel() {
  INTN x, y;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pixel;
  BOOLEAN reverse = false;
  for (y = 0; y < gop->Mode->Info->VerticalResolution; y++) {
    for (x = 0; x < gop->Mode->Info->HorizontalResolution; x++) {
      pixel = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)gop->Mode->FrameBufferBase + (gop->Mode->Info->HorizontalResolution * y) + x;
      if (x % 100 == 0) {
        reverse = !reverse;
      }
      if (reverse) {
        pixel->Red = 0;
        pixel->Green = 229;
        pixel->Blue = 255;
      } else {
        pixel->Red = 0;
        pixel->Green = 184;
        pixel->Blue = 212;
      }
    }
  }
}


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  gST = SystemTable;
  gBS = SystemTable->BootServices;
  gRT = SystemTable->RuntimeServices;

  gST->ConOut->ClearScreen(gST->ConOut);
  // init_event(SystemTable->BootServices);
  init_console(gST->ConIn, gST->ConOut);
  printf( L"[OK] loading kernel...\r\n");

  EFI_GUID fs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

  EFI_STATUS status = gST->BootServices->LocateProtocol(&fs_guid, NULL, (void **)&fs);
  if (status != EFI_SUCCESS) {
    return status;
  }
  status = gST->BootServices->LocateProtocol(&gop_guid, NULL, (void **)&gop);
  if (status != EFI_SUCCESS) {
    return status;
  }
  if (!gop) {
    printf(L"[ERROR] failed locate graphic\r\n");
  } else {
    printf(L"[OK] with: %d, height: %d\r\n", gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution);
  }

  gST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);

  EFI_FILE_PROTOCOL *root;
  status = fs->OpenVolume(fs, &root);
  if (status != EFI_SUCCESS) {
    return status;
  }

  EFI_FILE_PROTOCOL *kernel;
  EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  EFI_LOADED_IMAGE_PROTOCOL* lip;
  status = gBS->HandleProtocol(ImageHandle, &lip_guid, (VOID **)&lip);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to handle protocol lip: %d\r\n", status);
    halt();
  };
  status = gBS->HandleProtocol(lip->DeviceHandle, &fs_guid, (VOID **)&fs);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to handle protocol fs: %d\r\n", status);
    halt();
  };
  status = fs->OpenVolume(fs, &kernel);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to open volume: %d\r\n", status);
    halt();
  };

  EFI_FILE_PROTOCOL *handle = NULL;
  status = kernel->Open(kernel, &handle, KERN_PATH, EFI_FILE_MODE_READ, 0);
  if (status != EFI_SUCCESS) {
    halt();
  };
  UINT64 kernel_size = UINT64_MAX;
  status = handle->SetPosition(handle, kernel_size);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to set position: %d\r\n", status);
    halt();
  }
  status = handle->GetPosition(handle, &kernel_size);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to get position: %d\r\n", status);
    halt();
  }

  VOID *buffer = NULL;
  status = gBS->AllocatePool(EfiLoaderData, kernel_size, &buffer);
  if (status != EFI_SUCCESS || !buffer) {
    printf(L"[ERROR] failed to allocate kernel buffer: %d\r\n", status);
    halt();
  }

  UINTN buffer_size = kernel_size;
  status = handle->Read(handle, &buffer_size, buffer);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to load kernel to memory: %d\r\n", status);
    halt();
  }
  status = handle->Close(handle);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to close handle: %d\r\n", status);
    halt();
  }

  printf( L"[OK] load kernel: size=>%d, addr=>%x\r\n", kernel_size, buffer);

  EFI_TIME *Time;
  status = gBS->AllocatePool(EfiBootServicesData, sizeof(EFI_TIME), (VOID **)&Time);
  if (status != EFI_SUCCESS) {
    return status;
  }
  status = gRT->GetTime(Time, NULL);
  if (status != EFI_SUCCESS) {
    return status;
  }
  printf(L"[OK] Now => %d-%d-%d %d:%d:%d\r\n", Time->Year, Time->Month, Time->Day, Time->Hour, Time->Minute, Time->Second);

  gST->ConOut->ClearScreen(gST->ConOut);

  UINTN MemoryMapSize = 0;
  EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
  UINTN MapKey;
  UINTN DescriptorSize;
  UINT32 DescriptorVersion;

  status = gBS->GetMemoryMap(&MemoryMapSize, (EFI_MEMORY_DESCRIPTOR *)MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (status != EFI_BUFFER_TOO_SMALL) {
    return status;
  }
  MemoryMapSize += 1024 * DescriptorSize;
  status = gBS->AllocatePool(EfiLoaderData, MemoryMapSize, (void **)&MemoryMap);
  if (status != EFI_SUCCESS) {
    return status;
  }
  status = gBS->GetMemoryMap(&MemoryMapSize, (EFI_MEMORY_DESCRIPTOR *)MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (status != EFI_SUCCESS) {
    return status;
  }

  status = gBS->ExitBootServices(ImageHandle, MapKey);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to ExitBootServices\r\n");
    halt();
  }


  /*
  __asm__ volatile("mov %0, %%rsp\n"
                   "jmp *%1\n"::"r"(STACK_BASE), "r"(KERN_ADDR));
                   */
  start_kernel();
  halt();

  return EFI_SUCCESS;
}
