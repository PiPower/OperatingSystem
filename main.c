#include "memory_mappings.h"
#include "memory_layout.h"
#define VIDEO_ADDRESS 0xB8000

void main()
{
    const char* msg = "hello from kernel";
    
    char* real_msg = msg + 0x10000;  //  0x10259 
    char* video_mem = (char*)VIDEO_ADDRESS;

    int i =0;
    while (real_msg[i] != '\0')
    {
        video_mem[2 * i] = real_msg[i];
        i++;
    }
    
    while (1)
    {
      
    }
    
    return;
}