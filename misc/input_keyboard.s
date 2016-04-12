[org 0x7c00]

mov bp, 0x9000
mov sp, bp

mov bx, SETTING_IVT

cli
mov bx, 0x09
shl bx, 2
xor ax, ax
mov ds, ax
mov gs, ax
mov [gs:bx], word keyhandler
mov [gs:bx+2], ds
sti

jmp $

keyhandler:
in al, 0x60
mov bl, al
mov byte [port60], al

in al, 0x61
mov ah, al
or al, 0x80
out 0x61, al ; diabling bit 7
mov al, ah
out 0x61, al


mov al, 0x20
out 0x20, al

and bl, 0x80
jnz done

mov al, byte [port60]
mov ah, 0x0e
int 0x10

mov dx, [port60]
call print_hex

done:
iret

port60 dw 0

%include "print_string.s"
%include "print_hex.s"

SETTING_IVT db 'Setting up interrupt', 0

times 510 - ($ - $$) db 0
dw 0xaa55
