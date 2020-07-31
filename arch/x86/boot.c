#include <uefi.h>
#include <elf64.h>
#include <arch/string.h>

#define PAGE_SIZE 4096
#define KERN_ADDR  0x100000
#define KERN_PATH L"\\vmmyos"
#define UINT64_MAX  0xffffffffffffffff

EFI_SYSTEM_TABLE *gST;
EFI_BOOT_SERVICES *gBS;
EFI_RUNTIME_SERVICES *gRT;

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

BootInfo *boot_info;

void halt() {
  while (1) __asm__ volatile("hlt");
}

void printf(CHAR16 *format, ...) {
  CHAR16 str[256];
  va_list list;
  va_start(list, format);
  format_str(str, format, &list);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->OutputString(gST->ConOut, str);
  va_end(list);
}

EFI_STATUS load_kernel(VOID *ElfImage, VOID **entryPoint) {
  Elf64_Ehdr *ElfHdr = (Elf64_Ehdr *)ElfImage;
  UINT8  magic[4] = {0x7f, 0x45, 0x4c, 0x46};

  UINTN  i;
  for(i = 0; i < 4; i++){
    if(ElfHdr->e_ident[i] != magic[i]){
      printf(L"invalid elf header: expect: %x, but: %x\r\n", magic[i], ElfHdr->e_ident[i]);
      return EFI_INVALID_PARAMETER;
    }
  }

  UINT8 *ProgramHdr = (UINT8 *)ElfImage + ElfHdr->e_phoff;
  for(i = 0; i < ElfHdr->e_phnum; i++){
    Elf64_Phdr *ProgramHdrPtr = (Elf64_Phdr *)ProgramHdr;

    if(ProgramHdrPtr->p_type == PT_LOAD){
      VOID  *FileSegment;
      VOID  *MemSegment;
      VOID  *ExtraZeroes;
      UINTN  ExtraZeroesCount;

      MemSegment = (VOID *)ProgramHdrPtr->p_vaddr;
      FileSegment = (VOID *)((UINTN)ElfImage + ProgramHdrPtr->p_offset);
      // カーネルサイズが14.4KB以上になった場合に
      // -fPIEオプションを外すとCopyMemでフリーズする
      gBS->CopyMem(MemSegment, FileSegment, ProgramHdrPtr->p_filesz);

      ExtraZeroes = (UINT8 *)MemSegment + ProgramHdrPtr->p_filesz;
      ExtraZeroesCount = ProgramHdrPtr->p_memsz - ProgramHdrPtr->p_filesz;
      if(ExtraZeroesCount > 0){
        gBS->SetMem(ExtraZeroes, 0x00, ExtraZeroesCount);
      }
    }

    ProgramHdr += ElfHdr->e_phentsize;
  }

  *entryPoint = (VOID *)ElfHdr->e_entry;

  return EFI_SUCCESS;
}


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  gST = SystemTable;
  gBS = SystemTable->BootServices;
  gRT = SystemTable->RuntimeServices;

  gST->ConOut->ClearScreen(gST->ConOut);
  printf( L"[OK] loading kernel...\r\n");

  EFI_GUID fs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

  EFI_STATUS status = gST->BootServices->LocateProtocol(&fs_guid, NULL, (void **)&fs);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to handle protocol fs: %d\r\n", status);
    return status;
  }
  status = gST->BootServices->LocateProtocol(&gop_guid, NULL, (void **)&gop);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to gop: %d\r\n", status);
    return status;
  }
  if (!gop) {
    printf(L"[ERROR] failed locate graphic\r\n");
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

  UINTN kernel_size = UINT64_MAX;
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
  status = handle->SetPosition(handle, 0);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to set position zero: %d\r\n", status);
    halt();
  }

  printf(L"[OK] kernel size : %d\r\n", kernel_size);

  EFI_PHYSICAL_ADDRESS buffer = (EFI_PHYSICAL_ADDRESS)KERN_ADDR;
  status = gBS->AllocatePages(AllocateAddress, EfiLoaderData, ((kernel_size + PAGE_SIZE) / PAGE_SIZE), &buffer);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to allocate: %d\r\n", status);
    halt();
  }

  status = handle->Read(handle, &kernel_size, (VOID *)buffer);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to load kernel to memory: %d\r\n", status);
    halt();
  }

  status = handle->Close(handle);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to close handle: %d\r\n", status);
    halt();
  }

  status = root->Close(root);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to close root: %d\r\n", status);
    halt();
  }


  VOID *kernel_main;
  status = load_kernel((VOID *)buffer, &kernel_main);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to load kernel_main: %d\r\n", status);
    halt();
  }

  printf( L"[OK] load kernel: size=>%d, addr=>%x\r\n", kernel_size, kernel_main);

  EFI_TIME *Time;
  status = gBS->AllocatePool(EfiBootServicesData, sizeof(EFI_TIME), (VOID **)&Time);
  if (status != EFI_SUCCESS) {
    return status;
  }
  status = gRT->GetTime(Time, NULL);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to get time: %d\r\n", status);
    return status;
  }
  printf(L"[OK] Now => %d-%d-%d %d:%d:%d\r\n", Time->Year, Time->Month, Time->Day, Time->Hour, Time->Minute, Time->Second);
  printf(L"[OK] kernel has been loaded successfully.\r\n");

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

  boot_info->system = gST;
  boot_info->framebuffer = (VOID *)gop->Mode->FrameBufferBase;
  boot_info->width = gop->Mode->Info->HorizontalResolution;
  boot_info->height = gop->Mode->Info->VerticalResolution;

  __asm__ volatile (
      "mov %0, %%rdi\n"
      "jmp *%1\n" ::
      "m" (boot_info),
      "m" (kernel_main)
  );

  return EFI_SUCCESS;
}
