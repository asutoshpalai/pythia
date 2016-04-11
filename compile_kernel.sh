#!/bin/sh

gcc -ffreestanding -c kernel.c -o kernel.o

ld -o kernel.bin --oformat binary -Ttext 0x1000 kernel.o 
