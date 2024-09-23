#ifndef MEMORY_INTERNAL
#define MEMORY_INTERNAL
#include "../types.h"

 
#define BLOCK_UNALLOCATED 0x00
#define BLOCK_ALLOCATED 0x01

#define CANNOT_ALLOCATE 0x00
#define CAN_ALLOCATE 0x01
#define CAN_ALLOCATE_PERFECT_FIT 0x02

typedef struct block_header
{
    uint8_t status;
    char ignored[3]; // used for 4-byte alignment, can store additional data 
    uint32_t block_len;
} block_header_t;


void* heap_kmalloc(uint32_t size);
void* heap_kmalloc_aligned(uint32_t size, uint32_t alignment);
void setup_heap();

#endif