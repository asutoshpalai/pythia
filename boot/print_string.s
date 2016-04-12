[bits 16]
print_string:
pusha
mov ah, 0x0e
p_loop:
mov al, [bx]

cmp al, 0

je exit

int 0x10
inc bx 
jmp p_loop

exit:
popa
ret
