[bits 16]

switch_to_pm:
  mov bx, SWITCHING_PROT_MODE 
  call print_string

  cli

  lgdt [gdt_descriptor]

  mov eax, cr0
  or eax, 0x1
  mov cr0, eax

  jmp CODE_SEG:init_pm


[bits 32]

init_pm:
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x90000
  mov esp, ebp

  call START_PM

SWITCHING_PROT_MODE db 'Switching to 32 bit protected mode', 0
