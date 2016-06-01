#include "memory.h"
#include "memory/paging.h"
#include "stdlib.h"
#include "stdio.h"

#define PAGED_PD ((uint32_t *)0xFFFFF000)
#define PAGED_PT(PTN) (((uint32_t *)0xFFC00000) + (PTN << 10))

uint32_t *page_directory;
void *max_allocated_vrt_addr;

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
    first_page_table[i] = (i << 12) | 3;
  }

  max_allocated_vrt_addr = (void *)(i << 12) - 1;

  page_directory[0] = ((uint32_t)first_page_table) | 3;

  // Map page directory to itself. A super brilliant technique to handle the
  // page directory after enable paging. Page directory becomes a page table
  // mapping the last virtual address space 0xFFC00000 - 0XFFFFFFFF.
  // PD is availabe at 0xFFFFF000
  // Each entry in page directory (i.e. page tables) become pages that can be
  // accessed.
  // The page tables occupy pages from 0xFFC00000 on wards.
  page_directory[1023] = ((uint32_t)page_directory) | 3;

  load_pd(page_directory);
  enable_paging();
  init_malloc();
}

void show_page_entry(int pde, int pte) {
  uint32_t pd_entry = PAGED_PD[pde];
  if((pd_entry & 1) == 0) {
    printf("Page directory entry is %x\n", pd_entry);
    printf("Page table not loaded in the memory\n");
    return;
  }

  printf("Page table physical address 0x%x\n", pd_entry & 0xFFFFF000);
  uint32_t entry = PAGED_PT(pde)[pte];
  uint32_t e_h = entry >> 16;
  uint32_t e_l = entry & 0xFFFF;
  printf("Page table entry at %d of PT %d is 0x%x %x\n", pte, pde, e_h, e_l);
}

void *add_pt_entry(void *vrt_addr, void *p_addr, int flags) {
  int pdi = (uint32_t)vrt_addr >> 22;
  int pti = ((uint32_t)vrt_addr & 0x003FF000) >> 12;

  if((PAGED_PD[pdi] & 1) == 0) {
    void *pt_addr = kalloc_pframe();
    PAGED_PD[pdi] = ((uint32_t)pt_addr) | 3;

    // Initialize the new pt to blanks
    for(int i = 0; i < 1024; i++) {
      PAGED_PT(pdi)[i] = 2; // Mark them not present
    }
  }
  PAGED_PT(pdi)[pti] = (uint32_t)p_addr | (flags & 0xFFF) | 1;
  return vrt_addr;
}

inline void *get_new_page(void *vrt_addr, int flags) {
  void *p_addr = kalloc_pframe();
  add_pt_entry(vrt_addr, p_addr, flags | 1);
  return vrt_addr;
}

void *morecore(size_t n) {
  if(n == 0 || max_allocated_vrt_addr + n >= (void *)PAGED_PT(0))
    return NULL;

  size_t no_of_pages = 1 + ((n - 1) / PAGE_SIZE);

  void *ret_addr = max_allocated_vrt_addr  + 1;

  // Align the address to next page boundary
  if(((int)ret_addr & ((1 << 12) - 1)) != 0)
    ret_addr += (1 << 12) - ((int)ret_addr & ((1 << 12) - 1));

  size_t c = 0;
  while(c < no_of_pages) {
    get_new_page(ret_addr + (c++ << 12), 0x003);
  }

  max_allocated_vrt_addr = ret_addr + (c << 12) - 1;
  return ret_addr;
}

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
