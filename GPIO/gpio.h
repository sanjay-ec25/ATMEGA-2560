#ifndef GPIO_H
#define GPIO_H

#include "define.h"

#define INPUT   0
#define OUTPUT  1
#define LOW     0
#define HIGH    1

#define pinMode      pin_config
#define digitalWrite pin_write
#define digitalRead  pin_read

#define portMode     port_config
#define portWrite    port_write
#define portRead     port_read


/* PORT CONFIGURATION */

void port_config(uint8_t x, uint8_t value);
void pin_config(uint8_t x, uint8_t i, uint8_t value);


/* PORT WRITE */

void port_write(uint8_t x, uint8_t value);
void pin_write(uint8_t x, uint8_t i, uint8_t value);


/* PORT READ */

uint8_t port_read(uint8_t x);
uint8_t pin_read(uint8_t x, uint8_t i);

#endif
