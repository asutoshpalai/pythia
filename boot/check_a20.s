; ref http://wiki.osdev.org/A20_Line

[bits 16]

check_a20:
  push ds
  push es

  cli

  mov ax, 0xffff
  mov ds, ax

  xor ax, ax
  mov es, ax

  mov di, 0x0500
  mov si, 0x0510

  ; save the original values
  mov al, byte [es:di]
  push ax
  mov al, byte [ds:si]
  push ax

  ; load our values
  mov byte [es:di], 0x00
  mov byte [ds:si], 0xff

  ;check if they are equal, i.e. memory is still wrapping around
  cmp byte [es:di], 0xFF

  ; restote the original data
  pop ax
  mov byte [ds:si], al
  pop ax
  mov byte [es:di], al

  mov ax, 0
  je check_a20_exit

  mov ax, 1

check_a20_exit:
  pop ds
  pop es
  ret
