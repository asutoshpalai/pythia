LD=i686-elf-ld
CC=i686-elf-gcc

all: os-image

os-image: boot_sector.bin kernel.bin
	cat $^ > $@

boot_sector.bin: boot_sector.s
	nasm $< -f bin -o $@

kernel.bin: kernel_entry.o kernel.o vga/vga.o
	$(LD) -o $@ -shared --oformat binary -ffreestanding -O2 -nostdlib -m elf_i386 -Ttext 0x1000 $^ 

%.o: %.c
	$(CC) -m32 -ffreestanding -c $< -o $@

%.o: %.s
	nasm $< -f elf -o $@

rebuild: clean all

run: os-image
	qemu-system-i386 -drive format=raw,file=$<

rerun: clean run

clean:
	rm *.o *.bin
