#ifndef TYPES
#define TYPES

// header supports only gcc
#define NULL 0x00

typedef int int32_t;
typedef short int int16_t;
typedef char int8_t;

typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;

typedef struct quadword 
{
    uint32_t low;
    uint32_t high;
} quadword_t;

typedef quadword_t quadword_ptr;
#endif