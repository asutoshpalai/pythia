#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include "vga.h"
#include "system.h"
#include "string.h"

uint8_t make_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color) {
  return (uint16_t)c | (uint16_t)color << 8;
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
  memset(&terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH], 0, sizeof(uint16_t) * VGA_WIDTH);
  terminal_row--;
}

void terminal_putchar(char c) {
  if (c == '\n') {
    terminal_column = 0;
    ++terminal_row;
  }
  else if (c == '\t') {
    if (terminal_column < VGA_WIDTH - 9)
      terminal_column = (terminal_column + 8) & ~(8 - 1);
  }
  else if(c == '\b') {
    if(terminal_column == 0) {
      terminal_column = VGA_WIDTH - 1;
      terminal_row--;
    }
    else {
      terminal_column--;
    }
  }
  else if (c == '\r') {
      terminal_column = 0;
  }
  else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    if (++terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      ++terminal_row;
    }
  }

  if (terminal_row == VGA_HEIGHT)
      terminal_scroll();

  terminal_move_csr();

}

/* Updates the hardware cursor: the little blinking line
 * on the screen under the last character pressed! */
void terminal_move_csr() {
  size_t index = terminal_row * 80 + terminal_column;

  /* This sends a command to indicies 14 and 15 in the
   * CRT Control Register of the VGA controller. These
   * are the high and low bytes of the index that show
   * where the hardware cursor is to be 'blinking'. To
   * learn more, you should look up some VGA specific
   * programming documents. A great start to graphics:
   * http://www.brackeen.com/home/vga */
  outportb(0x3D4, 14);
  outportb(0x3D5, index>> 8);
  outportb(0x3D4, 15);
  outportb(0x3D5, index);

}

void terminal_writestring(const char *data) {
  size_t len = strlen(data);
  for(size_t i = 0; i < len; i++)
    terminal_putchar(data[i]);

}

