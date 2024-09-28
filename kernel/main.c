#include "../memory_mappings.h"
#include "../memory_layout.h"
#include "vga.h"
#include "./memory/memory.h"

const char* msg = "Hello world from THE kernel!!\n";

void  main() 
{
    clear_screen(); // usefull for emulator like qemu that edits video mem
    print(msg);
    init_memory_subsystem();
    setup_paging();
    //char* xd =(char* ) PAGE_TABLE_ADDR;
    //*xd = 0xaa;


    //print_memory_zones(1);

    while (1)
    {
      
    }
    
    return;
}