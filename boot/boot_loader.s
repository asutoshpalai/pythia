[org 0x1000]

; The actual bootloader loaded and is called by the bootsector code
KERNEL_SEGMENT equ 0x1000
KERNEL_OFFSET equ 0x0000
BOOT_LOADER_SIZE equ 2

  mov [BOOT_DRIVE], dl

  mov bp, 0x9000
  mov sp, bp
  mov bx, IN_REAL_MODE
  call print_line

  call load_kernel

  call switch_to_pm

  jmp $

[bits 32]

START_PM:
  mov ebx, IN_PROT_MODE
  call print_string_pm

  call (KERNEL_SEGMENT << 4) + KERNEL_OFFSET

  jmp $

%include "print_string.s"
%include "print_string_pm.s"
%include "disk_load.s"
%include "gdt.s"
%include "switch_to_pm.s"
%include "check_a20.s"
%include "enable_a20.s"

[bits 16]

load_kernel:

  mov bx, MSG_LOAD_KERNEL
  call print_line

  mov bx, KERNEL_SEGMENT
  mov es, bx
  mov bx, KERNEL_OFFSET
  mov dh, KERNEL_SIZE
  mov dl, [BOOT_DRIVE]
  mov cl, BOOT_LOADER_SIZE + 2 ; Boot sector 1 + boot loader size and it's 1 indexed
  call disk_load

  ret

BOOT_DRIVE db 0
IN_REAL_MODE db 'Started in 16 bit real mode', 0
IN_PROT_MODE db 'Switched to 32 bit protected mode', 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0

times 512 -(($ - $$) % 512) db 0 ; When compiled , the program should fit into a sector 
;so that kernel can be loaed from the next sector
