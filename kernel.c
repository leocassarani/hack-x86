#include "pci.h"
#include "stdint.h"
#include "vbe.h"

#define XRES 512
#define YRES 256

void kmain(void)
{
    vbe_write(VBE_DISPI_INDEX_ID, VBE_DISPI_ID3);
    vbe_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);

    // Set X and Y resolution.
    vbe_write(VBE_DISPI_INDEX_XRES, XRES);
    vbe_write(VBE_DISPI_INDEX_YRES, YRES);

    // Use 8 BPP for simplicity: each pixel is exactly one byte.
    vbe_write(VBE_DISPI_INDEX_BPP, VBE_DISPI_BPP_8);

    // Enable VBE in Linear Frame Buffer (LFB) mode.
    vbe_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);

    // Read the base address of the LFB by querying the PCI device.
    // Here we assume that bus = 0, slot = 2, func = 0. Offset is
    // set to 0x10, corresponding to BAR0. Note that the bottom 4
    // bits must be zeroed out to obtain the pixel at (0, 0).
    uintptr_t address = pci_read(0, 2, 0, 0x10) & 0xFFFFFFF0;
    uint8_t *vidptr = (uint8_t *)address;
    unsigned i = 0;

    while (i < XRES * YRES)
        vidptr[i++] = 0x3f; // White = 0b00001111

    return;
}
