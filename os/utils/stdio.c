#include <stdbool.h>
#include <stdarg.h>
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

static void print(const char* data, size_t data_length) {
  const unsigned char* ptr = (const unsigned char*) data;
  while (data_length --) {
    putchar((int) *(ptr++));

  }
}

int printf(const char* restrict format, ...) {
  va_list parameters;
  va_start(parameters, format);

  int written = 0;
  size_t amount;
  bool rejected_bad_specifier = false;

  while (*format != 0) {
    if (*format != '%') {
print_c:
      amount = 1;

      while (format[amount] && format[amount] != '%')
        amount++;

      print(format, amount);
      format += amount;
      written += amount;
      continue;

    }

    const char* format_begun_at = format;

    if (*(++format) == '%')
      goto print_c;

    if (rejected_bad_specifier) {
incomprehensible_conversion:
      rejected_bad_specifier = true;
      format = format_begun_at;
      goto print_c;

    }

    if (*format == 'c') {
      format++;
      char c = (char) va_arg(parameters, int);
      print(&c, sizeof(c));
      written += sizeof(c);

    }
    else if (*format == 's') {
      format++;
      const char* s = va_arg(parameters, const char*);
      size_t l = strlen(s);
      print(s, l);
      written += l;

    }
    else if (*format == 'd' || *format == 'x') {
      int base = (*format == 'd')?10:16;
      format++;
      int i = va_arg(parameters, int);
      char s[20];
      itoa(i, s, base);
      size_t l = strlen(s);
      print(s, l);
      written += l;

    }
    else {
      goto incomprehensible_conversion;

    }

  }

  va_end(parameters);
  return written;

}

