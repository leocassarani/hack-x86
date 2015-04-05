#include "intr.h"
#include "io.h"

// Number of interrupts.
#define IDT_SIZE 256

// Master PIC registers.
#define PIC0_CTRL 0x20
#define PIC0_DATA 0x21

// Slave PIC registers.
#define PIC1_CTRL 0xA0
#define PIC1_DATA 0xA1

// Interrupt Descriptor Table (IDT).
static uint64_t idt[IDT_SIZE];

static void pic_init(void);
static void idt_load(void);

void intr_init(void)
{
    pic_init();
    idt_load();
}

static void pic_init(void)
{
    // Mask interrupts on both PICs.
    outb(PIC0_DATA, 0xFF);
    outb(PIC1_DATA, 0xFF);

    // Initialize master PIC.
    outb(PIC0_CTRL, 0x11); // ICW1
    outb(PIC0_DATA, 0x20); // ICW2: Remap IRQ0...7 to 0x20...0x27
    outb(PIC0_DATA, 0x04); // ICW3: Cascade on line IR2
    outb(PIC0_DATA, 0x01); // ICW4

    // Initialize slave PIC.
    outb(PIC1_CTRL, 0x11); // ICW1
    outb(PIC1_DATA, 0x28); // ICW2: Remap IRQ8...15 to 0x28...0x2F
    outb(PIC1_DATA, 0x02); // ICW3: Slave ID = 2
    outb(PIC1_DATA, 0x01); // ICW4
}

static void idt_load(void)
{
    uint16_t size = sizeof idt - 1;
    uint64_t idtr_operand = size | ((uint64_t) (uintptr_t) idt << 16);
    asm volatile ("lidt %0" : : "m" (idtr_operand));
}
