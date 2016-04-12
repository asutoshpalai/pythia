print_func:
push ax
mov al, [sp + 1]
mov ah, 0x0e
int 0x10
pop ax
ret
