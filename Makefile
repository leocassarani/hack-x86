.PHONY: all clean

all: kernel

kernel: link.ld kernel.o vbe.o loader.o
	ld -T link.ld -m elf_i386 -o kernel loader.o kernel.o vbe.o

kernel.o: kernel.c vbe.o loader.o stdint.h
	gcc -m32 -c kernel.c -o kernel.o

vbe.o: vbe.c vbe.h io.h
	gcc -m32 -c vbe.c -o vbe.o

loader.o: loader.s
	nasm -f elf32 loader.s -o loader.o

clean:
	rm -f kernel kernel.o loader.o vbe.o
