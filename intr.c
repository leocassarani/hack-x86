#include "intr.h"
#include "io.h"

// Number of interrupts.
#define IDT_SIZE 256

// Master PIC.
#define PIC0_CTRL  0x20
#define PIC0_DATA  0x21
#define PIC0_START 0x20
#define PIC0_END   0x27

// Slave PIC.
#define PIC1_CTRL  0xA0
#define PIC1_DATA  0xA1
#define PIC1_START 0x28
#define PIC1_END   0x30

// Interrupt acknowledgment sequence.
#define PIC_ACK 0x20

// Kernel code segment offset selector.
#define SEL_KCSEG 0x08

#define TYPE_INTR 14
#define TYPE_TRAP 15

// Interrupt Descriptor Table (IDT).
static uint64_t idt[IDT_SIZE];

// Interrupts masks for each of the two PICs.
static uint8_t intr_masks[2] = { 0xFF, 0xFF };
static void intr_unmask(uint8_t intr_no);

static void pic_init(void);
static void idt_load(void);

static void register_handler(uint64_t *addr, intr_handler_func *handler, int dpl, int type);

void intr_init(void)
{
    pic_init();
    idt_load();
}

void intr_enable(void)
{
    asm volatile ("sti");
}

void intr_disable(void)
{
    asm volatile ("cli" : : : "memory");
}

void intr_register(uint8_t intr_no, intr_handler_func *handler)
{
    register_handler(&idt[intr_no], handler, 0, TYPE_INTR);
    intr_unmask(intr_no);
}

void intr_acknowledge(uint8_t intr_no)
{
    if (intr_no < PIC0_START || intr_no >= PIC1_END)
        return;

    // Route the acknowledgment to either the master or slave
    // PIC, depending on which interrupt we're acknowledging.
    if (intr_no < PIC1_START)
        outb (PIC0_CTRL, PIC_ACK);
    else
        outb (PIC1_CTRL, PIC_ACK);
}

static void pic_init(void)
{
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

    // Mask interrupts on both PICs.
    outb(PIC0_DATA, intr_masks[0]);
    outb(PIC1_DATA, intr_masks[1]);
}

static void idt_load(void)
{
    uint16_t size = sizeof idt - 1;
    uint64_t idtr_operand = size | ((uint64_t) (uint32_t) idt << 16);
    asm volatile ("lidt %0" : : "m" (idtr_operand));
}

static void register_handler(uint64_t *addr, intr_handler_func *handler, int dpl, int type)
{
    uint32_t e0, e1;

    e0 = (((uint32_t) handler & 0xFFFF)
          | (SEL_KCSEG << 16));

    e1 = (((uint32_t) handler & 0xFFFF0000)
          | (1 << 15)
          | ((uint32_t) dpl << 13)
          | (0 << 12)
          | ((uint32_t) type << 8));

    *addr = e0 | ((uint64_t) e1 << 32);
}

static void intr_unmask(uint8_t intr_no)
{
    uint8_t irq = intr_no - PIC0_START;

    if (intr_no < PIC1_START)
    {
        // Zero out the nth bit, where n = irq.
        intr_masks[0] &= ~(1 << irq);
        outb(PIC0_DATA, intr_masks[0]);
    }
    else
    {
        irq -= (PIC1_START - PIC0_START);
        intr_masks[1] &= ~(1 << irq);
        outb(PIC1_DATA, intr_masks[1]);
    }
}
