#ifndef PCI
#define PCI

#include "../types.h"
typedef struct pci_config_space
{
    uint16_t device_id;
    uint16_t vendor_id;
} pci_config_space_t;

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