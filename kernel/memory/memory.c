#include "memory.h" 
#include "acpi.h"
#include "../vga.h"
#include "../../memory_mappings.h"
#include "memory_internal.h"
#include "paging.h"

const char* heap_base;
const char* heap_top;
char* page_journal; // keeps track of which pages are available in the system

void setup_memory_zones()
{

    rsdp_t* rsdp =  find_rsdp();
    parse_system_descriptor_table(rsdp);

    page_journal = heap_kmalloc(1048576);
    *page_journal++ = 0xaf;
    *page_journal = 0x99;
}

void* allocate_block(uint32_t size, block_header_t* header)
{
    char* curr_pos = (char*)header;
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
    return allocate_block(size, header);
}

unsigned int can_allocate_page(block_header_t* header)
{
    uint32_t header_mem = (uint32_t)header;
    uint32_t header_block_start = header_mem + sizeof(block_header_t);
    if(header_block_start%PAGE_SIZE == 0 && header->block_len >= PAGE_SIZE)
    {
        return CAN_ALLOCATE_PERFECT_FIT;
    }
    uint32_t page_aligned_addr = (header_block_start + PAGE_SIZE + sizeof(block_header_t)) & 0xFFFFF000; // get addres of the next page
    uint32_t header_addr = page_aligned_addr - sizeof(block_header_t); // dont forget about header
    if(page_aligned_addr + PAGE_SIZE - header_block_start <= header->block_len)
    {
        return CAN_ALLOCATE;
    }

    return CANNOT_ALLOCATE;
}

void *heap_kmalloc_aligned(uint32_t size, uint32_t alignment)
{
    char* curr_pos = heap_base;
    block_header_t* header;
    int ret_code;
    while (curr_pos < heap_top )
    {
        header = (block_header_t*)curr_pos;
        ret_code = can_allocate_page(header);
        if(header->status == BLOCK_UNALLOCATED && ret_code != CANNOT_ALLOCATE )
        {
            break;
        }
        curr_pos += sizeof(block_header_t) + header->block_len;
    }

    if(curr_pos >= heap_top)
    {
        return 0x00;
    }

    if(ret_code == CAN_ALLOCATE_PERFECT_FIT)
    {
        return heap_kmalloc(PAGE_SIZE);
    } 
    uint32_t header_block_start = (uint32_t)curr_pos + sizeof(block_header_t);
    uint32_t page_aligned_addr = (header_block_start + PAGE_SIZE + sizeof(block_header_t)) & 0xFFFFF000;
    // create new block which can be allocated using allocate_block
    block_header_t* new_header = (block_header_t*)(page_aligned_addr - sizeof(block_header_t));
    new_header->status + BLOCK_UNALLOCATED;
    new_header->block_len = (header_block_start + header->block_len) -(page_aligned_addr - sizeof(block_header_t));

    header->block_len = (page_aligned_addr - sizeof(block_header_t)) - header_block_start;

    return allocate_block(PAGE_SIZE, new_header);
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
    heap_base = base_addr_of_largest_chunk;
    heap_top = base_addr_of_largest_chunk + addr_length;

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