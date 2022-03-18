#ifndef MYOS_XHCI_H
#define MYOS_XHCI_H

#include <type.h>

#define CONFIG_ADDRESS 0x0CF8
#define CONFIG_DATA 0x0CFC
#define INVALID_ID 0xFFFFU

typedef int STATUS;

#define DEVICE_SIZE 32
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

#endif
