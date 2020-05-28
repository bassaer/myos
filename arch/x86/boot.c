#include <uefi.h>
#include <console.h>
#include <event.h>
#include <lib/string.h>

#define BUF_SIZE 4*1024
#define PAGE_SIZE 4*1024

//#define KERN_ADDR (UINTN)0x0000000000001000
//#define KERN_ADDR (UINTN)0x0000000000002000
#define KERN_ADDR 0x1000
#define STACK_BASE (UINTN)0x0000000000010000
#define KERN_PATH L"kernel.bin"

EFI_SYSTEM_TABLE *gST;
EFI_BOOT_SERVICES *gBS;
EFI_RUNTIME_SERVICES *gRT;

void halt() {
  while (1) __asm__ volatile("hlt");
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  gST = SystemTable;
  gBS = SystemTable->BootServices;
  gRT = SystemTable->RuntimeServices;

  gST->ConOut->ClearScreen(gST->ConOut);
  // init_event(SystemTable->BootServices);
  init_console(gST->ConIn, gST->ConOut);
  printf( L"[OK] loading kernel...\r\n");

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
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
    printf(L"failed locate graphic\r\n");
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
  status = root->Open(root, &kernel, KERN_PATH, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
  if (status != EFI_SUCCESS) {
    printf(L"faild to open kernel: %d\r\n", status);
    halt();
  }
  UINTN buf_size = BUF_SIZE;
  EFI_PHYSICAL_ADDRESS buf = (EFI_PHYSICAL_ADDRESS)KERN_ADDR;
  status = gBS->AllocatePages(AllocateAddress, EfiLoaderData, buf_size / PAGE_SIZE, &buf);
  if (status != EFI_SUCCESS) {
    printf(L"faild to allocate page: %d\r\n", status);
    halt();
  }
  status = kernel->Read(kernel, &buf_size, (VOID *)buf);
  if (status != EFI_SUCCESS) {
    printf(L"faild to read kenel: %d\r\n", status);
    halt();
  }

  kernel->Close(kernel);
  root->Close(root);
  gBS->FreePages(buf, BUF_SIZE);

  printf( L"[OK] load kernel: size=>%d, addr=>%x\r\n", buf_size, &buf);

  //gBS->SetMem(header.bss_start, header.bss_size, 0);

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

  // printf(L"try ExitBootServices\r\n");
  status = gBS->ExitBootServices(ImageHandle, MapKey);
  if (status != EFI_SUCCESS) {
    printf(L"failed to ExitBootServices\r\n");
    while(1);
  }
  return status;


  if (status != EFI_SUCCESS) {
    printf(L"failed to exit boot services\r\n");
    return status;
  }

  __asm__ volatile("mov %0, %%rsp\n"
                   "jmp *%1\n"::"r"(STACK_BASE), "r"(KERN_ADDR));

  return EFI_SUCCESS;
}
