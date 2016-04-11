#!/bin/sh


./compile_kernel.sh

nasm boot_sector.s -f bin -o boot_sector.bin

cat boot_sector.bin kernel.bin > os-image

