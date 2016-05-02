#include "system.h"
#include "isr.h"

/* This will keep track of how many ticks that the system
 * has been running for */
unsigned int timer_ticks = 0;
unsigned int timer_seconds = 0;

/* Handles the timer. In this case it's very simple: We
 * increment the 'timer_ticks' variable every time the
 * timer fires. By default, the timer fires 18.222 times
 * per second. Why 18.222Hz? Some engineer at IBM must've
 * been smoking something funky
 * We change it using the timer_phase function*/
void timer_handler(struct interrupt_handler_regs *r) {
  /* Increment our 'tick count' */
  timer_ticks++;

  /* Every TIMER_FREQUENCY clocks (approximately 1 second), we will
   * display a message on the screen */
  if (timer_ticks % TIMER_FREQUENCY == 0)
  {
    /*puts("One second has passed\n");*/
    timer_seconds++;
  }

}

void timer_phase(int hz)
{
  int divisor = 1193180 / hz;       /* Calculate our divisor */
  outportb(0x43, 0x36);             /* Set our command byte 0x36 */
  outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
  outportb(0x40, divisor >> 8);     /* Set high byte of divisor */

}

/* Sets up the system clock by installing the timer handler
 * into IRQ0 */
void timer_install() {
  /* Installs 'timer_handler' to IRQ0 */
  timer_phase(TIMER_FREQUENCY);
  irq_install_handler(0, timer_handler);
}

void timer_wait(unsigned int seconds) {
  unsigned int end = seconds + timer_seconds;

  while(timer_seconds < end) {
    // Otherwise compiler optimisation breks the code
    // I don't claim to fully understand this, but I know enough to make this work
    asm volatile ("" : : : "memory");
  }
}
