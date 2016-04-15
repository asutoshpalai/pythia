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

/* itoa:  convert n to characters in s K&R implemantaion */
void itoa(int n, char s[], int index) {
  int i, sign;

  if ((sign = n) < 0)  /* record sign */
    n = -n;          /* make n positive */
  i = 0;
  do {       /* generate digits in reverse order */
    s[i] = n % index + '0';   /* get next digit */
    if(s[i] > '9')
      s[i] += 7;
    i++;
  } while ((n /= index) > 0);     /* delete it */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}

void reverse(char s[]) {
  int i, j;
  char c;

  for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;

  }

}
