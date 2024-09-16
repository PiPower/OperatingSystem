#include "memory_mappings.h"

# define VIDEO_ADDRESS 0xB8000

void main()
{
    char* video_mem = (char*) VIDEO_ADDRESS;
    *video_mem = 'd';
    while (1)
    {
      
    }
    
    return;
}