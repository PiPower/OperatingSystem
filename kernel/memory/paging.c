#include "paging.h"
#include "../types.h"
#include "../vga.h"
/*
    bit indicies of specified options within PDE/PTE
    every option except for available(4 bit) is 1 bit long
    prefix D corresponds to Page Directory Entry and T to 
    Page Table Entry

    Refrence: Intel combined manual volume 3(A,B,C,D) chapter 4 paging
*/
#define D_PRESENT 0
#define D_READ_WRITE 1
#define D_USER_SUPERVISOR 2
#define D_WRITE_THROUGH 3
#define D_CACHE_DISABLE 4
#define D_ACCESSED 5
#define D_IGNORED_LOW 6 // 1 bit lenght
#define D_PAGE_SIZE 7
#define D_IGNORED_HIGH 8 // bits [8,11]
#define D_ADDRESS 12

#define SET_D_PRESENT (0x01 << D_PRESENT) 
#define SET_D_READ_WRITE (0x01 << D_READ_WRITE) 
#define SET_D_USER_SUPERVISOR (0x01 << D_USER_SUPERVISOR) 
#define SET_D_WRITE_THROUGH (0x01 << D_WRITE_THROUGH) 
#define SET_D_CACHE_DISABLE (0x01 << D_CACHE_DISABLE) 
#define SET_D_ACCESSED (0x01 << D_ACCESSED) 
#define SET_D_PAGE_SIZE (0x01 << D_PAGE_SIZE) 
#define SET_D_ADDRESS(addr) (addr & 0xFFFFF000) 


#define T_PRESENT 0
#define T_READ_WRITE 1
#define T_USER_SUPERVISOR 2
#define T_WRITE_THROUGH 3
#define T_CACHE_DISABLE 4
#define T_ACCESSED 5
#define T_DIRTY 6 
#define T_PAGE_ATTRIBUTE_TABLE 7
#define T_GLOBAL 8 
#define T_IGNORED 9 // [9, 11]
#define T_ADDRESS 12

#define SET_T_PRESENT (0x01 << T_PRESENT) 
#define SET_T_READ_WRITE (0x01 << T_READ_WRITE) 
#define SET_T_USER_SUPERVISOR (0x01 << T_USER_SUPERVISOR) 
#define SET_T_WRITE_THROUGH (0x01 << T_WRITE_THROUGH) 
#define SET_T_CACHE_DISABLE (0x01 << T_CACHE_DISABLE) 
#define SET_T_ACCESSED (0x01 << T_ACCESSED) 
#define SET_T_DIRTY (0x01 << T_DIRTY)
#define SET_T_PAGE_ATTRIBUTE_TABLE (0x01 << T_PAGE_ATTRIBUTE_TABLE) 
#define SET_T_GLOBAL (0x01 << T_GLOBAL) 
#define SET_T_ADDRESS(addr) (addr & 0xFFFFF000) 

typedef uint32_t page_table_t;
typedef uint32_t page_dir_t;

static void start_paging(uint32_t page_dir)
{
    asm volatile("movl %0, %%cr3" :: "r"(page_dir));
    asm volatile("movl %cr0, %eax");
    asm volatile("orl $0x80000000, %eax") ;
    asm volatile("movl %eax, %cr0");
}

void setup_paging()
{
    page_dir_t* page_dir =  (page_dir_t*) PAGE_DIR_ADDR;
    page_dir_t* page_dir_bound =  (page_dir_t*)(PAGE_DIR_ADDR + PAGE_SIZE);
    while (page_dir < page_dir_bound)
    {
        *page_dir = 0x00; // set page  dir to nonpresent
        page_dir += 1;
    }
    
    page_table_t* page_table = (page_table_t*)PAGE_TABLE_ADDR;
    page_table_t* page_table_bound = (page_table_t*)(PAGE_TABLE_ADDR + PAGE_SIZE);
    while (page_table < page_table_bound)
    {
        *page_table = 0x00; // set page  table  to nonpresent
        page_table += 1;
    }    


    page_dir =  (page_dir_t*) PAGE_DIR_ADDR;
    // to keep global state consistent for now disable caching and set write through
    *page_dir =  SET_D_ADDRESS(PAGE_TABLE_ADDR) | SET_D_WRITE_THROUGH  | SET_D_READ_WRITE | SET_D_PRESENT;

    uint32_t const_flags = 0;
    const_flags = SET_T_WRITE_THROUGH | SET_T_READ_WRITE | SET_T_PRESENT;
    page_table = (page_table_t*)PAGE_TABLE_ADDR;

    // map 1:1 addresses in range 0:0x00102000
    uint32_t base_addr = 0;
    while (base_addr < PAGE_TABLE_ADDR + PAGE_SIZE)
    {
        *page_table = const_flags | SET_T_ADDRESS(base_addr);

        base_addr += PAGE_SIZE;
        page_table += 1;
    }

    page_table -=1 ;
    *page_table = const_flags | SET_T_ADDRESS(0);

    start_paging(PAGE_DIR_ADDR);
    return;
}   