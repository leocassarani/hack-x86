#include "hack.h"
#include "screen.h"
#include "stdint.h"

#define RAM_SIZE    (1 << 14)
#define SCREEN_SIZE (1 << 13)

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

static uint16_t hack_ram[RAM_SIZE];
static uint16_t hack_screen[SCREEN_SIZE];

void kmain(void)
{
    screen_init();

    struct hack_memory memory = {
        .ram = hack_ram,
        .screen = hack_screen
    };

    struct hack_computer computer;
    hack_init(&computer, &memory, hack_program);

    while (1)
        hack_step(&computer);

    return;
}
