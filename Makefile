LD=i686-elf-ld
CC=i686-elf-gcc

KERNEL_OBJ_LIST:=\
kernel/kernel_entry.o \
kernel/kernel.o \
kernel/system.o \
kernel/string.o \
kernel/vga/vga.o \


all: build/os-image

build/os-image: boot/boot_sector.bin kernel/kernel.bin
	mkdir -p build
	cat $^ > $@

kernel/kernel.bin: $(KERNEL_OBJ_LIST)
	$(LD) -o $@ -shared --oformat binary -ffreestanding -O2 -nostdlib -m elf_i386 -Ttext 0x10000 $^

%.o: %.c
	$(CC) -m32 -ffreestanding -c $< -o $@

%.o: %.s
	nasm $< -I"$(shell dirname $<)/" -f elf -o $@

%.bin: %.s
	nasm $< -I"$(shell dirname $<)/" -f bin -o $@

rebuild: clean all

run: build/os-image
	qemu-system-i386 -drive format=raw,file=$<

rerun: clean run

clean:
	find -type f \( -name "*.o" -o -name "*.bin" \) -delete
	rm -rf build
