#include "vga.h"
#include "../memory_mappings.h"
const static char* numbers = "0123456789ABCDEF";

const char* addr_desc_type[] ={ 
    " Undefined" ,
    " AddressRangeMemory",
    " AddressRangeReserved",
    " AddressRangeACPI",
    " AddressRangeNVS",
    " AddressRangeUnusuable",
    " AddressRangeUnusuable",
    " AddressRangeDisabled"
};

int curr_row = 0;
int curr_col = 0;
char* video_mem = (char*)CHAR_ADDRESS;

void scroll()
{
    int y, x;
    for(y=0; y < MAX_ROWS - 1; y++)
    {
        for(x =0; x < MAX_COLS; x++)
        {
            video_mem[2 * (y * MAX_COLS + x)] =  video_mem[2 * ((y + 1) * MAX_COLS + x)];
        }
    }   

    for(x =0; x < MAX_COLS; x++)
    {
        video_mem[2 * (y * MAX_COLS + x)] =  ' '; // clear next row
    }

}
void printc(char c)
{
    if( curr_row >= MAX_ROWS)
    { 
        curr_row = MAX_ROWS - 1;
        scroll();
    }
    if( c == '\n')
    {
        curr_col = 0;
        curr_row++;
        return;
    }

    video_mem[2 * (curr_row * MAX_COLS + curr_col)] = c;
    curr_col++;
    if( curr_col >= MAX_COLS)
    { 
        curr_col = 0; 
        curr_row++;
    }
}

void clear_screen()
{
    int y, x;
    for(y =0; y < MAX_ROWS; y++)
    {
        for(x =0; x < MAX_COLS; x++)
        {
            video_mem[2 * (y * MAX_COLS + x)] = ' ';
        }
    }
    curr_col = 0;
    curr_row = 0;
}

// print number in hex
void printh(uint32_t number, char add_prefix)
{
    if(add_prefix)
    {
        print("0x");
    }

    int i =0;
    while (i < 8 )
    {
        uint32_t index = ((number << (i * 4) )& 0xF0000000) >> 28;

        char c = numbers[index];
        printc(c);
        i++;
    }
    
}


void print(const char* str)
{
    int i =0;
    while (str[i] != '\0' )
    {
        printc(str[i]);
        i++;
    }
}

void print_memory_zones()
{
    char* ptr = (char*)&map_entries_size;
    ptr+= 0x10000;
    uint16_t count = *(uint16_t*)ptr;

    ptr = (char*)&map_entries;
    ptr += 0x10000;
    memory_map_entry_t* zone_entries = (memory_map_entry_t*)ptr;

    print("There are ");
    printh(count, 1);
    print(" memory zones\n");

    int i;
    for(i =0; i < count; i++)
    {
        printh(zone_entries[i].high_base_address, 1);
        printc(':');
        printh(zone_entries[i].low_base_address, 0);
        printc('-');
        printh(zone_entries[i].high_length, 1);
        printc(':');
        printh(zone_entries[i].low_length, 0);

        if(zone_entries[i].range_type > 0 && zone_entries[i].range_type <= AddressRangeDisabled)
        {
            print(addr_desc_type[zone_entries[i].range_type]);
            printc('\n');
        }
        else
        {
            print(addr_desc_type[0]);
            printc('\n');
        }
    }

}