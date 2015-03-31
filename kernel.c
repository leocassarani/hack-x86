#include "stdint.h"
#include "pci.h"
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

    uint32_t lfb_base = pci_readl(0, 2, 0);
    char *vidptr = (char *)VBE_DISPI_LFB_PHYSICAL_ADDRESS;
    unsigned i = 0;

    uint32_t *myptr = (uint32_t *)0xbeef;
    *myptr = lfb_base;

    char color = (lfb_base < VBE_DISPI_LFB_PHYSICAL_ADDRESS) ? 0x03: 0x3f;

    while (i < XRES * YRES)
        vidptr[i++] = color; // White = 0b00001111

    return;
}
