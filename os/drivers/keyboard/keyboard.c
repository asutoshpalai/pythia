#include "keyboard.h"
#include "isr.h"
#include "system.h"

#define ALT_KEY 0x38
#define CTRL_KEY 0x1D
#define LEFT_SHIFT_KEY 0x2A
#define RIGHT_SHIFT_KEY 0x36
#define CAPSLOCK_KEY 0x3A

#define KEYPRESS_BIT 0x0001
#define ALT_BIT 0x0002
#define CTRL_BIT 0x0004
#define LEFT_SHIFT_BIT 0x0008
#define RIGHT_SHIFT_BIT 0x0010
#define CAPSLOCK_BIT 0x0020

const char keyboard_map[128] = {
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	'9', '0', '-', '=', '\b',	/* Backspace */
	'\t',			/* Tab */
	'q', 'w', 'e', 'r',	/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
	0,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`', 0,		/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', ',', '.', '/',  0,				/* Right shift */
	'*',
	0,	/* Alt */
	' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};

const char keyboard_map_shift[128] = {
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
	'(', ')', '_', '+', '\b',	/* Backspace */
	'\t',			/* Tab */
	'q', 'w', 'e', 'r',	/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n',	/* Enter key */
	0,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':',	/* 39 */
	'"', '~', 0,		/* Left shift */
	'|', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', '<', '>', '?',  0,				/* Right shift */
	'*',
	0,	/* Alt */
	' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};

volatile uint16_t keyboard_status;

volatile unsigned int keyboard_keycount;

volatile char keyboard_last_printable_char;

volatile uint8_t keyboard_prev_scancode;

void keyboard_handler() {
  uint8_t scancode;
  unsigned char mappedchar;

  scancode = inportb(0x60);
  /*printhex(scancode);*/

  uint8_t check_scode = scancode & 0x7F;

  if(scancode == 0xE0)
    return;

  mappedchar = keyboard_map[check_scode];

  if(scancode & 0x80) {
    // The key is released

    keyboard_status = keyboard_status & ~KEYPRESS_BIT;
    if (check_scode == ALT_KEY) {
      keyboard_status = keyboard_status & ~ALT_BIT;
    }
    else if (check_scode == CTRL_KEY) {
      keyboard_status = keyboard_status & ~CTRL_BIT;
    }
    else if (check_scode == LEFT_SHIFT_KEY) {
      keyboard_status = keyboard_status & ~LEFT_SHIFT_BIT;
    }
    else if (check_scode == RIGHT_SHIFT_KEY) {
      keyboard_status = keyboard_status & ~RIGHT_SHIFT_BIT;
    }
    else if (check_scode == CAPSLOCK_KEY) {
      /*keyboard_status = keyboard_status & ~CAPSLOCK_BIT;*/
    }
    keyboard_prev_scancode = 0;
  }
  else {
    // The key is pressed
    keyboard_status = keyboard_status | KEYPRESS_BIT;
    if ((0x20 <= mappedchar && mappedchar <= 0x7E)
        || mappedchar == '\n'
        || mappedchar == '\t'
        || mappedchar == '\b'
        || mappedchar == '\r') {
      // Letter entered
      if (keyboard_status & (RIGHT_SHIFT_BIT | LEFT_SHIFT_BIT)) {
        mappedchar = keyboard_map_shift[scancode & 0x7F];
      }
      if (('a' <= mappedchar && mappedchar <= 'z')
          && (((keyboard_status & CAPSLOCK_BIT) != 0) ^ (((keyboard_status & (RIGHT_SHIFT_BIT | LEFT_SHIFT_BIT))) != 0)))
        mappedchar -= 'a' - 'A';

      keyboard_last_printable_char = mappedchar;
      keyboard_keycount++;
    }
    else if (check_scode == ALT_KEY) {
      keyboard_status = keyboard_status | ALT_BIT;
    }
    else if (check_scode == CTRL_KEY) {
      keyboard_status = keyboard_status | CTRL_BIT;
    }
    else if (check_scode == LEFT_SHIFT_KEY) {
      keyboard_status = keyboard_status | LEFT_SHIFT_BIT;
    }
    else if (check_scode == RIGHT_SHIFT_KEY) {
      keyboard_status = keyboard_status | RIGHT_SHIFT_BIT;
    }
    else if (check_scode == CAPSLOCK_KEY) {
      keyboard_status = keyboard_status ^ CAPSLOCK_BIT;
    }

    keyboard_prev_scancode = check_scode;
  }
}

void keyboard_install() {
  keyboard_status = 0;
  keyboard_prev_scancode = 0;
  keyboard_keycount = 0;
  irq_install_handler(1, keyboard_handler);
}

char keyboard_getchar() {
  unsigned int prev_keyc = keyboard_keycount;
  while (prev_keyc == keyboard_keycount);
  return keyboard_last_printable_char;
}
