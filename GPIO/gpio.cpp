#include "gpio.h"

/* =========================
   PORT CONFIGURATION
   ========================= */

void port_config(uint8_t x, uint8_t value)
{
    *ddr[x] = value;
}


/* =========================
   PIN CONFIGURATION
   ========================= */

void pin_config(uint8_t x, uint8_t i, uint8_t value)
{
    if (value)
    {
        *ddr[x] |= (1 << i);
    }
    else
    {
        *ddr[x] &= ~(1 << i);
    }
}


/* =========================
   PORT WRITE
   ========================= */

void port_write(uint8_t x, uint8_t value)
{
    *port[x] = value;
}


/* =========================
   PIN WRITE
   ========================= */

void pin_write(uint8_t x, uint8_t i, uint8_t value)
{
    if (value)
    {
        *port[x] |= (1 << i);
    }
    else
    {
        *port[x] &= ~(1 << i);
    }
}


/* =========================
   PORT READ
   ========================= */

uint8_t port_read(uint8_t x)
{
    return *pin[x];
}


/* =========================
   PIN READ
   ========================= */

uint8_t pin_read(uint8_t x, uint8_t i)
{
    return (*pin[x] >> i) & 1;
}
