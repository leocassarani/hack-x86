.PHONY: all clean

OBJECTS = kernel.o hack.o loader.o pci.o screen.o vbe.o

all: kernel

kernel: link.ld $(OBJECTS)
	ld -T link.ld -m elf_i386 -o kernel $(OBJECTS)

%.o: %.c
	gcc -m32 -c $< -o $@

%.o: %.s
	nasm -f elf32 $< -o $@

clean:
	rm -f kernel $(OBJECTS)
