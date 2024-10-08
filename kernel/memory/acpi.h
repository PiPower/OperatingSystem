#ifndef ACPI
#define ACPI

#include "../types.h"


typedef struct rsdp
{
    char signature[8] ;
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision;
    uint32_t rsdt_address;
    // fields below are supported ONLY if revision > 0
    uint32_t e_lenght ;
    uint32_t e_low_address;
    uint32_t e_hi_address;
    uint8_t e_checksum;
    char  e_reserved[3];
} rsdp_t;


rsdp_t* find_rsdp();
void parse_system_descriptor_table( rsdp_t* rsdp);

#endif