#include "acpi_internal.h"
#include "../vga.h"

#define SUCCESS 0x01
#define CHECKSUM_FAIL -0x01
#define UNSUPPORTED_TABLE -0x02

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
    ST_SIZE
};

static const char* system_tables = "FACP" "APIC" "SSDT" "HPET" "MCFG" "WAET" "SIZE" "DSDT" "FACS";
typedef int (*system_table_handler)(sdt_header_t* header);
static const system_table_handler st_handlers[ST_SIZE] = {
    process_facp, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
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
    print_at(header->signature[0], j, 0);
    print_at(header->signature[1], j, 1);
    print_at(header->signature[2], j, 2);
    print_at(header->signature[3], j, 3);
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
       print_str("Incorrect FACP table", 0, 0);
       return CHECKSUM_FAIL;
    }

    fadt_t* fadt =  (fadt_t*)header;
    if(fadt->x_dsdt.high != 0)
    {
        print_str("ERROR: dsdt is in memory out of 4GB range", 0 ,0 );
        printh_uint(fadt->x_dsdt.high , 1, 0, 1);
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


    if(fadt->x_firmware_ctrl.high != 0 )
    {
        print_str("ERROR: facs is in memory out of 4GB range", 0 ,0 );
        printh_uint(fadt->x_dsdt.high , 1, 0, 1);
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