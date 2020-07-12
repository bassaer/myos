#include <uefi.h>
#include <console.h>
#include <event.h>
#include <lib/string.h>
#include <elf64.h>

#define BUF_SIZE 4*1024
#define PAGE_SIZE 4*1024

#define KERN_ADDR  0x0000000000002000
#define STACK_BASE 0x0000000000100000
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

/*
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
*/

EFI_STATUS load_kernel(VOID *ElfImage, VOID **EntryPoint) {
  Elf64_Ehdr *ElfHdr;
  UINT8 *ProgramHdr;
  Elf64_Phdr *ProgramHdrPtr;
  UINTN  Index;
  UINT8    IdentMagic[4] = {0x7f, 0x45, 0x4c, 0x46};

  ElfHdr = (Elf64_Ehdr *)ElfImage;
  ProgramHdr = (UINT8 *)ElfImage + ElfHdr->e_phoff;

  for(Index=0; Index<4; Index++){
    if(ElfHdr->e_ident[Index] != IdentMagic[Index]){
      printf(L"invalid paramater\r\n", IdentMagic[Index], ElfHdr->e_ident[Index]);
      return EFI_INVALID_PARAMETER;
    }
  }

  // Load every loadable ELF segment into memory
  for(Index = 0; Index < ElfHdr->e_phnum; Index++){
    ProgramHdrPtr = (Elf64_Phdr *)ProgramHdr;

    // Only consider PT_LOAD type segments
    if(ProgramHdrPtr->p_type == PT_LOAD){
      VOID  *FileSegment;
      VOID  *MemSegment;
      VOID  *ExtraZeroes;
      UINTN  ExtraZeroesCount;

      // Load the segment in memory
      FileSegment = (VOID *)((UINTN)ElfImage + ProgramHdrPtr->p_offset);
      MemSegment = (VOID *)ProgramHdrPtr->p_vaddr;
      gBS->CopyMem(MemSegment, FileSegment, ProgramHdrPtr->p_filesz);

      // Fill memory with zero for .bss section and ...
      ExtraZeroes = (UINT8 *)MemSegment + ProgramHdrPtr->p_filesz;
      ExtraZeroesCount = ProgramHdrPtr->p_memsz - ProgramHdrPtr->p_filesz;
      if(ExtraZeroesCount > 0){
        gBS->SetMem(ExtraZeroes, 0x00, ExtraZeroesCount);
      }
    }

    // Get next program header
    ProgramHdr += ElfHdr->e_phentsize;
  }

  *EntryPoint = (VOID *)ElfHdr->e_entry;

  return EFI_SUCCESS;
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
  printf(L"kernel_size = %d\r\n", kernel_size);


  UINT8 *buffer = (VOID *)KERN_ADDR;
  status = gST->BootServices->AllocatePool(EfiLoaderData, kernel_size, (VOID **)&buffer);
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

  VOID *start_kernel = NULL;
  status = load_kernel(buffer, &start_kernel);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to load start_kernel: %d\r\n", status);
    halt();
  }

  printf( L"[OK] load kernel: size=>%d, addr=>%x\r\n", kernel_size, &start_kernel);

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

  status = gBS->ExitBootServices(ImageHandle, MapKey);
  if (status != EFI_SUCCESS) {
    printf(L"[ERROR] failed to ExitBootServices\r\n");
    halt();
  }

  boot_info->system = gST;
  boot_info->framebuffer = gop->Mode->FrameBufferBase;
  boot_info->width = gop->Mode->Info->HorizontalResolution;
  boot_info->height = gop->Mode->Info->VerticalResolution;

  UINTN kern_addr = KERN_ADDR;
  UINTN stack_base = STACK_BASE;
  UINTN boot_info_addr = (UINTN)&boot_info;

  /*
  __asm__ volatile ("mov %0, %%rdi\n"
                    "mov %1, %%rsp\n"
                    "jmp *%2\n"::
                    "m"(boot_info_addr),
                    "m"(stack_base),
                    "m"(kern_addr));

  __asm__ volatile ("mov %0, %%rdi\n"
                    "mov %1, %%rsp\n"::
                    "m"(boot_info_addr),
                    "m"(stack_base));
                    */

  halt();

  return EFI_SUCCESS;
}
