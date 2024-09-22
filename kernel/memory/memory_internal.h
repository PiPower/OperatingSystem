#ifndef MEMORY_INTERNAL
#define MEMORY_INTERNAL
#include "../types.h"

 
#define BLOCK_UNALLOCATED 0x00
#define BLOCK_ALLOCATED 0x01

typedef struct block_header
{
    uint8_t status;
    char alignment[3];
    uint32_t block_len;
} block_header_t;


void* heap_kmalloc(uint32_t size);
void setup_heap();

#endif