#include "hack.h"
#include "intr.h"
#include "keyboard.h"
#include "program.h"
#include "screen.h"
#include "stdint.h"

#define RAM_SIZE    16384
#define SCREEN_SIZE 8192

static uint16_t hack_ram[RAM_SIZE];
static uint16_t hack_screen[SCREEN_SIZE];

void kmain(void)
{
    screen_init();
    intr_init();
    keyboard_init();

    intr_enable();

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
