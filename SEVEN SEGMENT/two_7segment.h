#ifndef TWO_7SEGMENT_H
#define TWO_7SEGMENT_H

#include <stdint.h>
#include "gpio.h"

/*
 * 2-Digit 7-Segment Driver
 * Assumption: Common-cathode displays
 *
 * Segment port:
 * bit 0 -> a
 * bit 1 -> b
 * bit 2 -> c
 * bit 3 -> d
 * bit 4 -> e
 * bit 5 -> f
 * bit 6 -> g
 * bit 7 -> dp
 *
 * digit1_pin -> tens digit enable
 * digit2_pin -> units digit enable
 *
 * The two digits are multiplexed.
 * Call sevensegment2_refresh() repeatedly from loop/timer.
 */

void sevensegment2_init(uint8_t segment_port,
                        uint8_t digit_port,
                        uint8_t digit1_pin,
                        uint8_t digit2_pin);
void sevensegment2_set_number(uint8_t number);
void sevensegment2_refresh(void);
void sevensegment2_clear(void);

#endif
