#include "pci.h"
#include "../vga.h"

static uint16_t mm_read_word(uint32_t addr)
{
    uint16_t out;
    asm("movl %1, %%ebx \n"
        "movw 0x00(%%ebx), %0" : "=r"(out) : "r"(addr) );
    return out;
}

void checkDevice(uint8_t bus, uint8_t device, uint32_t base_addr)
{
    uint8_t function = 0;
    uint32_t addr = ((bus) << 20 | device << 15 | function << 12) + base_addr;

    uint16_t  vendorID= mm_read_word(addr);
    if (vendorID == 0xFFFF)
    {
        return;
    }  
    
    printh(bus, 1);
    printc(' ');
    printh(device, 1);
    printc(' ');
    printh(function, 1);
    printc(' ');
    print("PCI device found, vendorID is ");
    printh(vendorID, 1);
    printc('\n');
    

    for (function = 1; function  < 8; function++)
    {
        uint32_t addr = ((bus) << 20 | device << 15 | function << 12) + base_addr;
        uint16_t  vendorID= mm_read_word(addr);
        if (vendorID != 0xFFFF)  // Device doesn't exist
        {
            printh(bus, 1);
            printc(' ');
            printh(device, 1);
            printc(' ');
            printh(function, 1);
            printc(' ');
            print("PCI device found, vendorID is ");
            printh(vendorID, 1);
            printc('\n');
        }
    }
    
}

void detect_pci_devices(ecam_desc_t* pci_cs_base_addr)
{
    uint16_t bus;
    uint8_t device;

    for (bus = pci_cs_base_addr->start_bus; bus <= pci_cs_base_addr->end_bus; bus++)
    {
        for (device = 0; device < 32; device++) 
        {   
            checkDevice(bus, device, pci_cs_base_addr->base_addr.low);
        }
     }
}