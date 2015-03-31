#include "io.h"
#include "pci.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

#define PCI_ENABLED (1 << 31)

static uint32_t pci_address(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;

    return PCI_ENABLED | (lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC);
}

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address = pci_address(bus, slot, func, offset);
    outl(address, PCI_CONFIG_ADDRESS);
    return inl(PCI_CONFIG_DATA);
}
