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

UINT32 read_config(UINT8 bus, UINT8 device, UINT8 function, UINT8 offset) {
  set_config_address(build_address(bus, device, function, offset));
  return read_data();
}

UINT8 read_header_type(UINT8 bus, UINT8 device, UINT8 function) {
  return (read_config(bus, device, function, 0x0C) >> 16) & 0xFFU;
}

void read_class_code(UINT8 bus, UINT8 device, UINT8 function, class_code_t* class_code) {
  UINT32 data = read_config(bus, device, function, 0x08);
  class_code->base = (data >> 24) & 0xFFU;
  class_code->sub = (data >> 16) & 0xFFU;
  class_code->interface = (data >> 8) & 0xFFU;
}

UINT32 read_bus_number(UINT8 bus, UINT8 device, UINT8 function) {
  return read_config(bus, device, function, 0x18);
}

UINT16 get_vendor_id(UINT8 bus, UINT8 device, UINT8 function) {
  return read_config(bus, device, function, 0x00) & 0xFFFFU;
}

STATUS add_device(UINT8 bus, UINT8 device, UINT8 function, UINT8 header_type, class_code_t class_code) {
  if (device_num >= DEVICE_SIZE) {
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

UINT64 read_bar(device_t* dev, UINT8 index) {
  UINT8 bar_address = 0x10 + 4 * index;
  UINT32 lower = read_config(dev->bus, dev->device, dev->function, bar_address);
  UINT32 upper = read_config(dev->bus, dev->device, dev->function, bar_address + 4);
  return lower | (((UINT64)upper) << 32);
}

void set_msi(device_t* dev) {
  UINT8 local_apic_id = *(UINT32*)(0xFEE00020) >> 24;
  UINT32 msg_addr = 0xFEE00000U | (local_apic_id << 12);
  UINT32 msg_data = 0xC000U | 0x40U;
  UINT8 cap_pointer = read_config(dev->bus, dev->device, dev->function, 0x34);
}

void wait() {
  unsigned long long a = 0;
  while(1) {
    if (++a == 1000000000) {
      break;
    }
  }
}

void start_xhc(UINT64 mmio) {
  capability_registers_t *cr = (capability_registers_t *)mmio;
  log("mmio : %x\n", mmio + cr->CAPLENGTH);
  log("CAPLENGTH : %d, HCIVERSION: %x, HCCPARAMS1: %x\n", cr->CAPLENGTH, cr->HCIVERSION, cr->HCCPARAMS1);
  operational_registers_t *or = (operational_registers_t *)(mmio + cr->CAPLENGTH);
  usbsts_t usbsts = (usbsts_t)or->USBSTS;
  log("hch: %d, hse: %d, ei: %d, pcd: %d, sss: %d, rss: %d, sre: %d, cnr: %d, hce: %d \n",
      usbsts.hcHalted,
      usbsts.hostSystemError,
      usbsts.eventInterrupt,
      usbsts.portChangeDetect,
      usbsts.saveStateStatus,
      usbsts.restoreStateStatus,
      usbsts.saveRestoreError,
      usbsts.controllerNotReady,
      usbsts.hostControllerError
  );
  usbcmd_t usbcmd = (usbcmd_t)or->USBCMD;
  if (!usbsts.hcHalted) {
    usbcmd.runStop = false;
    or->USBCMD = usbcmd;
    while(!usbsts.hcHalted) {
      usbsts = (usbsts_t)or->USBSTS;
    };
  }
  usbcmd.hostControllerReset = true;
  or->USBCMD = usbcmd;
  while(usbcmd.hostControllerReset || usbsts.controllerNotReady) {
    usbcmd = (usbcmd_t)or->USBCMD;
    usbsts = (usbsts_t)or->USBSTS;
  }
  hcsparams1_t hcsparams1 = (hcsparams1_t)cr->HCSPARAMS1;
  log("max_device_slots: %d\n", hcsparams1.number_of_device_slots);
  config_t config = (config_t)or->CONFIG;
  config.max_device_slots_enabled = 8;
  or->CONFIG = config;


  log("done xhc");
}

void init_xhci() {
  STATUS status = find_all_device();
  log("init xhci => %d\n", status);
  device_t* xhc_device = NULL;
  for(int i = 0; i < device_num; i++) {
    device_t device = device_list[i];
    UINT16 vendor_id = get_vendor_id(device.bus, device.device, device.function);
    if (is_xhci(&device.class_code)) {
      log("bus: %d, device: %d, func: %d, base: %x, sub: %x, interface: %x, vendor_id: %x, header: %x\n",
        device.bus, device.device, device.function, device.class_code.base, device.class_code.sub, device.class_code.interface, vendor_id, device.header_type);
      xhc_device = &device;
    }
  }
  if (xhc_device == NULL) {
    log("not found\n");
    return;
  }
  UINT64 bar = read_bar(xhc_device, 0);
  UINT64 mmio = bar & ~0xFUL; // mask lower 4bit
  log("bar: %x, mmio: %x", bar, mmio);
  start_xhc(mmio);
  log("done xhci\n", status);
}
