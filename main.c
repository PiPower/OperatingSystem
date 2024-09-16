#include "memory_mappings.h"
#include "memory_layout.h"
# define VIDEO_ADDRESS 0xB8000


void main()
{
    char entry_msg[] = { 'h', 'e', 'l', 'l', 'o',' ', 'f', 'r', 'o', 'm', ' ', 'k', 'e', 'r', 'n',  'e', 'l', '\0'};
    char* video_mem = (char*)VIDEO_ADDRESS;

    int i =0;
    while (entry_msg[i] != '\0')
    {
        video_mem[2 * i] = entry_msg[i];
        i++;
    }
    
    while (1)
    {
      
    }
    
    return;
}