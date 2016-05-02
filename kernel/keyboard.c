#include "keyboard.h"
#include "isr.h"

#define ALT_KEY 255
#define CTRL_KEY 254
#define LEFT_SHIFT_KEY 253
#define RIGHT_SHIFT_KEY 252

#define KEYPRESS_BIT 0x0001
#define ALT_BIT 0x0002
#define CTRL_BIT 0x0004
#define LEFT_SHIFT_BIT 0x0008
#define RIGHT_SHIFT_BIT 0x0010

char keyboard_map[128] = {
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	'9', '0', '-', '=', '\b',	/* Backspace */
	'\t',			/* Tab */
	'q', 'w', 'e', 'r',	/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
	CTRL_KEY,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`', LEFT_SHIFT_KEY ,		/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', ',', '.', '/',  RIGHT_SHIFT_KEY,				/* Right shift */
	'*',
	ALT_KEY,	/* Alt */
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

uint16_t keyboard_status;

char keyboard_prev_key;

void keyboard_handler() {
  uint8_t scancode;
  unsigned char mappedchar;

  scancode = inportb(0x60);
  mappedchar = keyboard_map[scancode & 0x7F];

  if(scancode & 0x80) {
    // The key is released

    keyboard_status = keyboard_status & ~KEYPRESS_BIT;
    if (mappedchar == ALT_KEY) {
      keyboard_status = keyboard_status & ~ALT_BIT;
    }
    else if (mappedchar == CTRL_KEY) {
      keyboard_status = keyboard_status & ~CTRL_BIT;
    }
    else if (mappedchar == LEFT_SHIFT_KEY) {
      keyboard_status = keyboard_status & ~LEFT_SHIFT_BIT;
    }
    else if (mappedchar == RIGHT_SHIFT_KEY) {
      keyboard_status = keyboard_status & ~RIGHT_SHIFT_BIT;
    }
    keyboard_prev_key = 0;
  }
  else {
    // The key is pressed

    keyboard_status = keyboard_status | KEYPRESS_BIT;
    if (mappedchar == ALT_KEY) {
      keyboard_status = keyboard_status | ALT_BIT;
    }
    else if (mappedchar == CTRL_KEY) {
      keyboard_status = keyboard_status | CTRL_BIT;
    }
    else if (mappedchar == LEFT_SHIFT_KEY) {
      keyboard_status = keyboard_status | LEFT_SHIFT_BIT;
    }
    else if (mappedchar == RIGHT_SHIFT_KEY) {
      keyboard_status = keyboard_status | RIGHT_SHIFT_BIT;
    }
    else if (('0' <= mappedchar && mappedchar <= '9') || ('a' <= mappedchar && mappedchar <= 'z')) {
      // Letter entered
      putchar(mappedchar);
    }
    keyboard_prev_key = mappedchar;
  
  }
}

void keyboard_install() {
  keyboard_status = 0;
  keyboard_prev_key = 0;
  irq_install_handler(1, keyboard_handler);
}

