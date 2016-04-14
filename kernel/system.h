#ifndef __SYSTEM_H
#define __SYSTEM_H
#include <stddef.h>
#include <stdint.h>

unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);

struct gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;

} __attribute__((packed));

struct gdt_ptr {
  uint16_t limit;
  uint32_t base;

} __attribute__((packed));

struct gdt_entry __gdt[3];

struct gdt_ptr __gp;

extern void gdt_flush();

void gdt_install();

typedef struct gdt_entry gdt_entry;

struct idt_entry {
  uint16_t offset_lo;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_hi;
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(struct idt_entry *idt, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();

extern void idt_load();

#endif
