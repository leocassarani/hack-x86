#ifndef __INTR_H
#define __INTR_H

#include "stdint.h"

typedef void intr_handler_func(void);

void intr_init(void);

void intr_enable(void);
void intr_disable(void);

void intr_register(uint8_t intr_no, intr_handler_func *handler);
void intr_acknowledge(uint8_t intr_no);

#endif
