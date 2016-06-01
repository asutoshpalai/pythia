#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stdint.h>
#include <stddef.h>
#include "stdlib.h"
#include "stdio.h"
#include "memory.h"

struct memory_block_header {
  struct memory_block_header* prev_header;
  struct memory_block_header* next_header;
  void *start;
  unsigned int size;
}__attribute__((packed));

typedef struct memory_block_header memory_block_header;

memory_block_header *free_pool_start;
memory_block_header *free_pool_end;


void init_malloc() {
  int memory_size = sizeof(memory_block_header) << 12;
  void *first_memory_ptr = morecore(memory_size);


  // Sentinel start header
  free_pool_start = first_memory_ptr;
  free_pool_start->prev_header = NULL;
  free_pool_start->next_header = first_memory_ptr + sizeof(memory_block_header);
  free_pool_start->start = first_memory_ptr + sizeof(memory_block_header);
  free_pool_start->size = 0;

  // Main header occupying the whole remaining memory
  memory_block_header *new_h = free_pool_start->next_header;
  new_h->prev_header = free_pool_start;
  // Tail header's address
  new_h->next_header = first_memory_ptr + memory_size - sizeof(memory_block_header);
  new_h->start = free_pool_start->start + sizeof(memory_block_header);
  new_h->size = memory_size - (3 * sizeof(memory_block_header)); // HEAD + MAIN + TAIL

  // Sentinel tail header
  free_pool_end = new_h->next_header;
  free_pool_end->prev_header = new_h;
  free_pool_end->next_header = NULL;
  free_pool_end->start = first_memory_ptr + memory_size;
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
  printf("Free_pool_start 0x%x\n", (int)free_pool_start);

  while(ptr) {
    print_memory_header(ptr);
    ptr = ptr->next_header;
  }
}
