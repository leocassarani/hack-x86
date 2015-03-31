#include "io.h"

uint16_t inw(unsigned port)
{
    uint16_t data;
    asm volatile ("inw %w1, %w0" : "=a" (data) : "Nd" (port));
    return data;
}

void outw(uint16_t data, unsigned port)
{
    asm volatile ("outw %w0, %w1" : : "a" (data), "Nd" (port));
}

uint32_t inl(unsigned port)
{
    uint32_t data;
    asm volatile ("inl %w1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

void outl(uint32_t data, unsigned port)
{
    asm volatile ("outl %0, %w1" : : "a" (data), "Nd" (port));
}
