#include "pci.h"
#include "../vga.h"
#include "../io.h"
#include "../memory/acpi.h"
// pci_config_space_info_t should be casted to appriopriate hn type
// based on value in header_type
// if addr is invalid return NULL

typedef struct driver_list
{
    pci_driver_t driver;
    struct driver_list* next;
} driver_list_t;

pci_config_space_info_t read_device_cfs(uint32_t addr)
{
    pci_config_space_info_t base_info;
    base_info.vendor_id = mmio_inw(addr);   
    if(base_info.vendor_id == 0xFFFF)
    {
        return base_info;
    }
    addr+=2;
    base_info.device_id = mmio_inw(addr); 
    addr+=2;
    base_info.command = mmio_inw(addr); 
    addr+=2;
    base_info.status = mmio_inw(addr); 
    addr+=2;
    base_info.revision_id = mmio_inb(addr);
    addr+=1; 
    base_info.prog_if = mmio_inb(addr); 
    addr+=1; 
    base_info.subclass = mmio_inb(addr); 
    addr+=1; 
    base_info.class_code = mmio_inb(addr); 
    addr+=1; 
    base_info.cacheline_size = mmio_inb(addr); 
    addr+=1; 
    base_info.latency_timer = mmio_inb(addr);
    addr+=1;  
    base_info.header_type = mmio_inb(addr); 
    addr+=1; 
    base_info.bist = mmio_inb(addr); 

    return base_info;

}

void checkDevice(uint8_t bus, uint8_t device, uint32_t base_addr)
{
    uint8_t function = 0;
    uint32_t addr = ((bus) << 20 | device << 15 | function << 12) + base_addr;

    pci_config_space_info_t  info = read_device_cfs(addr);
    if (info.vendor_id == 0xFFFF)
    {
        return;
    }  
    probe_drivers(bus, device, function, &info);

    for (function = 1; function  < 8; function++)
    {
        uint32_t addr = ((bus) << 20 | device << 15 | function << 12) + base_addr;
        pci_config_space_info_t  info = read_device_cfs(addr);
        if (info.vendor_id  != 0xFFFF)  // Device doesn't exist
        {
            probe_drivers(bus, device, function, &info);
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

void start_device_subsystem()
{
    rsdp_t* rsdp =  find_rsdp();
    parse_system_descriptor_table(rsdp);
}

extern uint32_t __pci_driver_start;
extern uint32_t __pci_driver_end;

void probe_drivers(uint8_t bus, uint8_t device, uint8_t function,const pci_config_space_info_t* config_info)
{
    pci_driver_t** drivers_start_addr_ptr = (pci_driver_t**) &__pci_driver_start;
    pci_driver_t** drivers_end_addr_ptr = (pci_driver_t**) &__pci_driver_end;


    pci_driver_t** current_driver_ptr = drivers_start_addr_ptr;
    while (current_driver_ptr <  drivers_end_addr_ptr)
    {
        pci_driver_t* current_driver = *current_driver_ptr;
        current_driver->interface->probe(bus, device, function, config_info);
        current_driver_ptr++;
    }
    
}
