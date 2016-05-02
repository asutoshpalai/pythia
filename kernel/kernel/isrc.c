#include "isr.h"

/* This is a simple string array. It contains the message that
 *  corresponds to each and every exception. We get the correct
 *  message by accessing like:
 *  exception_message[interrupt_number] */
const char *exception_messages[] =
{
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",

  "Double Fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault",
  "Unknown Interrupt",

  "Coprocessor Fault",
  "Alignment Check",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",

  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};

/* This is a very repetitive function... it's not hard, it's
 *  just annoying. As you can see, we set the first 32 entries
 *  in the IDT to the first 32 ISRs. We can't use a for loop
 *  for this, because there is no way to get the function names
 *  that correspond to that given entry. We set the access
 *  flags to 0x8E. This means that the entry is present, is
 *  running in ring 0 (kernel level), and has the lower 5 bits
 *  set to the required '14', which is represented by 'E' in
 *  hex. */
void isrs_install() {
    idt_set_gate(&idt[0], (uint32_t)_isr0, 0x08, 0x8E);
    idt_set_gate(&idt[1], (uint32_t)_isr1, 0x08, 0x8E);
    idt_set_gate(&idt[2], (uint32_t)_isr2, 0x08, 0x8E);
    idt_set_gate(&idt[3], (uint32_t)_isr3, 0x08, 0x8E);
    idt_set_gate(&idt[4], (uint32_t)_isr4, 0x08, 0x8E);
    idt_set_gate(&idt[5], (uint32_t)_isr5, 0x08, 0x8E);
    idt_set_gate(&idt[6], (uint32_t)_isr6, 0x08, 0x8E);
    idt_set_gate(&idt[7], (uint32_t)_isr7, 0x08, 0x8E);

    idt_set_gate(&idt[8], (uint32_t)_isr8, 0x08, 0x8E);
    idt_set_gate(&idt[9], (uint32_t)_isr9, 0x08, 0x8E);
    idt_set_gate(&idt[10], (uint32_t)_isr10, 0x08, 0x8E);
    idt_set_gate(&idt[11], (uint32_t)_isr11, 0x08, 0x8E);
    idt_set_gate(&idt[12], (uint32_t)_isr12, 0x08, 0x8E);
    idt_set_gate(&idt[13], (uint32_t)_isr13, 0x08, 0x8E);
    idt_set_gate(&idt[14], (uint32_t)_isr14, 0x08, 0x8E);
    idt_set_gate(&idt[15], (uint32_t)_isr15, 0x08, 0x8E);

    idt_set_gate(&idt[16], (uint32_t)_isr16, 0x08, 0x8E);
    idt_set_gate(&idt[17], (uint32_t)_isr17, 0x08, 0x8E);
    idt_set_gate(&idt[18], (uint32_t)_isr18, 0x08, 0x8E);
    idt_set_gate(&idt[19], (uint32_t)_isr19, 0x08, 0x8E);
    idt_set_gate(&idt[20], (uint32_t)_isr20, 0x08, 0x8E);
    idt_set_gate(&idt[21], (uint32_t)_isr21, 0x08, 0x8E);
    idt_set_gate(&idt[22], (uint32_t)_isr22, 0x08, 0x8E);
    idt_set_gate(&idt[23], (uint32_t)_isr23, 0x08, 0x8E);

    idt_set_gate(&idt[24], (uint32_t)_isr24, 0x08, 0x8E);
    idt_set_gate(&idt[25], (uint32_t)_isr25, 0x08, 0x8E);
    idt_set_gate(&idt[26], (uint32_t)_isr26, 0x08, 0x8E);
    idt_set_gate(&idt[27], (uint32_t)_isr27, 0x08, 0x8E);
    idt_set_gate(&idt[28], (uint32_t)_isr28, 0x08, 0x8E);
    idt_set_gate(&idt[29], (uint32_t)_isr29, 0x08, 0x8E);
    idt_set_gate(&idt[30], (uint32_t)_isr30, 0x08, 0x8E);
    idt_set_gate(&idt[31], (uint32_t)_isr31, 0x08, 0x8E);
}

/* All of our Exception handling Interrupt Service Routines will
 *  point to this function. This will tell us what exception has
 *  happened! Right now, we simply halt the system by hitting an
 *  endless loop. All ISRs disable interrupts while they are being
 *  serviced as a 'locking' mechanism to prevent an IRQ from
 *  happening and messing up kernel data structures */
void fault_handler(struct interrupt_handler_regs *r) {
  if(r->int_no < 32) {
    puts("\nException caught\nSystem halted\n");
    puts(exception_messages[r->int_no]);
    for (;;);
  }
}
