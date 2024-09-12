#ifndef BOOT_INFO
#define BOOT_INFO

#define Undefined 0x00 // set if not equal to any address range 
#define AddressRangeMemory 0x01
#define AddressRangeReserved 0x02
#define AddressRangeACPI 0x03
#define AddressRangeNVS  0x04
#define AddressRangeUnusuable 0x05
#define AddressRangeUnusuable 0x05
#define AddressRangeDisabled 0x06


typedef struct memory_map_entry
{
    char low_base_address[4];
    char high_base_address[4];
    char low_length[4];
    char high_length[4];
    char range_type[4];
} memory_map_entry_t;

// creater in kernelboot.S
extern memory_map_entry_t entries[];

#endif