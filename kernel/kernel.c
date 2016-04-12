#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include "vga/vga.h"

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
  terminal_initialize();

  terminal_writestring("Hello, kernel World!\n");
  terminal_writestring("Here is some text in a new line\n");
  terminal_writestring("Here is some more text\n");
  terminal_writestring("Here is a new line\n");
  terminal_writestring("Here is some text after terminal scroll\n");
  for (size_t i = 0; i < 25; i++) {
    terminal_writestring("This is line number ");
    char temp[10];
    if (i < 10) {
      temp[0] = i + '0';
      temp[1] = 0;
    }
    else {
      temp[0] = i/10 + '0';
      temp[1] = i%10 + '0';
      temp[2] = 0;
    }
    terminal_writestring(temp);
    terminal_writestring("\n");
  }

}

