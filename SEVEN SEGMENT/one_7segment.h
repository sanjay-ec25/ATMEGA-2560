#ifndef ONE_7SEGMENT_H
#define ONE_7SEGMENT_H

#include <stdint.h>
#include "gpio.h"

/*
 * 1-Digit 7-Segment Driver
 * Assumption: Common-cathode 7-segment display
 *
 * Segment port connection:
 * bit 0 -> a
 * bit 1 -> b
 * bit 2 -> c
 * bit 3 -> d
 * bit 4 -> e
 * bit 5 -> f
 * bit 6 -> g
 * bit 7 -> dp
 */

void sevensegment1_init(uint8_t segment_port);
void sevensegment1_display(uint8_t value);
void sevensegment1_clear(void);

#endif
