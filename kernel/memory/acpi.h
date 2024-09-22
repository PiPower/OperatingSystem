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
    uint32_t lenght;
    char xsdt_address[8] ;
    uint8_t extended_checksum;
    char reserved[3];
    // fields below are support ONLY if revision > 0
    uint32_t e_lenght ;
    uint32_t e_low_address;
    uint32_t e_hi_address;
    uint8_t e_checksum;
    char  e_reserved[3];
} rsdp_t;


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

rsdp_t* find_rsdp();
void parse_system_descriptor_table( rsdp_t* rsdp);

#endif