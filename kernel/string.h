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

  void puts(const char *data);
  void itoa(int n, char* str, int index);
  void reverse(char str[]);

#ifdef __cplusplus

}
#endif

#endif
