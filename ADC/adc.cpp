#include "adc.h"
#include <avr/io.h>

/*
 * ADC driver for ATmega2560 @ 16 MHz
 *
 * Resolution    : 10 bits (0 to 1023)
 * Reference     : AVcc (5 V)
 * Prescaler     : 128
 * ADC clock     : 16,000,000 / 128 = 125,000 Hz (125 kHz)
 *                 (the ADC needs 50 kHz to 200 kHz for full accuracy)
 * One conversion: 13 ADC clock cycles = 13 / 125,000 = 104 us
 *
 * Formula       : ADC value = (Vin / Vref) x 1024
 *                 Vin       = (ADC value x Vref) / 1024
 */

void adc_init(void)
{
    /* REFS0 = 1, REFS1 = 0 -> reference voltage is AVcc (5 V) */
    ADMUX = (1 << REFS0);

    /* ADEN = 1                    -> turn the ADC on
       ADPS2, ADPS1, ADPS0 = 1 1 1 -> prescaler 128 (125 kHz ADC clock) */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel)
{
    /* Only channels 0 to 15 exist */
    channel &= 0x0F;

    /* Step 1: choose the input pin.
       ADMUX bits MUX2:0 hold channel bits 2:0 (keep the reference bits). */
    ADMUX = (ADMUX & 0xE0) | (channel & 0x07);

    /* Channels 8 to 15 also need MUX5 in ADCSRB */
    if (channel & 0x08)
    {
        ADCSRB |= (1 << MUX5);
    }
    else
    {
        ADCSRB &= ~(1 << MUX5);
    }

    /* Step 2: start the conversion (ADSC = ADC Start Conversion) */
    ADCSRA |= (1 << ADSC);

    /* Step 3: wait. The hardware clears ADSC when the result is ready. */
    while (ADCSRA & (1 << ADSC))
    {
        /* wait about 104 us */
    }

    /* Step 4: read the 10-bit result (0 to 1023) */
    return ADC;
}

uint16_t adc_read_mv(uint8_t channel)
{
    uint16_t raw = adc_read(channel);

    /* mV = raw x 5000 / 1024
       Example: raw = 512 -> 512 x 5000 / 1024 = 2500 mV (2.5 V)
       uint32_t is used so raw x 5000 does not overflow 16 bits. */
    return (uint16_t)(((uint32_t)raw * ADC_VREF_MV) / ADC_LEVELS);
}
