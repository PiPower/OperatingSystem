#ifndef VGA_DRIVER
#define VGA_DRIVER
#include "types.h"

#define VIDEO_BASE_ADDRESS 0xA0000
#define CHAR_ADDRESS 0xB8000
#define MAX_ADDRESS 0xBFFFF

#define MAX_ROWS 25
#define MAX_COLS 80


void print_at(char c, uint8_t row, uint8_t col);
void printh_uint(uint32_t number, uint8_t start_row, uint8_t start_column, char add_prefix);
void print_str(char* str, uint8_t start_row, uint8_t start_column);
void print_memory_zones(uint8_t start_row);
#endif