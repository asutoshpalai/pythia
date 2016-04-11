#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>


enum vga_color {
  COLOR_BLACK = 0,
  COLOR_BLUE = 1,
  COLOR_GREEN = 2,
  COLOR_CYAN = 3,
  COLOR_RED = 4,
  COLOR_MAGENTA = 5,
  COLOR_BROWN = 6,
  COLOR_LIGHT_GREY = 7,
  COLOR_DARK_GREY = 8,
  COLOR_LIGHT_BLUE = 9,
  COLOR_LIGHT_GREEN = 10,
  COLOR_LIGHT_CYAN = 11,
  COLOR_LIGHT_RED = 12,
  COLOR_LIGHT_MAGENTA = 13,
  COLOR_LIGHT_BROWN = 14,
  COLOR_WHITE = 15,

};

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

static const size_t VGA_HEIGHT = 25;
static const size_t VGA_WIDTH = 80;

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

void swap(char *c1, char *c2) {
	char c3 = *c1;
	*c1 = *c2;
	*c2 = c3;
}

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap((str+start), (str+end));
        start++;
        end--;
    }
}
 
// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}

#if defined(__cplusplus)
extern "C"
#endif
void kernel_main() {
  terminal_initialize();

  terminal_writestring("Hello, kernel World!\n");
  terminal_writestring("Here is some text in a new line\n");
  terminal_writestring("Here is some more text\n");
  terminal_writestring("Here is a new line\n");
  terminal_scroll();
  terminal_writestring("Here is some text after terminal scroll\n");
  for (size_t i = 0; i < 15; i++) {
    terminal_writestring("This is line number ");
    char temp[10];
    terminal_writestring(itoa(i, temp, 10));
    terminal_writestring("\n");

  }


}

