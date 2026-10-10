#include "define.h"
#include "registers.h"

volatile uint8_t *const ddr[PORT_COUNT] =
{
    (volatile uint8_t *)DDRA_ADDR,
    (volatile uint8_t *)DDRB_ADDR,
    (volatile uint8_t *)DDRC_ADDR,
    (volatile uint8_t *)DDRD_ADDR,
    (volatile uint8_t *)DDRE_ADDR,
    (volatile uint8_t *)DDRF_ADDR,
    (volatile uint8_t *)DDRG_ADDR,
    (volatile uint8_t *)DDRH_ADDR,
    (volatile uint8_t *)DDRJ_ADDR,
    (volatile uint8_t *)DDRK_ADDR,
    (volatile uint8_t *)DDRL_ADDR
};

volatile uint8_t *const port[PORT_COUNT] =
{
    (volatile uint8_t *)PORTA_ADDR,
    (volatile uint8_t *)PORTB_ADDR,
    (volatile uint8_t *)PORTC_ADDR,
    (volatile uint8_t *)PORTD_ADDR,
    (volatile uint8_t *)PORTE_ADDR,
    (volatile uint8_t *)PORTF_ADDR,
    (volatile uint8_t *)PORTG_ADDR,
    (volatile uint8_t *)PORTH_ADDR,
    (volatile uint8_t *)PORTJ_ADDR,
    (volatile uint8_t *)PORTK_ADDR,
    (volatile uint8_t *)PORTL_ADDR
};

volatile uint8_t *const pin[PORT_COUNT] =
{
    (volatile uint8_t *)PINA_ADDR,
    (volatile uint8_t *)PINB_ADDR,
    (volatile uint8_t *)PINC_ADDR,
    (volatile uint8_t *)PIND_ADDR,
    (volatile uint8_t *)PINE_ADDR,
    (volatile uint8_t *)PINF_ADDR,
    (volatile uint8_t *)PING_ADDR,
    (volatile uint8_t *)PINH_ADDR,
    (volatile uint8_t *)PINJ_ADDR,
    (volatile uint8_t *)PINK_ADDR,
    (volatile uint8_t *)PINL_ADDR
};
