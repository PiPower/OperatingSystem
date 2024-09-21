#include "../memory_mappings.h"
#include "../memory_layout.h"
#include "vga.h"
#include "./memory/memory.h"

const char* msg = "Hello world from THE kernel!!";

void  main() 
{
    init_memory_subsystem();
    setup_paging();
    //char* xd =(char* ) PAGE_TABLE_ADDR;
    //*xd = 0xaa;

    int i =0;
    while (msg[i] != '\0')
    {
        print_at(msg[i], 0, i) ;
        i++;
    }

    print_memory_zones(1);

    while (1)
    {
      
    }
    
    return;
}