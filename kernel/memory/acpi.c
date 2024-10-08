#include "acpi.h"
#include "acpi_internal.h"
#include "../vga.h"
#define RSDP_SIZE 20 // base rsdp
#define eRSDP_SIZE 36 // extendet rsdp 


extern uint8_t mcfg_found; // defined in acpi_system_table.c

static int mem_cmp(const char* l, const char* r, uint32_t size)
{
    uint32_t i = 0;
    for(; i < size; i++)
    {
        if(l[i] != r[i])
        {
            return 0;
        }
    }
    return 1;

}

// return 1 on succes
int validate_checksum(char* start_table_ptr, uint32_t byte_count)
{
    char checksum;
    int i =0;
    while (i < byte_count)
    {
        checksum += start_table_ptr[i++];
    }

    return checksum == 0;

}

rsdp_t* find_rsdp()
{
    char *bios_ebda = (char *)0x9fc00;
    char *bios_ebda_upperbound = (char *)0x9fc00 + 0x400; // first KB = 2^10
    rsdp_t* rsdp;
    while (bios_ebda < bios_ebda_upperbound)
    {
        rsdp = (rsdp_t*) bios_ebda;
        if(mem_cmp("RSD PTR ", rsdp->signature, 8) && validate_checksum((char*)rsdp, RSDP_SIZE))
        {
            if( rsdp->revision == 0  || (rsdp->revision > 0 && validate_checksum((char*)rsdp, eRSDP_SIZE ) ))
            {
                return rsdp;
            }
        }

        bios_ebda += 16;
    }

    char *bios_ro = (char *)0xE0000;
    char *bios_ro_upperbound = (char *)0xFFFFF; 
    while (bios_ro < bios_ro_upperbound)
    {
        rsdp = (rsdp_t*) bios_ro;
        if(mem_cmp("RSD PTR ", rsdp->signature, 8) && validate_checksum((char*)rsdp, RSDP_SIZE))
        {
            if( rsdp->revision == 0  || (rsdp->revision > 0 && validate_checksum((char*)rsdp, eRSDP_SIZE ) ))
            {
                return rsdp;
            }
        }

        bios_ro += 16;
    }

    return 0x00;
}

void parse_system_descriptor_table( rsdp_t* rsdp)
{
    if(rsdp->revision > 0 &&  rsdp->e_hi_address > 0 )
    {
        print("ERROR: xsdt is at address beyond 4GB what is not supported");
        while (1){}  
    }
  
    sdt_header_t* sdt_header = (sdt_header_t* )( rsdp->revision == 0 ? rsdp->rsdt_address : rsdp->e_low_address );  
    // entry is in fact pointer to pointer, yer for ease of increments 
    // it is defined as ptr to char
    // NEEDS to be casted before accesing acpi table
    char* entry = (char*)sdt_header + 36; // first entry, incase of 64 bit it is lower 32 bits of address
    char* entry_mem_bound = (char*)sdt_header + sdt_header->lenght; 
    uint8_t increment = rsdp->revision == 0 ? 4 : 8;

    int i =0;
    while (entry < entry_mem_bound)
    {
        if(increment == 8 && *(uint32_t*)(entry + 4) != 0) //check if ptr lies in 4GB boundary
        {
            print("ACPI tables cannot be parse as they are out of memory bounds for 4GB");
            while (1){}
        }

        proces_system_table(*(sdt_header_t**)entry );
        i++;
        entry += increment;
    }

    if(mcfg_found == 0)
    {
        print("ERROR: Could not find mcfg acpi table, PCI cannot be configured");
        while (1){}
    }
}

/*
void init_memory_subsystem()
{
    rsdp_t* rsdp = find_rsdp();

    if(rsdp)
    {
        print_str("rsdp has been found", 12, 0);
        printh_uint((uint32_t)rsdp, 13, 0, 1);
    }
    else
    {
        print_str("ERROR: rsdp has NOT been found", 12, 0);
        while (1){}
        
    }

    parse_system_descriptor_table(rsdp);
    
}
*/
