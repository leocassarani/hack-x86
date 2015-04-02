#ifndef __HACK_H
#define __HACK_H

#include "stdint.h"

struct hack_computer {
  uint16_t A, D, PC;
  uint16_t *program;
  struct hack_memory *memory;
};

struct hack_memory {
  uint16_t *ram;
};

void hack_init(struct hack_computer *comp, struct hack_memory *memory, uint16_t *program);
void hack_step(struct hack_computer *comp);

#endif
