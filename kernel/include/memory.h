#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

#define MEMORY_FIRST_MEM_ADDR 0x100000
#define MEMORY_MAX_MEMORY 16*1024*1024 // 16MB expected memory to allocate

struct ACPI_m_map {
  uint32_t base_addr;
  uint32_t base_addr_h;
  uint64_t length;
  uint32_t type;
  uint32_t acpi_attr;
};

struct boot_loader_mmap {
  uint16_t count;
  //uint32_t mmap_loc;
  struct ACPI_m_map *mmap;
} __attribute__((packed));

extern struct boot_loader_mmap *memory_map_address;

void print_memory_map();
void init_memory_allocator();
void print_memory_pool_list();

#endif
