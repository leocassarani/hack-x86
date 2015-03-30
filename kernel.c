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

    return;
}
