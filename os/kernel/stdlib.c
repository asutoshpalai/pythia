#include "stdio.h"

__attribute__((__noreturn__))
void abort() {
  printf("Kernel aborting...");
  while (1);

  __builtin_unreachable();
}
