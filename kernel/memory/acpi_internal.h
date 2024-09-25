#ifndef ACPI_INTERNAL
#define ACPI_INTERNAL
#include "../types.h"

typedef struct sdt_header
{
    char signature[4];
    uint32_t lenght;
    uint8_t revision;
    uint8_t checksum;
    char OEMID[6];
    char OEMID_table_id[8];
    uint32_t OEM_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} sdt_header_t;

void proces_system_table(sdt_header_t* header);
int process_facp(sdt_header_t* header);
#endif