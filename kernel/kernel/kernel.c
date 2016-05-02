#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include "vga.h"
#include "keyboard.h"
#include "system.h"
#include "string.h"
#include "stdio.h"
#include "isr.h"

#if defined(__linux__)
#error "You are not using cross-compiler, please switch"
#endif

#if !defined(__i386__)
#error "You are not using ix86-elf compiler"
#endif

#if defined(__cplusplus)
extern "C"
#endif
void kernel_main() {
  gdt_install();
  idt_install();
  isrs_install();
  irq_install();
  enable_interrupts();
  terminal_initialize();
  timer_install();
  keyboard_install();

  terminal_writestring("Hello, kernel World!\n");
  terminal_writestring("Here is some text in a new line\n");
  terminal_writestring("Here is some more text\n");
  terminal_writestring("Here is a new line\n");
  terminal_writestring("Here is some text after terminal scroll\n");
  for (size_t i = 0; i < 5; i++) {
    terminal_writestring("This is line number ");

    char temp[10];
    itoa(i, temp, 10);
    terminal_writestring(temp);
    terminal_writestring("\n");
  }
  puts("waiting for 2 sec\n");
  timer_wait(2);
  puts("waiting complete\n");

  char buffer[200];
  gets(buffer);
  puts(buffer);

}
