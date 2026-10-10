#ifndef DEFINE_H
#define DEFINE_H

#include <stdint.h>

/* Number of GPIO ports */
#define PORT_COUNT 11

/* Port numbers */
#define PORT_A  0
#define PORT_B  1
#define PORT_C  2
#define PORT_D  3
#define PORT_E  4
#define PORT_F  5
#define PORT_G  6
#define PORT_H  7
#define PORT_J  8
#define PORT_K  9
#define PORT_L  10


/* GPIO register pointer arrays */

extern volatile uint8_t *const port[PORT_COUNT];
extern volatile uint8_t *const ddr[PORT_COUNT];
extern volatile uint8_t *const pin[PORT_COUNT];

#endif
