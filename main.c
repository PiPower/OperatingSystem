#include "memory_mappings.h"
#include "memory_layout.h"
#define VIDEO_ADDRESS 0xB8000

const char* msg = "hello world from THE kernel!!";

void main()
{

    char* video_mem = (char*)VIDEO_ADDRESS;
    int i =0;
    while (msg[i] != '\0')
    {
        video_mem[2 * i] = msg[i];
        i++;
    }
    
    while (1)
    {
      
    }
    
    return;
}