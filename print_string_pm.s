[bits 32]

VIDEO_MEMORY equ 0xb800
WHITE_ON_BLACK equ 0x0f

print_string_pm:
pusha

mov ah, WHITE_ON_BLACK
mov edx, VIDEO_MEMORY

print_string_pm_loop:

mov al, [ebx]
cmp al, 0
je print_string_pm_exit

mov [edx], ax

inc ebx
add edx, 2

jmp print_string_pm_loop

print_string_pm_exit:

popa
ret
