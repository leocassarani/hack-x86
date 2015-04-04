#include "hack.h"
#include "screen.h"
#include "stdbool.h"

#define I_MASK  (1 << 15)
#define A_MASK  (1 << 12)
#define C1_MASK (1 << 11)
#define C2_MASK (1 << 10)
#define C3_MASK (1 << 9)
#define C4_MASK (1 << 8)
#define C5_MASK (1 << 7)
#define C6_MASK (1 << 6)
#define D1_MASK (1 << 5)
#define D2_MASK (1 << 4)
#define D3_MASK (1 << 3)
#define J1_MASK (1 << 2)
#define J2_MASK (1 << 1)
#define J3_MASK (1)

#define MSB_MASK    (1 << 15)
#define RAM_MASK    (1 << 14)
#define SCREEN_MASK (1 << 13)

static void execute_C_inst(struct hack_computer *comp, uint16_t inst);
static void execute_A_inst(struct hack_computer *comp, uint16_t inst);

static uint16_t memory_get(struct hack_memory *memory, uint16_t addr);
static void memory_set(struct hack_memory *memory, uint16_t addr, uint16_t value);
static void screen_repaint(uint16_t *screen, uint16_t addr, uint16_t value);

struct alu_result {
    uint16_t output;
    bool zero, negative, positive;
};

static void alu_compute(struct alu_result *result, uint16_t x, uint16_t y, uint16_t inst);

void hack_init(struct hack_computer *comp, struct hack_memory *memory, uint16_t *program)
{
    comp->A = 0;
    comp->D = 0;
    comp->PC = 0;
    comp->memory = memory;
    comp->program = program;
}

void hack_step(struct hack_computer *comp)
{
    uint16_t inst = comp->program[comp->PC];

    if (inst & I_MASK)
        execute_C_inst(comp, inst);
    else
        execute_A_inst(comp, inst);
}

static void execute_C_inst(struct hack_computer *comp, uint16_t inst)
{
    uint16_t y;
    bool jump;

    if (inst & A_MASK)
        y = memory_get(comp->memory, comp->A);
    else
        y = comp->A;

    struct alu_result result;
    alu_compute(&result, comp->D, y, inst);

    if (inst & D3_MASK)
        memory_set(comp->memory, comp->A, result.output);
    if (inst & D2_MASK)
        comp->D = result.output;
    if (inst & D1_MASK)
        comp->A = result.output;

    jump = (inst & J1_MASK) && result.negative || // JLT
           (inst & J2_MASK) && result.zero     || // JEQ
           (inst & J3_MASK) && result.positive;   // JGT

    if (jump)
        comp->PC = comp->A;
    else
        comp->PC++;
}

static void execute_A_inst(struct hack_computer *comp, uint16_t inst)
{
    comp->A = inst;
    comp->PC++;
}

static void alu_compute(struct alu_result *result, uint16_t x, uint16_t y, uint16_t inst)
{
    if (inst & C1_MASK)
        x = 0;
    if (inst & C2_MASK)
        x = ~x;
    if (inst & C3_MASK)
        y = 0;
    if (inst & C4_MASK)
        y = ~y;

    if (inst & C5_MASK)
        result->output = x + y;
    else
        result->output = x & y;

    if (inst & C6_MASK)
        result->output = ~(result->output);

    result->zero = 0 == result->output;
    result->negative = result->output & (1 << 15);
    result->positive = !result->zero && !result->negative;
}


static uint16_t memory_get(struct hack_memory *memory, uint16_t addr)
{
    if (~addr & RAM_MASK)
        return memory->ram[addr];

    if (~addr & SCREEN_MASK)
    {
        addr = addr & (RAM_MASK - 1);
        return memory->screen[addr];
    }

    return 0;
}

static void memory_set(struct hack_memory *memory, uint16_t addr, uint16_t value)
{
    if (~addr & RAM_MASK)
    {
        memory->ram[addr] = value;
    }
    else if (~addr & SCREEN_MASK)
    {
        addr = addr & (RAM_MASK - 1);
        memory->screen[addr] = value;
        screen_repaint(memory->screen, addr, value);
    }
}

static void screen_repaint(uint16_t *screen, uint16_t addr, uint16_t value)
{
    unsigned offset = 16 * addr;
    uint8_t color;
    unsigned i = 0;

    for (; i < 16; i++)
    {
        color = (value & (1 << i)) ? 0x00 : 0x3f;
        screen_set_color(offset + i, color);
    }
}
