#ifndef PCI
#define PCI

#include "../types.h"

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

typedef struct ecam_desc
{
    quadword_ptr base_addr;
    uint16_t pci_segment;
    uint8_t start_bus;
    uint8_t end_bus;
    uint32_t reserved;
}ecam_desc_t;

void detect_pci_devices(ecam_desc_t* ecam);
#endif