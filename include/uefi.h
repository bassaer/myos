#ifndef MYOS_UEFI_H
#define MYOS_UEFI_H

#include <type.h>

/**
 * UEFI SPECIFICATIONS 2.8 Errata A, February 2020
 * https://uefi.org/specifications
 */

typedef UINTN EFI_STATUS;
typedef UINTN EFI_TPL;
typedef void * EFI_HANDLE;
typedef void * EFI_EVENT;

typedef struct {
  UINT16 ScanCode;
  CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef struct {
  UINT64 Signature;
  UINT32 Revision;
  UINT32 HeaderSize;
  UINT32 CRC32;
  UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
  EFI_STATUS _buf;
  EFI_STATUS (*ReadKeyStroke)(struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, EFI_INPUT_KEY *Key);
  void *WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  EFI_STATUS Reset;
  EFI_STATUS (*OutputString)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, unsigned short *String);
  EFI_STATUS TestString;
  EFI_STATUS QueryMode;
  EFI_STATUS SetMode;
  EFI_STATUS (*SetAttribute)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Attribute);
  EFI_STATUS (*ClearScreen)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
  EFI_STATUS (*SetCursorPosition)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Column, UINTN Row);
  EFI_STATUS (*EnableCursor)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, BOOLEAN Visible);
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct {

} EFI_RUNTIME_SERVICES;

typedef struct {
  EFI_TABLE_HEADER Hdr;
  EFI_TPL RaiseTPL;
  void *RestoreTPL;
  EFI_STATUS AllocatePages;
  EFI_STATUS FreePages;
  EFI_STATUS GetMemoryMap;
  EFI_STATUS AllocatePool;
  EFI_STATUS FreePool;
  EFI_STATUS CreateEvent;
  EFI_STATUS SetTimer;
  EFI_STATUS (*WaitForEvent)(UINTN NumberOfEvents, EFI_EVENT *Event, UINTN *index);
  EFI_STATUS SignalEvent;
  EFI_STATUS CloseEvent;
  EFI_STATUS CheckEvent;
  EFI_STATUS InstallProtocolInterface;
  EFI_STATUS ReinstallProtocolInterface;
  EFI_STATUS UninstallProtocolInterface;
  EFI_STATUS HandleProtocol;
  void *Reserved;
  EFI_STATUS RegisterProtocolNotify;
  EFI_STATUS LocateHandle;
  EFI_STATUS LocateDevicePath;
  EFI_STATUS InstallConfigurationTable;
  EFI_STATUS LoadImage;
  EFI_STATUS StartImage;
  EFI_STATUS Exit;
  EFI_STATUS UnloadImage;
  EFI_STATUS ExitBootServices;
  EFI_STATUS GetNextMonotonicCount;
  EFI_STATUS Stall;
  EFI_STATUS (* SetWatchdogTimer)(UINTN Timeout, UINT64 WatchdogCode, UINTN DataSize, CHAR16 *WatchdogData);
  EFI_STATUS ConnectController;
  EFI_STATUS DisconnectController;
  EFI_STATUS OpenProtocol;
  EFI_STATUS CloseProtocol;
  EFI_STATUS OpenProtocolInformation;
  EFI_STATUS ProtocolsPerHandle;
  EFI_STATUS LocateHandleBuffer;
  EFI_STATUS LocateProtocol;
  EFI_STATUS InstallMultipleProtocolInterfaces;
  EFI_STATUS UninstallMultipleProtocolInterfaces;
  EFI_STATUS CalculateCrc32;
  void *CopyMem;
  void *SetMem;
  EFI_STATUS CreateEventEx;
} EFI_BOOT_SERVICES;

typedef struct {
  EFI_TABLE_HEADER _Hdr;
  CHAR16 *FirmwareVendor;
  UINT32 FirmwareRevision;
  EFI_HANDLE ConsoleInHandle;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
  EFI_STATUS ConsoleOutHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
  EFI_HANDLE StandardErrorHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
  EFI_RUNTIME_SERVICES *RuntimeServices;
  EFI_BOOT_SERVICES *BootServices;
} EFI_SYSTEM_TABLE;

#endif
