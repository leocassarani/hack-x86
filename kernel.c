#include "hack.h"
#include "pci.h"
#include "stdint.h"
#include "vbe.h"

#define XRES 512
#define YRES 256

uint16_t hack_program[] = {
    0b0000000000000000,
    0b1111110000010000,
    0b0000000000010111,
    0b1110001100000110,
    0b0000000000010000,
    0b1110001100001000,
    0b0100000000000000,
    0b1110110000010000,
    0b0000000000010001,
    0b1110001100001000,
    0b0000000000010001,
    0b1111110000100000,
    0b1110111010001000,
    0b0000000000010001,
    0b1111110000010000,
    0b0000000000100000,
    0b1110000010010000,
    0b0000000000010001,
    0b1110001100001000,
    0b0000000000010000,
    0b1111110010011000,
    0b0000000000001010,
    0b1110001100000001,
    0b0000000000010111,
    0b1110101010000111
};

static uint16_t hack_ram[16384];

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

    struct hack_memory memory = {
        .ram = hack_ram
    };

    struct hack_computer computer;
    hack_init(&computer, &memory, hack_program);

    while (1)
        hack_step(&computer);

    return;
}
