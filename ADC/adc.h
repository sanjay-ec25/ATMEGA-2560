#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Reference voltage in millivolts (AVcc = 5 V) */
#define ADC_VREF_MV   5000UL

/* Resolution: 10 bits = 1024 levels (0 to 1023) */
#define ADC_LEVELS    1024UL

/* Turn the ADC on: reference = AVcc, ADC clock = 16 MHz / 128 = 125 kHz */
void     adc_init(void);

/* Read one channel (0 to 15). Returns a raw value from 0 to 1023. */
uint16_t adc_read(uint8_t channel);

/* Read one channel and return the voltage in millivolts (0 to 5000). */
uint16_t adc_read_mv(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif
