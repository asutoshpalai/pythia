[ bits 32 ]

global load_pd
load_pd:
  push ebp
  mov ebp, esp
  mov eax, [ebp + 8]
  mov cr3, eax
  mov esp, ebp
  pop ebp
  ret

global enable_paging
enable_paging:
  push ebp
  mov ebp, esp
  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax
  mov esp, ebp
  pop ebp
  ret

global disable_paging
disable_paging:
  push ebp
  mov ebp, esp
  mov eax, cr0
  and eax, 0x7fffffff
  mov cr0, eax
  mov esp, ebp
  pop ebp
  ret
