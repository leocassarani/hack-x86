#ifndef __IO_H
#define __IO_H

#include "stdint.h"

uint16_t inw(unsigned port);
void outw(uint16_t data, unsigned port);

uint32_t inl(unsigned port);
void outl(uint32_t data, unsigned port);

#endif
