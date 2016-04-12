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

print_line:
pusha
call print_string
mov ah, 0x0e
mov al, 0x0a
int 0x10
mov al, 0x0d
int 0x10
popa
ret
