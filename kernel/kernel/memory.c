#include "memory.h"
#include "stdio.h"

void print_memory_map() {
  printf("\nNumber of entries are %d\n", memory_map_address->count);
  printf("Location of mmap %x\n", (int)memory_map_address->mmap);

  for(int i = 0; i < memory_map_address->count; i++) {
    printf("Base addr: 0x%x, Size: 0x%x, Status: %d\n",
        (int) memory_map_address->mmap[i].base_addr,
        (int) memory_map_address->mmap[i].length,
        memory_map_address->mmap[i].type);
  }
}
