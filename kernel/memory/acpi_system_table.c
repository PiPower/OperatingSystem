#include "acpi_internal.h"
#include "../vga.h"
#include "../device/pci.h"

#define SUCCESS 0x01
#define CHECKSUM_FAIL -0x01
#define UNSUPPORTED_TABLE -0x02

uint8_t mcfg_found;

enum system_tables
{
    ST_UNUSED = 0,
    ST_FACP,
    ST_APIC,
    ST_SSDT,
    ST_HPET,
    ST_MCFG,
    ST_WAET,
    ST_DSDT,
    ST_FACS,
    ST_SIZE = ST_FACS
};

static const char* system_tables = "FACP" "APIC" "SSDT" "HPET" "MCFG" "WAET" "DSDT" "FACS";
typedef int (*system_table_handler)(sdt_header_t* header);
static const system_table_handler st_handlers[ST_SIZE] = {
    process_facp, NULL, NULL, NULL, process_mcfg, NULL, NULL, NULL
};

int proces_system_table(sdt_header_t* header)
{
    unsigned int i=0;
    while (system_tables[i] != '\0')
    {
        if(header->signature[0] == system_tables[i* 4 + 0] 
        && header->signature[1] == system_tables[i* 4 + 1]
        && header->signature[2] == system_tables[i* 4 + 2]
        && header->signature[3] == system_tables[i* 4 + 3] )
        {
            break;
        }
        i++;
    }

    static int j =0;
    printc(header->signature[0]);
    printc(header->signature[1]);
    printc(header->signature[2]);
    printc(header->signature[3]);
    printc('\n');
    j++;

    if(system_tables[i] == '\0' )
    {
        return UNSUPPORTED_TABLE;
    }
    system_table_handler handler = st_handlers[i];
    if(handler != NULL)
    {
        return handler(header);
    }

    return UNSUPPORTED_TABLE;
}

int process_facp(sdt_header_t* header)
{
    if(!validate_checksum((char*)header, header->lenght))
    {
       print("Incorrect FACP table");
       return CHECKSUM_FAIL;
    }

    fadt_t* fadt =  (fadt_t*)header;
    // dsdt table
    if(fadt->x_dsdt.high != 0)
    {
        print("ERROR: dsdt is in memory out of 4GB range");
        printh(fadt->x_dsdt.high, 1);
        while (1){} 
    }
    sdt_header_t* dsdt;
    if(fadt->x_dsdt.low != 0)
    {
        dsdt = (sdt_header_t*)fadt->x_dsdt.low;
    }
    else
    {
        dsdt = (sdt_header_t*)fadt->dsdt;
    }
    
    proces_system_table(dsdt);

    // facs table
    if(fadt->x_firmware_ctrl.high != 0 )
    {
        print("ERROR: facs is in memory out of 4GB range" );
        printh(fadt->x_dsdt.high, 1);
        while (1){} 
    }
    sdt_header_t* facs;
    if(fadt->x_firmware_ctrl.low != 0)
    {
        facs = (sdt_header_t*)fadt->x_firmware_ctrl.low;
    }
    else
    {
        facs = (sdt_header_t*)fadt->firmware_ctrl;
    }
    proces_system_table(facs);

    return SUCCESS;
}

int process_mcfg(sdt_header_t* header)
{
    mcfg_found = 1; // inform system that mcfg has been found
    unsigned int ecam_entries = (header->lenght - sizeof(sdt_header_t) - 8 )/ sizeof(ecam_desc_t);
    ecam_desc_t* entries = (ecam_desc_t*)((char*)header + sizeof(sdt_header_t) + 8);

    unsigned int i;
    for(i =0; i < ecam_entries; i++)
    {
        if(entries->base_addr.high > 0)
        {
            print("ERROR: ecam outside of 4GB memory region");
            while (1){}
        }
        detect_pci_devices(entries);
        i++;
    }
    return SUCCESS;
}