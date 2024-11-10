#ifndef AHCI_DRIVER
#define AHCI_DRIVER

#include "../types.h"
#include "pci_driver.h"
uint8_t ahci_probe(uint8_t bus, uint8_t device, uint8_t function,const pci_config_space_info_t* config_info);

#endif