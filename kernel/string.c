#include "string.h"
#include "vga/vga.h"

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
  unsigned char* dst = (unsigned char*) dstptr;
  const unsigned char* src = (const unsigned char*) srcptr;
  while (size--)
    *(dst++) = *src++;

  return dstptr;

}

void* memset(void* bufptr, int value, size_t size) {
  unsigned char* buf = (unsigned char*) bufptr;
  unsigned char val = (unsigned char) value;

  while (size--) {
    *(buf++) = val;
  }

  return bufptr;
}

void* memsetw(void* bufptr, int value, size_t size) {
  unsigned short* buf = (unsigned short*) bufptr;
  unsigned short val = (unsigned short) value;

  while (size--) {
    *(buf++) = val;
  }

  return bufptr;
}

size_t strlen(const char* data) {
  size_t result = 0;
  while (data[result])
    result++;
  return result;
}

void puts(const char *data) {
  terminal_writestring(data);
}
