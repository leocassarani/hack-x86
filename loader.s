bits 32

global loader
global keyboard_intr_handler

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x00
CHECKSUM     equ -(MAGIC_NUMBER + FLAGS)

section .text
align 4
  dd MAGIC_NUMBER
  dd FLAGS
  dd CHECKSUM

extern kmain
extern keyboard_handler

loader:
  cli
  mov esp, stack_space
  call kmain
  hlt

keyboard_intr_handler:
  call keyboard_handler
  iret

section .bss
resb 8192

stack_space:
