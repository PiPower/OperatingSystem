#include "acpi.h"
#define RSDP_SIZE 20 // base rsdp
#define eRSDP_SIZE 36 // extendet rsdp 

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

static int validate_checksum(rsdp_t* rsdp, uint32_t byte_count)
{
    char* rsdp_base= (char*)rsdp;
    char checksum;
    int i =0;
    while (i < byte_count)
    {
        checksum += rsdp_base[i++];
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
        if(mem_cmp("RSD PTR ", rsdp->signature, 8) && validate_checksum(rsdp, RSDP_SIZE))
        {
            if( rsdp->revision == 0  || (rsdp->revision > 0 && validate_checksum(rsdp, eRSDP_SIZE ) ))
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
        if(mem_cmp("RSD PTR ", rsdp->signature, 8) && validate_checksum(rsdp, RSDP_SIZE))
        {
            if( rsdp->revision == 0  || (rsdp->revision > 0 && validate_checksum(rsdp, eRSDP_SIZE ) ))
            {
                return rsdp;
            }
        }

        bios_ro += 16;
    }

    return 0x00;
}