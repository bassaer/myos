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
typedef UINTN EFI_TPL;

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

typedef enum {
  AllocateAnyPages,
  AllocateMaxAddress,
  AllocateAddress,
  MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum {
  TimerCancel,
  TimerPeriodic,
  TimerRelative
} EFI_TIMER_DELAY;

typedef enum {
  EFI_NATIVE_INTERFACE
} EFI_INTERFACE_TYPE;

typedef enum {
  AllHandles,
  ByRegisterNotify,
  ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef struct {
  UINT32 Type;
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  EFI_VIRTUAL_ADDRESS VirtualStart;
  UINT64 NumberOfPages;
  UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef struct _EFI_DEVICE_PATH_PROTOCOL {
  UINT8 Type;
  UINT8 SubType;
  UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

typedef
EFI_DEVICE_PATH_PROTOCOL*
(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_NODE) (
  IN CONST CHAR16* TextDeviceNode
);

typedef
EFI_DEVICE_PATH_PROTOCOL*
(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_PATH) (
  IN CONST CHAR16* TextDevicePath
);

typedef struct _EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL {
  EFI_DEVICE_PATH_FROM_TEXT_NODE ConvertTextToDevicNode;
  EFI_DEVICE_PATH_FROM_TEXT_PATH ConvertTextToDevicPath;
} EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL;

typedef struct {
  EFI_HANDLE AgentHandle;
  EFI_HANDLE ControllerHandle;
  UINT32 Attributes;
  UINT32 OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef
EFI_TPL
(EFIAPI *EFI_RAISE_TPL) (
  IN EFI_TPL NewTpl
);

typedef
VOID
(EFIAPI *EFI_RESTORE_TPL) (
  IN EFI_TPL OldTpl
);

typedef
EFI_STATUS
(EFIAPI *EFI_ALLOCATE_PAGES) (
  IN EFI_ALLOCATE_TYPE Type,
  IN EFI_MEMORY_TYPE MemoryType,
  IN UINTN Pages,
  IN OUT EFI_PHYSICAL_ADDRESS *Memory
);

typedef
EFI_STATUS
(EFIAPI *EFI_FREE_PAGES) (
  IN EFI_PHYSICAL_ADDRESS Memory,
  IN UINTN Pages
);

typedef
EFI_STATUS
(EFIAPI *EFI_GET_MEMORY_MAP) (
  IN OUT UINTN *MemoryMapSize,
  OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
  OUT UINTN *MapKey,
  OUT UINTN *DescriptorSize,
  OUT UINT32 *DescriptorVersion
);

typedef
EFI_STATUS
(EFIAPI *EFI_ALLOCATE_POOL) (
  IN EFI_MEMORY_TYPE PoolType,
  IN UINTN Size,
  OUT VOID **Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_FREE_POOL) (
  IN VOID *Buffer
);

typedef
VOID
(EFIAPI *EFI_EVENT_NOTIFY) (
  IN EFI_EVENT Event,
  IN VOID *Context
);

typedef
EFI_STATUS
(EFIAPI *EFI_CREATE_EVENT) (
  IN UINT32 Type,
  IN EFI_TPL NotifyTpl,
  IN EFI_EVENT_NOTIFY NotifyFunction, OPTIONAL
  IN VOID *NotifyContext, OPTIONAL
  OUT EFI_EVENT *Event
);

typedef
EFI_STATUS
(EFIAPI *EFI_SET_TIMER) (
  IN EFI_EVENT Event,
  IN EFI_TIMER_DELAY Type,
  IN UINT64 TriggerTime
);

typedef
EFI_STATUS
(EFIAPI *EFI_WAIT_FOR_EVENT) (
  IN UINTN NumberOfEvents,
  IN EFI_EVENT *Event,
  OUT UINTN *Index
);

typedef
EFI_STATUS
(EFIAPI *EFI_SIGNAL_EVENT) (
  IN EFI_EVENT Event
);

typedef
EFI_STATUS
(EFIAPI *EFI_CLOSE_EVENT) (
   IN EFI_EVENT Event
);

typedef
EFI_STATUS
(EFIAPI *EFI_CHECK_EVENT) (
   IN EFI_EVENT Event
);

typedef
EFI_STATUS
(EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE) (
  IN OUT EFI_HANDLE *Handle,
  IN EFI_GUID *Protocol,
  IN EFI_INTERFACE_TYPE InterfaceType,
  IN VOID *Interface
);

typedef
EFI_STATUS
(EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE) (
  IN EFI_HANDLE Handle,
  IN EFI_GUID *Protocol,
  IN VOID *OldInterface,
  IN VOID *NewInterface
);

typedef
EFI_STATUS
(EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE) (
  IN EFI_HANDLE Handle,
  IN EFI_GUID *Protocol,
  IN VOID *Interface
);

typedef
EFI_STATUS
(EFIAPI *EFI_HANDLE_PROTOCOL) (
  IN EFI_HANDLE Handle,
  IN EFI_GUID *Protocol,
  OUT VOID **Interface
);

typedef
EFI_STATUS
(EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY) (
  IN EFI_GUID *Protocol,
  IN EFI_EVENT Event,
  OUT VOID **Registration
);

typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_HANDLE) (
  IN EFI_LOCATE_SEARCH_TYPE SearchType,
  IN EFI_GUID *Protocol OPTIONAL,
  IN VOID *SearchKey OPTIONAL,
  IN OUT UINTN *BufferSize,
  OUT EFI_HANDLE *Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_DEVICE_PATH) (
  IN EFI_GUID *Protocol,
  IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath,
  OUT EFI_HANDLE *Device
);

typedef
EFI_STATUS
(EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE) (
  IN EFI_GUID *Guid,
  IN VOID *Table
);

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_LOAD) (
  IN BOOLEAN BootPolicy,
  IN EFI_HANDLE ParentImageHandle,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN VOID *SourceBuffer OPTIONAL,
  IN UINTN SourceSize,
  OUT EFI_HANDLE *ImageHandle
);

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_START) (
  IN EFI_HANDLE ImageHandle,
  OUT UINTN *ExitDataSize,
  OUT CHAR16 **ExitData OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_EXIT) (
  IN EFI_HANDLE ImageHandle,
  IN EFI_STATUS ExitStatus,
  IN UINTN ExitDataSize,
  IN CHAR16 *ExitData OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_UNLOAD) (
  IN EFI_HANDLE ImageHandle
);

typedef
EFI_STATUS
(EFIAPI *EFI_EXIT_BOOT_SERVICES) (
  IN EFI_HANDLE ImageHandle,
  IN UINTN MapKey
);

typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT) (
  OUT UINT64 *Count
);

typedef
EFI_STATUS
(EFIAPI *EFI_STALL) (
  IN UINTN Microseconds
);

typedef
EFI_STATUS
(EFIAPI *EFI_SET_WATCHDOG_TIMER) (
  IN UINTN Timeout,
  IN UINT64 WatchdogCode,
  IN UINTN DataSize,
  IN CHAR16 *WatchdogData OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_CONNECT_CONTROLLER) (
  IN EFI_HANDLE ControllerHandle,
  IN EFI_HANDLE *DriverImageHandle OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL,
  IN BOOLEAN Recursive
);

typedef
EFI_STATUS
(EFIAPI *EFI_DISCONNECT_CONTROLLER) (
  IN EFI_HANDLE ControllerHandle,
  IN EFI_HANDLE DriverImageHandle OPTIONAL,
  IN EFI_HANDLE ChildHandle OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_OPEN_PROTOCOL) (
  IN EFI_HANDLE Handle,
  IN EFI_GUID *Protocol,
  OUT VOID **Interface OPTIONAL,
  IN EFI_HANDLE AgentHandle,
  IN EFI_HANDLE ControllerHandle,
  IN UINT32 Attributes
);

typedef
EFI_STATUS
(EFIAPI *EFI_CLOSE_PROTOCOL) (
  IN EFI_HANDLE Handle,
  IN EFI_GUID *Protocol,
  IN EFI_HANDLE AgentHandle,
  IN EFI_HANDLE ControllerHandle
);

typedef
EFI_STATUS
(EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION) (
  IN EFI_HANDLE Handle,
  IN EFI_GUID *Protocol,
  OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
  OUT UINTN *EntryCount
);

typedef
EFI_STATUS
(EFIAPI *EFI_PROTOCOLS_PER_HANDLE) (
  IN EFI_HANDLE Handle,
  OUT EFI_GUID ***ProtocolBuffer,
  OUT UINTN *ProtocolBufferCount
);

typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_HANDLE_BUFFER) (
  IN EFI_LOCATE_SEARCH_TYPE SearchType,
  IN EFI_GUID *Protocol OPTIONAL,
  IN VOID *SearchKey OPTIONAL,
  OUT UINTN *NoHandles,
  OUT EFI_HANDLE **Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_PROTOCOL) (
  IN EFI_GUID *Protocol,
  IN VOID *Registration OPTIONAL,
  OUT VOID **Interface
);

