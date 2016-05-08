[org 0x7c00]

BOOT_LOADER_OFFSET equ 0x1000

  mov [BOOT_DRIVE], dl

  mov bp, 0x9000
  mov sp, bp

  call load_boot_loader

  jmp BOOT_LOADER_OFFSET

load_boot_loader:

  mov bx, MSG_LOAD_BOOT_LOADER
  call print_line

  xor bx, bx
  mov es, bx
  mov bx, BOOT_LOADER_OFFSET
  mov dh, BOOT_LOADER_SIZE
  mov dl, [BOOT_DRIVE]
  mov cl, 0x02 ; Boot sector size is 1 and it's 1 indexed
  call disk_load
  ret

BOOT_DRIVE db 0

MSG_LOAD_BOOT_LOADER db "Loading the bootloader", 0

%include "disk_load.s"
%include "print_string.s"

times 510 -( $ - $$  ) db 0 ; When compiled , our program must fit into 512 bytes ,
; with the last two bytes being the magic number ,
; so here , tell our assembly compiler to pad out our
; program with enough zero bytes (db 0) to bring us to the
; 510 th byte.
dw 0xaa55 ; Last two bytes ( one word  ) form the magic number ,
; so BIOS knows we are a boot sector.
