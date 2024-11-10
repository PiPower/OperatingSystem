#include "../memory_mappings.h"
#include "../memory_layout.h"
#include "vga.h"
#include "./memory/memory.h"
#include "device/pci.h"

const char* msg = "Hello world from THE kernel!!\n";

extern uint32_t __start_data_section;

void  main() 
{
    clear_screen(); // usefull for emulator like qemu that edits video mem
    print(msg);
    print_memory_zones();
    // initialization part of the kernel
    init_memory_subsystem(); 
    //setup_paging();
    start_device_subsystem();
    //char* xd =(char* ) PAGE_TABLE_ADDR;
    //*xd = 0xaa;


    //print_memory_zones(1);

    while (1)
    {
      
    }
    
    return;
}