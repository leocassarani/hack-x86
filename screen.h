#ifndef __SCREEN_H
#define __SCREEN_H

#include "stdint.h"

/* Base address of the memory-mapped screen. */
uint8_t *screen_ptr;

void screen_init(void);
void screen_set_color(unsigned offset, uint8_t color);

#endif