typedef
EFI_STATUS
(EFIAPI *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
  IN OUT EFI_HANDLE           *Handle,
  ...
);

typedef
EFI_STATUS
(EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
 IN EFI_HANDLE Handle,
 ...
);

typedef
EFI_STATUS
(EFIAPI *EFI_CALCULATE_CRC32) (
 IN VOID *Data,
 IN UINTN DataSize,
 OUT UINT32 *Crc32
);

typedef
VOID
(EFIAPI *EFI_COPY_MEM) (
 IN VOID *Destination,
 IN VOID *Source,
 IN UINTN Length
);

typedef
VOID
(EFIAPI *EFI_SET_MEM) (
  IN VOID *Buffer,
  IN UINTN Size,
  IN UINT8 Value
);

typedef
EFI_STATUS
(EFIAPI *EFI_CREATE_EVENT_EX) (
  IN UINT32 Type,
  IN EFI_TPL NotifyTpl,
  IN EFI_EVENT_NOTIFY NotifyFunction OPTIONAL,
  IN CONST VOID *NotifyContext OPTIONAL,
  IN CONST EFI_GUID *EventGroup OPTIONAL,
  OUT EFI_EVENT *Event
);

typedef struct {
  EFI_TABLE_HEADER Hdr;
  EFI_RAISE_TPL RaiseTPL;
  EFI_RESTORE_TPL RestoreTPL;
  EFI_ALLOCATE_PAGES AllocatePages;
  EFI_FREE_PAGES FreePages;
  EFI_GET_MEMORY_MAP GetMemoryMap;
  EFI_ALLOCATE_POOL AllocatePool;
  EFI_FREE_POOL FreePool;
  EFI_CREATE_EVENT CreateEvent;
  EFI_SET_TIMER SetTimer;
  EFI_WAIT_FOR_EVENT WaitForEvent;
  EFI_SIGNAL_EVENT SignalEvent;
  EFI_CLOSE_EVENT CloseEvent;
  EFI_CHECK_EVENT CheckEvent;
  EFI_INSTALL_PROTOCOL_INTERFACE InstallProtocolInterface;
  EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface;
  EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface;
  EFI_HANDLE_PROTOCOL HandleProtocol;
  VOID* Reserved; 
  EFI_REGISTER_PROTOCOL_NOTIFY RegisterProtocolNotify;
  EFI_LOCATE_HANDLE LocateHandle;
  EFI_LOCATE_DEVICE_PATH LocateDevicePath;
  EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable;
  EFI_IMAGE_LOAD LoadImage;
  EFI_IMAGE_START StartImage;
  EFI_EXIT Exit;
  EFI_IMAGE_UNLOAD UnloadImage;
  EFI_EXIT_BOOT_SERVICES ExitBootServices;
  EFI_GET_NEXT_MONOTONIC_COUNT GetNextMonotonicCount;
  EFI_STALL Stall;
  EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;
  EFI_CONNECT_CONTROLLER ConnectController;
  EFI_DISCONNECT_CONTROLLER DisconnectController;
  EFI_OPEN_PROTOCOL OpenProtocol;
  EFI_CLOSE_PROTOCOL CloseProtocol;
  EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation;
  EFI_PROTOCOLS_PER_HANDLE ProtocolsPerHandle;
  EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer;
  EFI_LOCATE_PROTOCOL LocateProtocol;
  EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES InstallMultipleProtocolInterfaces;
  EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces;
  EFI_CALCULATE_CRC32 CalculateCrc32;
  EFI_COPY_MEM CopyMem;
  EFI_SET_MEM SetMem;
  EFI_CREATE_EVENT_EX CreateEventEx;
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
  unsigned long long _buf[3];
  EFI_DEVICE_PATH_PROTOCOL *(*AppendDeviceNode)(
    const EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    const EFI_DEVICE_PATH_PROTOCOL *DeviceNode);
} EFI_DEVICE_PATH_UTILITIES_PROTOCOL;


#endif
