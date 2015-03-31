#include "io.h"
#include "vbe.h"

uint16_t vbe_read(uint16_t index) {
    outw(index, VBE_DISPI_IOPORT_INDEX);
    return inw(VBE_DISPI_IOPORT_DATA);
}

void vbe_write(uint16_t index, uint16_t data) {
    outw(index, VBE_DISPI_IOPORT_INDEX);
    outw(data, VBE_DISPI_IOPORT_DATA);
}
