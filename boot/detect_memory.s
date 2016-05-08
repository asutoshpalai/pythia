[bits 16]

detect_memory:
  pusha
  mov esi, esp
  mov bx, DETECTING_MEMORY
  call print_string

  mov ax, 0
  mov es, ax
  mov di, mmap
  call do_e820
  call print_mmap
  mov esp, esi
  popa
  ret

; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
; inputs: es:di -> destination buffer for 24 byte entries
; outputs: bp = entry count, trashes all registers except esi
do_e820:
	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes
	int 0x15
	jc short .failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
	jc short .e820f		; carry set means "end of list already reached"
	mov edx, 0x0534D4150	; repair potentially trashed register
.jmpin:
	jcxz .skipent		; skip any 0 length entries
	cmp cl, 20		; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skipent		; if length uint64_t is 0, skip entry
	inc bp			; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx		; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
	mov [mmap_ent], bp	; store the entry count
	clc			; there is "jc" on end of list to this point, so the carry must be cleared
	ret
.failed:
	mov bx, FUNCTION_UNSUPPORTED 
	call print_string
	stc			; "function unsupported" error exit
	ret

print_mmap:
  mov bx, PRINTING_MEMORY_MAP
  call print_line
  mov bx, NEWLINE
  call print_line
	mov cx, [mmap_ent]
	mov ax, 0
	mov es, ax
	mov di, mmap
print_mmap_loop:
	test cx, cx
	je print_mmap_exit
	mov ax, 12
	
print_mmap_loop2:
	test ax, ax
	je print_mmap_loop2_exit

	mov dx, [di]
	call print_hex

	dec ax
	add di, 2
	mov bx, SPACE
	call print_string
	jmp print_mmap_loop2
print_mmap_loop2_exit:
	mov bx, NEWLINE
	call print_string
	dec cx
	jmp print_mmap_loop

print_mmap_exit:
	ret

DETECTING_MEMORY db 'Detecting Memory ', 0
FUNCTION_UNSUPPORTED db 'Memory detection BIOS function unsupported ', 0
PRINTING_MEMORY_MAP db 'Printing memory map ', 0
NEWLINE db 0x0a, 0x0d, 0
SPACE db ' ', 0
