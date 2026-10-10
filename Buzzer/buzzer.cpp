#include "buzzer.h"
#include "gpio.h"
#include <util/delay.h>

static uint8_t buz_port;
static uint8_t buz_pin;

void buzzer_init(uint8_t port, uint8_t pin)
{
    buz_port = port;
    buz_pin = pin;
    
    /* Set signal pin as output */
    pin_config(buz_port, buz_pin, OUTPUT);
    pin_write(buz_port, buz_pin, LOW);
}

void buzzer_set(uint8_t state)
{
    pin_write(buz_port, buz_pin, state);
}

void buzzer_update_pattern(uint16_t distance_cm)
{
    /* 
     * Hackathon Sound Intervals Pattern:
     * - SAFE (>50 cm): Silent / Off
     * - CAUTION (30-50 cm): Slow beep (100ms ON, 500ms OFF)
     * - WARNING (15-30 cm): Fast beep (100ms ON, 150ms OFF)
     * - STOP (<15 cm): Continuous sound (HIGH)
     */
    if (distance_cm == 0 || distance_cm > 50)
    {
        // SAFE: Silent
        pin_write(buz_port, buz_pin, LOW);
    }
    else if (distance_cm > 30)
    {
        // CAUTION: Slow Beep
        pin_write(buz_port, buz_pin, HIGH);
        _delay_ms(100);
        pin_write(buz_port, buz_pin, LOW);
        _delay_ms(400);
    }
    else if (distance_cm > 15)
    {
        // WARNING: Fast Beep
        pin_write(buz_port, buz_pin, HIGH);
        _delay_ms(100);
        pin_write(buz_port, buz_pin, LOW);
        _delay_ms(100);
    }
    else
    {
        // STOP: Continuous Sound
        pin_write(buz_port, buz_pin, HIGH);
    }
}