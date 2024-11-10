#ifndef PCI_DRIVER
#define PCI_DRIVER

#include "../types.h"

#define PROBE_FAILURE 0x00
#define PROBE_SUCCES 0x01


typedef struct pci_config_space_info
{
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cacheline_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
} pci_config_space_info_t;


typedef struct pci_config_space_h0
{
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cacheline_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    uint32_t cardbud_cis_pointer;
    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;
    uint16_t expansion_rom_base_addr;
    uint8_t capabilities_pointer;
    uint8_t reserved1;
    uint16_t reserved2;
    uint32_t reserved3;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;
} pci_config_space_h0_t;

typedef struct pci_config_space_h1
{
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cacheline_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint32_t bar0;
    uint32_t bar1;
    uint8_t primary_bus_nr;
    uint8_t secondary_bus_nr;
    uint8_t subordinate_bus_nr;
    uint8_t secondary_latency_timer;
    uint8_t io_base;
    uint8_t io_limit;
    uint16_t secondary_status;
    uint16_t memory_base;
    uint16_t memory_limit;
    uint16_t pref_memory_base;
    uint16_t pref_memory_limit;
    uint32_t pref_base_upper_32bits;
    uint32_t pref_limit_upper_32bits;
    uint16_t io_base_upper_16bits;
    uint16_t io_limit_upper_16bits;
    uint8_t capability_pointer;
    uint8_t reserved[3];
    uint32_t expansion_rom_base_addr;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;
} pci_config_space_h1_t;

typedef struct pci_config_space_h2
{
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cacheline_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint32_t cardbus_socket_exca_base_addr;
    uint8_t offset_of_capabilities_list;
    uint8_t reserved;
    uint16_t secondary_status;
    uint8_t ocu_bus_number;
    uint8_t cardbus_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t cardbus_latency_timer;
    uint32_t memory_base_addr_0;
    uint32_t memory_limit_0;
    uint32_t memory_base_addr_1;
    uint32_t memory_limit_1;
    uint32_t io_base_addr_0;
    uint32_t io_limit_0;
    uint32_t io_base_addr_1;
    uint32_t io_limit_1;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;
    uint16_t subsystem_device_id;
    uint16_t subsystem_vendor_id;
    uint32_t pccard_legacy_mode_base_addr_16bit;
} pci_config_space_h2_t;

typedef uint8_t (*ProbePtr)(uint8_t bus, uint8_t device, uint8_t function,const pci_config_space_info_t* config_info);

typedef struct pci_driver_interface
{
    ProbePtr probe;
} pci_driver_interface_t;


typedef struct pci_driver
{
    pci_driver_interface_t* interface;
    char driver_name[64];
}pci_driver_t;

#define DRIVER_SPACE  __attribute__ ((section (".pci_driver"))) __attribute__ ((used))
#define REGISTER_PCI_DRIVER(driver) static pci_driver_t* drv DRIVER_SPACE = &driver;

#endif