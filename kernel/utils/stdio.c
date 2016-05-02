#include "stdio.h"
#include "string.h"
#include "keyboard.h"

char getc() {
  return keyboard_getchar();
}

char *gets(char *buffer) {
  char *ptr = buffer;
  do {
    char c = getc();
    if(c == '\n')
      break;

    *ptr = c;
    if (c == '\b') {
      if(ptr > buffer) {
        ptr--;
        puts("\b ");
      }
      else
        continue;
    }
    else
      ptr++;
    putchar(c);
  }while(1);
  putchar('\n');
  *ptr = 0;

  return buffer;
}
