#include "intr.h"
#include "io.h"
#include "keyboard.h"
#include "stdbool.h"

#define KBD_INTR_NO   0x21
#define KBD_DATA_PORT 0x60

static uint8_t current_char = 0;

extern intr_handler_func keyboard_intr_handler;

void keyboard_init(void)
{
    intr_register(KBD_INTR_NO, keyboard_intr_handler);
}

void keyboard_handler(void)
{
    intr_acknowledge(KBD_INTR_NO);

    uint8_t code = inb(KBD_DATA_PORT);
    bool release = (code & 0x80) != 0;

    // If the key was released rather than pressed,
    // clear the current character stored.
    if (release)
        current_char = 0;
    else
        current_char = code + 100;
}

uint8_t keyboard_get_current_char(void)
{
    return current_char;
}
