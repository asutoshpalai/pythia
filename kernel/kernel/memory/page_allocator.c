#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include "memory/paging.h"
#include "stdlib.h"
#include "stdio.h"

#define MEMORY_MANAGER_RESERVE (1024 * 1024 * 3)

int total_pages, stack_size;
uint32_t first_usable_memory;
uint32_t first_allocable_memory;

void **stack;
int stack_top;

/* Checks if the memory 0x100000 and above is available.
 * It returns the index of that memory segment in mmap*/
int find_memory() {
  bool found = false;
  int i;

  struct ACPI_m_map* mmap = memory_map_address->mmap;
  for(i = 0; i < memory_map_address->count; i++)
    if(mmap[i].base_addr <= first_usable_memory &&
        mmap[i].base_addr + mmap[i].length > first_usable_memory) {
      found = true;
      break;
    }

  if(!found) {
    printf("Memory address 0x%x is not available\n", first_usable_memory);
    abort();
  }

  return i;
}

void kalloc_pframe_init() {
  first_usable_memory = MEMORY_FIRST_MEM_ADDR;

  int index = find_memory();
  struct ACPI_m_map mmap = memory_map_address->mmap[index];

  int usable_memory = mmap.base_addr + mmap.length - first_usable_memory;
  first_allocable_memory = first_usable_memory + MEMORY_MANAGER_RESERVE;

  if(usable_memory < MEMORY_MANAGER_RESERVE) {
    printf("Memory is less than 4MB, can't continue\n");
    abort();
  }

  // Align the address to page boundary
  if((first_allocable_memory & ((1 << 12) - 1)) != 0)
    first_allocable_memory += (1 << 12) - (first_allocable_memory & ((1 << 12) - 1));

  stack = (void **)first_usable_memory; // 0x100000
  total_pages = usable_memory / PAGE_SIZE;
  stack_size = total_pages - 1024;

  // Push all the page address to stack
  for(stack_top = 0; stack_top < stack_size; stack_top++) {
    stack[stack_top] = (void *) first_usable_memory + (stack_top * PAGE_SIZE);
  }
  stack_top--;
}

void* kalloc_pframe() {
  if (stack_top == 0) {
    printf("Out of page frames to allocate...\n");
    abort();
  }

  return stack[stack_top--];
}

void kfree_pframe(void* addr) {
  if(((int)first_allocable_memory & ((1 << 12) - 1)) != 0) {
    printf("Non aligned frame received to free\n");
    abort();
  }

  if(++stack_top == stack_size) {
    printf("Error! Received frame to free even though stack is free");
    abort();
  }

  stack[stack_top] = addr;
}
