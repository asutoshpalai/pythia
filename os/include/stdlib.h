#ifndef STDLIB_H
#define STDLIB_H

__attribute__((__noreturn__))
void abort();

void init_malloc();

void *malloc(size_t n);

void free(void *ptr);

void print_memory_pool_list();

#endif
