.PHONY: all clean

all: kernel

kernel: kernel.o
	ld -m elf_i386 -T link.ld -o kernel loader.o kernel.o

kernel.o: loader.o
	gcc -m32 -c kernel.c -o kernel.o

loader.o:
	nasm -f elf32 loader.s -o loader.o

clean:
	rm -f kernel kernel.o loader.o
