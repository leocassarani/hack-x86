OBJECTS = kernel.o hack.o intr.o loader.o pci.o screen.o vbe.o

.PHONY: all clean

all: kernel.elf

kernel.elf: link.ld $(OBJECTS)
	ld -T link.ld -m elf_i386 -o kernel.elf $(OBJECTS)

%.o: %.c
	gcc -m32 -c $< -o $@

%.o: %.s
	nasm -f elf32 $< -o $@

clean:
	rm -f kernel.elf $(OBJECTS)
