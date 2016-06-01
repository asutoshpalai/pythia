#ifndef MEMORY_PAGE_ALLOCATOR_H
#define MEMORY_PAGE_ALLOCATOR_H
#include "memory.h"

/*
 * The main memory is divided in the following manner:
 * 0-1MB: BIOS reserve and the current location of the kernel (0x10000)
 * 1-3MB: Reserved for memory management
 *
 * 4MB+: Used to allocated pages
 *
 * 0-4MB: reserved and shall be directly mapped to virtual address. This shall,
 * until the kernel fits in, save us from remapping a lot of stuffs (including
 * the video memory).
 */

#define PAGE_SIZE 4096 // 4KB

// Should be called before call to any other function
void kalloc_pframe_init();

// Returns a page frame
void* kalloc_pframe();
void kfree_pframe(void* addr);

extern void load_pd(uint32_t *);
extern void enable_paging();
extern void disable_paging();

#endif
