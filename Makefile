all: os-image

os-image: boot_sector.bin kernel.bin
	cat $^ > $@

boot_sector.bin: boot_sector.s
	nasm $< -f bin -o $@

kernel.bin: kernel_entry.o kernel.o
	ld -o $@ --oformat binary -m elf_i386 -Ttext 0x1000 $^

kernel.o: kernel.c
	gcc -m32 -ffreestanding -c $< -o $@

kernel_entry.o: kernel_entry.s
	nasm $< -f elf -o $@

rebuild: clean all

run: os-image
	qemu-system-i386 -drive format=raw,file=$<

rerun: clean run

clean:
	rm *.o *.bin
