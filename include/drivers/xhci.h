#ifndef MYOS_XHCI_H
#define MYOS_XHCI_H

#include <type.h>

#define CONFIG_ADDRESS 0x0CF8
#define CONFIG_DATA 0x0CFC
#define INVALID_ID 0xFFFFU

typedef int STATUS;

#define DEVICE_SIZE 64
#define MAX_PCI_DEVICE_NUM 32
#define MAX_FUNCTION_NUM 8
#define CONVERT_STATUS(code) ((STATUS)code)
#define OK CONVERT_STATUS(0)
#define ERROR CONVERT_STATUS(1)

#define BASE_CLASS 0x06U
#define SUB_CLASS 0x04U

typedef struct {
  UINT8 base;
  UINT8 sub;
  UINT8 interface;
} class_code_t;

typedef struct {
  UINT8 bus;
  UINT8 device;
  UINT8 function;
  UINT8 header_type;
  class_code_t class_code;
} device_t;

void init_xhci();

UINT32 build_address(UINT8 bus, UINT8 device, UINT8 function, UINT8 offset);
void set_config_address(UINT32 address);
void write_config_data(UINT32 data);
void outl(CHAR16 port, UINT32 value);
UINT32 inl(CHAR16 port);
UINT8 read_header_type(UINT8 bus, UINT8 device, UINT8 function);
void read_class_code(UINT8 bus, UINT8 device, UINT8 function, class_code_t* class_code);
UINT32 read_bus_number(UINT8 bus, UINT8 device, UINT8 function);
UINT16 get_vendor_id(UINT8 bus, UINT8 device, UINT8 function);
STATUS add_device(UINT8 bus, UINT8 device, UINT8 function, UINT8 header_type, class_code_t class_code);
STATUS find_function(UINT8 bus, UINT8 device);
STATUS find_device(UINT8 bus);

/*
typedef union {
  UINT32 data[1];
  struct {
    UINT8 CAPLENGTH;
    UINT8 : 8;
    UINT16 HCIVERSION;
    UINT32 HCSPARAMS1;
    UINT32 HCSPARAMS2;
    UINT32 HCSPARAMS3;
    UINT32 HCCPARAMS1;
    UINT32 DBOFF;
    UINT32 RTSOFF;
    UINT32 HCCPARAMS2;
  } __attribute__((packed)) bits;
} __attribute__((packed)) capability_registers_t;
*/

typedef struct {
  UINT8 CAPLENGTH;
  UINT8 : 8;
  UINT16 HCIVERSION;
  UINT32 HCSPARAMS1;
  UINT32 HCSPARAMS2;
  UINT32 HCSPARAMS3;
  UINT32 HCCPARAMS1;
  UINT32 DBOFF;
  UINT32 RTSOFF;
  UINT32 HCCPARAMS2;
} capability_registers_t;



typedef struct {
  UINT32 runStop: 1;
  UINT32 hostControllerRest: 1;
  UINT32 interrupterEnable: 1;
  UINT32 hostSystemErrorEnable: 1;
  UINT32 : 3;
  UINT32 hightHostControllerRest: 1;
  UINT32 controllerSaveState: 1;
  UINT32 controllerRestoreState: 1;
  UINT32 enableWrapEvent: 1;
  UINT32 enableU3MfindexStop: 1;
  UINT32 : 1;
  UINT32 cemEnable: 1;
  UINT32 extendedTbcEnable: 1;
  UINT32 extendedTbcstatusEnable: 1;
  UINT32 vtioEnable: 1;
  UINT32 : 15;
} usbcmd_t;

typedef struct {
  UINT32 hcHalted: 1;
  UINT32 : 1;
  UINT32 hostSystemError: 1;
  UINT32 eventInterrupt: 1;
  UINT32 portChangeDetect: 1;
  UINT32 : 3;
  UINT32 saveStateStatus: 1;
  UINT32 restoreStateStatus: 1;
  UINT32 saveRestoreError: 1;
  UINT32 controllerNotReady: 1;
  UINT32 hostControllerError: 1;
  UINT32 : 19;
} usbsts_t;

typedef struct {

} pagesize_t;

typedef struct {

} dnctrl_t;

typedef struct {

} crcr_t;

typedef struct {

} dcbaap_t;

typedef struct {
} config_t;

typedef struct {
  usbcmd_t USBCMD;
  usbsts_t USBSTS;
  UINT32 pagesize; //pagesize_t pagesize;
  UINT32 reserved_1[1];
  UINT32 dnctrl; // dnctrl_t dnctrl;
  UINT32 crcr; //crcr_t crcr;
  UINT32 reserved_2[4];
  UINT64 dcbaap;
  config_t config;
} __attribute__((packed)) operational_registers_t;

#endif
