#include "stdio.h"
#include "string.h"
#include "keyboard.h"

char getc() {
  return keyboard_getchar();
}

char *gets(char *buffer) {
  char *ptr = buffer;
  do {
    *ptr = getc();
    putchar(*ptr);
    ptr++;
  }while (*(ptr - 1) != '\n');

  return buffer;
}
