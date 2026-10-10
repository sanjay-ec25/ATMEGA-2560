#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

/* Initialize buzzer signal pin */
void buzzer_init(uint8_t port, uint8_t pin);

/* Set buzzer state directly */
void buzzer_set(uint8_t state);

/* Update buzzer tone patterns according to measured distance */
void buzzer_update_pattern(uint16_t distance_cm);

#endif