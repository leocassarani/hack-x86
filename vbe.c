#include "vbe.h"

static void outw(uint16_t word, unsigned port)
{
    asm volatile ("outw %w0, %w1" : : "a" (word), "Nd" (port));
}

static uint16_t inw(unsigned port)
{
    uint16_t word;
    asm volatile ("inw %w1, %w0" : "=a" (word) : "Nd" (port));
    return word;
}

uint16_t vbe_read(uint16_t index) {
    outw(index, VBE_DISPI_IOPORT_INDEX);
    return inw(VBE_DISPI_IOPORT_DATA);
}

void vbe_write(uint16_t index, uint16_t data) {
    outw(index, VBE_DISPI_IOPORT_INDEX);
    outw(data, VBE_DISPI_IOPORT_DATA);
}
