#ifndef MYOS_UEFI_H
#define MYOS_UEFI_H

#include <type.h>
#include <err.h>
#include <guid.h>

#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

/**
 * UEFI SPECIFICATIONS 2.8 Errata A, February 2020
 * https://uefi.org/specifications
 */


typedef UINTN EFI_STATUS;
typedef UINTN EFI_TPL;
typedef void *EFI_HANDLE;
typedef void *EFI_EVENT;
typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

typedef struct {
  UINT16 Year; // 1900 – 9999
  UINT8 Month; // 1 – 12
  UINT8 Day; // 1 – 31
  UINT8 Hour; // 0 – 23
  UINT8 Minute; // 0 – 59
  UINT8 Second; // 0 – 59
  UINT8 Pad1;
  UINT32 Nanosecond; // 0 – 999,999,999
  INT16 TimeZone; // -1440 to 1440 or 2047
  UINT8 Daylight;
  UINT8 Pad2;
} EFI_TIME;

typedef enum {
  NULL_SCAN = 0x00,
  CURSOR_UP,
  CURSOR_DOWN,
  CURSOR_RIGHT,
  CURSOR_LEFT,
  HOME,
  END,
  INSERT,
  DELETE,
  PAGE_UP,
  PAGE_DOWN
} SPECIAL_KEY;

