#include "system.h"
#include "string.h"

unsigned char inportb (unsigned short port) {
  unsigned char rv;

  __asm__ __volatile__ ("inb %1, %0": "=a" (rv): "dN" (port));

  return rv;
}

void outportb(unsigned short port, unsigned char data) {
  __asm__ __volatile__ ("outb %1, %0": : "dN" (port), "a" (data));
}

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(struct gdt_entry *gdt, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
  /* Setup the descriptor base address */
  (*gdt).base_low = (base & 0xFFFF);
  (*gdt).base_middle = (base >> 16) & 0xFF;
  (*gdt).base_high = (base >> 24) & 0xFF;

  /* Setup the descriptor limits */
  (*gdt).limit_low = (limit & 0xFFFF);
  (*gdt).granularity = ((limit >> 16) & 0x0F);

  /* Finally, set up the granularity and access flags */
  (*gdt).granularity |= (gran & 0xF0);
  (*gdt).access = access;

}

void gdt_install() {
  __gp.limit = (sizeof (gdt_entry)) * 3 - 1;
  __gp.base = (uint32_t) &__gdt;

  /* Our NULL descriptor */
  gdt_set_gate(&__gdt[0], 0, 0, 0, 0);

  /* The second entry is our Code Segment. The base address
   * is 0, the limit is 4GBytes, it uses 4KByte granularity,
   * uses 32-bit opcodes, and is a Code Segment descriptor.
   * Please check the table above in the tutorial in order
   * to see exactly what each value means */
  gdt_set_gate(&__gdt[1], 0, 0xFFFFFFFF, 0x9A, 0xCF);

  /* The third entry is our Data Segment. It's EXACTLY the
   * same as our code segment, but the descriptor type in
   * this entry's access byte says it's a Data Segment */
  gdt_set_gate(&__gdt[2], 0, 0xFFFFFFFF, 0x92, 0xCF);

  /* Flush out the old GDT and install the new changes! */
  gdt_flush();
}

void idt_set_gate(struct idt_entry *idt, uint32_t base, uint16_t sel, uint8_t flags)
{
  idt->offset_lo = base & 0xFFFFFFFF;
  idt->offset_hi = (base >> 16) & 0xFFFFFFFF;
  idt->selector = sel;
  idt->zero = 0;
  idt->type_attr = flags;
}

/* Installs the IDT */
void idt_install()
{
  /* Sets the special IDT pointer up, just like in 'gdt.c' */
  idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
  idtp.base = (uint32_t) &idt;

  /* Clear out the entire IDT, initializing it to zeros */
  memset(&idt, 0, sizeof(struct idt_entry) * 256);

  /* Add any new ISRs to the IDT here using idt_set_gate */

  /* Points the processor's internal register to the new IDT */
  idt_load();
}
