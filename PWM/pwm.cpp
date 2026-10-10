#include "pwm.h"
#include <avr/io.h>

/*
 * PWM driver for ATmega2560 @ 16 MHz
 *
 * Timer used : Timer2 (Timer0 is already used by timer_delay_ms)
 * Mode       : Fast PWM, 8-bit (counter runs 0 to 255)
 * Output pin : OC2A = PB4 (Arduino Mega pin 10)
 * Prescaler  : 64
 *
 * PWM frequency = 16,000,000 / (64 x 256) = 976.56 Hz
 */

void pwm_init(void)
{
    /* PB4 (OC2A) as output, start LOW */
    DDRB  |= (1 << PB4);
    PORTB &= ~(1 << PB4);

    /* Fast PWM, TOP = 255 (WGM21 = 1, WGM20 = 1, WGM22 = 0) */
    TCCR2A = (1 << WGM21) | (1 << WGM20);

    /* Prescaler 64 (CS22 = 1) -> timer starts counting */
    TCCR2B = (1 << CS22);

    /* Start with compare value 0 and counter 0 */
    OCR2A = 0;
    TCNT2 = 0;
}

void pwm_set_duty(uint8_t duty)
{
    /* Duty 0 is treated as "off" to avoid the small spike
       Fast PWM produces at OCR2A = 0 */
    if (duty == 0)
    {
        pwm_stop();
        return;
    }

    /* Pin goes LOW when the counter reaches this value */
    OCR2A = duty;

    /* Non-inverting mode: HIGH at counter 0, LOW at compare match.
       This connects the timer to the PB4 pin. */
    TCCR2A |= (1 << COM2A1);
}

void pwm_stop(void)
{
    /* Disconnect the timer from the pin */
    TCCR2A &= ~(1 << COM2A1);

    /* Force the pin LOW so the buzzer is silent */
    PORTB &= ~(1 << PB4);
}