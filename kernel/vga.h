#ifndef VGA_DRIVER
#define VGA_DRIVER
#include "types.h"

#define VIDEO_BASE_ADDRESS 0xA0000
#define CHAR_ADDRESS 0xB8000
#define MAX_ADDRESS 0xBFFFF

#define MAX_ROWS 25
#define MAX_COLS 80

void printc(char c);
void clear_screen();
void printh(uint32_t number, char add_prefix);
void print(const char* str);
void print_memory_zones();
#endif