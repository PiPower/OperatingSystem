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
void print_at(char c, uint8_t row, uint8_t col)
{
    char* video_mem = (char*)CHAR_ADDRESS;
    video_mem[2 * (row * MAX_COLS + col)] = c;
}

// print number in hex
void printh_uint(uint32_t number, uint8_t start_row, uint8_t start_column, char add_prefix)
{
    uint8_t row = start_row;
    uint8_t col = start_column + 2;
    if( col >= MAX_COLS){ col = 0; row++;}
    if( row >= MAX_ROWS){ row = 0;}
    if(add_prefix)
    {
        print_str("0x", start_row , start_column);
    }
    int i =0;
    while (i < 8 )
    {
        uint32_t index = ((number << (i * 4) )& 0xF0000000) >> 28;

        char c = numbers[index];
        print_at(c, row, col);

        col++;
        if( col >= MAX_COLS)
        {
            col = 0;
            row++;
            if( row >= MAX_ROWS)
            {
                row = 0;
            }
        }
        i++;
    }
    
}


void print_str(char* str, uint8_t start_row, uint8_t start_column)
{

    uint8_t row = start_row;
    uint8_t col = start_column;
    if( col >= MAX_COLS){ col = 0; row++;}
    if( row >= MAX_ROWS){ row = 0;}

    int i =0;
    while (str[i] != '\0' )
    {
        print_at(str[i], row, col);

        col++;
        if( col >= MAX_COLS)
        {
            col = 0;
            row++;
            if( row >= MAX_ROWS)
            {
                row = 0;
            }
        }
        i++;
    }
}

void print_memory_zones(uint8_t start_row)
{
    char* ptr = (char*)&map_entries_size;
    ptr+= 0x10000;
    uint16_t count = *(uint16_t*)ptr;

    ptr = (char*)&map_entries;
    ptr += 0x10000;
    memory_map_entry_t* zone_entries = (memory_map_entry_t*)ptr;

    uint8_t start_column = 0;
    print_str("There are ", start_row, start_column);
    printh_uint(count, start_row, start_column + 10, 1);
    print_str(" memory zones.", start_row, start_column + 20 );

    int i;
    for(i =0; i < count; i++)
    {
        printh_uint(zone_entries[i].high_base_address, start_row + i + 1, start_column, 1);
        printh_uint(zone_entries[i].low_base_address, start_row + i + 1, start_column + 8, 0);
        print_at('-', start_row + i + 1, start_column + 18);
        printh_uint(zone_entries[i].high_length, start_row + i + 1, start_column + 19, 1);
        printh_uint(zone_entries[i].low_length, start_row + i + 1, start_column + 27, 0);

        if(zone_entries[i].range_type > 0 && zone_entries[i].range_type <= AddressRangeDisabled)
        {
            print_str(addr_desc_type[zone_entries[i].range_type], start_row + i + 1, start_column + 37 );
        }
        else
        {
            print_str(addr_desc_type[0], start_row + i + 1, start_column + 37 );
        }
    }

}