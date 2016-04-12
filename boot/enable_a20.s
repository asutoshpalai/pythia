;ref http://wiki.osdev.org/A20_Line

[bits 16]

enable_a20:
  pusha

  call check_a20
  cmp ax, 1
  je enable_a20_success

  ; Try the BIOS int 15, 2401 method
  mov ax, 0x2401
  int 0x15
  jnc enable_a20_success

enable_a20_failed:
  mov bx, A20_FAILED
  call print_string
  jmp $

enable_a20_success:
  ; Just a precaution check
  call check_a20
  cmp ax, 1
  jne enable_a20_failed

enable_a20_exit:
  popa
  ret

A20_FAILED db 'Failed to enabled A20 line', 0
