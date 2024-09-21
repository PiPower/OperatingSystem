#include "memory.h" 
#include "acpi.h"
#include "../vga.h"

void init_memory_subsystem()
{
    rsdp_t* rsdp = find_rsdp();

    if(rsdp)
    {
        print_str("rsdp has been found", 15, 0);
        printh_uint((uint32_t)rsdp, 16, 0, 1);
    }
    else
    {
        print_str("rsdp has NOT been found", 15, 0);
    }

    if(rsdp->revision  > 0 && rsdp->e_hi_address > 0)
    {
        print_str("ERROR: xsdt is at address beyond 4GB what is not supported", 16,0);
        while (1){}  
    }
    printh_uint(rsdp->revision, 16, 0, 1);
}