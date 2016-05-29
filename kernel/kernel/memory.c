#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include "memory.h"
#include "memory/paging.h"
#include "stdlib.h"
#include "stdio.h"

uint32_t *page_directory;
void memory_manager_init() {
  kalloc_pframe_init();

  page_directory = (uint32_t *)kalloc_pframe();

  int i;
  for(i = 0; i < 1024; i++) {
    page_directory[i] = 0x00000002;
  }

  uint32_t *first_page_table = (uint32_t *)kalloc_pframe();
  // Map the first 4 MB onto itself
  for(i = 0; i < 1024; i++) {
    first_page_table[i] = (i * 4096) | 3;
  }

  page_directory[0] = ((uint32_t)first_page_table) | 3;

  load_pd(page_directory);
  enable_paging();
}

void show_page_entry(int pde, int pte) {
  disable_paging();
  uint32_t pt_address = page_directory[pte] & ~((1 << 12) - 1);
  printf("Page table address 0x%x\n", pt_address);
  uint32_t entry = ((uint32_t *)(page_directory[pte] & ~((1 << 12) - 1)))[pte];
  uint32_t e_h = entry >> 16;
  uint32_t e_l = entry & 0xFFFF;
  printf("Page table entry at %d of PT %d is 0x%x %x\n", pte, pde, e_h, e_l);
  enable_paging();
}

/*
struct memory_block_header {
  struct memory_block_header* prev_header;
  struct memory_block_header* next_header;
  void *start;
  unsigned int size;
}__attribute__((packed));

typedef struct memory_block_header memory_block_header;

void *first_memory_ptr;

int memory_size;

memory_block_header *free_pool_start;
memory_block_header *free_pool_end;

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

void init_malloc() {
  int index = find_memory();

  struct ACPI_m_map mmap = memory_map_address->mmap[index];

  first_memory_ptr = (void *)mmap.base_addr;

  memory_size = (mmap.length < MEMORY_MAX_MEMORY)? (int)mmap.length : MEMORY_MAX_MEMORY;

  // Sentinel start header
  free_pool_start = (void *)MEMORY_FIRST_MEM_ADDR;
  free_pool_start->prev_header = NULL;
  free_pool_start->next_header = (void *)MEMORY_FIRST_MEM_ADDR + sizeof(memory_block_header);
  free_pool_start->start = (void *)MEMORY_FIRST_MEM_ADDR + sizeof(memory_block_header);
  free_pool_start->size = 0;

  // Main header occupying the whole remaining memory
  memory_block_header *new_h = free_pool_start->next_header;
  new_h->prev_header = free_pool_start;
  // Tail header's address
  new_h->next_header = (void *)MEMORY_FIRST_MEM_ADDR + memory_size - sizeof(memory_block_header);
  new_h->start = free_pool_start->start + sizeof(memory_block_header);
  new_h->size = memory_size - (3 * sizeof(memory_block_header)); // HEAD + MAIN + TAIL

  // Sentinel tail header
  free_pool_end = new_h->next_header;
  free_pool_end->prev_header = new_h;
  free_pool_end->next_header = NULL;
  free_pool_end->start = (void *)MEMORY_FIRST_MEM_ADDR + memory_size;
  free_pool_end->size = 0;
}

void *malloc(size_t size) {
  // Size including the new header that will be created
  size_t size_s = size + sizeof(memory_block_header);

  memory_block_header* ptr = free_pool_start;

  // First fit
  while(ptr && ptr->size < size_s) {
    ptr = ptr->next_header;
  }

  if (!ptr)
    return NULL;

  if(ptr->size - size_s > 0) {
    memory_block_header *new_h = ptr->start + size;

    new_h->prev_header = ptr->prev_header;
    new_h->next_header = ptr->next_header;
    new_h->start = ptr->start + size_s;
    new_h->size = ptr->size - size_s;

    new_h->prev_header->next_header = new_h;
    new_h->next_header->prev_header = new_h;
    ptr->size = size;
  }
  else {
    ptr->prev_header->next_header = ptr->next_header;
    ptr->next_header->prev_header = ptr->prev_header;
  }

  return ptr->start;
}

void free(void * addr) {
  // Only reliable info available in the header is size
  memory_block_header *header = addr - sizeof(memory_block_header);

  memory_block_header* ptr = free_pool_start;

  // Find the header after which this should be inserted
  // Tail sentinel ensures that we will find one
  while(ptr && (void *)ptr->next_header < addr) {
    ptr = ptr->next_header;
  }

  if(!ptr) {
    printf("Memory address provided to free exceeds the last address");
    abort();
  }

  bool merged = false;
  if(ptr->next_header != free_pool_end &&
      header->start + header->size == ptr->next_header) {
    // Can be merged with next block
    merged = true;
    header->size += ptr->next_header->size + sizeof(memory_block_header);
    header->next_header = ptr->next_header->next_header;
    ptr->next_header->next_header->prev_header = header;
    ptr->next_header = header;
    header->prev_header = ptr;
  }

  if(ptr != free_pool_start &&
      ptr->start + ptr->size == header) {
    // Can be merged with previous block
    merged = true;
    ptr->size += header->size + sizeof(memory_block_header);

    if(merged) {
      ptr->next_header = header->next_header;
      ptr->next_header->prev_header = ptr;
    }
  }

  if(!merged) {
    header->next_header = ptr->next_header;
    ptr->next_header->prev_header = header;
    header->prev_header = ptr;
    ptr->next_header = header;
  }
}

void print_memory_header(memory_block_header* ptr) {
  printf("---------------------------------\n");
  printf("Header address = 0x%x\n", (int)ptr);
  printf("Prev header address = 0x%x\n", (int)ptr->prev_header);
  printf("Next header address = 0x%x\n", (int)ptr->next_header);
  printf("Starting address = 0x%x\n", (int)ptr->start);
  printf("Size = 0x%x\n",ptr->size);
  printf("----------------------------------\n");
}

void print_memory_pool_list() {
  memory_block_header* ptr = free_pool_start;

  printf("Memory size: %d (in dec) %x (in hex)\n", memory_size, memory_size);

  while(ptr) {
    print_memory_header(ptr);
    ptr = ptr->next_header;
  }
}
*/