typedef struct {
  UINT16 ScanCode;
  CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef struct {
  UINT32 Data1;
  UINT16 Data2;
  UINT16 Data3;
  UINT8 Data4[8];
} EFI_GUID;

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

typedef struct {
 INT32 MaxMode;
 INT32 Mode;
 INT32 Attribute;
 INT32 CursorColumn;
 INT32 CursorRow;
 BOOLEAN CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  EFI_STATUS Reset;
  EFI_STATUS (*OutputString)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, unsigned short *String);
  EFI_STATUS TestString;
  EFI_STATUS (*QueryMode)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber, UINTN *Columns, UINTN *Rows);
  EFI_STATUS SetMode;
  EFI_STATUS (*SetAttribute)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Attribute);
  EFI_STATUS (*ClearScreen)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
  EFI_STATUS (*SetCursorPosition)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Column, UINTN Row);
  EFI_STATUS (*EnableCursor)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, BOOLEAN Visible);
  SIMPLE_TEXT_OUTPUT_MODE *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef enum {
  PixelRedGreenBlueReserved8BitPerColor,
  PixelBlueGreenRedReserved8BitPerColor,
  PixelBitMask,
  PixelBltOnly,
  PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
  UINT32 Version;
  UINT32 HorizontalResolution;
  UINT32 VerticalResolution;
  EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
  INTN PixelInformation;
  UINT32 PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
  UINT32 MaxMode;
  UINT32 Mode;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
  UINTN SizeOfInfo;
  EFI_PHYSICAL_ADDRESS FrameBufferBase;
  UINTN FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct EFI_GRAPHICS_OUTPUT_PROTCOL {
  EFI_STATUS QueryMode;
  EFI_STATUS SetMode;
  EFI_STATUS Blt;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef enum {
  EfiResetCold,
  EfiResetWarm,
  EfiResetShutdown,
  EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef struct {
  EFI_TABLE_HEADER Hdr;
  EFI_STATUS GetTime;
  EFI_STATUS SetTime;
  EFI_STATUS GetWakeupTime;
  EFI_STATUS SetWakeupTime;
  EFI_STATUS SetVirtualAddressMap;
  EFI_STATUS ConvertPointer;
  EFI_STATUS GetVariable;
  EFI_STATUS GetNextVariableName;
  EFI_STATUS SetVariable;
  EFI_STATUS GetNextHighMonotonicCount;
  void (*ResetSystem)(EFI_RESET_TYPE ResetType, EFI_STATUS ResetStatus, UINTN DataSize, void *ResetData);
  EFI_STATUS UpdateCapsule;
  EFI_STATUS QueryCapsuleCapabilities;
  EFI_STATUS QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

typedef enum {
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef struct {
  UINT32 Type;
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  EFI_VIRTUAL_ADDRESS VirtualStart;
  UINT64 NumberOfPages;
  UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef struct {
  EFI_TABLE_HEADER Hdr;
  EFI_TPL RaiseTPL;
  void *RestoreTPL;
  EFI_STATUS AllocatePages;
  EFI_STATUS FreePages;
  EFI_STATUS (*GetMemoryMap)(UINTN *MemoryMapSize, EFI_MEMORY_DESCRIPTOR *MemoryMap, UINTN *MapKey, UINTN *DescriptorSize, UINT32 *DescriptorVersion);
  EFI_STATUS (*AllocatePool)(EFI_MEMORY_TYPE PoolType, UINTN size, void **Buffer);
  EFI_STATUS (*FreePool)(void *Buffer);
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
  EFI_STATUS (*ExitBootServices)(EFI_HANDLE ImageHandle, UINTN MapKey);
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
  EFI_STATUS (* LocateProtocol)(EFI_GUID *Protocol, void *Registration, void **Interface);
  EFI_STATUS InstallMultipleProtocolInterfaces;
  EFI_STATUS UninstallMultipleProtocolInterfaces;
  EFI_STATUS CalculateCrc32;
  void *CopyMem;
  void* (*SetMem)(void *Buffer, UINTN Size, UINT8 Value);
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

typedef struct EFI_FILE_PROTOCOL {
  UINT64 Revision;
  EFI_STATUS (* Open)(struct EFI_FILE_PROTOCOL *This, struct EFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, UINT64 OpenMode, UINT64 Attributes);
  EFI_STATUS (* Close)(struct  EFI_FILE_PROTOCOL *This);
  EFI_STATUS Delete;
  EFI_STATUS (* Read)(struct EFI_FILE_PROTOCOL *This, UINTN *BufferSize, void *Buffer);
  EFI_STATUS Write;
  EFI_STATUS GetPosition;
  EFI_STATUS SetPosition;
  EFI_STATUS GetInfo;
  EFI_STATUS SetInfo;
  EFI_STATUS Flush;
  EFI_STATUS OpenEx; // Added for revision 2
  EFI_STATUS ReadEx; // Added for revision 2
  EFI_STATUS WriteEx; // Added for revision 2
  EFI_STATUS FlushEx; // Added for revision 2
} EFI_FILE_PROTOCOL;

typedef struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
  UINT64 Revision;
  EFI_STATUS (* OpenVolume)(struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This, EFI_FILE_PROTOCOL **Root);
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef struct {
  UINT64 Size;
  UINT64 FileSize;
  UINT64 PhysicalSize;
  EFI_TIME CreateTime;
  EFI_TIME LastAccessTime;
  EFI_TIME ModificationTime;
  UINT64 Attribute;
  CHAR16 FileName[];
} EFI_FILE_INFO;


// TODO : fix

typedef struct {
  unsigned char Type;
  unsigned char SubType;
  unsigned char Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

typedef struct {
  unsigned int KeyShiftState;
  unsigned char KeyToggleState;
} EFI_KEY_STATE;

typedef struct {
  EFI_INPUT_KEY Key;
  EFI_KEY_STATE KeyState;
} EFI_KEY_DATA;

typedef struct {
  int RelativeMovementX;
  int RelativeMovementY;
  int RelativeMovementZ;
  unsigned char LeftButton;
  unsigned char RightButton;
} EFI_SIMPLE_POINTER_STATE;


typedef struct EFI_SIMPLE_POINTER_PROTOCOL {
  unsigned long long (*Reset)(
    struct EFI_SIMPLE_POINTER_PROTOCOL *This,
    unsigned char ExtendedVerification);
  unsigned long long (*GetState)(
    struct EFI_SIMPLE_POINTER_PROTOCOL *This,
    EFI_SIMPLE_POINTER_STATE *State);
  void *WaitForInput;
} EFI_SIMPLE_POINTER_PROTOCOL;

typedef struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL {
  unsigned long long (*Reset)(
    struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    unsigned char ExtendedVerification);
  unsigned long long (*ReadKeyStrokeEx)(
    struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    EFI_KEY_DATA *KeyData);
  void *WaitForKeyEx;
  unsigned long long (*SetState)(
    struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    unsigned char *KeyToggleState);
  unsigned long long (*RegisterKeyNotify)(
    struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    EFI_KEY_DATA *KeyData,
    unsigned long long (*KeyNotificationFunction)(
      EFI_KEY_DATA *KeyData),
    void **NotifyHandle);
  unsigned long long (*UnregisterKeyNotify)(
    struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    void *NotificationHandle);
} EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;

typedef struct {
  unsigned long long _buf;
  unsigned short *(*ConvertDevicePathToText)(
    const EFI_DEVICE_PATH_PROTOCOL* DeviceNode,
    unsigned char DisplayOnly,
    unsigned char AllowShortcuts);
} EFI_DEVICE_PATH_TO_TEXT_PROTOCOL;

typedef struct {
  EFI_DEVICE_PATH_PROTOCOL *(*ConvertTextToDeviceNode) (
    const unsigned short *TextDeviceNode);
  EFI_DEVICE_PATH_PROTOCOL *(*ConvertTextToDevicePath) (
    const unsigned short *TextDevicePath);
} EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL;

typedef struct {
  unsigned long long _buf[3];
  EFI_DEVICE_PATH_PROTOCOL *(*AppendDeviceNode)(
    const EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    const EFI_DEVICE_PATH_PROTOCOL *DeviceNode);
} EFI_DEVICE_PATH_UTILITIES_PROTOCOL;


#endif
