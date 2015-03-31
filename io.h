#ifndef __IO_H
#define __IO_H

#include "stdint.h"

static inline uint16_t inw(unsigned port)
{
    uint16_t data;
    asm volatile ("inw %w1, %w0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void outw(uint16_t data, unsigned port)
{
    asm volatile ("outw %w0, %w1" : : "a" (data), "Nd" (port));
}

static inline uint32_t inl(unsigned port)
{
    uint32_t data;
    asm volatile ("inl %w1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void outl(uint32_t data, unsigned port)
{
    asm volatile ("outl %0, %w1" : : "a" (data), "Nd" (port));
}

#endif
