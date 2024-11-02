#ifndef IO
#define IO
#include "types.h"

// memory mapped IO

static uint16_t mmio_inb(uint32_t addr)
{
    uint8_t result;
    asm("movb 0x00(%1), %0" : "=r"(result) : "r"(addr) );
    return result;
}

static uint16_t mmio_inw(uint32_t addr)
{
    uint16_t result;
    asm("movw 0x00(%1), %0" : "=r"(result) : "r"(addr) );
    return result;
}

// port IO
static inline uint8_t pio_inb(int port)
{
	uint8_t result;
    asm("inb %w1, %b0": "=a"(result):"Nd"(port));
	return result;
}

static inline uint16_t pio_inw(int port)
{
	uint16_t result;
    asm("inw %w1, %w0": "=a"(result):"Nd"(port));
	return result;
}

static inline uint32_t pio_inl(int port)
{
	uint32_t result;
    asm("inl %w1, %0": "=a"(result):"Nd"(port));
	return result;
}

static inline void pio_outb(uint8_t value, int port)
{
    asm("outb %b0, %w1": :"a"(value), "Nd"(port));
}

static inline void pio_outw(uint16_t value, int port)
{
    asm("outw %w0, %w1": :"a"(value), "Nd"(port));
}

static inline void pio_outl(uint32_t value, int port)
{
    asm("outl %0, %w1": :"a"(value), "Nd"(port));
}

#endif