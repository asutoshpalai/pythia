;
; A simple boot sector program that demonstrates addressing.
;
mov ah , 0x0e ; int 10/ ah = 0eh -> scrolling teletype BIOS routine
mov al, '1'
int 0x10
; First attempt
mov al , the_secret
int 0x10 ; Does this print an X?
mov al, '2'
int 0x10
; Second attempt
mov al , [the_secret]
int 0x10 ; Does this print an X?
mov al, '3'
int 0x10
; Third attempt
mov bx , the_secret
add bx , 0x7c00
mov al , [bx]
int 0x10 ; Does this print an X?
mov al, '4'
int 0x10
; Fourth attempt
mov al , [0x7c1e ]
int 0x10 ; Does this print an X?
mov al, 'E'
int 0x10
jmp $ ; Jump forever.
the_secret :
db "X "
; Padding and magic BIOS number.
times 510 -( $ - $$  ) db 0
dw 0xaa55
