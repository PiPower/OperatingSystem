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


typedef struct dsdt
{
    char signature[4];
} dsdt_t;

typedef struct facs
{
    char signature[4];
} facs_t;
typedef struct fadt
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
    facs_t* firmware_ctrl;
    dsdt_t* dsdt;
    uint8_t reserved;
    uint8_t preferred_pm_profile;
    uint16_t sci_int;
    uint32_t smi_cmd;
    uint8_t acpi_enable;
    uint8_t acpi_disable;
    uint8_t s4bios_req;
    uint8_t pstate_cnt;
    uint32_t pm1a_evt_blk;
    uint32_t pm1b_evt_blk;
    uint32_t pm1a_cnt_blk;
    uint32_t pm1b_cnt_blk;
    uint32_t pm2_cnt_blk;
    // 76 bytes, 192 remaining
    char unused_part[56];
    quadword_ptr x_firmware_ctrl;
    quadword_ptr x_dsdt;
    char unused_part_2[120];
} fadt_t;


int proces_system_table(sdt_header_t* header);
int process_facp(sdt_header_t* header);
int process_mcfg(sdt_header_t* header);
int validate_checksum(char* start_table_ptr, uint32_t byte_count);
#endif