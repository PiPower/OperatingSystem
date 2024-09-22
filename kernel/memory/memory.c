#include "memory.h" 
#include "acpi.h"
#include "../vga.h"
#include "../../memory_mappings.h"
#include "memory_internal.h"

char* heap_base;
char* heap_top;
char* page_journal; // keeps track of which pages are available in the system

void setup_memory_zones()
{
    page_journal = heap_kmalloc(1048576);
    *page_journal++ = 0xaf;
    *page_journal = 0x99;
}

// allocate memory on heap
// if not enough memory is available return 0
void* heap_kmalloc(uint32_t size)
{
    char* curr_pos = heap_base;
    block_header_t* header;
    while (curr_pos < heap_top )
    {
        header = (block_header_t*)curr_pos;
        if(header->status == BLOCK_UNALLOCATED && header->block_len >= size + sizeof(block_header_t))
        {
            break;
        }
        curr_pos += sizeof(block_header_t) + header->block_len;
    }
    
    if(curr_pos >= heap_top)
    {
        return 0x00;
    }
    uint32_t unallocated_chunk = header->block_len;
    header->status = BLOCK_ALLOCATED;
    header->block_len = size;
    char* ptr = curr_pos + sizeof(block_header_t);

    if(unallocated_chunk - header->block_len <= sizeof(block_header_t))
    {
        // if untouched memory can fit atmost block_header_t add remaining memory to to he block
        // as block of memory of size 0 is useless
        header->block_len = unallocated_chunk;
        return ptr;
    }


    curr_pos = curr_pos + sizeof(block_header_t) + size;
    block_header_t* new_header = (block_header_t*)curr_pos;
    new_header->status = BLOCK_UNALLOCATED;
    new_header->block_len = unallocated_chunk - header->block_len - sizeof(block_header_t);

    return ptr;
}

void setup_heap()
{
    char* ptr = (char*)&map_entries_size;
    ptr+= 0x10000;
    uint16_t count = *(uint16_t*)ptr;

    ptr = (char*)&map_entries;
    ptr += 0x10000;
    memory_map_entry_t* zone_entries = (memory_map_entry_t*)ptr;


    char* base_addr_of_largest_chunk;
    uint32_t addr_length = 0;
    int i;
    for(i =0; i < count; i++)
    {
        memory_map_entry_t* entry = &zone_entries[i];
        if(  entry->high_base_address > 0 // skip addr range above 4GB
            || entry->low_base_address < 0x9fc00  // skip area 0x00 - 0x9fc00
            || entry->range_type != AddressRangeMemory) // allow only usable memory for heap
        {
            continue;
        }

        if(entry->low_length >  addr_length)
        {
            base_addr_of_largest_chunk = (char*) entry->low_base_address;
            addr_length = entry->low_length; // if length is beyond 4GB trim it
        }

    }
    
    if( addr_length == 0)
    {
        print_str("ERROR: no free memory for heap", 1, 0);
        while (1){}
    }
    heap_base = (char*) base_addr_of_largest_chunk;
    heap_top = (char*) base_addr_of_largest_chunk + addr_length;

    block_header_t* heap_curr;
    heap_curr = (block_header_t*) base_addr_of_largest_chunk;
    heap_curr->status = BLOCK_UNALLOCATED;
    heap_curr->block_len = addr_length - sizeof(block_header_t);
}
void init_memory_subsystem()
{
    setup_heap();
    setup_memory_zones();
}

void *kmalloc()
{
    return 0x00;
}