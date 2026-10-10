#ifndef ULTRA_H
#define ULTRA_H

#include <stdint.h>

void ultra_init(uint8_t port,
                uint8_t trig_pin,
                uint8_t echo_pin);
void ultra_trigger(void);
uint16_t ultra_get_distance_cm(void);
uint16_t ultra_get_distance_mm(void);

#endif