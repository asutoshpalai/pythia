#!/bin/sh

gcc -m32 -ffreestanding -c kernel.c -o kernel.o

nasm kernel_entry.s -f elf -o kernel_entry.o

ld -o kernel.bin --oformat binary -m elf_i386 -Ttext 0x1000 kernel_entry.o kernel.o
