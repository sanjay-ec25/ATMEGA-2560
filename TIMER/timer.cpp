#include "timer.h"

/*
 * ATmega2560
 * CPU Clock = 16 MHz
 
 * Timer0 is used in CTC mode.
 
 * Prescaler = 64
 *
 * Timer frequency:
 *
 * 16,000,000 / 64 = 250,000 Hz
 *
 * Timer tick:
 *
 * 1 / 250,000 = 4 us
 *
 * OCR0A = 249
 *
 * 250 counts × 4 us = 1 ms
 */

/* Timer0 Registers */
#define TCCR0A (*(volatile uint8_t *)0x44)
#define TCCR0B (*(volatile uint8_t *)0x45)
#define TCNT0  (*(volatile uint8_t *)0x46)
#define OCR0A  (*(volatile uint8_t *)0x47)

/* Timer0 CTC mode */
#define WGM01 1

/* Timer0 prescaler bits */
#define CS01 1
#define CS00 0

/* Output Compare Flag Register 0 */
#define TIFR0  (*(volatile uint8_t *)0x35)

/* Output Compare A Match Flag */
#define OCF0A 1


/* =========================
   TIMER INITIALIZATION
   ========================= */

void timer_init(void)
{
    /* CTC mode */
    TCCR0A = (1 << WGM01);

    /* Normal port operation */
    TCCR0A &= ~(1 << 6);
    TCCR0A &= ~(1 << 7);

    /* Clear counter */
    TCNT0 = 0;

    /* Compare value for 1 ms */
    OCR0A = 249;

    /* Clear pending compare flag */
    TIFR0 |= (1 << OCF0A);

    /* Start Timer0
       Prescaler = 64
       CS01 = 1
       CS00 = 1
    */
    TCCR0B = (1 << CS01) | (1 << CS00);
}


/* =========================
   DELAY MILLISECONDS
   ========================= */

void timer_delay_ms(uint16_t ms)
{
    uint16_t i;

    for(i = 0; i < ms; i++)
    {
        /* Wait until Timer0 reaches OCR0A */
        while(!(TIFR0 & (1 << OCF0A)))
        {
        }

        /* Clear compare flag */
        TIFR0 |= (1 << OCF0A);
    }
}


/* =========================
   DELAY MICROSECONDS
   ========================= */

void timer_delay_us(uint16_t us)
{
    uint16_t i;

    /*
     * Timer tick = 4 us
     *
     * This function therefore works
     * approximately in 4 us steps.
     */

    for(i = 0; i < us / 4; i++)
    {
        /* Wait for compare match */
        while(!(TIFR0 & (1 << OCF0A)))
        {
        }

        /* Clear flag */
        TIFR0 |= (1 << OCF0A);
    }
}