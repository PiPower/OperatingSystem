#include "ahci_driver.h"
#include "../vga.h"

static pci_driver_interface_t ahci_driver_interface =
{
    .probe = ahci_probe
};

static pci_driver_t ahci_driver =
{
    .interface = &ahci_driver_interface
};

uint8_t ahci_probe(uint8_t bus, uint8_t device, uint8_t function,const pci_config_space_info_t* config_info)
{
    print("Hello from ahci driver \n");
    ahci_driver.driver_name[0] = 's';
    return PROBE_FAILURE;
}


REGISTER_PCI_DRIVER(ahci_driver)