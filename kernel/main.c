#include "../memory_mappings.h"
#include "../memory_layout.h"
#include "vga.h"

const char* msg = "Hello world from THE kernel!!";

void main()
{
    char* video_mem = (char*)CHAR_ADDRESS;
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