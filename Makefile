LD=i686-elf-ld
CC=i686-elf-gcc

CFLAGS=-Wall -Wextra -O2 -m32 -ffreestanding -Ios/include

KERNEL_OBJ_LIST:=\
os/kernel/kernel_entry.o \
os/kernel/isr.o \
os/kernel/isrc.o \
os/kernel/irqc.o \
os/kernel/kernel.o \
os/kernel/memory/page_allocator.o \
os/kernel/memory/paging_s.o \
os/kernel/stdlib.o \
os/kernel/system.o \
os/kernel/memory.o \
os/utils/shell.o \
os/utils/string.o \
os/utils/stdio.o \
os/utils/stdlib.o \
os/drivers/vga/vga.o \
os/drivers/keyboard/keyboard.o \
os/drivers/clock/timer.o \

all: build/os-image

build/os-image: boot/boot_sector.bin boot/boot_loader.bin os/kernel.bin
	mkdir -p build
	cat $^ > $@

boot/boot_loader.bin: boot/boot_loader.s os/kernel.bin
	$(eval KERNEL_SIZE=$(shell stat -c"%s" os/kernel.bin))
	$(eval KERNEL_SECTORS=$(shell python3 -c "import math; print (math.ceil($(KERNEL_SIZE).0 / 512))"))
	nasm $< -I"$(shell dirname $<)/" -f bin -dKERNEL_SIZE=$(KERNEL_SECTORS) -o $@

boot/boot_sector.bin: boot/boot_sector.s boot/boot_loader.bin
	$(eval BOOTLOADER_SIZE=$(shell stat -c"%s" boot/boot_loader.bin))
	$(eval BOOTLOADER_SECTORS=$(shell python3 -c "import math; print (math.ceil($(BOOTLOADER_SIZE).0 / 512))"))
	nasm $< -I"$(shell dirname $<)/" -f bin -dBOOT_LOADER_SIZE=$(BOOTLOADER_SECTORS) -o $@

os/kernel.bin: $(KERNEL_OBJ_LIST)
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
