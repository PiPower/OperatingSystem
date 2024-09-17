#ifndef BOOT_INFO
#define BOOT_INFO
#include "kernel/types.h"

#define Undefined 0x00 // set if not equal to any address range 
#define AddressRangeMemory 0x01
#define AddressRangeReserved 0x02
#define AddressRangeACPI 0x03
#define AddressRangeNVS  0x04
#define AddressRangeUnusuable 0x05
#define AddressRangeUnusuable 0x05
#define AddressRangeDisabled 0x06

#define MAX_ENTRIES 16

typedef struct memory_map_entry
{
    uint32_t low_base_address;
    uint32_t high_base_address;
    uint32_t low_length;
    uint32_t high_length;
    uint32_t range_type;
} memory_map_entry_t;

// creater in kernelboot.S
extern uint16_t map_entries_size;
extern memory_map_entry_t map_entries[];

#endif