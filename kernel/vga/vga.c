#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include "vga.h"

uint8_t make_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color) {
  return (uint16_t)c | (uint16_t)color << 8;
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len] != 0)
    len++;
  return len;
}

size_t terminal_column;
size_t terminal_row;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_initialize() {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = make_vgaentry(' ', terminal_color);
    }
  }
}

void terminal_setcolot(uint8_t color) {
  terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_scroll() {
  for (size_t y = 1; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index - VGA_WIDTH] = terminal_buffer[index];
    }
  }
  terminal_row--;
}

void terminal_putchar(char c) {
  if (c == '\n') {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
      terminal_scroll();
    return;
  }

  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
      terminal_scroll();
    }
  }

}

void terminal_writestring(const char *data) {
  size_t len = strlen(data);
  for(size_t i = 0; i < len; i++)
    terminal_putchar(data[i]);

}

