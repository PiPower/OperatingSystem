#include "acpi_internal.h"
#include "../vga.h"

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

void proces_system_table(sdt_header_t* header)
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
    
    system_table_handler handler = st_handlers[i];
    if(handler != NULL)
    {
        handler(header);
    }

}

int process_facp(sdt_header_t* header)
{
    print_str("FACP", 0, 0);
}