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
    ST_SIZE
};

static const char* system_tables = "FACPAPICSSDTHPETMCFGWAETSIZE";
typedef int (*system_table_handler)(sdt_header_t* header);
static const system_table_handler st_handlers[ST_SIZE] = {
    NULL, process_facp, NULL, NULL, NULL, NULL, NULL
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
    
    if(system_tables[i] == '\0' )
    {
        return UNSUPPORTED_TABLE;
    }

    system_table_handler handler = st_handlers[i];
    if(handler != NULL)
    {
        handler(header);
    }

    static unsigned int row = 0;
    print_at(header->signature[0], row, 0);
    print_at(header->signature[1], row, 1);
    print_at(header->signature[2], row, 2);
    print_at(header->signature[3], row, 3);
    row++;

    return SUCCESS;
}

int process_facp(sdt_header_t* header)
{
    if(!validate_checksum((char*)header, header->lenght))
    {
       print_str("Incorrect FACP table", 0, 0);
       return CHECKSUM_FAIL;
    }
}