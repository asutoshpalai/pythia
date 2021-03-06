#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  void* memcpy(void* __restrict, const void* __restrict, size_t);
  void* memset(void*, int, size_t);
  void* memsetw(void* bufptr, int value, size_t size);
  size_t strlen(const char*);
  char* strcpy(char*, const char*);

  void puts(const char *data);
  void putchar(const char data);
  void itoa(int n, char* str, int index);
  int atoi(char* str, int index);
  void reverse(char str[]);

  void printi(int n);
  void printhex(int n);
  int strcmp(const char *str1, const char *str2);
  char* strtok(char *str1, char delim);

#ifdef __cplusplus

}
#endif

#endif
