#include "ir.h"
#include "gpio.h"

static uint8_t ir_port;
static uint8_t sensor_pin;

void ir_init(uint8_t port, uint8_t pin)
{
    ir_port = port;
    sensor_pin = pin;

    /* Configure IR sensor pin as INPUT */
    pin_config(ir_port, sensor_pin, INPUT);
}

uint8_t ir_is_occupied(void)
{
    /* Active-LOW: Returns 1 if car detected (0V), 0 if clear (5V) */
    return (pin_read(ir_port, sensor_pin) == 0) ? 1 : 0;
}

uint8_t ir_get_available_slots(void)
{
    /* If occupied return 0 slots free; if clear return 1 slot free */
    return ir_is_occupied() ? 0 : 1;
}