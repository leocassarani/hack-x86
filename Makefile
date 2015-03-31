.PHONY: all clean

all: kernel

kernel: link.ld kernel.o vbe.o pci.o loader.o
	ld -T link.ld -m elf_i386 -o kernel loader.o kernel.o vbe.o pci.o io.o

kernel.o: kernel.c vbe.o loader.o stdint.h
	gcc -m32 -c kernel.c -o kernel.o

vbe.o: vbe.c vbe.h io.o
	gcc -m32 -c vbe.c -o vbe.o

pci.o: pci.c pci.h io.o
	gcc -m32 -c pci.c -o pci.o

io.o: io.c io.h stdint.h
	gcc -m32 -c io.c -o io.o

loader.o: loader.s
	nasm -f elf32 loader.s -o loader.o

clean:
	rm -f kernel kernel.o vbe.o loader.o
