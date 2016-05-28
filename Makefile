LD=i686-elf-ld
CC=i686-elf-gcc

CFLAGS=-Wall -Wextra -O2 -m32 -ffreestanding -Ikernel/include

KERNEL_OBJ_LIST:=\
kernel/kernel/kernel_entry.o \
kernel/kernel/isr.o \
kernel/kernel/isrc.o \
kernel/kernel/irqc.o \
kernel/kernel/kernel.o \
kernel/kernel/stdlib.o \
kernel/kernel/system.o \
kernel/kernel/memory.o \
kernel/utils/string.o \
kernel/utils/stdio.o \
kernel/utils/shell.o \
kernel/drivers/vga/vga.o \
kernel/drivers/keyboard/keyboard.o \
kernel/drivers/clock/timer.o \

all: build/os-image

build/os-image: boot/boot_sector.bin boot/boot_loader.bin kernel/kernel.bin
	mkdir -p build
	cat $^ > $@

boot/boot_loader.bin: boot/boot_loader.s kernel/kernel.bin
	$(eval KERNEL_SIZE=$(shell stat -c"%s" kernel/kernel.bin))
	$(eval KERNEL_SECTORS=$(shell python -c "import math; print (math.ceil($(KERNEL_SIZE).0 / 512))"))
	nasm $< -I"$(shell dirname $<)/" -f bin -dKERNEL_SIZE=$(KERNEL_SECTORS) -o $@

boot/boot_sector.bin: boot/boot_sector.s boot/boot_loader.bin
	$(eval BOOTLOADER_SIZE=$(shell stat -c"%s" boot/boot_loader.bin))
	$(eval BOOTLOADER_SECTORS=$(shell python -c "import math; print (math.ceil($(BOOTLOADER_SIZE).0 / 512))"))
	nasm $< -I"$(shell dirname $<)/" -f bin -dBOOT_LOADER_SIZE=$(BOOTLOADER_SECTORS) -o $@

kernel/kernel.bin: $(KERNEL_OBJ_LIST)
	$(LD) -o $@ -shared --oformat binary -ffreestanding -O2 -nostdlib -m elf_i386 -Ttext 0x10000 $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	nasm $< -I"$(shell dirname $<)/" -f elf -o $@

%.bin: %.s
	nasm $< -I"$(shell dirname $<)/" -f bin -o $@

rebuild: clean all

run: build/os-image
	qemu-system-i386 -drive format=raw,file=$<

rerun: clean run

crun: build/os-image
	(sleep $(filter-out $@,$(MAKECMDGOALS)); pkill qemu) & qemu-system-i386 -drive format=raw,file=$< -curses

%:
	@:

clean:
	find -type f \( -name "*.o" -o -name "*.bin" \) -delete
	rm -rf build
