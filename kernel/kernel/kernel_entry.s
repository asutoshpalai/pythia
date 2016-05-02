; Ensures that we jump straight into the kernel ’s entry function.
[ bits 32 ] ; We ’re in protected mode by now , so use 32 - bit instructions.
[ extern kernel_main  ] ; Declate that we will be referencing the external symbol ’main ’,
; so the linker can substitute the final address

call kernel_main

jmp $

global gdt_flush
[ extern __gp ]

gdt_flush:
  lgdt [__gp]
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:gdt_flush_fjmp
gdt_flush_fjmp:
  ret

global idt_load
[extern idtp]

idt_load:
  lidt [idtp]
  ret
