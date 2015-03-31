#ifndef __PCI_H
#define __PCI_H

#include "stdint.h"

uint32_t pci_readl(uint8_t bus, uint8_t slot, uint8_t func);

#endif
