[org 0x7c00 ]

mov ah, 0x0e
mov al, 'H'
int 0x10
mov al, 'e'
int 0x10

mov bx, string

loop:

mov al, [bx]

cmp al, 0

je exit

int 0x10
inc bx 
jmp loop

exit:
  jmp exit


string:
  db "Hello people"
  db 0

times 510 -( $ - $$  ) db 0

dw 0xaa55
