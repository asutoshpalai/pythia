print_hex:
pusha
mov ax, 5

begin_hex:
mov bx, HEX_OUT
add bx, ax

mov cx, dx
and cl, 0x0f

cmp cl, 0x0a
jge set_alpha_char

add cl, '0'
jmp end_of_dec_or_alpha_if

set_alpha_char:

add cl, 'W'

end_of_dec_or_alpha_if:

mov [bx], cl

shr dx, 4
dec ax
cmp ax, 1
jne begin_hex

mov bx, HEX_OUT
call print_string
popa
ret
HEX_OUT: db '0x0000',0
