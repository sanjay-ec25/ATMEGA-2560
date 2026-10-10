#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/* Initialize Timer0 */
void timer_init(void);

/* Delay in milliseconds */
void timer_delay_ms(uint16_t ms);

/* Delay in microseconds */
void timer_delay_us(uint16_t us);

#endif