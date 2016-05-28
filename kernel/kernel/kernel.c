#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include "vga.h"
#include "keyboard.h"
#include "system.h"
#include "memory.h"
#include "string.h"
#include "stdio.h"
#include "isr.h"
#include "shell.h"

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
  shell_init();

  terminal_writestring("Hello, kernel World!\n");
  terminal_writestring("Here is some text in a new line\n");
  terminal_writestring("Here is some more text\n");
  terminal_writestring("Here is a new line\n");
  terminal_writestring("Here is some text after terminal scroll\n");
  char str[] = "A string";
  char ch = 'c';
  int integer = 45;
  printf("This is a test for printf.\nThis is a char %c and this a string %s\n", ch, str);
  printf("This is an int %d. This is the same in hex %x\n", integer, integer);

  for (size_t i = 0; i < 5; i++) {
    printf("This is line number %d\n", i);
  }

  print_memory_map();
  init_memory_allocator();

  puts("waiting for 2 sec\n");
  timer_wait(2);
  puts("waiting complete\n");

  char buffer[200];
  gets(buffer);
  printf("%s\n", buffer);
  while(1) {
    shell();
  }

}
