#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

struct ACPI_m_map {
  uint64_t base_addr;
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

#endif
