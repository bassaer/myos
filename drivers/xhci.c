#include <drivers/xhci.h>
#include <graphics/desktop.h>

device_t device_list[DEVICE_SIZE];
int device_num = 0;

UINT32 build_address(UINT8 bus, UINT8 device, UINT8 function, UINT8 offset) {
  return
    (1 << 31) | // enable bit
    (0 << 24) | // reserved
    (bus << 16) | // bus number
    (device << 11) | // device number
    (function << 8) | // function number
    (offset & 0xFCU); // register offset & mask lower 2bit
}

void set_config_address(UINT32 address) {
  outl(CONFIG_ADDRESS, address);
}

void write_config_data(UINT32 data) {
  outl(CONFIG_DATA, data);
}

UINT32 read_data() {
  return inl(CONFIG_DATA);
}

void outl(CHAR16 port, UINT32 value) {
  __asm__ volatile ("outl %0, %1": : "a"(value), "Nd"(port));
}

UINT32 inl(CHAR16 port) {
  UINT32 ret;
  __asm__ volatile ("inl %1, %0": "=a"(ret): "Nd"(port));
  return ret;
}

UINT8 read_header_type(UINT8 bus, UINT8 device, UINT8 function) {
  set_config_address(build_address(bus, device, function, 0x0C));
  return (read_data() >> 16) & 0xFFU;
}

void read_class_code(UINT8 bus, UINT8 device, UINT8 function, class_code_t* class_code) {
  set_config_address(build_address(bus, device, function, 0x08));
  UINT32 data = read_data();
  class_code->base = (data >> 24) & 0xFFU;
  class_code->sub = (data >> 16) & 0xFFU;
  class_code->interface = (data >> 8) & 0xFFU;
}

UINT32 read_bus_number(UINT8 bus, UINT8 device, UINT8 function) {
  set_config_address(build_address(bus, device, function, 0x18));
  return read_data();
}

UINT16 get_vendor_id(UINT8 bus, UINT8 device, UINT8 function) {
  set_config_address(build_address(bus, device, function, 0x00));
  return read_data() & 0xFFFFU;
}

STATUS add_device(UINT8 bus, UINT8 device, UINT8 function, UINT8 header_type, class_code_t class_code) {
  if (device_num >= DEVICE_SIZE*2) {
    return ERROR;
  }
  device_t found_device = {bus, device, function, header_type, class_code};
  device_list[device_num++] = found_device;
  return OK;
}


STATUS find_function(UINT8 bus, UINT8 device) {
  for (UINT8 function = 0; function < 32; ++function) {
    if (get_vendor_id(bus, device, function) == INVALID_ID) {
      continue;
    }
    UINT8 header_type = read_header_type(bus, device, function);
    class_code_t class_code;
    read_class_code(bus, device, function, &class_code);
    STATUS status = add_device(bus, device, function, header_type, class_code);
    if (status != OK) {
      return status;
    }
    if (class_code.base == BASE_CLASS && class_code.sub == SUB_CLASS) {
      UINT32 bus_number = read_bus_number(bus, device, function);
      UINT8 secondary_bus = (bus_number >> 8) & 0xFFU;
      return find_device(secondary_bus);
    }
  }
  return OK;
}

STATUS find_device(UINT8 bus) {
  for (UINT8 device = 0; device < MAX_PCI_DEVICE_NUM; ++device) {
    if (get_vendor_id(bus, device, 0) == INVALID_ID) {
      continue;
    }
    STATUS status = find_function(bus, device);
    if (status != OK) {
      return status;
    }
  }
  return OK;
}

bool is_single_function(UINT8 header_type) {
  return (header_type & 0x80U) == 0;
}

STATUS find_all_device() {
  UINT8 header_type = read_header_type(0, 0, 0);
  if (is_single_function(header_type)) {
    return find_device(0);
  }
  for (UINT8 function = 0; function < MAX_FUNCTION_NUM; ++function) {
    if (get_vendor_id(0, 0, function) == INVALID_ID) {
      continue;
    }
    STATUS status = find_device(function);
    if (status != OK) {
      return status;
    }
  }
  return OK;
}

bool is_xhci(class_code_t* code) {
  return code->base == 0x0CU && code->sub == 0x03U && code->interface == 0x30U;
}

void init_xhci() {
  int y = 0;
  STATUS status = find_all_device();
  printf(0, y, "init xhci => %d\n", status);
  y += 20;
  device_t* xhc_device = NULL;
  for(int i = 0; i < device_num; i++) {
    device_t device = device_list[i];
    UINT16 vendor_id = get_vendor_id(device.bus, device.device, device.function);
    if (is_xhci(&device.class_code)) {
      printf(0, y, "bus: %d, device: %d, func: %d, base: %x, sub: %x, interface: %x, vendor_id: %x, header: %x\n",
        device.bus, device.device, device.function, device.class_code.base, device.class_code.sub, device.class_code.interface, vendor_id, device.header_type);
      xhc_device = &device;
      y += 20;
    }
  }
  if (xhc_device == NULL) {
    printf(0, y, "not found\n");
    y += 20;
  }
  printf(0, y, "done xhci\n", status);
}
