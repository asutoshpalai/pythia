#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

  void* memcpy(void* __restrict, const void* __restrict, size_t);
  void* memset(void*, int, size_t);
  void* memsetw(void* bufptr, int value, size_t size);
  size_t strlen(const char*);

#ifdef __cplusplus

}
#endif

#endif
