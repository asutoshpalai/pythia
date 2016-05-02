#include "isr.h"

#define PIC1    0x20    /* IO base address for master PIC */
#define PIC2    0xA0    /* IO base address for slave PIC */
#define PIC1_COMMAND  PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND  PIC2
#define PIC2_DATA (PIC2+1)

/* This array is actually an array of function pointers. We use
 * this to handle custom IRQ handlers for a given IRQ */
void *irq_routines[16] =
{
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler) (struct interrupt_handler_regs *r)) {
  irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
  irq_routines[irq] = NULL;
}

void irq_remap(uint8_t offset1, uint8_t offset2) {
  uint8_t a1, a2;

  a1 = inportb(PIC1_DATA); // save masks
  a2 = inportb(PIC2_DATA);

  outportb(PIC1_COMMAND, 0x11);  // starts the initialization sequence (in cascade mode)
  outportb(PIC2_COMMAND, 0x11);

  outportb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
  outportb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset

  outportb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
  outportb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)

  outportb(PIC1_DATA, 0x01);                    // ICW4: 8086/88 (MCS-80/85) mode
  outportb(PIC2_DATA, 0x01);

  outportb(PIC1_DATA, a1);   // restore saved masks.
  outportb(PIC2_DATA, a2);
}

/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers */
void irq_install() {
    irq_remap(0x20, 0x28);

    idt_set_gate(&idt[32], (uint32_t)_irq0, 0x08, 0x8E);
    idt_set_gate(&idt[33], (uint32_t)_irq1, 0x08, 0x8E);
    idt_set_gate(&idt[34], (uint32_t)_irq2, 0x08, 0x8E);
    idt_set_gate(&idt[35], (uint32_t)_irq3, 0x08, 0x8E);
    idt_set_gate(&idt[36], (uint32_t)_irq4, 0x08, 0x8E);
    idt_set_gate(&idt[37], (uint32_t)_irq5, 0x08, 0x8E);
    idt_set_gate(&idt[38], (uint32_t)_irq6, 0x08, 0x8E);
    idt_set_gate(&idt[39], (uint32_t)_irq7, 0x08, 0x8E);

    idt_set_gate(&idt[40], (uint32_t)_irq8, 0x08, 0x8E);
    idt_set_gate(&idt[41], (uint32_t)_irq9, 0x08, 0x8E);
    idt_set_gate(&idt[42], (uint32_t)_irq10, 0x08, 0x8E);
    idt_set_gate(&idt[43], (uint32_t)_irq11, 0x08, 0x8E);
    idt_set_gate(&idt[44], (uint32_t)_irq12, 0x08, 0x8E);
    idt_set_gate(&idt[45], (uint32_t)_irq13, 0x08, 0x8E);
    idt_set_gate(&idt[46], (uint32_t)_irq14, 0x08, 0x8E);
    idt_set_gate(&idt[47], (uint32_t)_irq15, 0x08, 0x8E);
}

/* Each of the IRQ ISRs point to this function, rather than
*  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
void irq_handler(struct interrupt_handler_regs *r) {
  /* This is a blank function pointer */
  void (*handler)(struct interrupt_handler_regs *r);

  handler = irq_routines[r->int_no - 32];
  if(handler) {
    handler(r);
  }

  /* If the IDT entry that was invoked was greater than 40
   * (meaning IRQ8 - 15), then we need to send an EOI to
   * the slave controller */
  if(r->int_no > 0x28) {
    outportb(PIC2_COMMAND, 0x20); /* End-of-interrupt command code */
  }

  outportb(PIC1_COMMAND, 0x20);
}
