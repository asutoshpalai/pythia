[org 0x7c00]

KERNEL_OFFSET equ 0x1000

  mov [BOOT_DRIVE], dl

  mov bp, 0x9000
  mov sp, bp

  mov bx, IN_REAL_MODE
  call print_string

  call load_kernel

  call switch_to_pm

  jmp $

[bits 32]

START_PM:
  mov ebx, IN_PROT_MODE
  call print_string_pm

  call KERNEL_OFFSET

  jmp $

%include "print_string.s"
%include "disk_load.s"
%include "print_string_pm.s"
%include "gdt.s"
%include "switch_to_pm.s"

[bits 16]

load_kernel:

  mov bx, MSG_LOAD_KERNEL
  call print_string

  mov bx, KERNEL_OFFSET
  mov dh, 2
  mov dl, [BOOT_DRIVE]
  call disk_load

  ret

BOOT_DRIVE db 0
IN_REAL_MODE db 'Started in 16 bit real mode', 0
IN_PROT_MODE db 'Switched to 32 bit protected mode', 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0

times 510 -( $ - $$  ) db 0 ; When compiled , our program must fit into 512 bytes ,
; with the last two bytes being the magic number ,
; so here , tell our assembly compiler to pad out our
; program with enough zero bytes (db 0) to bring us to the
; 510 th byte.
dw 0xaa55 ; Last two bytes ( one word  ) form the magic number ,
; so BIOS knows we are a boot sector.
