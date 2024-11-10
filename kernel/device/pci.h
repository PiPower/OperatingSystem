#ifndef PCI
#define PCI

#include "pci_driver.h"

typedef struct ecam_desc
{
    quadword_ptr base_addr;
    uint16_t pci_segment;
    uint8_t start_bus;
    uint8_t end_bus;
    uint32_t reserved;
}ecam_desc_t;


void detect_pci_devices(ecam_desc_t* ecam);
void start_device_subsystem();
void probe_drivers(uint8_t bus, uint8_t device, uint8_t function,const pci_config_space_info_t* config_space);
#endif